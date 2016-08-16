#include "Pattern.hpp"

void Pattern::enter(CRGB leds[NUM_STRIPS][NUM_LEDS]) {
    for (int strip = 0; strip < NUM_STRIPS; strip++) {
        for (int led = 0; led < NUM_LEDS; led++) {
            leds[strip][led] = CRGB(0, 0, 0);
        }
    }
}

SimplePattern::SimplePattern(Channel channel) {
    this->brightness = 0;
    this->increment = 1;
    this->channel = channel;
    LEDS.setBrightness(255);
}

void SimplePattern::show(CRGB leds[NUM_STRIPS][NUM_LEDS]) {
    for (int strip = 0; strip < NUM_STRIPS; strip++) {
        for (int led = 0; led < NUM_LEDS; led++) {
            leds[strip][led].r = (uint8_t)(channel == rchannel ? brightness : 0);
            leds[strip][led].g = (uint8_t)(channel == gchannel ? brightness : 0);
            leds[strip][led].b = (uint8_t)(channel == bchannel ? brightness : 0);
        }
    }
    this->brightness += this->increment;
    if (this->brightness > 48) {
        this->brightness = 48;
        this->increment *= -1;
    } else if (this->brightness < 0) {
        this->brightness = 0;
        this->increment *= -1;
    }
}

String SimplePattern::name() {
    if (channel == rchannel) String("Simple: red");
    if (channel == bchannel) String("Simple: blue");
    if (channel == gchannel) String("Simple: green");
    return String("Unreachable");
}


MultiCyclon::MultiCyclon(const TProgmemRGBPalette16& pal, String name) {
    head = 0;
    hue = 0;
    go_upwards = true;
    hue_upwards = true;
    pallet = CHSVPalette16(pal);
    _name = String("MultiCylon - ").concat(name);
}

void MultiCyclon::show(CRGB leds[NUM_STRIPS][NUM_LEDS]) {
    if (head >= NUM_LEDS-1) go_upwards = false;
    if (head <= 0) go_upwards = true;
    if (hue == 255) hue_upwards = false;
    if (hue == 0) hue_upwards = true;
    head += go_upwards ? 1 : -1;

    for (int strip = 0; strip < NUM_STRIPS; strip++) {
        fadeall(leds[strip]);
        leds[strip][head] = ColorFromPalette(pallet, hue + (uint8_t)(strip * 10));
    }
    hue += hue_upwards ? 1 : -1;
}

void MultiCyclon::fadeall(CRGB leds[NUM_LEDS]) {
    for(int i = 0; i < NUM_LEDS; i++) {
        leds[i].nscale8(252);
    }
}

String MultiCyclon::name() {
    return _name;
}

/*
// The jellyfish sends gentle sine-waves down its
// tendrils.
JellyFish::JellyFish() { }
void JellyFish::show(CRGB leds[NUM_STRIPS][NUM_LEDS]) {
}
*/
