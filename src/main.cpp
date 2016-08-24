#if NATIVE_BUILD == 1
#include "TestLED.hpp"
#else
#include "FastLED.h"
#include "EEPROM.h"
#endif

#include "Pattern.hpp"

const int num_patterns = 15;
int current_pattern = 0;
Pattern* pattern;
Pattern* patterns[num_patterns];
int num_cycles;
CRGB leds[NUM_STRIPS][NUM_LEDS];

void pick_pattern() {
	current_pattern = static_cast<int>(random(num_patterns));
	//current_pattern = (current_pattern + 1) % num_patterns;
	pattern = patterns[current_pattern];
	pattern->enter(leds);
	Serial.write("Running pattern: ");
	Serial.print(pattern->name());
	Serial.print("\n");
}

void re_seed() {
	uint8_t s = EEPROM.read(0);
	randomSeed(s);
	EEPROM.write(0, s+(uint8_t)1);
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

	patterns[0]  = new MultiCyclon(HeatColors_p,    String("HeatColors_p"));
	patterns[1]  = new MultiCyclon(ForestColors_p,  String("ForestColors_p"));
	patterns[2]  = new MultiCyclon(LavaColors_p,    String("LavaColors_p"));
	patterns[3]  = new MultiCyclon(OceanColors_p,   String("OceanColors_p"));
	patterns[4]  = new MultiCyclon(RainbowColors_p, String("RainbowColors_p"));
	patterns[5]  = new MultiCyclon(PartyColors_p,   String("PartyColors_p"));
	patterns[6]  = new JellyFish(HeatColors_p,    String("HeatColors_p"));
	patterns[7]  = new JellyFish(ForestColors_p,  String("ForestColors_p"));
	patterns[8]  = new JellyFish(LavaColors_p,    String("LavaColors_p"));
	patterns[9]  = new JellyFish(OceanColors_p,   String("OceanColors_p"));
	patterns[10] = new JellyFish(RainbowColors_p, String("RainbowColors_p"));
	patterns[11] = new JellyFish(PartyColors_p,   String("PartyColors_p"));
	patterns[12] = new Spiral(true);
	patterns[13] = new Spiral(false);
	patterns[14] = new Juggle();

	pattern = patterns[14];
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
		if (num_cycles > 2000) {
			num_cycles = 0;
			pick_pattern();
		}
		last_run = now;
	}
	delay(1); // We do like, 8K cycles per display loop without this (teensy31)
}
