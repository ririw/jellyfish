#if NATIVE_BUILD == 1
#include "TestLED.hpp"
#else
#include "FastLED.h"
#include "EEPROM.h"
#endif

#include "kvec.h"
#include "Pattern.hpp"

int current_pattern = 0;
Pattern* pattern;
//Pattern* patterns[num_patterns];

kvec_t(Pattern*) patterns;

int num_cycles;
CRGB leds[NUM_STRIPS][NUM_LEDS];
bool just_lights;


void pick_pattern() {
	if (just_lights) {
		return; // The pattern should already be JustLights, as it was initialized that way.
	} else {
		current_pattern = static_cast<int>(random(kv_size(patterns)));
		pattern = kv_A(patterns, current_pattern);
		pattern->enter(leds);
		Serial.write("Running pattern: ");
		Serial.print(pattern->name());
		Serial.print("\n");
	}
}

void re_seed() {
	uint8_t s = EEPROM.read(0);
	randomSeed(s);
	EEPROM.write(0, s+(uint8_t)1);
	just_lights = random(0, 2) == 0;
}

void setup() {
	re_seed();
	FastLED.addLeds<APA102,12,13,BGR>(leds[0], NUM_LEDS); // 1
	FastLED.addLeds<APA102,11,14,BGR>(leds[1], NUM_LEDS); // 2
	FastLED.addLeds<APA102,5, 20,BGR>(leds[2], NUM_LEDS); // 3
	FastLED.addLeds<APA102,9 ,16,BGR>(leds[3], NUM_LEDS); // 4
	FastLED.addLeds<APA102,8 ,17,BGR>(leds[4], NUM_LEDS); // 5
	FastLED.addLeds<APA102,7 ,18,BGR>(leds[5], NUM_LEDS); // 6
	FastLED.addLeds<APA102,6 ,19,BGR>(leds[5], NUM_LEDS); // 7
	FastLED.setBrightness(255);

	kv_push(Pattern*, patterns, new MultiCyclon(HeatColors_p,    String("HeatColors_p")));
	kv_push(Pattern*, patterns, new MultiCyclon(ForestColors_p,  String("ForestColors_p")));
	kv_push(Pattern*, patterns, new MultiCyclon(LavaColors_p,    String("LavaColors_p")));
	kv_push(Pattern*, patterns, new MultiCyclon(OceanColors_p,   String("OceanColors_p")));
	kv_push(Pattern*, patterns, new MultiCyclon(RainbowColors_p, String("RainbowColors_p")));
	kv_push(Pattern*, patterns, new MultiCyclon(PartyColors_p,   String("PartyColors_p")));
	kv_push(Pattern*, patterns, new JellyFish(HeatColors_p,      String("HeatColors_p")));
	kv_push(Pattern*, patterns, new JellyFish(ForestColors_p,    String("ForestColors_p")));
	kv_push(Pattern*, patterns, new JellyFish(LavaColors_p,      String("LavaColors_p")));
	kv_push(Pattern*, patterns, new JellyFish(OceanColors_p,     String("OceanColors_p")));
	kv_push(Pattern*, patterns, new JellyFish(RainbowColors_p,   String("RainbowColors_p")));
	kv_push(Pattern*, patterns, new Spiral(true));
	kv_push(Pattern*, patterns, new Spiral(false));
	kv_push(Pattern*, patterns, new Juggle());
	kv_push(Pattern*, patterns, new Asparabreath());

	pattern = kv_A(patterns, 14);
	if (just_lights)
		pattern = new JustLights();
	pattern->enter(leds);
	num_cycles = 0;
}

void loop() {
	static uint32_t last_run;
	uint32_t now = millis();
	if (now - last_run > 30) {
		pattern->show(leds);
		FastLED.show();

		num_cycles += 1;
		if (num_cycles > 3000) {
			num_cycles = 0;
			pick_pattern();
		}
		last_run = now;
	}
	delay(1); // We do like, 8K cycles per display loop without this (teensy31)
}
