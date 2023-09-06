#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
// https://randomnerdtutorials.com/guide-for-oled-display-with-arduino/
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "EmonLib.h"
// https://simplyexplained.com/blog/Home-Energy-Monitor-ESP32-CT-Sensor-Emonlib/?ref=localhost
// https://olimex.wordpress.com/2015/09/29/energy-monitoring-with-arduino-and-current-clamp-sensor/
// https://www.engineersgarage.com/non-invasive-current-sensor-with-arduino/

#define analogPin A0 /* ESP8266 Analog Pin ADC0 = A0 */

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Force EmonLib to use 10bit ADC resolution
#define ADC_BITS    10
#define ADC_COUNTS  (1<<ADC_BITS)

// ADC_MODE(ADC_TOUT);

EnergyMonitor emon1;
double Irms, thePower;

void testdrawchar(void) {
  display.clearDisplay();

  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font

  // Not all the characters will fit on the display. This is normal.
  // Library will draw what it can and the rest will be clipped.
  for(int16_t i=0; i<256; i++) {
    if(i == '\n') display.write(' ');
    else          display.write(i);
  }

  display.display();
  delay(2000);
}

void testdrawstyles(void) {
  display.clearDisplay();

  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  display.println(F("Hello, world!"));

  display.setTextColor(BLACK, WHITE); // Draw 'inverse' text
  display.println(3.141592);

  display.setTextSize(2);             // Draw 2X-scale text
  display.setTextColor(WHITE);
  display.print(F("0x")); display.println(0xDEADBEEF, HEX);

  display.display();
  delay(2000);
}

void setup()
{
  Serial.begin(115200); /* Initialize serial communication at 115200 */

  // emon1.current(analogPin, 111.1);  //pin for current measurement, calibration value
  emon1.current(analogPin, 60.6);  //pin for current measurement, calibration value

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

    // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds

  //   Clear the buffer
  display.clearDisplay();

  // Draw a single pixel in white
//   display.drawPixel(10, 10, WHITE);

  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
  display.display();
  delay(2000);
  //   display.display() is NOT necessary after every single drawing command,
  //   unless that's what you want...rather, you can batch up a bunch of
  //   drawing operations and then update the screen all at once by calling
  //   display.display(). These examples demonstrate both approaches...

//   testdrawchar();      // Draw characters of the default font

  testdrawstyles();    // Draw 'stylized' characters
}

void loop()
{
  // Irms = emon1.calcIrms(1480); // measure current
  Irms = emon1.calcIrms(1484); // measure current
  thePower = Irms*230.0;   // we assume voltage is 230VAC if you add transformer to
 
  /* Print the output in the Serial Monitor */
  Serial.print("Power: ");
  Serial.println(thePower);
  Serial.print("Current: ");
  Serial.println(Irms);
  // Serial.print("ADC: ");
  // Serial.println(analogRead(analogPin));
  
  display.clearDisplay();

  display.setTextSize(2);             // Normal 1:1 pixel scale
  display.setTextColor(WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  display.println(F("Power: "));
  display.print(thePower);
  display.println(F("?"));
  display.println(F("Current: "));
  display.print(Irms);
  display.println(F("?A"));
  // display.print(F("ADC: "));
  // display.println(analogRead(analogPin));
  display.display();

  delay(300);
}