#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1    // Reset pin # (or -1 if sharing Arduino reset pin)

#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32#include

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


int oled_period = 2000;
unsigned long oled_time_now = 0;

void oledPrint(String msg, String label)
{
  oled_time_now = millis();

  while (millis() < oled_time_now + oled_period)
  {
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    if (label != "")
    {
      display.print(label);
      display.print(": ");
    }
    display.print(msg);
    display.print(" ");
    display.display();
  }

}

void oledSetup()
{

  // Initialising the UI will init the display too.
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
  {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }
  oledPrint("Waiting...", "");
}
