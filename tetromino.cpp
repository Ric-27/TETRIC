#include "tetromino.hpp"

Tetromino::Tetromino()
{
}
Tetromino::Tetromino(Tetromino &argTetromino)
{
    pixels = argTetromino.pixels;
    active = argTetromino.active;
    color = argTetromino.color;
}
Tetromino::Tetromino(Pixel coorZero)
{
    active = true;
    pixels.resize(4);
    srand(time(NULL));    
    int random = (rand()%8) + 1;  
    switch (random)
    {
    case 1: //o
        pixels[0].setX(coorZero.getX());
        pixels[0].setY(coorZero.getY());
        
        pixels[1].setX(coorZero.getX() + 1);
        pixels[1].setY(coorZero.getY());
        
        pixels[2].setX(coorZero.getX());
        pixels[2].setY(coorZero.getY() - 1);
        
        pixels[3].setX(coorZero.getX() + 1);
        pixels[3].setY(coorZero.getY() - 1);

        color = "15.0.194";
        break;
    case 2: //i
        pixels[0].setX(coorZero.getX());
        pixels[0].setY(coorZero.getY());
        
        pixels[1].setX(coorZero.getX() + -1);
        pixels[1].setY(coorZero.getY());
        
        pixels[2].setX(coorZero.getX() + 1);
        pixels[2].setY(coorZero.getY());
        
        pixels[3].setX(coorZero.getX() + 2);
        pixels[3].setY(coorZero.getY());

        color = "215.33.33";
        break;
    case 3: //l
        pixels[0].setX(coorZero.getX());
        pixels[0].setY(coorZero.getY());
        
        pixels[1].setX(coorZero.getX() - 1);
        pixels[1].setY(coorZero.getY());
        
        pixels[2].setX(coorZero.getX() + 1);
        pixels[2].setY(coorZero.getY());
        
        pixels[3].setX(coorZero.getX() + 1);
        pixels[3].setY(coorZero.getY() - 1);

        color = "216.36.122";
        break;
    case 4: //j
        pixels[0].setX(coorZero.getX());
        pixels[0].setY(coorZero.getY());
        
        pixels[1].setX(coorZero.getX() + 1);
        pixels[1].setY(coorZero.getY());
        
        pixels[2].setX(coorZero.getX() -1);
        pixels[2].setY(coorZero.getY());
        
        pixels[3].setX(coorZero.getX() - 1);
        pixels[3].setY(coorZero.getY() - 1);

        color = "241.196.15";
        break;
    case 5: //s
        pixels[0].setX(coorZero.getX());
        pixels[0].setY(coorZero.getY());
        
        pixels[1].setX(coorZero.getX() - 1);
        pixels[1].setY(coorZero.getY());
        
        pixels[2].setX(coorZero.getX());
        pixels[2].setY(coorZero.getY() - 1);
        
        pixels[3].setX(coorZero.getX() + 1);
        pixels[3].setY(coorZero.getY() - 1);

        color = "211.84.0";
        break;
    case 6: //z
        pixels[0].setX(coorZero.getX());
        pixels[0].setY(coorZero.getY());
        
        pixels[1].setX(coorZero.getX() + 1);
        pixels[1].setY(coorZero.getY());
        
        pixels[2].setX(coorZero.getX());
        pixels[2].setY(coorZero.getY() - 1);
        
        pixels[3].setX(coorZero.getX() - 1);
        pixels[3].setY(coorZero.getY() - 1);

        color = "52.152.219";
        break;
    case 7: //t
        pixels[0].setX(coorZero.getX());
        pixels[0].setY(coorZero.getY());
        
        pixels[1].setX(coorZero.getX());
        pixels[1].setY(coorZero.getY() - 1);
        
        pixels[2].setX(coorZero.getX() + 1);
        pixels[2].setY(coorZero.getY());
        
        pixels[3].setX(coorZero.getX() - 1);
        pixels[3].setY(coorZero.getY());

        color = "46.204.113 ";
        break;
    case 8: //.
        pixels[0].setX(coorZero.getX());
        pixels[0].setY(coorZero.getY());
        
        pixels[1].setX(coorZero.getX());
        pixels[1].setY(coorZero.getY());
        
        pixels[2].setX(coorZero.getX());
        pixels[2].setY(coorZero.getY());
        
        pixels[3].setX(coorZero.getX());
        pixels[3].setY(coorZero.getY());

        color = "75.0.130";
        break;    
    }
}
std::vector<Pixel> Tetromino::getPixels()
{
    return pixels;
}
std::string Tetromino::getColor()
{
    return color;
}
void Tetromino::setPixels(std::vector<Pixel> argPixels)
{
    std::copy(argPixels.begin(),argPixels.end(), pixels.begin());
}
void Tetromino::deactivate()
{
    active = false;
}
Tetromino::~Tetromino()
{
}
