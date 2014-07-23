#include "ImageDisplay.h"
#include <cmath>

template <class T>
static void _swap(T& a, T& b)
{
    T temp = a;
    a = b;
    b = temp;
}

ImageDisplay::ImageDisplay(Image& renderTarget)
{
    _i = &renderTarget;
}

void ImageDisplay::triArea(const Triangle2d& tri, const G3D_Color& color)
{
    int16_t wx1 = (int16_t)tri.A.x;
    int16_t wy1 = (int16_t)tri.A.y;
    int16_t wx2 = (int16_t)tri.B.x;
    int16_t wy2 = (int16_t)tri.B.y;
    int16_t wx3 = (int16_t)tri.C.x;
    int16_t wy3 = (int16_t)tri.C.y;
    int16_t wx4 = wx1;
    int16_t wy4 = wy1;
    int16_t wx5 = wx1;
    int16_t wy5 = wy1;

    bool changed1 = false;
    bool changed2 = false;

    int16_t dx1 = abs(wx2 - wx1);
    int16_t dy1 = abs(wy2 - wy1);

    int16_t dx2 = abs(wx3 - wx1);
    int16_t dy2 = abs(wy3 - wy1);

    int16_t signx1 = (wx2 >= wx1) ? +1 : -1;
    int16_t signx2 = (wx3 >= wx1) ? +1 : -1;

    int16_t signy1 = (wy2 >= wy1) ? +1 : -1;
    int16_t signy2 = (wy3 >= wy1) ? +1 : -1;

    if (dy1 > dx1)     // swap values
    {
        _swap<int16_t>(dx1, dy1);
        changed1 = true;
    }

    if (dy2 > dx2)     // swap values
    {
        _swap<int16_t>(dx2, dy2);
        changed2 = true;
    }

    int16_t e1 = 2 * dy1 - dx1;
    int16_t e2 = 2 * dy2 - dx2;

    for (int i = 0; i <= dx1; i++)
    {
        drawLine({{wx4, wy4}, {wx5, wy5}}, color);

        while (e1 >= 0)
        {
            if (changed1) wx4 += signx1;
            else wy4 += signy1;
            e1 = e1 - 2 * dx1;
        }

        if (changed1) wy4 += signy1;
        else wx4 += signx1;

        e1 = e1 + 2 * dy1;

        while (wy5 != wy4)
        {
            while (e2 >= 0)
            {
                if (changed2) wx5 += signx2;
                else wy5 += signy2;
                e2 = e2 - 2 * dx2;
            }

            if (changed2) wy5 += signy2;
            else wx5 += signx2;

            e2 = e2 + 2 * dy2;
        }
    }
}

void ImageDisplay::drawTri(const Triangle2d& tri, const G3D_Color& color, bool filled)
{
    if (filled)
    {
        int16_t x1 = (int16_t)tri.A.x;
        int16_t y1 = (int16_t)tri.A.y;
        int16_t x2 = (int16_t)tri.B.x;
        int16_t y2 = (int16_t)tri.B.y;
        int16_t x3 = (int16_t)tri.C.x;
        int16_t y3 = (int16_t)tri.C.y;

        bool b = true;

        // Graham Scan + Andrew's Monotone Chain Algorithm
        // Sort by ascending y
        while (b)
        {
            b = false;
            if ((b == false) && (y1 > y2))
            {
                _swap<int16_t>(x1, x2);
                _swap<int16_t>(y1, y2);
                b = true;
            }
            if ((b == false) && (y2 > y3))
            {
                _swap<int16_t>(x3, x2);
                _swap<int16_t>(y3, y2);
                b = true;
            }
        }

        if (y2 == y3)
        {
            triArea({{x1, y1}, {x2, y2}, {x3, y3}}, color);

        }
        else if (y1 == y2)
        {
            triArea({{x3, y3}, {x1, y1}, {x2, y2}}, color);

        }
        else
        {
            uint16_t x4 = (uint16_t)( (int32_t)x1 + (y2 - y1) * (x3 - x1) / (y3 - y1) );
            uint16_t y4 = y2;

            triArea({{x1, y1}, {x2, y2}, {x4, y4}}, color);
            triArea({{x3, y3}, {x2, y2}, {x4, y4}}, color);
        }
    }
    else
    {
        drawLine({tri.A, tri.B}, color);
        drawLine({tri.B, tri.C}, color);
        drawLine({tri.C, tri.A}, color);
    }
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
