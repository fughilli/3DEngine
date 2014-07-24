#ifndef IMAGE_H
#define IMAGE_H

#include <stdint.h>
#include <string>

using namespace std;

struct Pixel
{
    uint8_t r, g, b;
};

class Image
{
public:
    Image(uint16_t width, uint16_t height);

    uint16_t getWidth() const;
    uint16_t getHeight() const;

    Pixel getPixel(uint16_t x, uint16_t y) const;
    void setPixel(uint16_t x, uint16_t y, const Pixel& pixel);

    bool writeToFile(const string& filename);
    bool readFromFile(const string& filename);
private:
    Pixel* _data;
    uint16_t _width, _height;
};

#endif // IMAGE_H
