#include <random>
#include <cstdlib>
#include <ctime>
#include <vector>
#include "pixel.hpp"

class Tetromino
{
private:
    std::vector<Pixel> pixels;
    bool active;
    std::string color;
public:
    Tetromino();
    Tetromino(Pixel coorZero);
    Tetromino(Tetromino &argTetromino);
    std::vector<Pixel> getPixels();
    std::string getColor();
    void setPixels(std::vector<Pixel> argPixels);
    void deactivate();
    ~Tetromino();
};