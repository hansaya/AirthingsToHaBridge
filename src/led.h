#ifndef led_h
#define led_h

#include <Arduino.h>
#include <Ticker.h>

/**
 * Class Led handles led outputs.
 */
class Led
{
public:
    /**
     * Execute anything that belong in setup ().
     */
    void begin();
    /**
     * Double blink the led once.
     */
    void doubleFastBlink();
    /**
     * Blink the led continuesly.
     */
    void blinkLed();
    /**
     * Stop the led blinking.
     */
    void stopBlinkLed();

private:
    /**
     * Change the led status.
     */
    void IRAM_ATTR changeLedState(uint16_t blinkCount = 0);

    Ticker m_lightTicker;
};

extern Led g_led;

#endif