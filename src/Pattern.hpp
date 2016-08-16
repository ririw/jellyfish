#pragma once
#include "FastLED.h"
#include "config.h"
#include <stdint.h>

class Pattern {
public:
    virtual void show(CRGB leds[NUM_STRIPS][NUM_LEDS]);
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
    String _name;
public:
    String name();
    MultiCyclon(const TProgmemRGBPalette16& pal, String name);
    void show(CRGB leds[NUM_STRIPS][NUM_LEDS]);
};
/*
class JellyFish: public Pattern {
private:
    int wave_location;
    uint8_t wave_hue;
public:
    JellyFish();
    void show(CRGB leds[NUM_STRIPS][NUM_LEDS]);
};
 */