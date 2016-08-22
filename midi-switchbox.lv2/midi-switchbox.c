/*
 */

#include <lv2/lv2plug.in/ns/lv2core/lv2.h>
#include <lv2/lv2plug.in/ns/ext/atom/util.h>

#include <stdlib.h>

typedef enum {
    PORT_CONTROL_TARGET = 0,
    PORT_ATOM_IN,
    PORT_ATOM_OUT1,
    PORT_ATOM_OUT2
} PortEnum;

typedef struct {
    // control ports
    const float* port_target;
    // atom ports
    const LV2_Atom_Sequence* port_events_in;
    LV2_Atom_Sequence* port_events_out1;
    LV2_Atom_Sequence* port_events_out2;
} Data;

static LV2_Handle instantiate(const LV2_Descriptor*     descriptor,
                              double                    rate,
                              const char*               path,
                              const LV2_Feature* const* features)
{
    return (LV2_Handle)calloc(1, sizeof(Data));
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

static void run(LV2_Handle instance, uint32_t sample_count)
{
    Data* self = (Data*)instance;

    const int target_second = (*self->port_target) > 0.5f;

    // Get the capacity
    const uint32_t out_capacity_1 = self->port_events_out1->atom.size;
    const uint32_t out_capacity_2 = self->port_events_out2->atom.size;

    // Write an empty Sequence header to the outputs
    lv2_atom_sequence_clear(self->port_events_out1);
    lv2_atom_sequence_clear(self->port_events_out2);

    // Read incoming events
    LV2_ATOM_SEQUENCE_FOREACH(self->port_events_in, ev)
    {
        if (target_second)
        {
            lv2_atom_sequence_append_event(self->port_events_out2,
                                           out_capacity_2,
                                           ev);
        }
        else
        {
            lv2_atom_sequence_append_event(self->port_events_out1,
                                           out_capacity_1,
                                           ev);
        }
    }
}

static void cleanup(LV2_Handle instance)
{
    free(instance);
}

static const LV2_Descriptor descriptor = {
    .URI = "http://moddevices.com/plugins/mod-devel/MIDI-Switchbox",
    .instantiate = instantiate,
    .connect_port = connect_port,
    .activate = NULL,
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
