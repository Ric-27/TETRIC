#include <random>
#include <cstdlib>
#include <vector>
#include "pixel.hpp"
#include <chrono>
class Tetromino
{
private:
    std::vector<Pixel> pixels;
    bool active;
    std::string color;
    int id;
public:
    Tetromino();
    Tetromino(Pixel, int);
    Tetromino(Tetromino &);
    std::vector<Pixel> getPixels();
    std::string getColor();
    int GetId();
    void setPixels(std::vector<Pixel>);
    void deactivate();
    ~Tetromino();
};