#include "BIG7USBHub.hpp"


void BIG7Hub::begin()
{
    pinMode(_enablePIN, OUTPUT);
    pinMode(_led1PIN, INPUT_PULLDOWN);
    pinMode(_led2PIN, INPUT_PULLDOWN);
    pinMode(_led3PIN, INPUT_PULLDOWN);
    pinMode(_led4PIN, INPUT_PULLDOWN);
    pinMode(_led5PIN, INPUT_PULLDOWN);
    pinMode(_led6PIN, INPUT_PULLDOWN);
    pinMode(_led7PIN, INPUT_PULLDOWN);
    pinMode(_powerledPIN, INPUT_PULLDOWN);

    turnOn();
    
}

void BIG7Hub::turnOn()
{
    WebSerialLogger.println("BIG7Hub turnOn");
    digitalWrite(_enablePIN, HIGH);
    _hubPower = true;
}

void BIG7Hub::turnOff()
{
    WebSerialLogger.println("BIG7Hub turnOff");
    digitalWrite(_enablePIN, LOW);
    _hubPower = false;
}

uint8_t BIG7Hub::getLEDState(uint8_t led)
{
    switch (led)
    {
    case 1:
        return digitalRead(_led1PIN);
        break;
    case 2:
        return digitalRead(_led2PIN);
        break;
    case 3:
        return digitalRead(_led3PIN);
        break;
    case 4:
        return digitalRead(_led4PIN);
        break;
    case 5:
        return digitalRead(_led5PIN);
        break;
    case 6:
        return digitalRead(_led6PIN);
        break;
    case 7:
        return digitalRead(_led7PIN);
        break;
    case 8:
        return digitalRead(_powerledPIN);
        break;
    default:
        return 0;
        break;
    }
}

void BIG7Hub::DisplayInfo(Adafruit_SSD1306 *screen)
{
    for(int i = 0; i < 7; i++)
    {
        screen->setCursor(0, i*8);
        screen->print("LED ");
        screen->print(i+1);
        screen->print(": ");
        screen->print(getLEDState(i+1));

        if(_hubPower)
        {
            screen->fillRect(120, 0, 8, 8, WHITE);
        }
        else
        {
            screen->drawRect(120, 0, 8, 8, WHITE);
        }

    }

    if(_hubPower)
    {
        screen->fillRect(120, 0, 8, 8, WHITE);
    }
    else
    {
        screen->drawRect(120, 0, 8, 8, WHITE);
    }
}

void BIG7Hub::loop()
{
    //WebSerialLogger.println("BIG7Hub loop");
}