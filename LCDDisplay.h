#ifndef LCD_DISPLAY_H
#define LCD_DISPLAY_H

#ifdef ENERGIA

#include "../LCDSuite/LCD_screen_font.h"
#include "3DEngineRenderModule/DisplayInterface.h"
#include "VectorLib/Vector.h"
#include "3DEngineRenderModule/G3DUtilities.h"

#include "3DEngineRenderModule/DrawableObject.h"
#include "res/headers/cube.h"
#include "res/headers/monkey.h"
#include "res/headers/twocubes.h"

class LCDDisplay : public DisplayInterface
{
private:
    LCD_screen * diScreen;
public:
    LCDDisplay(LCD_screen_font& _diScreen);

    void drawPoint(const Vector2d& point, const G3D_Color& color);
    void clear(const G3D_Color& color);
};

#endif // ENERGIA

#endif // LCD_DISPLAY_H
