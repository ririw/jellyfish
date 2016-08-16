#include "FastLED.h"
#include "Pattern.h"
#include "config.h"


const int num_patterns = 4;
int current_pattern = 0;
Pattern* pattern;
Pattern* patterns[num_patterns];
int num_cycles;
CRGB leds[NUM_STRIPS][NUM_LEDS];

void setup() {
	LEDS.addLeds<APA102,0,1,RGB>(leds[0], NUM_LEDS);
	LEDS.setBrightness(84);
	patterns[0] = new SimplePattern(rchannel);
	patterns[1] = new SimplePattern(bchannel);
	patterns[2] = new SimplePattern(gchannel);
	patterns[3] = new MultiCyclon();
	pattern = patterns[current_pattern];
	num_cycles = 0;
}

void loop() {
	pattern->show(leds);
	FastLED.show();

	if (num_cycles > 1000) {
		current_pattern = (current_pattern + 1) % num_patterns;
		pattern = patterns[current_pattern];
		pattern->enter(leds);
	}
}
