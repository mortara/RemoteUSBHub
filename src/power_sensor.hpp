#include <Arduino.h>
#include <INA3221.h>
#include <driver/adc.h>
#include "esp_adc_cal.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "I2C/i2cdevice.hpp"

struct INAMeasurement
{
    float Current = 0;
    float Busvoltage = 0;
    float Shuntvoltage = 0;
    float Loadvoltage = 0;
    float Power = 0;
};
class PowerSensor : i2cdevice
{
    private:
        INA3221   *INA;
        volatile uint8_t  deviceNumber    = UINT8_MAX;  ///< Device Number to use in example
        const uint32_t SERIAL_SPEED{115200};     ///< Use fast serial speed
        const uint32_t SHUNT_MICRO_OHM{100000};  ///< Shunt resistance in Micro-Ohm, e.g. 100000 is 0.1 Ohm
        const uint16_t MAXIMUM_AMPS{1};          ///< Max expected amps, clamped from 1A to a max of 1022A
        uint8_t        devicesFound{0};          ///< Number of INAs found
        unsigned long _lastRead;
        bool setupmqtt = false;
        bool _active = false;

        INAMeasurement _measurements[3];
        void measure(ina3221_ch_t ch, uint8_t pos);
        
        bool mqttSetup();
        float VBAT= 0; // battery voltage from ESP32 ADC read
    public:
        PowerSensor(uint8_t adr);
        void Loop();
        void DisplayInfo();
        void DisplayMeasurements(Adafruit_SSD1306 *screen);

        INAMeasurement GetMeasurement(uint8_t ch);
};