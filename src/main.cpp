#include <Arduino.h>	
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include "pmCommonLib.hpp"
#include "webserver.hpp"
#include "power_sensor.hpp"
#include "BIG7USBHub.hpp"


PowerSensor *_power1;
BIG7Hub _hub;

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     0 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 *_display;
bool _displaypresent = false;


void mqtt_callback(char* topic, byte* payload, unsigned int length) {
    
  String msg;
  for (byte i = 0; i < length; i++) {
      char tmp = char(payload[i]);
      msg += tmp;
  }
  pmLogging.LogLn(msg);
}

void setup() 
{
  setCpuFrequencyMhz(80);

  Serial.begin(115200);
  _hub.begin();

  Wire.begin(5,6);

  pmCommonLib.Setup();

  _display = new Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

  _power1 = new PowerSensor(0x40);

  pmCommonLib.MQTTConnector.ConfigureDevice("RemoteuSBHub", "Patrick Mortara", "MKI");
 
  if(!_display->begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    pmLogging.LogLn(F("SSD1306 allocation failed"));
  }
  else
  {
    pmLogging.LogLn(F("SSD1306 allocation succesfull"));

    _display->setTextSize(1);      // Normal 1:1 pixel scale
    _display->setTextColor(WHITE, BLACK); // Draw white text
    _display->cp437(true);         // Use full 256 char 'Code Page 437' font
    _displaypresent = true;

    _display->clearDisplay();
    _display->setCursor(0, 0);
    _display->write("BIG7 USB Hub");
    _display->display();
  }
}


void loop() {
  
  pmCommonLib.Loop();
  
  _power1->Loop();

  INAMeasurement measure = _power1->GetMeasurement(2);
  _hub.loop();

  char ch = ' ';
  if (Serial.available())
      ch = Serial.read();

  switch(ch)
  {
    case '1':
      _hub.turnOn();
      break;
    case '0':
      _hub.turnOff();
      break;
    case 'i':
      _power1->DisplayInfo();
      break;
  }

  if(_displaypresent)
  {
    _display->clearDisplay();
    _hub.DisplayInfo(_display);
    _display->display();
  }

  delay(100);
}
