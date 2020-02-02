#include "tetromino.hpp"

#include <iostream>

Tetromino::Tetromino()
{
}
Tetromino::Tetromino(Tetromino &argTetromino)
{
    pixels = argTetromino.pixels;
    active = argTetromino.active;
    color = argTetromino.color;
}
Tetromino::Tetromino(Pixel coorZero,int argId)
{
    active = true;
    pixels.resize(4);

    int random = argId;
    while (random == argId)
    {
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::default_random_engine generator(seed);
        std::uniform_int_distribution<int> distribution(1,8);
        random = distribution(generator);
    }
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
        id = random;
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
        id = random;
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
        id = random;
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
        id = random;
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
        id = random;
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
        id = random;
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
        id = random;
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
        id = random;
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
int Tetromino::GetId()
{
    return id;
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
