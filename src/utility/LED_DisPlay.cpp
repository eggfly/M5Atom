#include "LED_DisPaly.h"

LED_DiaPaly::LED_DiaPaly(uint8_t LEDNumbre)
{
    FastLED.addLeds<NEOPIXEL, DATA_PIN>(_ledbuff, LEDNumbre);
    _numberled = LEDNumbre;
}

LED_DiaPaly::~LED_DiaPaly()
{
}

void LED_DiaPaly::run(void *data)
{
    data = nullptr;

    for (int num = 0; num < 26; num++)
    {
        _ledbuff[num] = 0x000000;
    }
    FastLED.show();
    FastLED.setBrightness(20);

    while (1)
    {
        if (_mode == kAnmiation_run)
        {
            if ((_am_mode & kMoveRight) || (_am_mode & kMoveLeft))
            {
                if (_am_mode & kMoveRight)
                {
                    _count_x++;
                }
                else
                {
                    _count_x--;
                }
            }
            if ((_am_mode & kMoveTop) || (_am_mode & kMoveButtom))
            {
                if (_am_mode & kMoveTop)
                {
                    _count_y--;
                }
                else
                {
                    _count_y++;
                }
            }
            if ((_am_count != -1) && (_am_count != 0))
            {
                _am_count--;
                if (_am_count == 0)
                {
                    _mode = kAnmiation_stop;
                }
            }
            displaybuff(_am_buffptr, _count_x, _count_y);
            delay(_am_speed);
        }
        FastLED.show();
        delay(10);
    }
}

void LED_DiaPaly::animation(uint8_t *buffptr, uint8_t amspeed, uint8_t ammode, int64_t amcount)
{
    if (_mode == kAnmiation_run)
    {
        _mode = kAnmiation_stop;
    }
    _am_buffptr = buffptr;
    _am_speed = amspeed;
    _am_mode = ammode;
    _am_count = amcount;
    _count_x = _count_y = 0;
    _mode = kAnmiation_run;
}

void LED_DiaPaly::displaybuff(uint8_t *buffptr, int8_t offsetx, int8_t offsety)
{
    uint16_t xsize = 0, ysize = 0;
    xsize = buffptr[0];
    ysize = buffptr[1];

    offsetx = offsetx % xsize;
    offsety = offsety % ysize;

    int8_t setdatax = (offsetx < 0) ? (-offsetx) : (xsize - offsetx);
    int8_t setdatay = (offsety < 0) ? (-offsety) : (ysize - offsety);
    for (int x = 0; x < 5; x++)
    {
        for (int y = 0; y < 5; y++)
        {
            _ledbuff[x + y * 5].raw[0] = buffptr[2 + ((setdatax + x) % xsize + ((setdatay + y) % ysize) * xsize) * 3 + 0];
            _ledbuff[x + y * 5].raw[1] = buffptr[2 + ((setdatax + x) % xsize + ((setdatay + y) % ysize) * xsize) * 3 + 1];
            _ledbuff[x + y * 5].raw[2] = buffptr[2 + ((setdatax + x) % xsize + ((setdatay + y) % ysize) * xsize) * 3 + 2];
        }
    }
    FastLED.setBrightness(20);
}

void LED_DiaPaly::setBrightness(uint8_t brightness)
{
    FastLED.setBrightness(brightness);
}

void LED_DiaPaly::drawpix(uint8_t xpos, uint8_t ypos, CRGB Color)
{
    if ((xpos >= 5) || (ypos >= 5))
    {
        return;
    }
    _ledbuff[xpos + ypos * 5] = Color;
}

void LED_DiaPaly::drawpix(uint8_t Number, CRGB Color)
{
    if (Number >= 25)
    {
        return;
    }
    _ledbuff[Number] = Color;
}

void LED_DiaPaly::clear()
{
    for (int8_t i = 0; i < 25; i++)
    {
        _ledbuff[i] = 0;
    }
}
