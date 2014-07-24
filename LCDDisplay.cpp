#include "LCDDisplay.h"

#ifdef ENERGIA

LCDDisplay::LCDDisplay(LCD_screen_font& _diScreen)
{
    diScreen = &_diScreen;
}

//void LCDDisplay::drawTri(tri_2d_t& tri, uint16_t color)
//{
//    diScreen->triangle(tri.x1, tri.y1, tri.x2, tri.y2, tri.x3, tri.y3, color);
//}

static uint16_t colorToUintColor(const G3D_Color& color)
{
    uint16_t retcolor = (color.r >> 3);
    retcolor <<= 6;
    retcolor |= (color.g >> 2);
    retcolor <<= 5;
    retcolor |= (color.b >> 3);
    return retcolor;
}

static uint16_t grayscaleToUintColor(uint8_t grayscale)
{
    return colorToUintColor({grayscale, grayscale, grayscale});
}

void LCDDisplay::drawPoint(const Vector2d& point, const G3D_Color& color)
{
    int x = round(point.x) + (diScreen->screenSizeX()/2);
    int y = round(point.y) + (diScreen->screenSizeY()/2);

    if(((x >= 0) && (x < diScreen->screenSizeX())) && ((y >= 0) && (y < diScreen->screenSizeY())))
    {
        diScreen->point(x, y, colorToUintColor(color));
    }
}

void LCDDisplay::clear(const G3D_Color& color)
{
//    for(int i = 0; i < diScreen->screenSizeX(); i++)
//        for(int j = 0; j < diScreen->screenSizeY(); j++)
//            diScreen->point(i, j, colorToUintColor(color));
    diScreen->clear(colorToUintColor(color));
}

#endif // ENERGIA
