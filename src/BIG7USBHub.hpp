#include "Arduino.h"
#include "Webserial/webserial.hpp"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

class BIG7Hub
{
    private:
        uint8_t _enablePIN = 14;
        uint8_t _led1PIN = 27;
        uint8_t _led2PIN = 26;
        uint8_t _led3PIN = 25;
        uint8_t _led4PIN = 0;
        uint8_t _led5PIN = 0;
        uint8_t _led6PIN = 0;
        uint8_t _led7PIN = 0;
        uint8_t _powerledPIN= 0;

        bool _hubPower = false;

    public:
        void begin();
        
        void turnOn();
        void turnOff();
        uint8_t getLEDState(uint8_t led);
        void DisplayInfo(Adafruit_SSD1306 *screen);
        void loop();

};