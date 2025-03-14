#ifndef EEZ_LVGL_UI_SCREENS_H
#define EEZ_LVGL_UI_SCREENS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _objects_t {
    lv_obj_t *main;
    lv_obj_t *relays;
    lv_obj_t *battery_page;
    lv_obj_t *time_label;
    lv_obj_t *relay1;
    lv_obj_t *relay2;
    lv_obj_t *battery_meter;
    lv_obj_t *battery_label;
} objects_t;

extern objects_t objects;

enum ScreensEnum {
    SCREEN_ID_MAIN = 1,
    SCREEN_ID_RELAYS = 2,
    SCREEN_ID_BATTERY_PAGE = 3,
};

void create_screen_main();
void tick_screen_main();

void create_screen_relays();
void tick_screen_relays();

void create_screen_battery_page();
void tick_screen_battery_page();

void tick_screen_by_id(enum ScreensEnum screenId);
void tick_screen(int screen_index);

void create_screens();


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_SCREENS_H*/