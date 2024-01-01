
#include <Arduino.h>
#include <init_disp.h>
#include <lvgl.h>

LGFX tft;

#define screenWidth 480
#define screenHeight 320

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[screenWidth * 10];

// headers
void setupLvgl();
static void drawUI();
void my_touch_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data);
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);
static void handleClick(lv_event_t *event);

void setup()
{
  Serial.begin(115200);

  tft.begin();
  tft.setRotation(1);
  tft.setBrightness(255);

  setupLvgl();
  drawUI();
}

void loop()
{
  lv_timer_handler();
  delay(5);
}

void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);
  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, w, h);
  tft.writePixels((lgfx::rgb565_t *)&color_p->full, w * h);
  tft.endWrite();
  lv_disp_flush_ready(disp);
}

void my_touch_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{
  uint16_t touchX, touchY;
  bool touched = tft.getTouch(&touchX, &touchY);
  if (!touched)
  {
    data->state = LV_INDEV_STATE_REL;
  }
  else
  {
    data->state = LV_INDEV_STATE_PR;
    data->point.x = touchX;
    data->point.y = touchY;
  }
}

void setupLvgl()
{
  lv_init();
  lv_disp_draw_buf_init(&draw_buf, buf, NULL, screenWidth * 10);
  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  disp_drv.hor_res = screenWidth;
  disp_drv.ver_res = screenHeight;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);
  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = my_touch_read;
  lv_indev_drv_register(&indev_drv);
}

static void drawUI()
{
  lv_obj_t *main_screen, *btn, *label;
  lv_style_t red;
  lv_style_init(&red);
  lv_style_set_bg_color(&red, LV_COLOR_MAKE(255, 0, 0));
  main_screen = lv_scr_act();
  btn = lv_btn_create(main_screen);
  label = lv_label_create(btn);
  lv_obj_center(btn);
  lv_obj_add_style(btn, &red, LV_STATE_DEFAULT);
  lv_label_set_text(label, "my button");
  lv_obj_add_event_cb(btn,handleClick,LV_EVENT_ALL,NULL);
}

static void handleClick(lv_event_t *event)
{
  
  lv_event_code_t code = lv_event_get_code(event);
  lv_obj_t *btn = lv_event_get_current_target(event);
  if (code == LV_EVENT_CLICKED)
  {
    lv_obj_t *label = lv_obj_get_child(btn,0);
    
    lv_label_set_text(label, "Clicked!!!");
    
  }
}