#include <string.h>

#include "screens.h"
#include "images.h"
#include "fonts.h"
#include "actions.h"
#include "vars.h"
#include "styles.h"
#include "ui.h"

#include <string.h>

objects_t objects;
lv_obj_t *tick_value_change_obj;

static lv_meter_scale_t * scale0;
static lv_meter_indicator_t * indicator1;
static lv_meter_indicator_t * indicator2;
static lv_meter_indicator_t * indicator3;

void create_screen_main() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.main = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 240, 240);
    lv_obj_add_event_cb(obj, action_global_swipe_event, LV_EVENT_GESTURE, (void *)0);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff504287), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *parent_obj = obj;
        {
            // timeLabel
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.time_label = obj;
            lv_obj_set_pos(obj, 28, 103);
            lv_obj_set_size(obj, 184, 35);
            lv_label_set_text(obj, "99:99:99");
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffc2c87c), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_32, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
    }
    
    tick_screen_main();
}

void tick_screen_main() {
}

void create_screen_relays() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.relays = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 240, 240);
    lv_obj_add_event_cb(obj, action_global_swipe_event, LV_EVENT_GESTURE, (void *)0);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff504287), LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *parent_obj = obj;
        {
            // relay1
            lv_obj_t *obj = lv_switch_create(parent_obj);
            objects.relay1 = obj;
            lv_obj_set_pos(obj, 70, 33);
            lv_obj_set_size(obj, 100, 50);
            lv_obj_add_event_cb(obj, action_global_eez_event, LV_EVENT_VALUE_CHANGED, (void *)0);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff942222), LV_PART_INDICATOR | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffc7a6a6), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // relay2
            lv_obj_t *obj = lv_switch_create(parent_obj);
            objects.relay2 = obj;
            lv_obj_set_pos(obj, 70, 167);
            lv_obj_set_size(obj, 100, 50);
            lv_obj_add_event_cb(obj, action_global_eez_event, LV_EVENT_VALUE_CHANGED, (void *)0);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffc7a6a6), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 70, 17);
            lv_obj_set_size(obj, 100, 16);
            lv_label_set_text(obj, "Relay 1\n");
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 70, 151);
            lv_obj_set_size(obj, 100, 16);
            lv_label_set_text(obj, "Relay 2");
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
    }
    
    tick_screen_relays();
}

void tick_screen_relays() {
}

void create_screen_battery_page() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.battery_page = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 240, 240);
    lv_obj_add_event_cb(obj, action_global_swipe_event, LV_EVENT_GESTURE, (void *)0);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff504287), LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *parent_obj = obj;
        {
            // batteryMeter
            lv_obj_t *obj = lv_meter_create(parent_obj);
            objects.battery_meter = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 240, 240);
            {
                lv_meter_scale_t *scale = lv_meter_add_scale(obj);
                scale0 = scale;
                lv_meter_set_scale_ticks(obj, scale, 41, 1, 5, lv_color_hex(0xffa0a0a0));
                lv_meter_set_scale_major_ticks(obj, scale, 8, 3, 10, lv_color_hex(0xffcfc0c0), 10);
                lv_meter_set_scale_range(obj, scale, 0, 100, 300, 120);
                {
                    lv_meter_indicator_t *indicator = lv_meter_add_needle_line(obj, scale, 3, lv_color_hex(0xff9a9672), -28);
                    indicator1 = indicator;
                }
                {
                    lv_meter_indicator_t *indicator = lv_meter_add_arc(obj, scale, 4, lv_color_hex(0xffec3333), 5);
                    indicator2 = indicator;
                    lv_meter_set_indicator_start_value(obj, indicator, 0);
                    lv_meter_set_indicator_end_value(obj, indicator, 60);
                }
                {
                    lv_meter_indicator_t *indicator = lv_meter_add_arc(obj, scale, 5, lv_color_hex(0xff818ec7), 10);
                    indicator3 = indicator;
                    lv_meter_set_indicator_start_value(obj, indicator, 50);
                    lv_meter_set_indicator_end_value(obj, indicator, 100);
                }
            }
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff504287), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffcfc0c0), LV_PART_TICKS | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // batteryLabel
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.battery_label = obj;
                    lv_obj_set_pos(obj, 80, 190);
                    lv_obj_set_size(obj, 50, 20);
                    lv_label_set_text(obj, "Text");
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffcfc0c0), LV_PART_MAIN | LV_STATE_DEFAULT);
                }
            }
        }
    }
    
    tick_screen_battery_page();
}

void tick_screen_battery_page() {
    {
        lv_meter_indicator_t *indicator;
        
        lv_ll_t *indicators = &((lv_meter_t *)objects.battery_meter)->indicator_ll;
        int index = 0;
        for (indicator = _lv_ll_get_tail(indicators); index > 0 && indicator != NULL; indicator = _lv_ll_get_prev(indicators, indicator), index--);
        
        if (indicator) {
            int32_t new_val = get_var_meter_indicator_value();
            int32_t cur_val = indicator->start_value;
            if (new_val != cur_val) {
                tick_value_change_obj = objects.battery_meter;
                lv_meter_set_indicator_value(objects.battery_meter, indicator, new_val);
                tick_value_change_obj = NULL;
            }
        }
    }
}



typedef void (*tick_screen_func_t)();
tick_screen_func_t tick_screen_funcs[] = {
    tick_screen_main,
    tick_screen_relays,
    tick_screen_battery_page,
};
void tick_screen(int screen_index) {
    tick_screen_funcs[screen_index]();
}
void tick_screen_by_id(enum ScreensEnum screenId) {
    tick_screen_funcs[screenId - 1]();
}

void create_screens() {
    lv_disp_t *dispp = lv_disp_get_default();
    lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), false, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    
    create_screen_main();
    create_screen_relays();
    create_screen_battery_page();
}
