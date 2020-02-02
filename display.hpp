#include <SFML/Graphics.hpp>
#include "logic.hpp"
#include <chrono>
#include <iostream>

class Display
{
private:
    int xSize;
    int ySize;

    int counterGravity;
    int sleepCont;

    int pixel_factor;
    int window_width;
    int window_height;

    int colorChanging;
    bool colorAug;
    
    int level;
    
    sf::RenderWindow window;
    sf::Event event;
    
    Logic game;    
public:
    Display();
    void Run();
    void Events();
    void DrawTitleScreen();
    void DrawGame(vector<string>);
    void DrawPlayers(vector<string>,int,vector<string>,int,vector<string>,int);
    void DrawNext(vector<string>);
    void DrawPoints(int, int, int);
    void DrawGameOver(int, int, int);
    void DrawBackground();
    void DrawPause();
    void DrawLevelUp();
    void ChangeColor();
    ~Display();
};