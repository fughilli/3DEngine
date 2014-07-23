#include "../Graphics3D/DisplayInterface.h"

#include "image.h"

class ImageDisplay : public DisplayInterface
{
private:
    Image* _i;

public:
    ImageDisplay(Image& renderTarget);

    void drawTri(const Triangle2d& tri, const G3D_Color& color);
    void drawLine(const Line2d& line, const G3D_Color& color);
    void drawPoint(const Vector2d& point, const G3D_Color& color);
    void drawBox(const Line2d& diag, const G3D_Color& color);
    void clear(const G3D_Color& color);
};
