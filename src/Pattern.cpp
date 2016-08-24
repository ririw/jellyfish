#include "Pattern.hpp"
#include <stdlib.h>

void Pattern::enter(CRGB leds[NUM_STRIPS][NUM_LEDS]) {
    for (int strip = 0; strip < NUM_STRIPS; strip++) {
        for (int led = 0; led < NUM_LEDS; led++) {
            leds[strip][led] = CRGB(0, 0, 0);
        }
    }
}

String Pattern::name() {
    return _name;
}


SimplePattern::SimplePattern(Channel channel) {
    this->brightness = 0;
    this->increment = 1;
    this->channel = channel;
    FastLED.setBrightness(255);
    if (channel == rchannel) this->_name = String("Simple: red");
    if (channel == bchannel) this->_name = String("Simple: blue");
    if (channel == gchannel) this->_name = String("Simple: green");
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
        leds[strip][head] = ColorFromPalette(pallet, hue + static_cast<uint8_t>(strip * 10));
    }
    hue += hue_upwards ? 1 : -1;
}

void MultiCyclon::fadeall(CRGB leds[NUM_LEDS]) {
    for(int i = 0; i < NUM_LEDS; i++) {
        leds[i].nscale8(252);
    }
}

JellyFish::JellyFish(const TProgmemRGBPalette16 &pal, String name) {
    this->pallet = CHSVPalette16(pal);
    this->_name = String("Jellyfish - ").concat(name);
    this->wave_center = NUM_LEDS/2;
    this->wave_hue = 128;
}

/**
 * The jellyfish has a center, which moves in a random walk
 * up and down the strand. The center spits out sparks, a
 * lot like the fire animation, and then the sparks leave
 * trails, and the trails are cooled.
 *
 * Fingers crossed that the arduino is clever enough to
 * handle all this.
 */
void JellyFish::show(CRGB leds[NUM_STRIPS][NUM_LEDS]) {
    int step = random(-3, 4);
    wave_center = abs(wave_center + step) % NUM_LEDS;                            // Random walk the wave center by a step.
    if (random(0, 3) == 2) sparks[wave_center] = 255;                            // Push out a spark one in 5 times
    else sparks[wave_center] = 0;
    for (int spark_loc = NUM_LEDS - 1; spark_loc >= wave_center; spark_loc--) { // walk from the last LED to the center
        uint8_t sub = random(0, 3) == 0 ? (uint8_t)3 : (uint8_t)0;
        sparks[spark_loc + 1] = qsub8(sparks[spark_loc], sub);                  // and move the sparks towards NUM_LEDS
        sparks[spark_loc + 2] = qsub8(sparks[spark_loc], sub * 10);             // and diffuse a little
    }                                                                           //  NUM_LEDS |>>>>>> center -------| 0
    for (int spark_loc = 2; spark_loc <= wave_center; spark_loc++) {            // walk from the first LEDS to the center
        uint8_t sub = random(0, 5) == 0 ? (uint8_t)1 : (uint8_t)0;
        sparks[spark_loc - 1] = qsub8(sparks[spark_loc], sub);                  // and move the sparks towards 0
        sparks[spark_loc - 2] = qsub8(sparks[spark_loc], sub * 10);             // and diffuse a little
    }                                                                           //  NUM_LEDS |------ center <<<<<<<| 0

    for (int strip = 0; strip < NUM_STRIPS; strip++) {                         // Finally, just blit everything onto the
        for (int led = 0; led < NUM_LEDS; led++) {                             // strip, using the given pallet.
            leds[strip][led] = ColorFromPalette(pallet, sparks[led]);
        }
    }
}


void Spiral::show(CRGB leds[NUM_STRIPS][NUM_LEDS]) {
    for (int strip = 0; strip < NUM_STRIPS; strip++) {
        for (int led = 0; led < NUM_LEDS; led++) {
            leds[strip][led] = ((strip + led) % 16 == spiral_loc) ? CHSV(spiral_loc << 4, 255, 255) : CHSV(0, 0, 0);
        }
    }
    if (innerloop) {
        int8_t step = (int8_t) (up ? 1 : -1);
        if (spiral_loc == 0 && step == -1) {
            spiral_loc = 15;
        } else {
            spiral_loc = (spiral_loc + step) % (uint8_t)16;
        }
    }
    innerloop = !innerloop;
}


