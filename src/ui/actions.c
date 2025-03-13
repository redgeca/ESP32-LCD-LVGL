#include <stdbool.h>
#include "actions.h"

lv_event_t eezEvent = {};
bool eezEventIsAvailable = false;

void action_global_eez_event(lv_event_t *event) {
    eezEvent = *event;
    eezEventIsAvailable = true;
}