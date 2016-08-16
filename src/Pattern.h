#pragma once
#include "FastLED.h"
#include "config.h"
#include <stdint.h>

class Pattern {
public:
    virtual void show(CRGB leds[NUM_STRIPS][NUM_LEDS]);
    virtual void enter(CRGB leds[NUM_STRIPS][NUM_LEDS]);
};


enum Channel { rchannel, gchannel, bchannel } ;
/**
 * A very simple pattern. It just sets
 * the color to either R, G or B, and then
 * cycles through brightness for that color.
 */
class SimplePattern: public Pattern {
private:
    uint8_t brightness;
    Channel channel;
public:
    SimplePattern(Channel chan);
    void show(CRGB leds[NUM_STRIPS][NUM_LEDS]);
};

/**
 * This pattern does the cylon pattern
 * back and forth. Pretty simple stuff.
 */
class MultiCyclon: public Pattern {
private:
    int head;
    bool go_upwards;
    void fadeall(CRGB leds[NUM_LEDS]);
public:
    MultiCyclon();
    void show(CRGB leds[NUM_STRIPS][NUM_LEDS]);
};

class JellyFish: public Pattern {
private:
public:
    JellyFish();
    void show(CRGB leds[NUM_STRIPS][NUM_LEDS]);
};