#include <Arduino.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include <Wire.h>
#include <lvgl.h>
#include "WiFiManager.h"
//#include "TouchDrvCSTXXX.hpp"
#include "CST816S.h"
#include <NTPClient.h>
#include <time.h>
#include "ui/ui.h"
#include "ui/actions.h"
#include "ui/screens.h"
#include "ui/vars.h"

#define SENSOR_SDA 6
#define SENSOR_SCL 7
#define SENSOR_RST 13
#define SENSOR_IRQ 5

TFT_eSPI lcd = TFT_eSPI();
TFT_eSprite sprite = TFT_eSprite(&lcd);
TFT_eSprite sprite2 = TFT_eSprite(&lcd);
TFT_eSprite backSprite = TFT_eSprite(&lcd);

CST816S touch(6, SENSOR_SCL, SENSOR_RST, SENSOR_IRQ);	// sda, scl, rst, irq

//TouchDrvCSTXXX touch;
static lv_disp_draw_buf_t draw_buf;

// EEZ Studio events
extern lv_event_t eezEvent;
extern bool eezEventIsAvailable;
extern lv_obj_t *swipeObj;
extern lv_dir_t swipeDir;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

void TFTSetBrightness(uint8_t Value) {
  if (Value < 0 || Value > 100) {
    printf("TFTSetBrightness Error \r\n");
  } else {
    analogWrite(TFT_BL, Value * 2.55);
  }
}

void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    int w = (area->x2 - area->x1 + 1);
    int h = (area->y2 - area->y1 + 1);

    lcd.startWrite();                               /* Start new TFT transaction */
    lcd.setAddrWindow(area->x1, area->y1, w, h);    /* set the working window */
    lcd.pushColors(&color_p->full, w * h, true);    //true

    lcd.endWrite();            /* terminate TFT transaction */
    lv_disp_flush_ready(disp); /* tell lvgl that flushing is done */
}

void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{
    uint32_t touchX = 0;
    uint32_t touchY = 0;
    uint32_t touchFingers = 0;

    if (touch.available()) {
        touchX = touch.data.x;
        touchY = touch.data.y;

        data->state = LV_INDEV_STATE_PRESSED;
        data->point.x = touchX;
        data->point.y = touchY;
    } else {
        data->state = LV_INDEV_STATE_RELEASED;
    }


//    uint8_t touched;
//    uint8_t gesture;
//    int16_t touchX[5], touchY[5];

//    touched = touch.getPoint(touchX, touchY, touch.getSupportTouchPoint());

//     if (!touched)
//     {
//         data->state = LV_INDEV_STATE_REL;
//     }
//     else
//     {
//         data->state = LV_INDEV_STATE_PR;
// //        Serial.printf("Touched : %d x %d\n", touchX[0], touchY[0]);
//         /*Set the coordinates*/
//         data->point.x = touchX[0];
//         data->point.y = touchY[0];
//     }
}

void setup() {
    Serial.begin(115200);

    pinMode(15, OUTPUT);
    pinMode(16, OUTPUT);
    digitalWrite(15, HIGH);
    digitalWrite(16, HIGH);

    Serial.println("Initializing LCD");
    lcd.begin();
    lcd.setRotation(0);     // Adjust Rotation of your screen (0-3)
    // TFTSetBrightness(5);
    Serial.println("Done.");

    Serial.println("Initializing Touch");
    touch.begin();
    // touch.setPins(SENSOR_RST, SENSOR_IRQ);
    
    // bool result = touch.begin(Wire, CST816_SLAVE_ADDRESS, SENSOR_SDA, SENSOR_SCL);
    // if (result == false) {
    //     while (1) {
    //         Serial.println("Failed to initialize CST series touch, please check the connection...");
    //         delay(1000);
    //     }
    // }
    // Serial.printf("Touch Model : %s\nDone.\n", touch.getModelName());

    Serial.println("Initializing WIFI");
    WiFiManager wifiManager;

//    wifiManager.resetSettings();
    lcd.fillScreen(TFT_BLACK);
    lcd.setCursor(25, 103, 2);
    // Set the font colour to be white with a black background, set text size multiplier to 1
    lcd.setTextColor(TFT_WHITE, TFT_BLACK);  
    lcd.setTextSize(2);
    lcd.printf("Setup WIFI first");
  
    bool connected = wifiManager.autoConnect("ESP32-S3-LCD", "12345678");
    if (connected) {
        Serial.println("Connected to WIFI");
    } else {
        Serial.println("Failed to connect to WIFI");
    }
    timeClient.begin();
    setenv("TZ", "EST5EDT,M3.2.0,M11.1.0", 1);
    tzset();
    timeClient.update();
    Serial.println("Done.");

    Serial.println("Initializing LVGL");
    lv_init();
    Serial.println("Done.");

    Serial.println("Initializing buffers");
    lv_color_t *buf1 = (lv_color_t *)heap_caps_malloc(TFT_HEIGHT * 150 * sizeof(lv_color_t), MALLOC_CAP_DMA | MALLOC_CAP_INTERNAL);
    lv_color_t *buf2 = (lv_color_t *)heap_caps_malloc(TFT_HEIGHT * 150 * sizeof(lv_color_t), MALLOC_CAP_DMA | MALLOC_CAP_INTERNAL);
    lv_disp_draw_buf_init(&draw_buf, buf1, buf2, TFT_HEIGHT * 150);
    Serial.println("Done...");

    // initialise the display
    Serial.println("Initializing Display");
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);

    // settings for display driver
    disp_drv.hor_res = TFT_WIDTH;
    disp_drv.ver_res = TFT_HEIGHT;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);
    Serial.println("Done...");

    Serial.println("Assigning touch driver");
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = my_touchpad_read;
    lv_indev_drv_register(&indev_drv);
    Serial.println("Done...");

    Serial.println("Creating UI");
    ui_init();
    Serial.println("Done...");

}

void loop() {
    lv_timer_handler(); /* let the GUI do its work */
    
    lv_label_set_text_fmt(objects.time_label, "%s", timeClient.getFormattedTime());
    uint16_t result = analogReadMilliVolts(1);;
    const float conversion_factor = 3.3f / (1 << 12) * 3;
    const uint16_t percent = (result * conversion_factor) / 3.75 * 100;
    lv_label_set_text_fmt(objects.battery_label, "%0.2f V", (result * conversion_factor));

    lv_ll_t *indicators = &((lv_meter_t *)objects.battery_meter)->indicator_ll;
    int index = 0;
    lv_meter_indicator_t *indicator;
    for (indicator = (lv_meter_indicator_t*) _lv_ll_get_tail(indicators); index > 0 && indicator != NULL; indicator = (lv_meter_indicator_t*) _lv_ll_get_prev(indicators, indicator), index--);
    lv_meter_set_indicator_value(objects.battery_meter, indicator, percent);

    ui_tick();

    if (eezEventIsAvailable)
    {
        eezEventIsAvailable = false;
        lv_obj_t *obj = lv_event_get_target(&eezEvent);
        lv_event_code_t code = lv_event_get_code(&eezEvent);
        //Serial.printf("Received event %d from %u\n", code, obj);
        if (code == LV_EVENT_VALUE_CHANGED) {
            bool checked = lv_obj_has_state(obj, LV_STATE_CHECKED);
            Serial.printf("Relay%d now turned %s\n", obj==objects.relay1 ? 1 : 2, checked ? "on" : "off");
            digitalWrite(obj == objects.relay1 ? 15 : 16, checked ? LOW : HIGH);
        }
    } 

    if (swipeObj != NULL) {
        Serial.printf("Swiping %s\n", swipeDir == LV_DIR_LEFT ? "left" : "right");
        if (swipeDir == LV_DIR_RIGHT) {
            if (swipeObj == objects.relays) {
                lv_scr_load(objects.main);
            } else if (swipeObj == objects.battery_page)  {
                lv_scr_load(objects.relays);
            }
        } else if (swipeDir == LV_DIR_LEFT) {
            if (swipeObj == objects.main) {
                lv_scr_load(objects.relays);
            } else if (swipeObj == objects.relays) {
                lv_scr_load(objects.battery_page);
            }
        }
        swipeObj = NULL;
        swipeDir = LV_DIR_NONE;
    }

    vTaskDelay(5 / portTICK_PERIOD_MS); 
}

