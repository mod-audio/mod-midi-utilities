/*
 */

#include <lv2/lv2plug.in/ns/lv2core/lv2.h>
#include <lv2/lv2plug.in/ns/ext/atom/util.h>
#include <lv2/lv2plug.in/ns/ext/midi/midi.h>
#include <lv2/lv2plug.in/ns/ext/urid/urid.h>

#include "peakmeter/kmeterdsp.cc"

typedef enum {
    PORT_CONTROL_TARGET = 0,
    PORT_AUDIO_IN,
    PORT_ATOM_OUT
} PortEnum;

typedef struct {
    // history, send data when changes happen
    int prev_cc_num;
    int prev_cc_value;

    // URIDs
    LV2_URID urid_atomSequence;
    LV2_URID urid_midiEvent;

    // control ports
    const float* port_ctrl_target;

    // data flow ports
    const float* port_audio_in;
    LV2_Atom_Sequence* port_events_out;

    // peak meter class
    Kmeterdsp meter;
} Data;

// Struct for a 3 byte MIDI event
typedef struct {
    LV2_Atom_Event event;
    uint8_t        msg[3];
} LV2_Atom_MIDI;

static LV2_Handle instantiate(const LV2_Descriptor*     descriptor,
                              double                    rate,
                              const char*               path,
                              const LV2_Feature* const* features)
{
    Data* const self = new Data();

    // Get host features
    const LV2_URID_Map* map = NULL;

    for (int i = 0; features[i]; ++i) {
        if (!strcmp(features[i]->URI, LV2_URID__map)) {
            map = (const LV2_URID_Map*)features[i]->data;
            break;
        }
    }
    if (!map) {
        delete self;
        return NULL;
    }

    // Map URIs
    self->urid_atomSequence = map->map(map->handle, LV2_ATOM__Sequence);
    self->urid_midiEvent    = map->map(map->handle, LV2_MIDI__MidiEvent);

    return self;
}

static void connect_port(LV2_Handle instance, uint32_t port, void* data)
{
    Data* self = (Data*)instance;

    switch (port)
    {
    case PORT_CONTROL_TARGET:
            self->port_ctrl_target = (const float*)data;
            break;
    case PORT_AUDIO_IN:
            self->port_audio_in = (const float*)data;
            break;
    case PORT_ATOM_OUT:
            self->port_events_out = (LV2_Atom_Sequence*)data;
            break;
    }
}

static void activate(LV2_Handle instance)
{
    Data* self = (Data*)instance;

    self->prev_cc_num = -1;
    self->prev_cc_value = -1;
}

static int midimax(int v)
{
    return v > 127 ? 127 : v;
}

static void run(LV2_Handle instance, uint32_t sample_count)
{
    Data* self = (Data*)instance;

    const float peak = fabs(self->meter.process(self->port_audio_in, sample_count));

    const int cur_num   = (int)(*self->port_ctrl_target + 0.5f);
    const int cur_value = midimax((int)(peak*127.0f));

    if (self->prev_cc_num == cur_num && self->prev_cc_value == cur_value)
        return;

    // Get the capacity
    const uint32_t out_capacity = self->port_events_out->atom.size;

    // Write an empty Sequence header to the output port
    lv2_atom_sequence_clear(self->port_events_out);

    // Set port type
    self->port_events_out->atom.type = self->urid_atomSequence;

    LV2_Atom_MIDI msg;
    memset(&msg, 0, sizeof(LV2_Atom_MIDI));

    msg.event.body.size = 3;
    msg.event.body.type = self->urid_midiEvent;

    msg.msg[0] = LV2_MIDI_MSG_CONTROLLER;
    msg.msg[1] = cur_num;
    msg.msg[2] = cur_value;

    lv2_atom_sequence_append_event(self->port_events_out, out_capacity, (LV2_Atom_Event*)&msg);

    self->prev_cc_num   = cur_num;
    self->prev_cc_value = cur_value;
}

static void cleanup(LV2_Handle instance)
{
    delete (Data*)instance;
}

static const LV2_Descriptor descriptor = {
    .URI = "http://moddevices.com/plugins/mod-devel/PeakToCC",
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
