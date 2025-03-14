#include <stdbool.h>
#include "actions.h"

lv_event_t eezEvent = {};
bool eezEventIsAvailable = false;

lv_dir_t swipeDir = LV_DIR_NONE;
lv_obj_t *swipeObj = NULL;
void action_global_eez_event(lv_event_t *event) {
    eezEvent = *event;
    eezEventIsAvailable = true;
}

void action_global_swipe_event(lv_event_t * e) {
    swipeObj = lv_event_get_current_target(e);
    swipeDir = lv_indev_get_gesture_dir(lv_indev_get_act());
}