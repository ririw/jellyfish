#include "TestLED.hpp"
#if NATIVE_BUILD==1

#include <stdlib.h>
#include <string.h>
#include <assert.h>

uint8_t brightness = 255;
void CFastLED::setBrightness(uint8_t b) {
    brightness = b;
}

String::String(const char *s) {
    int i = 0;
    while (s[i]) {
        i++;
    }
    this->ptr = (char*)malloc(sizeof(char) * i);
    assert(this->ptr || i == 0);
    memcpy(this->ptr, s, i);
    this->length = i;
}

String String::concat(String other) {
    String res = String();
    res.length = this->length + other.length;
    res.ptr = (char*)malloc(sizeof(char) * res.length);
    assert(res.ptr || res.length == 0);

    char *p = res.ptr;
    char *op = other.ptr;
    char *tp = this->ptr;
    memcpy(&p[0], op, this->length);
    memcpy(&p[this->length], tp, other.length);
    return res;
}

CRGB::CRGB() {
    this->r = 0;
    this->g = 0;
    this->b = 0;
}

void delay(int ms) {}
int millis() {
    return 0;
}
#endif
