#include <Arduino.h>
#include "FastLED.h"

#define DATA_PIN 8
#define CLOCK_PIN 7

#define NUM_LEDS 64
CRGB leds[NUM_LEDS];
int brightness = 64;

#define btn_1 5
#define btn_2 6

#define led1 11
#define led2 12
#define onboard_LED 13

int program_selector = 0;
int num_programs = 3;

int  btn_1_down_at, btn_2_down_at = -1;
int  btn_1_up_at, btn_2_up_at = -1;
int  btn_1_held_for, btn_2_held_for = -1;
bool btn_1_read, btn_2_read;
bool btn_1_last, btn_2_last;
bool btn_1_ignore_press, btn_2_ignore_press = false;

int debounce_time = 20;
int press_time = 500;
int hold_time = 1500;

int hold_poll_time = 250; // report that the button was held however often


void setup() {
    pinMode(btn_1, INPUT);
    pinMode(btn_2, INPUT);

    pinMode(led1, OUTPUT);
    pinMode(led2, OUTPUT);
    pinMode(onboard_LED, OUTPUT);

    LEDS.addLeds<APA102, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
    LEDS.setBrightness(brightness);
}

void get_inputs();
void first_led();
void first_led();
void second_led();
void cylon();
void cylon_sweep();
void third_led();
void no_leds();
void animation_wrapper(int pattern);

void program_up(){
    program_selector++;
    program_selector %= num_programs;
}

void program_down(){
    program_selector += num_programs;
    program_selector--;
    program_selector %= num_programs;
}

void loop() {
    get_inputs();
    switch(program_selector){
        case 0:
            first_led();
            break;
        case 1:
            second_led();
            animation_wrapper(2);
            break;
        case 2:
            third_led();
            animation_wrapper(2);
            break;
        default:
            no_leds();
            break;
    }
}


void fadeall() { for(int i = 0; i < NUM_LEDS; i++) { leds[i].nscale8(250); } }


// This re-does cylon, so that it doesn't need a FOR
// loop. Basically, it keeps track of the head of the
// cylon-snake (I know you know what that means) and
// its direction. Then, it moves the cylon snake a
// step, and paints that area with the selected hue.
//
// Next, it calls fadeall(), which fades every LED by
// a little bit.
void cylon_step(CRGB leds[NUM_LEDS]) {
    static int cylon_head;
    static bool cylon_up;
    static uint8_t hue;
    if (cylon_head == NUM_LEDS) cylon_up = false;
    if (cylon_head == 0) cylon_up = true;
    cylon_head += cylon_up ? 1 : -1;
    hue++;

    leds[cylon_head] = CHSV(hue, 255, 255);
    fadeall();
}

// Animation wrapper will call an animation ONLY
// if at least 10 ms have elapsed since the last
// call. Basically, it calls the animation function
// and then once the animator has updated the
// leds, in paints them.
void animation_wrapper(int anim) {
    static uint32_t last_paint;
    uint32_t now = millis();
    if (now - last_paint > 10) {
        switch (anim) {
            case 2:
                cylon_step(leds);
                break;
            default:
                // Bad! Put your own calls to step type functions
                // in here.
                NULL;
        }
        last_paint = now;
        FastLED.show();
    }
}


void cylon(CRGB leds[NUM_LEDS]){
    static uint8_t hue = 0;
    Serial.print("x");
    // First slide the led in one direction
    for(int i = 0; i < NUM_LEDS; i++) {
        // Set the i'th led to red
        leds[i] = CHSV(hue++, 255, 255);
        // Show the leds
        FastLED.show();
        // now that we've shown the leds, reset the i'th led to black
        leds[i] = CRGB::Black;
        //fadeall();
        // Wait a little bit before we loop around and do it again
        delay(10);
    }
    Serial.print("x");

    // Now go in the other direction.
    for(int i = (NUM_LEDS)-1; i >= 0; i--) {
        // Set the i'th led to red
        leds[i] = CHSV(hue++, 255, 255);
        // Show the leds
        FastLED.show();
        // now that we've shown the leds, reset the i'th led to black
        leds[i] = CRGB::Black;
        //fadeall();
        // Wait a little bit before we loop around and do it again
        delay(10);
    }
}


void get_inputs(){
    static int singleton_1, singleton_2;
    static unsigned long timer_1, timer_1_elapsed, timer_1_held_last_triggered;
    static unsigned long timer_2, timer_2_elapsed, timer_2_held_last_triggered;
    static unsigned long long_hold = 500;


    if(digitalRead(btn_1) && !digitalRead(btn_2)){
        if (singleton_1 == 0) {//this is the first loop where this button has been pressed
            singleton_1 = 1; // set the singleton to true, so we know for next time.
            timer_1 = millis(); //reset the timer for function 1
            timer_1_held_last_triggered = 0; //when the last time the "Held"
        }else{
            timer_1_elapsed = millis() - timer_1;

            if(timer_1_elapsed >= long_hold){
                if(timer_1_elapsed >= (timer_1_held_last_triggered + (long_hold / 10))){

                    //Here is where anything that is held happens.
                    timer_1_held_last_triggered = timer_1_elapsed;

//          Serial.println("button 1 held");
                }

            }
        }

    }else{
        if(singleton_1 == 1){ //this is the first loop since the button was pressed
            singleton_1 = 0; //reset the singleton

            if(timer_1_elapsed < long_hold && timer_1_elapsed >20){
                program_up();
            }else{
                timer_1_elapsed = 0;
            }

        }
    }


    if(!digitalRead(btn_1) && digitalRead(btn_2)){
        if (singleton_2 == 0) {//this is the first loop where this button has been pressed
            singleton_2 = 1; // set the singleton to true, so we know for next time.
            timer_2 = millis(); //reset the timer for function 1
            timer_2_held_last_triggered = 0; //when the last time the "Held"
        }else{
            timer_2_elapsed = millis() - timer_2;

            if(timer_2_elapsed >= long_hold){
                if(timer_2_elapsed >= (timer_2_held_last_triggered + (long_hold / 10))){

                    //Here is where anything that is held happens.
                    timer_2_held_last_triggered = timer_2_elapsed;

//          Serial.println("button 2 held");
                }

            }
        }

    }else{
        if(singleton_2 == 1){ //this is the first loop since the button was pressed
            singleton_2 = 0; //reset the singleton

            if(timer_2_elapsed < long_hold && timer_2_elapsed >20){
                program_down();
            }else{
                timer_2_elapsed = 0;
            }

        }
    }

}




void no_leds(){
//  Serial.println("no leds");
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(onboard_LED, LOW);
}

void first_led(){
    Serial.println("1 led");
    digitalWrite(led1, HIGH);
    digitalWrite(led2, LOW);
    digitalWrite(onboard_LED, LOW);
}

void second_led(){
    Serial.println("2 led");
    digitalWrite(led1, LOW);
    digitalWrite(led2, HIGH);
    digitalWrite(onboard_LED, LOW);
}

void third_led(){
    Serial.println("3 led");
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(onboard_LED, HIGH);
}

// How to turn a looped animation into a stepped animation
// Step 1: get your animation working in looped mode.
void cylon(CRGB leds[NUM_LEDS]) {
    static uint8_t hue = 0;
    for (int i = 0; i < NUM_LEDS; i++) {
        // This is just in here to be dickish and
        // make the exercise a little harder.
        if (i % 100 == 0) hue = 0;
        leds[i] = CHSV(hue++, 255, 255);
        FastLED.show();
        fadeall();
        delay(10);
    }
}
// Step 2: identify the "pattern" code. This is anything that does the LED.
// Move it to a step version. Now, this is obviously wrong, but we'll fill
// in the rest as we go.
void cylon_step(CRGB leds[NUM_LEDS]) {
    if (i % 100 == 0) hue = 0;
    leds[i] = CHSV(hue++, 255, 255);

    fadeall();
}
// Step 3: identify sattes
// Now, obviously, this isn't right. You need to get in the "state" code.
// To do this you should look at the loops, because that's what we're
// eliminating. In this case, the states are:
// - hue
// - i
// So put them back in as STATIC ints. These will initialize to zero.
void cylon_step(CRGB leds[NUM_LEDS]) {
    static int i;
    static uint8_t hue = 0;

    if (i % 100 == 0) hue = 0;
    leds[i] = CHSV(hue++, 255, 255);

    fadeall();
}
// Step 4: update states
// Now, all we need to do is to replace the state updates. To do this,
// look at the loop condition. i is incremented, and there's an extra
// sneaky update - when the render is finished, the whole loop loops and
// returns to zero. So, we add that in. All those updates happen
// before the rendering
void cylon_step(CRGB leds[NUM_LEDS]) {
    static int i;
    static uint8_t hue = 0;
    // This is the whole state update.
    // reset to zero if greataer that
    // num leds, otherwise just incremeant
    if (i > NUM_LEDS) i = 0;
    else i++;

    if (i % 100 == 0) hue = 0;
    leds[i] = CHSV(hue++, 255, 255);

    fadeall();
}
// Cool, now we're done:
// 1. Each step of this code will simulate a step of the loop
// 2. There's no call to delay()
// 3. The delay is handled outside the loop
