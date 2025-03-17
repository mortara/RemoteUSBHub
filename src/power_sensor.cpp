#include "power_sensor.hpp"
#include "pmCommonLib.hpp"


PowerSensor::PowerSensor(uint8_t adr) : i2cdevice(adr)
{
    pmLogging.LogLn("Initializing power sensor");
    if(!isActive())
    {
        pmLogging.LogLn("Power sensor not found!");     
        return;   
    }
    INA = new  INA3221(INA3221_ADDR40_GND);
    INA->begin(&Wire);
    INA->reset();
    INA->setShuntRes(100, 100, 100);
    INA->setAveragingMode(INA3221_REG_CONF_AVG_16);
    
    _active = true;
    setupmqtt= false;
    pmLogging.LogLn("Power sensor runs!");  
}

bool PowerSensor::mqttSetup()
{
    if(setupmqtt)
        return false;

    pmLogging.LogLn("Setting up MQTT client for power sensor");

    if(!pmCommonLib.MQTTConnector.SetupSensor("CurrentCH1", "INA", "current", "mA", "mdi:current-dc"))
    {
        pmLogging.LogLn("Could not setup current sensor!");
        return false;
    }

    pmCommonLib.MQTTConnector.SetupSensor("BusVoltageCH1", "INA", "voltage", "V", "mdi:flash-triangle");
    pmCommonLib.MQTTConnector.SetupSensor("ShuntVoltageCH1", "INA", "voltage", "mV", "mdi:flash-triangle");
    pmCommonLib.MQTTConnector.SetupSensor("LoadVoltageCH1", "INA", "voltage", "V", "mdi:flash-triangle");
    pmCommonLib.MQTTConnector.SetupSensor("PowerCH1", "INA", "power", "W", "mdi:flash-triangle");

    pmCommonLib.MQTTConnector.SetupSensor("CurrentCH2",  "INA", "current", "mA", "mdi:current-dc");
    pmCommonLib.MQTTConnector.SetupSensor("BusVoltageCH2",  "INA", "voltage", "V", "mdi:flash-triangle");
    pmCommonLib.MQTTConnector.SetupSensor("ShuntVoltageCH2","INA", "voltage", "mV", "mdi:flash-triangle");
    pmCommonLib.MQTTConnector.SetupSensor("LoadVoltageCH2", "INA", "voltage", "V", "mdi:flash-triangle");
    pmCommonLib.MQTTConnector.SetupSensor("PowerCH2", "INA", "power", "W", "mdi:flash-triangle");

    pmCommonLib.MQTTConnector.SetupSensor("CurrentCH3", "INA", "current", "mA", "mdi:current-dc");
    pmCommonLib.MQTTConnector.SetupSensor("BusVoltageCH3", "INA", "voltage", "V", "mdi:flash-triangle");
    pmCommonLib.MQTTConnector.SetupSensor("ShuntVoltageCH3", "INA", "voltage", "mV", "mdi:flash-triangle");
    pmCommonLib.MQTTConnector.SetupSensor("LoadVoltageCH3",  "INA", "voltage", "V", "mdi:flash-triangle");
    pmCommonLib.MQTTConnector.SetupSensor("PowerCH3",  "INA", "power", "W", "mdi:flash-triangle");

    pmCommonLib.MQTTConnector.SetupSensor("BatteryPower", "INA", "voltage", "V", "mdi:flash-triangle");

    setupmqtt = true;

    pmLogging.LogLn("Power Sensor mqtt setup done!");
    return true;
}

void PowerSensor::DisplayInfo()
{
    if(!isActive())
    {
        pmLogging.LogLn("INA not active!");
        return;
    }
   
    pmLogging.LogLn("Current CH1= " + String(_measurements[0].Current) + "ma");
    pmLogging.LogLn("BusVoltage CH1= " + String(_measurements[0].Busvoltage) + " V");
    pmLogging.LogLn("ShuntVoltage CH1= " + String(_measurements[0].Shuntvoltage) + " mV");
    pmLogging.LogLn("Power CH1= " + String(_measurements[0].Power) + " W");

    pmLogging.LogLn("Current CH2= " + String(_measurements[1].Current) + "ma");
    pmLogging.LogLn("BusVoltage CH2= " + String(_measurements[1].Busvoltage) + " V");
    pmLogging.LogLn("ShuntVoltage CH2= " + String(_measurements[1].Shuntvoltage) + " mV");
    pmLogging.LogLn("Power CH2= " + String(_measurements[1].Power) + " W");

    pmLogging.LogLn("Current CH3= " + String(_measurements[2].Current) + "ma");
    pmLogging.LogLn("BusVoltage CH3= " + String(_measurements[2].Busvoltage) + " V");
    pmLogging.LogLn("ShuntVoltage CH3= " + String(_measurements[2].Shuntvoltage) + " mV");
    pmLogging.LogLn("Power CH3= " + String(_measurements[2].Power) + " W");

    pmLogging.LogLn("Battery = " + String(VBAT) + "v");

}

INAMeasurement PowerSensor::GetMeasurement(uint8_t ch)
{
    return _measurements[ch];
}

void PowerSensor::measure(ina3221_ch_t ch, uint8_t pos)
{
    _measurements[pos].Current = INA->getCurrent(ch) * 1000;
    _measurements[pos].Busvoltage = INA->getVoltage(ch) ;
    _measurements[pos].Shuntvoltage = (float)INA->getShuntVoltage(ch) / 1000.0 / 1000.0;
    _measurements[pos].Loadvoltage = _measurements[pos].Busvoltage + (_measurements[pos].Shuntvoltage);
    _measurements[pos].Power = _measurements[pos].Busvoltage * _measurements[pos].Current / 1000.0;
}

void PowerSensor::DisplayMeasurements(Adafruit_SSD1306 *screen)
{
    
    int cw = 55;
    int rh = 11;

    int x = 0;
    int y = 0;

    INAMeasurement ch1 = GetMeasurement(0);
    INAMeasurement ch2 = GetMeasurement(1);
    INAMeasurement ch3 = GetMeasurement(2);

    /*screen->drawString("CH1", x + 50, y);
    screen->drawString("CH2", x + 50 + cw, y);
    screen->drawString("CH3", x + 50 + 2 *cw, y);

    screen->drawString("Voltage: ", x, y + rh);
    screen->drawString(String(ch1.Busvoltage,1) +  "V", 50 + x, y + rh);
    screen->drawString(String(ch2.Busvoltage,1) +  "V", 50 + x + cw, y + rh);
    screen->drawString(String(ch3.Busvoltage,1) +  "V", 50 + x + 2 *cw, y + rh);

    screen->drawString("Current: ", x, y + 2*rh);
    screen->drawString(String(ch1.Current,1) +  "mA", 50 + x, y + 2*rh);
    screen->drawString(String(ch2.Current,1) +  "mA", 50 + x + cw, y + 2*rh);
    screen->drawString(String(ch3.Current,1) +  "mA", 50 + x + 2 *cw, y + 2*rh);

    screen->drawString("Power: ", x, y + 3*rh);
    screen->drawString(String(ch1.Power,2) +  "W", 50 + x, y + 3*rh);
    screen->drawString(String(ch2.Power,2) +  "W", 50 + x + cw, y + 3*rh);
    screen->drawString(String(ch3.Power,2) +  "W", 50 + x + 2 *cw, y + 3*rh);*/

}

void PowerSensor::Loop() {

    if(!_active)
        return;

    unsigned long now = millis();
    if(now - _lastRead < 5000)
        return;

    _lastRead = now;

    if(!setupmqtt && pmCommonLib.MQTTConnector.isActive())
        mqttSetup();

    measure(INA3221_CH1,0);
    measure(INA3221_CH2,1);
    measure(INA3221_CH3,2);

    //DisplayInfo();

    if(setupmqtt)
    {
        JsonDocument payload;
        payload["CurrentCH1"] = String(_measurements[0].Current);
        payload["BusVoltageCH1"] = String(_measurements[0].Busvoltage);
        payload["ShuntVoltageCH1"] = String(_measurements[0].Shuntvoltage);
        payload["LoadVoltageCH1"] = String(_measurements[0].Loadvoltage);
        payload["PowerCH1"] = String(_measurements[0].Power);

        payload["CurrentCH2"] = String(_measurements[1].Current);
        payload["BusVoltageCH2"] = String(_measurements[1].Busvoltage);
        payload["ShuntVoltageCH2"] = String(_measurements[1].Shuntvoltage);
        payload["LoadVoltageCH2"] = String(_measurements[1].Loadvoltage);
        payload["PowerCH2"] = String(_measurements[1].Power);

        payload["CurrentCH3"] = String(_measurements[2].Current);
        payload["BusVoltageCH3"] = String(_measurements[2].Busvoltage);
        payload["ShuntVoltageCH3"] = String(_measurements[2].Shuntvoltage);
        payload["LoadVoltageCH3"] = String(_measurements[2].Loadvoltage);
        payload["PowerCH3"] = String(_measurements[2].Power);
        payload["BatteryPower"] = String(VBAT);

        pmCommonLib.MQTTConnector.PublishMessage(payload, "INA");
    }
}