#pragma once
#if NATIVE_BUILD == 1
#include "TestLED.hpp"
#else
#include "FastLED.h"
#include "Arduino.h"
#endif

#include "config.h"
#include <stdint.h>

class Pattern {
protected:
    String _name;
public:
    virtual void show(CRGB leds[NUM_STRIPS][NUM_LEDS]) = 0;
    virtual void enter(CRGB leds[NUM_STRIPS][NUM_LEDS]);
    virtual String name();
};

enum Channel { rchannel, gchannel, bchannel } ;
/**
 * A very simple pattern. It just sets
 * the color to either R, G or B, and then
 * cycles through brightness for that color.
 */
class SimplePattern: public Pattern {
private:
    int brightness;
    int increment;
    Channel channel;
public:
    SimplePattern(Channel chan);
    void show(CRGB leds[NUM_STRIPS][NUM_LEDS]);
    String name();
};

/**
 * This pattern does the cylon pattern
 * back and forth. Pretty simple stuff.
 */
class MultiCyclon: public Pattern {
private:
    int head;
    bool go_upwards;
    bool hue_upwards;
    uint8_t hue;
    CHSVPalette16 pallet;
    void fadeall(CRGB leds[NUM_LEDS]);
public:
    MultiCyclon(const TProgmemRGBPalette16& pal, String name);
    void show(CRGB leds[NUM_STRIPS][NUM_LEDS]);
};


class JellyFish: public Pattern {
private:
    int wave_center;
    uint8_t wave_hue;
    CHSVPalette16 pallet;
    uint8_t sparks[NUM_LEDS];
public:
    JellyFish(const TProgmemRGBPalette16& pal, String name);
    void show(CRGB leds[NUM_STRIPS][NUM_LEDS]);
};

class Spiral: public Pattern {
private:
    uint8_t spiral_loc;
    CHSVPalette16 pallet;
    bool innerloop;
    bool up;
public:
    Spiral (bool _up) {
        spiral_loc = 0;
        _name = String("upspiral");
        innerloop = true;
        up = _up;
    }
    void show(CRGB leds[NUM_STRIPS][NUM_LEDS]);
};

class Juggle: public Pattern {
public:
    Juggle() {
        _name = String("Juggle");
    }
    void show(CRGB leds[NUM_STRIPS][NUM_LEDS]) {
        // eight colored dots, weaving in and out of sync with each other
        for (int strip = 0; strip < NUM_STRIPS; strip++) {
            fadeToBlackBy(leds[strip], NUM_LEDS, 20);
            byte dothue = 0;
            for (int i = 0; i < 8; i++) {
                leds[strip][beatsin16(static_cast<uint8_t>(i + 7), 0, NUM_LEDS)] |= CHSV(dothue, 200, 255);
                dothue += 32;
            }
        }
    }
};