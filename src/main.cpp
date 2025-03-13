#include <Arduino.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include <Wire.h>
#include <lvgl.h>
#include "WiFiManager.h"
//#include "TouchDrvCSTXXX.hpp"
#include "CST816S.h"
#include "ui/ui.h"
#include "ui/actions.h"
#include "ui/screens.h"

#define SENSOR_SDA 6
#define SENSOR_SCL 7
#define SENSOR_RST 13
#define SENSOR_IRQ 5

TFT_eSPI lcd = TFT_eSPI();
TFT_eSprite sprite = TFT_eSprite(&lcd);
TFT_eSprite sprite2 = TFT_eSprite(&lcd);
TFT_eSprite backSprite = TFT_eSprite(&lcd);

CST816S touch(6, 7, 13, 5);	// sda, scl, rst, irq

//TouchDrvCSTXXX touch;
static lv_disp_draw_buf_t draw_buf;

// EEZ Studio events
extern lv_event_t eezEvent;
extern bool eezEventIsAvailable;

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

    /**
     Serial.println("Initializing WIFI");
    WiFiManager wifiManager;
    bool connected = wifiManager.autoConnect("ESP32-S3-LCD", "12345678");
    if (connected) {
        Serial.println("Connected to WIFI");
    } else {
        Serial.println("Failed to connect to WIFI");
    }
    Serial.println("Done.");
    */

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
    ui_tick();

    if (eezEventIsAvailable)
    {
        eezEventIsAvailable = false;
        lv_obj_t *obj = lv_event_get_target(&eezEvent);
//        Serial.printf("Received event from %u\n", obj);
        if (obj == objects.button1)
        {
            Serial.println("button1 pressed");
        }
    }

    vTaskDelay(5 / portTICK_PERIOD_MS); 
}

