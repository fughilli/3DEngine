#include "image.h"
#include <iostream>
#include <cstdlib>
#include <fstream>

Image::Image(uint16_t width, uint16_t height)
{
    _width = width;
    _height = height;

    _data = (Pixel*)malloc(sizeof(Pixel) * width * height);

    for(int i = 0; i < width; i++)
    {
        for(int j = 0; j < width; j++)
        {
            setPixel(i, j, {0,0,0});
        }
    }
}

uint16_t Image::getWidth() const
{
    return _width;
}

uint16_t Image::getHeight() const
{
    return _height;
}

Pixel Image::getPixel(uint16_t x, uint16_t y) const
{
    return _data[x+y*_width];
}

void Image::setPixel(uint16_t x, uint16_t y, const Pixel& pixel)
{
    _data[x+y*_width] = pixel;
}

bool Image::writeToFile(const string& filename)
{
    ofstream file;
    file.open(filename);
    file << "P3\n";
    file << _width << " " << _height << "\n";
    file << "255\n";
    for(int i = 0; i < _height; i++)
    {
        for(int j = 0; j < _width; j++)
        {
            Pixel temp = getPixel(j, i);
            file << (int)temp.r << " " << (int)temp.g << " " << (int)temp.b << " ";
        }
        file << "\n";
    }
    file.close();

    return true;
}

bool Image::readFromFile(const string& filename)
{
    return false;
}
