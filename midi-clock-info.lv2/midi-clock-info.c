/*
 */

#include <lv2/lv2plug.in/ns/lv2core/lv2.h>
#include <lv2/lv2plug.in/ns/ext/atom/util.h>
#include <lv2/lv2plug.in/ns/ext/midi/midi.h>
#include <lv2/lv2plug.in/ns/ext/urid/urid.h>

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#define DEBUG_PLUGIN_LOG

typedef enum {
    PORT_RAW_MIDI_IN = 0,
    PORT_CTRL_OUT_PLAY_STATUS,
    PORT_CTRL_OUT_MTC_FRAME,
    PORT_CTRL_OUT_MTC_SECONDS,
    PORT_CTRL_OUT_MTC_MINUTES,
    PORT_CTRL_OUT_MTC_HOURS,
    PORT_CTRL_OUT_SONG_POSITION_POINTER,
    // TODO raw BPM based on clock pulse
    // TODO filtered BPM
    // TODO BPM/clock-pulse drift
} PortEnum;

typedef struct {
    int frame, frameLSB;
    int seconds, secondsLSB;
    int minutes, minutesLSB;
    int hours, hoursLSB;
} MTC;

typedef struct {
    // URIDs
    LV2_URID urid_atomSequence;
    LV2_URID urid_midiEvent;

    // data flow ports
    const LV2_Atom_Sequence* port_events_in;

    // control ports
    float* port_ctrl_out_play_status;
    float* port_ctrl_out_mtc_frame;
    float* port_ctrl_out_mtc_seconds;
    float* port_ctrl_out_mtc_minutes;
    float* port_ctrl_out_mtc_hours;
    float* port_ctrl_out_song_pos_ptr;

    // internal state
    bool needs_reset;
    MTC mtc;
} Data;

// values of `PORT_CTRL_OUT_PLAY_STATUS` port, as defined in the TTL
static const float kPlayStatusUndefined = 0.0f;
static const float kPlayStatusStart = 1.0f;
static const float kPlayStatusStop = 2.0f;
static const float kPlayStatusContinue = 3.0f;

static LV2_Handle instantiate(const LV2_Descriptor*     descriptor,
                              double                    rate,
                              const char*               path,
                              const LV2_Feature* const* features)
{
    Data* self = (Data*)calloc(1, sizeof(Data));

    // Get host features
    const LV2_URID_Map* map = NULL;

    for (int i = 0; features[i]; ++i) {
        if (!strcmp(features[i]->URI, LV2_URID__map)) {
            map = (const LV2_URID_Map*)features[i]->data;
            break;
        }
    }
    if (!map) {
        free(self);
        return NULL;
    }

    // Map URIs
    self->urid_atomSequence = map->map(map->handle, LV2_ATOM__Sequence);
    self->urid_midiEvent    = map->map(map->handle, LV2_MIDI__MidiEvent);

    return self;
}

static void connect_port(LV2_Handle instance, uint32_t port, void* data)
{
    Data* const self = (Data*)instance;

    switch (port)
    {
    case PORT_RAW_MIDI_IN:
            self->port_events_in = (const LV2_Atom_Sequence*)data;
            break;
    case PORT_CTRL_OUT_PLAY_STATUS:
            self->port_ctrl_out_play_status = (float*)data;
            break;
    case PORT_CTRL_OUT_MTC_FRAME:
            self->port_ctrl_out_mtc_frame = (float*)data;
            break;
    case PORT_CTRL_OUT_MTC_SECONDS:
            self->port_ctrl_out_mtc_seconds = (float*)data;
            break;
    case PORT_CTRL_OUT_MTC_MINUTES:
            self->port_ctrl_out_mtc_minutes = (float*)data;
            break;
    case PORT_CTRL_OUT_MTC_HOURS:
            self->port_ctrl_out_mtc_hours = (float*)data;
            break;
    case PORT_CTRL_OUT_SONG_POSITION_POINTER:
            self->port_ctrl_out_song_pos_ptr = (float*)data;
            break;
    }
}

static void activate(LV2_Handle instance)
{
    Data* const self = (Data*)instance;

    self->needs_reset = true;
    memset(&self->mtc, 0, sizeof(self->mtc));
}

static void run(LV2_Handle instance, uint32_t sample_count)
{
    Data* const self = (Data*)instance;

    if (self->needs_reset)
    {
        *self->port_ctrl_out_play_status = kPlayStatusUndefined;
        *self->port_ctrl_out_mtc_frame = 0.0f;
        *self->port_ctrl_out_mtc_seconds = 0.0f;
        *self->port_ctrl_out_mtc_minutes = 0.0f;
        *self->port_ctrl_out_mtc_hours = 0.0f;
        *self->port_ctrl_out_song_pos_ptr = 0.0f;
        self->needs_reset = false;
#ifdef DEBUG_PLUGIN_LOG
        fprintf(stdout, "MIDI Clock Info: reset\n");
        fflush(stdout);
#endif
    }

    // Read incoming events
    LV2_ATOM_SEQUENCE_FOREACH(self->port_events_in, ev)
    {
        if (ev->body.type == self->urid_midiEvent)
        {
            const uint8_t* const msg = (const uint8_t*)(ev + 1);

            switch (msg[0])
            {
            case 0xF1: // MIDI Time Code (Quarter Frame)
            {
                const int type  = (msg[1] >> 4) & 0xf;
                const int value =  msg[1] & 0xf;

                switch (type)
                {
                case 0:
                    self->mtc.frameLSB = value;
                    break;
                case 1:
                    self->mtc.frame = self->mtc.frameLSB + (value * 16);
                    break;
                case 2:
                    self->mtc.secondsLSB = value;
                    break;
                case 3:
                    self->mtc.seconds = self->mtc.secondsLSB + (value * 16);
                    break;
                case 4:
                    self->mtc.minutesLSB = value;
                    break;
                case 5:
                    self->mtc.minutes = self->mtc.minutesLSB + (value * 16);
                    break;
                case 6:
                    self->mtc.hoursLSB = value & 0x1f;
                    break;
                case 7:
                    self->mtc.hours = self->mtc.hoursLSB + ((value * 16) & 0x1f);

                    // only update after receiving all bits
                    *self->port_ctrl_out_mtc_frame = self->mtc.frame;
                    *self->port_ctrl_out_mtc_seconds = self->mtc.seconds;
                    *self->port_ctrl_out_mtc_minutes = self->mtc.minutes;
                    *self->port_ctrl_out_mtc_hours = self->mtc.hours;
#ifdef DEBUG_PLUGIN_LOG
                    fprintf(stdout, "MIDI Clock Info: MTC -> %02i:%02i:%02i:%03i\n",
                            self->mtc.hours, self->mtc.minutes, self->mtc.seconds, self->mtc.frame);
                    fflush(stdout);
#endif
                    break;
                }

                break;
            }

            case 0xF2: // MIDI Song Position Pointer
            {
                const int value = msg[1] + 128 * msg[2];
                *self->port_ctrl_out_song_pos_ptr = value;
#ifdef DEBUG_PLUGIN_LOG
                fprintf(stdout, "MIDI Clock Info: song pos ptr -> %i\n", value);
                fflush(stdout);
#endif
                break;
            }

            case 0xF8: // MIDI Clock "Pulse"
                // TODO
                break;

            case 0xFA: // MIDI Clock Start
                *self->port_ctrl_out_play_status = kPlayStatusStart;
#ifdef DEBUG_PLUGIN_LOG
                fprintf(stdout, "MIDI Clock Info: play status -> start\n");
                fflush(stdout);
#endif
                break;

            case 0xFB: // MIDI Clock Continue
                *self->port_ctrl_out_play_status = kPlayStatusContinue;
#ifdef DEBUG_PLUGIN_LOG
                fprintf(stdout, "MIDI Clock Info: play status -> continue\n");
                fflush(stdout);
#endif
                break;

            case 0xFC: // MIDI Clock Stop
                *self->port_ctrl_out_play_status = kPlayStatusStop;
#ifdef DEBUG_PLUGIN_LOG
                fprintf(stdout, "MIDI Clock Info: play status -> stop\n");
                fflush(stdout);
#endif
                break;
            }
        }
    }
}

static void cleanup(LV2_Handle instance)
{
    free((Data*)instance);
}

static const LV2_Descriptor descriptor = {
    .URI = "http://moddevices.com/plugins/mod-devel/midi-clock-info",
    .instantiate = instantiate,
    .connect_port = connect_port,
    .activate = activate,
    .run = run,
    .deactivate = NULL,
    .cleanup = cleanup,
    .extension_data = NULL
};

LV2_SYMBOL_EXPORT
const LV2_Descriptor* lv2_descriptor(uint32_t index)
{
    return (index == 0) ? &descriptor : NULL;
}
