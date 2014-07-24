#include "ImageDisplay.h"
#include <cmath>

ImageDisplay::ImageDisplay(Image& renderTarget)
{
    _i = &renderTarget;
}

void ImageDisplay::drawPoint(const Vector2d& point, const G3D_Color& color)
{
    Pixel p = {color.r, color.g, color.b};
    int x = round(point.x) + (_i->getWidth()/2);
    int y = round(point.y) + (_i->getHeight()/2);

    if(((x >= 0) && (x < _i->getWidth())) && ((y >= 0) && (y < _i->getHeight())))
    {
        _i->setPixel(x, y, p);
    }

}

void ImageDisplay::clear(const G3D_Color& color)
{
    Pixel p = {color.r, color.g, color.b};
    for(int i = 0; i < _i->getWidth(); i++)
        for(int j = 0; j < _i->getHeight(); j++)
            _i->setPixel(i, j, p);
}
