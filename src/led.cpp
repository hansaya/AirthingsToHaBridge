#include "led.h"
#include <Ticker.h>
#include "user_config.h"

void Led::begin()
{
    pinMode(STATUS_LED, OUTPUT);
    digitalWrite(STATUS_LED, LOW);
}

void IRAM_ATTR Led::changeLedState(uint16_t blinkCount)
{
    if (blinkCount != 0)
    {
        static short count;
        if (count++ > blinkCount)
        {
            count = 0;
            m_lightTicker.detach();
            digitalWrite(STATUS_LED, LOW);
            return;
        }
    }
    digitalWrite(STATUS_LED, !(digitalRead(STATUS_LED)));
}

void Led::doubleFastBlink()
{
    digitalWrite(STATUS_LED, LOW);
    m_lightTicker.attach_ms<Led *>(
        200, [](Led *led) {
            led->changeLedState(2);
        },
        this);
}

void Led::blinkLed()
{
    m_lightTicker.attach_ms<Led *>(
        200, [](Led *led) {
            led->changeLedState();
        },
        this);
}

void Led::stopBlinkLed()
{
    digitalWrite(STATUS_LED, LOW);
    m_lightTicker.detach();
}

Led g_led;