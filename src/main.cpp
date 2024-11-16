
#include <Arduino.h>
#include <init_disp.h>
#include <lvgl.h>

LGFX tft;
const int SCREEN_WIDTH = 480;
const int SCREEN_HEIGHT = 320;
unsigned long lastTickMillis = 0;


  
void my_touch_read(lv_indev_t *indev_driver,lv_indev_data_t *data)
{
  uint16_t touchX, touchY;
  
  bool touch = tft.getTouch(&touchX, &touchY);
    
  if(!touch){
    data->state = LV_INDEV_STATE_RELEASED;
  }
  else{   
    data->point.x = touchX;
    data->point.y = touchY;
    data->state = LV_INDEV_STATE_PRESSED;
  } 
}


void my_disp_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *color_p)
{
  //uint16_t *color_array_16 = (uint16_t *)color_p;
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);
  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, w, h);
  tft.writePixels((lgfx::rgb565_t *)color_p, w * h);
  tft.endWrite();
  lv_disp_flush_ready(disp);
}

static void event_handler(lv_event_t * e)
{
   static lv_obj_t * btn;
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * btn1 = lv_button_create(btn);
    if(code == LV_EVENT_CLICKED) {
        static uint8_t cnt = 0;
        cnt++;

        /*Get the first child of the button which is the label and change its text*/
        lv_obj_t * label = lv_obj_get_child(btn1, 0);
        lv_label_set_text_fmt(label, "Button: %d", cnt);
    }
}

void setupLVGL(){
  lv_init();
  
  static lv_color_t buf[SCREEN_WIDTH * 10];
  lv_display_t *display = lv_display_create(SCREEN_WIDTH, SCREEN_HEIGHT);
  /*Declare a buffer for 1/10 screen size*/
  #define BYTE_PER_PIXEL (LV_COLOR_FORMAT_GET_SIZE(LV_COLOR_FORMAT_RGB565)) /*will be 2 for RGB565 */
  static uint8_t buf1[SCREEN_WIDTH * SCREEN_HEIGHT / 10 * BYTE_PER_PIXEL];

  lv_display_set_buffers(display, buf1, NULL, sizeof(buf1), LV_DISPLAY_RENDER_MODE_PARTIAL);  /*Initialize the display buffer.*/
  lv_display_set_flush_cb(display,my_disp_flush);
  
  lv_indev_t * indev = lv_indev_create();           /*Create an input device*/
  lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);  /*Touch pad is a pointer-like device*/
  lv_indev_set_read_cb(indev, my_touch_read);

    

 /*Create a list*/
    static lv_obj_t * list1;
    list1 = lv_list_create(lv_screen_active());
    lv_obj_set_size(list1, 300, 260);
    lv_obj_center(list1);

    /*Add buttons to the list*/
    lv_obj_t * btn;
    lv_list_add_text(list1, "File");
    btn = lv_list_add_button(list1, LV_SYMBOL_FILE, "New");
    lv_obj_add_event_cb(btn, event_handler, LV_EVENT_CLICKED, NULL);
    btn = lv_list_add_button(list1, LV_SYMBOL_DIRECTORY, "Open");
    lv_obj_add_event_cb(btn, event_handler, LV_EVENT_CLICKED, NULL);
    btn = lv_list_add_button(list1, LV_SYMBOL_SAVE, "Save");
    lv_obj_add_event_cb(btn, event_handler, LV_EVENT_CLICKED, NULL);
    btn = lv_list_add_button(list1, LV_SYMBOL_CLOSE, "Delete");
    lv_obj_add_event_cb(btn, event_handler, LV_EVENT_CLICKED, NULL);
    btn = lv_list_add_button(list1, LV_SYMBOL_EDIT, "Edit");
    lv_obj_add_event_cb(btn, event_handler, LV_EVENT_CLICKED, NULL);

    lv_list_add_text(list1, "Connectivity");
    btn = lv_list_add_button(list1, LV_SYMBOL_BLUETOOTH, "Bluetooth");
    lv_obj_add_event_cb(btn, event_handler, LV_EVENT_CLICKED, NULL);
    btn = lv_list_add_button(list1, LV_SYMBOL_GPS, "Navigation");
    lv_obj_add_event_cb(btn, event_handler, LV_EVENT_CLICKED, NULL);
    btn = lv_list_add_button(list1, LV_SYMBOL_USB, "USB");
    lv_obj_add_event_cb(btn, event_handler, LV_EVENT_CLICKED, NULL);
    btn = lv_list_add_button(list1, LV_SYMBOL_BATTERY_FULL, "Battery");
    lv_obj_add_event_cb(btn, event_handler, LV_EVENT_CLICKED, NULL);

    lv_list_add_text(list1, "Exit");
    btn = lv_list_add_button(list1, LV_SYMBOL_OK, "Apply");
    lv_obj_add_event_cb(btn, event_handler, LV_EVENT_CLICKED, NULL);
    btn = lv_list_add_button(list1, LV_SYMBOL_CLOSE, "Close");
    lv_obj_add_event_cb(btn, event_handler, LV_EVENT_CLICKED, NULL);
    
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  tft.begin();
  tft.setRotation(1);
  
  tft.setBrightness(200);
  
  setupLVGL();

}

void loop() {
  //uint16_t touchX,touchY;
  // put your main code here, to run repeatedly:
  unsigned int tickPeriod = millis() - lastTickMillis;
  lv_tick_inc(tickPeriod);
  lastTickMillis = millis();
  lv_timer_handler();
  
}