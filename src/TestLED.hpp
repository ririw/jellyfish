#if NATIVE_BUILD==1
#pragma once
#include <stdint.h>

// This is a bunch of mock stuff that basically emulates
// the fastled library.

class CRGB {
public:
    uint8_t r;
    uint8_t g;
    uint8_t b;
    CRGB(uint8_t r, uint8_t g, uint8_t b);
    CRGB();
    CRGB nscale8(uint8_t v);
};

uint8_t qsub8(uint8_t i, uint8_t j) {
    int t = i - j;
    if( t < 0) t = 0;
    return t;
}

class CFastLED {
public:
    CFastLED();
    void setBrightness(uint8_t brightness);
    void show();
};

static CFastLED FastLED;


class String {
private:
    int length;
    char *ptr;
public:
    String(const char* s);
    String();
    String concat(String other);
};

class TProgmemRGBPalette16 {
public:
    TProgmemRGBPalette16();
};

class CHSVPalette16 {
public:
    CHSVPalette16();
    CHSVPalette16(const TProgmemRGBPalette16 src);
};

CRGB ColorFromPalette(CHSVPalette16 pal, uint8_t color);

int random(int low, int high);
int random(int high);
void delay(int ms);
int millis();

class CSerial {
public:
    void write(char *s);
    void print(char *s);
    void print(String s);
};
static CSerial Serial;

class CEEPROM {
public:
    uint8_t read(int addr);
};
#endif