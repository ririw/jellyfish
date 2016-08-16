#include "FastLED.h"
#include "Pattern.hpp"
#include "config.h"


const int num_patterns = 6;
int current_pattern = 0;
Pattern* pattern;
Pattern* patterns[num_patterns];
int num_cycles;
CRGB leds[NUM_STRIPS][NUM_LEDS];

void pick_pattern() {
	//current_pattern = (int)random(num_patterns);
	current_pattern = (current_pattern + 1) % num_patterns;
	pattern = patterns[current_pattern];
	pattern->enter(leds);
	Serial.write("Running pattern: ");
	Serial.print(pattern->name());
	Serial.print("\n");
}

void setup() {
	LEDS.addLeds<APA102,1,2,BGR>(leds[0], NUM_LEDS);
	LEDS.addLeds<APA102,3,4,BGR>(leds[1], NUM_LEDS);
	LEDS.addLeds<APA102,5,6,BGR>(leds[2], NUM_LEDS);
	LEDS.addLeds<APA102,6,8,BGR>(leds[3], NUM_LEDS);
	LEDS.addLeds<APA102,9,10,BGR>(leds[4], NUM_LEDS);
	LEDS.addLeds<APA102,11,12,BGR>(leds[5], NUM_LEDS);
	LEDS.setBrightness(255);

	patterns[0] = new MultiCyclon(HeatColors_p,    String("HeatColors_p"));
	patterns[1] = new MultiCyclon(ForestColors_p,  String("ForestColors_p"));
	//patterns[2] = new MultiCyclon(CloudColors_p,   String("CloudColors_p")); // too bright
	patterns[2] = new MultiCyclon(LavaColors_p,    String("LavaColors_p"));
	patterns[3] = new MultiCyclon(OceanColors_p,   String("OceanColors_p"));
	patterns[4] = new MultiCyclon(RainbowColors_p, String("RainbowColors_p"));
	patterns[5] = new MultiCyclon(PartyColors_p,   String("PartyColors_p"));

	pattern = patterns[0];
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
		if (num_cycles > 500) {
			num_cycles = 0;
			pick_pattern();
		}
		last_run = now;
	}
	delay(1); // We do like, 8K cycles per display loop without this (teensy31)
}
