/*
 */

#include <lv2/lv2plug.in/ns/lv2core/lv2.h>
#include <lv2/lv2plug.in/ns/ext/atom/util.h>
#include <lv2/lv2plug.in/ns/ext/midi/midi.h>
#include <lv2/lv2plug.in/ns/ext/urid/urid.h>

#include <stdbool.h>
#include <stdlib.h>

typedef enum {
    PORT_CONTROL_TARGET = 0,
    PORT_ATOM_IN,
    PORT_ATOM_OUT1,
    PORT_ATOM_OUT2
} PortEnum;

typedef enum {
    TARGET_PORT_1 = 0,
    TARGET_PORT_2
} TargetEnum;

typedef struct {

    int previous_target;

    // URIDs
    LV2_URID urid_midiEvent;

    // control ports
    const float* port_target;

    // atom ports
    const LV2_Atom_Sequence* port_events_in;
    LV2_Atom_Sequence* port_events_out1;
    LV2_Atom_Sequence* port_events_out2;
    LV2_Atom_Sequence* port_events_out;
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
    self->urid_midiEvent = map->map(map->handle, LV2_MIDI__MidiEvent);

    self->previous_target = 0;
    self->port_events_out = NULL;

    return self;
}

static void connect_port(LV2_Handle instance, uint32_t port, void* data)
{
    Data* self = (Data*)instance;

    switch (port)
    {
    case PORT_CONTROL_TARGET:
            self->port_target = (const float*)data;
            break;
    case PORT_ATOM_IN:
            self->port_events_in = (const LV2_Atom_Sequence*)data;
            break;
    case PORT_ATOM_OUT1:
            self->port_events_out1 = (LV2_Atom_Sequence*)data;
            break;
    case PORT_ATOM_OUT2:
            self->port_events_out2 = (LV2_Atom_Sequence*)data;
            break;
    }
}

static void activate(LV2_Handle instance)
{
}

static void run(LV2_Handle instance, uint32_t sample_count)
{
    Data* self = (Data*)instance;

    const int target = (int)(*self->port_target);

    // Get the capacity
    const uint32_t out_capacity_1 = self->port_events_out1->atom.size;
    const uint32_t out_capacity_2 = self->port_events_out2->atom.size;

    // Write an empty Sequence header to the outputs
    lv2_atom_sequence_clear(self->port_events_out1);
    lv2_atom_sequence_clear(self->port_events_out2);

    // LV2 is so nice...
    self->port_events_out1->atom.type = self->port_events_in->atom.type;
    self->port_events_out2->atom.type = self->port_events_in->atom.type;

    // Send note-offs if source port changed
    if (self->previous_target != target)
    {
        LV2_Atom_MIDI msg;
        memset(&msg, 0, sizeof(LV2_Atom_MIDI));

        msg.event.body.size = 3;
        msg.event.body.type = self->urid_midiEvent;
        msg.msg[2] = 0;

        uint32_t out_capacity;

        switch ((TargetEnum)self->previous_target)
        {
            case TARGET_PORT_1:
                out_capacity = self->port_events_out1->atom.size;
                self->port_events_out = self->port_events_out1;
                break;
            case TARGET_PORT_2:
                out_capacity = self->port_events_out2->atom.size;
                self->port_events_out = self->port_events_out2;
                break;
        }

        for (uint32_t c = 0; c < 0xf; ++c) {
            msg.msg[0] = 0xb0 | c;
            msg.msg[1] = 0x40; // sustain pedal
            lv2_atom_sequence_append_event(self->port_events_out,
                    out_capacity,
                    (LV2_Atom_Event*)&msg);
            msg.msg[1] = 0x7b; // all notes off
            lv2_atom_sequence_append_event(self->port_events_out,
                    out_capacity,
                    (LV2_Atom_Event*)&msg);
        }
        self->previous_target = target;
    }

    // Read incoming events
    LV2_ATOM_SEQUENCE_FOREACH(self->port_events_in, ev)
    {
        if (ev->body.type == self->urid_midiEvent)
        {
            switch ((TargetEnum)target)
            {
                case TARGET_PORT_1:
                    lv2_atom_sequence_append_event(self->port_events_out1,
                            out_capacity_1,
                            ev);
                    break;
                case TARGET_PORT_2:
                    lv2_atom_sequence_append_event(self->port_events_out2,
                            out_capacity_2,
                            ev);
                    break;

            }
        }
    }
}

static void cleanup(LV2_Handle instance)
{
    free(instance);
}

static const LV2_Descriptor descriptor = {
    .URI = "http://moddevices.com/plugins/mod-devel/MIDI-Switchbox_1-2",
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
