#ifndef _LCD_HPP
#define _LCD_HPP

#include "Gmbu.hpp"
#include "utils.hpp"

#define LCDS_MODE ((lcd_mode)(this->lcds & 0b11))
#define LCDS_MODE_SET(mode) { this->lcds &= ~0b11; this->lcds |= mode; }

typedef enum {
    MODE_HBLANK,
    MODE_VBLANK,
    MODE_OAM,
    MODE_XFER
}   lcdMode;

class Lcd
{
public:
    /* data */
public:
    Lcd(/* args */);
    ~Lcd();
};


#endif