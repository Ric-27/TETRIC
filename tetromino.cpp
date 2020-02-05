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
Tetromino::Tetromino(Pixel coorZero,int argId)
{
    active = true;
    pixels.resize(amount_of_pixels);
    for (int i = 0; i < pixels.size(); i++)
    {
        pixels[i].setX(-1);
        pixels[i].setY(-1);        
    }
    int random = argId;
    if (random < 0)
    {
        random = abs(random);
    } else {    
        while (random == argId)
        {
            unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
            std::default_random_engine generator(seed);
            std::uniform_int_distribution<int> distribution(1,variety);
            random = distribution(generator);
        }
    }
    //cout << "piece: " << random << endl;
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

        color = tetromino_o_color;
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

        color = tetromino_i_color;
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

        color = tetromino_l_color;
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

        color = tetromino_j_color;
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

        color = tetromino_s_color;
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

        color = tetromino_z_color;
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

        color = tetromino_t_color;
        id = random;
        break;
    case 8: //.
        pixels[0].setX(coorZero.getX());
        pixels[0].setY(coorZero.getY());
        
        color = tetromino_legendary;
        id = random;
        break;
    case 9: //u
        pixels[0].setX(coorZero.getX());
        pixels[0].setY(coorZero.getY());
        
        pixels[1].setX(coorZero.getX() + 1);
        pixels[1].setY(coorZero.getY());
        
        pixels[2].setX(coorZero.getX() - 1);
        pixels[2].setY(coorZero.getY());
        
        pixels[3].setX(coorZero.getX() + 1);
        pixels[3].setY(coorZero.getY() - 1);

        pixels[4].setX(coorZero.getX() - 1);
        pixels[4].setY(coorZero.getY() - 1);

        color = tetromino_legendary;
        id = random;
        break;
    case 10: //corner
        pixels[0].setX(coorZero.getX());
        pixels[0].setY(coorZero.getY());
        
        pixels[1].setX(coorZero.getX() + 1);
        pixels[1].setY(coorZero.getY());

        pixels[2].setX(coorZero.getX());
        pixels[2].setY(coorZero.getY() - 1);
        
        color = tetromino_legendary;
        id = random;
        break;
    default: //o
        pixels[0].setX(coorZero.getX());
        pixels[0].setY(coorZero.getY());
        
        pixels[1].setX(coorZero.getX() + 1);
        pixels[1].setY(coorZero.getY());
        
        pixels[2].setX(coorZero.getX());
        pixels[2].setY(coorZero.getY() - 1);
        
        pixels[3].setX(coorZero.getX() + 1);
        pixels[3].setY(coorZero.getY() - 1);

        color = "15.0.194";
        id = 1;
        break;
    }
    for (int i = 1; i < pixels.size(); i++)
    {
        if (pixels[i].getX() == -1 && pixels[i].getY() == -1)
        {
            pixels[i].setX(pixels[0].getX());
            pixels[i].setY(pixels[0].getY());
        }        
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
