#include "pixel.hpp"
Pixel::Pixel()
{
}
Pixel::Pixel(int argX, int argY)
{
    x = argX;
    y = argY;
}
int Pixel::getX()
{
    return x;
}
int Pixel::getY()
{
    return y;
}
void Pixel::setX(int argX)
{
    x = argX;
}
void Pixel::setY(int argY)
{
    y = argY;
}
void Pixel::modX(int argModX)
{
    x += argModX;
}
void Pixel::modY(int argModY)
{
    y += argModY;
}
void Pixel::Rotate()
{
    int temp = x;
    x = -y;
    y = temp;
}
Pixel::~Pixel()
{
}
