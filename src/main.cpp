#include <Arduino.h>
#include <init_disp.h>

LGFX tft;
void setup()
{
  tft.begin();
  tft.setRotation(1);
  tft.setTextColor(TFT_CYAN);
  tft.setTextSize(3);
  tft.setCursor(100, 100);
  tft.println("Hello Dev!");
}

void loop()
{
}
