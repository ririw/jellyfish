#include "Pattern.h"

void Pattern::enter(CRGB leds[NUM_STRIPS][NUM_LEDS]) {
    for (int strip = 0; strip < NUM_STRIPS; strip++) {
        for (int led = 0; led < NUM_LEDS; led++) {
            leds[strip][led] = CRGB(0, 0, 0);
        }
    }
}

SimplePattern::SimplePattern(Channel channel) {
    this->brightness = 0;
    this->channel = channel;
}

void SimplePattern::show(CRGB leds[NUM_STRIPS][NUM_LEDS]) {
    for (int strip = 0; strip < NUM_STRIPS; strip++) {
        for (int led = 0; strip < NUM_STRIPS; strip++) {
            leds[strip][led].r = channel == rchannel ? brightness : (uint8_t)0;
            leds[strip][led].g = channel == gchannel ? brightness : (uint8_t)0;
            leds[strip][led].b = channel == bchannel ? brightness : (uint8_t)0;
        }
    }
    this->brightness += 10;
}

MultiCyclon::MultiCyclon() {
    head = 0;
    go_upwards = true;
}

void MultiCyclon::show(CRGB leds[NUM_STRIPS][NUM_LEDS]) {
    if (head == NUM_LEDS) go_upwards = false;
    if (head == 0) go_upwards = true;
    head += go_upwards ? 1 : -1;

    for (int strip = 0; strip < NUM_STRIPS; strip++) {
        fadeall(leds[strip]);
        leds[strip][head] = CHSV(255, 255, 255);
    }
}

void MultiCyclon::fadeall(CRGB leds[NUM_LEDS]) {
    for(int i = 0; i < NUM_LEDS; i++) {
        leds[i].nscale8(250);
    }
}

JellyFish::JellyFish() { }
void JellyFish::show(CRGB leds[NUM_STRIPS][NUM_LEDS]) {
    NULL;
}

