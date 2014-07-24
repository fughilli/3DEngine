#include "../Graphics3D/DisplayInterface.h"

#include "image.h"

class ImageDisplay : public DisplayInterface
{
protected:
    Image* _i;

public:
    ImageDisplay(Image& renderTarget);

    void drawPoint(const Vector2d& point, const G3D_Color& color);
    void clear(const G3D_Color& color);
};
