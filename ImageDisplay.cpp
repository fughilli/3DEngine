#include "ImageDisplay.h"
#include <cmath>


ImageDisplay::ImageDisplay(Image& renderTarget)
{
    _i = &renderTarget;
}

void ImageDisplay::drawTri(const Triangle2d& tri, const G3D_Color& color)
{
    drawLine({tri.A, tri.B}, color);
    drawLine({tri.B, tri.C}, color);
    drawLine({tri.C, tri.A}, color);
}

void ImageDisplay::drawBox(const Line2d& diag, const G3D_Color& color)
{
    int16_t x1 = round(min(diag.A.x, diag.B.x));
    int16_t x2 = round(max(diag.A.x, diag.B.x));

    int16_t y1 = round(min(diag.A.y, diag.B.y));
    int16_t y2 = round(max(diag.A.y, diag.B.y));

    for(int16_t i = x1; i <= x2; i++)
    {
        for(int16_t j = y1; j <= y2; j++)
        {
            drawPoint({i, j}, color);
        }
    }

}

template <class T>
static void _swap(T& a, T& b)
{
    T temp = a;
    a = b;
    b = temp;
}

void ImageDisplay::drawLine(const Line2d& line, const G3D_Color& color)
{
    if ((line.A.x == line.B.x) && (line.A.y == line.B.y))
    {
        drawPoint(line.A, color);
    }
    else if ((line.A.x == line.B.x) || (line.A.y == line.B.y))
    {
        drawBox(line, color);
    }
    else
    {
        int16_t wx1 = round(line.A.x);
        int16_t wx2 = round(line.B.x);
        int16_t wy1 = round(line.A.y);
        int16_t wy2 = round(line.B.y);

        bool flag = abs(wy2 - wy1) > abs(wx2 - wx1);
        if (flag)
        {
            _swap<int16_t>(wx1, wy1);
            _swap<int16_t>(wx2, wy2);
        }

        if (wx1 > wx2)
        {
            _swap<int16_t>(wx1, wx2);
            _swap<int16_t>(wy1, wy2);
        }

        int16_t dx = wx2 - wx1;
        int16_t dy = abs(wy2 - wy1);
        int16_t err = dx / 2;
        int16_t ystep;

        if (wy1 < wy2) ystep = 1;
        else ystep = -1;

        for (; wx1<=wx2; wx1++)
        {
            if (flag) drawPoint({wy1, wx1}, color);
            else drawPoint({wx1, wy1}, color);

            err -= dy;
            if (err < 0)
            {
                wy1 += ystep;
                err += dx;
            }
        }
    }
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
