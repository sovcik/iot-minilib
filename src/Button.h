#ifndef __BUTTON_H__
#define __BUTTON_H__

#include <Arduino.h>
#include "Timer.h"
#include <Looper.h>

#define BUTTON_TIMER_UP_TIMEOUT     1000 // time in ms after which pushes will not be considered as subsequent
#define BUTTON_TIMER_LONGPUSH       5000 // max time in ms for longpush

class Button : public Looper {
    protected:
        Timer upTimer = Timer(BUTTON_TIMER_UP_TIMEOUT); 
        Timer longTimer = Timer(BUTTON_TIMER_LONGPUSH);
        int pushCount;
        bool btnDown = false;
        char* id;
        char* label;
        bool enabled = true;   // enable/disable
        
    public:
        Button(const char* id, bool enabled = true);
        virtual ~Button();
        virtual const char* getId();
        virtual const char* getLabel();
        virtual bool isEnabled() const;
        virtual void disable();
        virtual void enable();
        virtual void setStatus(bool status);

        /**
         * Returns true is button is pressed 
         */
        virtual bool isPressed() const =0;

        /**
         * Detect if button has been pushed anytime before calling this method.
         * 
         * @clear - reset wasPushed status
         */
        virtual bool wasPushed(bool clear = false);

        /**
         * Returns time in ms since button has been pushed down.
         * Useful for detecting "long-push"
         * Returns zero if release.
         */
        virtual unsigned long getPushDuration() const;


        virtual int getPushCount() const;

        /**
         * Check button status - non-blocking
         */
        void loop() override;
        virtual void setLabel(const char* label);
};

/**
 * Button connected to an analog pin.
 * Analog signal level threshold can be set in constructor.
 */
class AnalogButton : public Button {
    protected:
        int pin;
        int threshold; // value above which button is considered as "pushed down"

    public:
        /**
         * Button connected to an analog pin.
         * 
         * @id - button id/name
         * @pin - analog pin
         * @threshold - analog level threshold - exceeding this threshold = button is pushed
         */
        AnalogButton(const char* id, int pin, int threshold=500) : Button(id) {
            this->pin = pin;
            this->threshold = threshold;
            pinMode(pin, INPUT);
        }
        virtual bool isPressed() const override {return analogRead(pin) > threshold;};
        
};

class DigitalButton : public Button {
    protected:
        int pin;

    public:
        /**
         * Button connected to a digital pin
         * 
         * @id - button id/name
         * @pin - digital pin
         */
        DigitalButton(const char* id, int pin) : Button(id) {
            this->pin = pin;
            pinMode(pin, INPUT);
        }
        virtual bool isPressed() const override {return digitalRead(pin);};
        
};

#ifdef ESP32

/**
 * Button connected to a pin capable of reading capacitance.
 * So far available for ESP32 only
 */
class TouchButton : public Button {
    protected:
        int pin;
        int threshold; // value BELOW which button is considered as "pushed down"

    public:
        /**
         * Button connected to a pin capable of reading capacitance.
         * So far available for ESP32 only.
         * 
         * @id - button id/name
         * @pin - touch pin
         * @threshold - level below this threshold means button is pushed
         */
        TouchButton(const char* id, int pin, int threshold=20) : Button(id) {
            this->pin = pin;
            this->threshold = threshold;
            pinMode(pin, INPUT);
        }

        virtual bool isPressed() const override {return touchRead(pin) < threshold;};
        
};

#endif

#endif

