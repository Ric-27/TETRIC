#include <SFML/Graphics.hpp>
#include <chrono>
#include <iostream>

#include "config.inl"
#include "network_declarations.inl"

#include "logic.hpp"
#include "server.hpp"
#include "client.hpp"

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

    bool thread_runing;
    Thread* networking;

    short server_choosen;
    bool reload;
    status_type my_status;
    
    int level;

    string player_Name;
    
    sf::RenderWindow window;
    sf::Event event;
    
    Logic game;

    Server host;
    Client player; 
public:
    Display();
    void Run();
    void Events();
    void DrawTitleScreen();
    void Draw_Multiplayer_Name_Screen();
    void Draw_Multiplayer_Option_Screen();
    void Draw_Multiplayer_Join_Screen();
    void Draw_Multiplayer_Create_Screen();
    void Draw_Loading();
    void Draw_Lobby();
    void DrawGame(vector<string>);
    void DrawPlayers(vector<string>,int,vector<string>,int,vector<string>,int);
    void DrawNext(vector<string>);
    void DrawPoints(int, int, int);
    void DrawGameOver(int, int, int);
    void Writing();
    void DrawBackground();
    void DrawPause();
    void DrawLevelUp();
    void ChangeColor();
    ~Display();
};