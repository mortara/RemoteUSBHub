#include "Arduino.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

class BIG7Hub
{
    private:
        uint8_t _enablePIN = 15;
        uint8_t _led1PIN = 18;
        uint8_t _led2PIN = 19;
        uint8_t _led3PIN = 20;
        uint8_t _led4PIN = 21;
        uint8_t _led5PIN = 22;
        uint8_t _led6PIN = 23;
        uint8_t _led7PIN = 10;
        uint8_t _powerledPIN= 11;

        bool _hubPower = false;

    public:
        void begin();
        
        void turnOn();
        void turnOff();
        uint8_t getLEDState(uint8_t led);
        void DisplayInfo(Adafruit_SSD1306 *screen);
        void loop();

};