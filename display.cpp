#include <SFML/Graphics.hpp>
#include <iostream>
#include "logic.hpp"
#include <chrono>
#include <thread>


int const xSize = 10;
int const ySize = 22;
int counterGravityMax = 50;
int counterGravity = 0;
int gameStatus = 0; //0 tittle screen,1 playing, 2 pause, 6 gameover

const int pixel_factor = 30;

const int window_width = 39;
const int window_height = 22;

int colorChanging = 0;
bool colorAug = true;

void DrawTitleScreen();
void DrawGame(vector<string>);
void DrawNext(vector<string>);
void DrawPoints(int, int, int);
void DrawWindow();
void DrawPause();
void ChangeColor();

sf::RenderWindow window(sf::VideoMode(window_width * pixel_factor, window_height * pixel_factor), "RIC -- TETRIS");

int main()
{
   int rows = 0;
   int score = 0;
   int level = 0;   
   Logic game(xSize, ySize);
   while (window.isOpen())
   {
      sf::Event event;
      while (window.pollEvent(event))
      {
         if (event.type == sf::Event::Closed)
            window.close();
         if (event.type == sf::Event::KeyPressed)
         {
            if (event.key.code == sf::Keyboard::S && gameStatus == 0)
            {
               gameStatus = 1;
            }
            if (event.key.code == sf::Keyboard::P)
            {
               if (gameStatus == 1)
               {
                  gameStatus = 2;
               } else if (gameStatus == 2) 
               {
                  gameStatus = 1;
               }          
            }
            if (event.key.code == sf::Keyboard::Down && gameStatus == 1) //soft drop
            {
               game.moveTetromino('d');
            }
            if (event.key.code == sf::Keyboard::Right && gameStatus == 1) //move right
            {
               game.moveTetromino('r');
            }
            if (event.key.code == sf::Keyboard::Left && gameStatus == 1) //move left
            {
               game.moveTetromino('l');
            }
            if (event.key.code == sf::Keyboard::Up && gameStatus == 1) //turning clockwise
            {
               game.rotateTetromino();
            }
            if (event.key.code == sf::Keyboard::Q)
            {
               return 0;
            }        
         }
      }   
      window.clear();
      switch (gameStatus)
      {
      case 0:
         DrawTitleScreen();
         ChangeColor();
         break;         

      case 1:         
         counterGravity++;
         DrawWindow();
         DrawGame(game.getMatrix());
         DrawNext(game.getNext());
         DrawPoints(score,rows,level);

         if (counterGravity > counterGravityMax) 
         {
            counterGravity = 0;
            game.moveTetromino('d');
         }
         
         break;

      case 2:
         DrawWindow();
         DrawGame(game.getMatrix());
         DrawNext(game.getNext());
         DrawPoints(score,rows,level);
         DrawPause();
         ChangeColor();
         break;
      }
      window.display();      
   }
   return 0;
}

/////////////////////////////////////////////////////////
void ChangeColor()
{
   if (colorAug)
   {
      colorChanging++;
      colorAug = (colorChanging > 255) ? false : true;
   } else {
      colorChanging--;
      colorAug = (colorChanging < 50) ? true : false;
   }         
}


void DrawPause()
{
   sf::Font font;
   if (!font.loadFromFile("font.ttf"))
   {
      cout << "font load error..." << endl;
   }
   sf::Text text;
   text.setFont(font);
   text.setString("PAUSED");
   text.setCharacterSize(pixel_factor * 10);
   text.setFillColor(sf::Color(colorChanging,colorChanging,colorChanging,255));
   text.setPosition((window_width/2 * pixel_factor) - (15.5*pixel_factor), (4*pixel_factor));
   window.draw(text);
}

void DrawGame(vector<string> argMatrix)
{
   for (int j = 2; j < ySize; j++)
   {
      for (int i = 0; i < xSize; i++)
      {
         sf::RectangleShape pixel(sf::Vector2f(pixel_factor, pixel_factor));
         pixel.setOutlineThickness(-1.f);
         pixel.setOutlineColor(sf::Color(20,20,20,255));
         pixel.setPosition((i + 1) *pixel_factor, (j - 1)*pixel_factor);
         string k = argMatrix[i + xSize * j];
         if (k.length() > 0)
         {
            int posp1 = k.find('.');
            int posp2 = k.find('.',posp1 + 1);
            
            short r = stoi(k.substr(0,posp1));
            short g = stoi(k.substr(posp1 + 1,  posp2 - posp1 - 1));
            short b = stoi(k.substr(posp2 + 1,  k.length() - posp2 - 1));
            //cout << r << " " << g << " " << b << endl;
            pixel.setFillColor(sf::Color(r,g,b,255));
            //pixel.setFillColor(sf::Color(255,0,0,255));  
         } else {
            pixel.setFillColor(sf::Color::Black);
         }             
         window.draw(pixel);            
      }        
   }
}

void DrawTitleScreen()
{
   //DrawWindow();   
   sf::Font font;
   if (!font.loadFromFile("font.ttf"))
   {
      cout << "font load error..." << endl;
   }
   sf::Text text;
   text.setFont(font);
   text.setString("TETRIC");
   text.setCharacterSize(pixel_factor * 10);
   text.setFillColor(sf::Color::White);
   text.setPosition((window_width/2 * pixel_factor) - (15.5*pixel_factor), (-2*pixel_factor));
   window.draw(text);

   text.setString("Tetris   by   Ricardo  Rico");
   text.setCharacterSize(pixel_factor*2);
   text.setFillColor(sf::Color::White);
   text.setPosition((7.8*pixel_factor), (8.2*pixel_factor));
   window.draw(text);

   text.setString("IN204     ENSTA PARIS");
   text.setCharacterSize(pixel_factor*1.5);
   text.setFillColor(sf::Color::White);
   text.setPosition((12.5*pixel_factor),(10.5*pixel_factor));
   window.draw(text);

   text.setString("2020");
   text.setCharacterSize(pixel_factor*1.5);
   text.setFillColor(sf::Color::White);
   text.setPosition((17.5*pixel_factor),(12.5*pixel_factor));
   window.draw(text);

   text.setString("Press    S    to  play");
   text.setCharacterSize(pixel_factor*4);
   text.setFillColor(sf::Color(colorChanging,colorChanging,colorChanging,255));
   text.setPosition((3.9*pixel_factor),(17.2*pixel_factor));
   window.draw(text);
}

void DrawWindow()
{
   for (int i = 0; i < window_width; i++)
   {
      for (int j = 0; j < window_height; j++)
      {
         sf::RectangleShape pixel(sf::Vector2f(pixel_factor, pixel_factor));
         pixel.setOutlineThickness(-1.f);
         pixel.setOutlineColor(sf::Color(40,40,40,255));
         pixel.setPosition(i*pixel_factor, j*pixel_factor);
         pixel.setFillColor(sf::Color(61,61,61,255));
         window.draw(pixel);
      }      
   }
}

void DrawNext(vector<string> argMatrix)
{
   sf::Font font;
   if (!font.loadFromFile("font.ttf"))
   {
      cout << "font load error..." << endl;
   }
   sf::Text text;
   // select the font
   text.setFont(font); // font is a sf::Font
   // set the string to display
   text.setString("NEXT");
   // set the character size
   text.setCharacterSize(pixel_factor * 1.5); // in pixels, not points!
   // set the color
   text.setFillColor(sf::Color::White);
   // set the text style
   text.setPosition(2*pixel_factor + xSize*pixel_factor + 1.4*pixel_factor, 1.5*pixel_factor);
   // inside the main loop, between window.clear() and window.display()
   window.draw(text);
   for (int j = 0; j < 4; j++)
   {
      for (int i = 0; i < 6; i++)
      {
         sf::RectangleShape pixel(sf::Vector2f(pixel_factor, pixel_factor));
         pixel.setOutlineThickness(-1.f);
         pixel.setOutlineColor(sf::Color(20,20,20,255));
         pixel.setPosition((i + 2 + xSize) *pixel_factor, (j + 3)*pixel_factor);
         string k = argMatrix[i + 6 * j];
         //cout << k << endl;
         if (k.length() > 0)
         {
            int posp1 = k.find('.');
            int posp2 = k.find('.',posp1 + 1);
            
            short r = stoi(k.substr(0,posp1));
            short g = stoi(k.substr(posp1 + 1,  posp2 - posp1 - 1));
            short b = stoi(k.substr(posp2 + 1,  k.length() - posp2 - 1));
            //cout << r << " " << g << " " << b << endl;
            pixel.setFillColor(sf::Color(r,g,b,255));
            //pixel.setFillColor(sf::Color(255,0,0,255));  
         } else {
            pixel.setFillColor(sf::Color::Black);
         }             
         window.draw(pixel);            
      }        
   }
}

void DrawPoints(int argScore, int argRows, int argLevel)
{
   for (int i = 0; i < 6; i++)
   {
      sf::RectangleShape pixel(sf::Vector2f(pixel_factor, pixel_factor));
      pixel.setOutlineThickness(-1.f);
      pixel.setOutlineColor(sf::Color(20,20,20,255));
      pixel.setPosition((i + 2 + xSize) *pixel_factor, (10)*pixel_factor);
      pixel.setFillColor(sf::Color::Black);            
      window.draw(pixel);            
   }
   for (int i = 0; i < 6; i++)
   {
      sf::RectangleShape pixel(sf::Vector2f(pixel_factor, pixel_factor));
      pixel.setOutlineThickness(-1.f);
      pixel.setOutlineColor(sf::Color(20,20,20,255));
      pixel.setPosition((i + 2 + xSize) *pixel_factor, (14)*pixel_factor);
      pixel.setFillColor(sf::Color::Black);            
      window.draw(pixel);            
   }
   for (int i = 0; i < 6; i++)
   {
      sf::RectangleShape pixel(sf::Vector2f(pixel_factor, pixel_factor));
      pixel.setOutlineThickness(-1.f);
      pixel.setOutlineColor(sf::Color(20,20,20,255));
      pixel.setPosition((i + 2 + xSize) *pixel_factor, (18)*pixel_factor);
      pixel.setFillColor(sf::Color::Black);            
      window.draw(pixel);            
   }
   sf::Font font;
   if (!font.loadFromFile("font.ttf"))
   {
      cout << "font load error..." << endl;
   }
   sf::Text text;
   text.setFont(font);
   text.setString("SCORE");
   text.setCharacterSize(pixel_factor * 1.5);
   text.setFillColor(sf::Color::White);
   text.setPosition(2*pixel_factor + xSize*pixel_factor + 0.9*pixel_factor, pixel_factor*9 - pixel_factor/2);
   window.draw(text);
   string k = to_string(argScore);
   text.setString(k);
   text.setCharacterSize(pixel_factor * 1.5);
   text.setFillColor(sf::Color::White);
   text.setPosition(2*pixel_factor + xSize*pixel_factor, pixel_factor*10 - pixel_factor/2);
   window.draw(text);

   text.setString("ROWS");
   text.setCharacterSize(pixel_factor * 1.5);
   text.setFillColor(sf::Color::White);
   text.setPosition(2*pixel_factor + xSize*pixel_factor + 1.3*pixel_factor, pixel_factor*13 - pixel_factor/2);
   window.draw(text);
   k = to_string(argRows);
   text.setString(k);
   text.setCharacterSize(pixel_factor * 1.5);
   text.setFillColor(sf::Color::White);
   text.setPosition(2*pixel_factor + xSize*pixel_factor, pixel_factor*14 - pixel_factor/2);
   window.draw(text);

   text.setString("LEVEL");
   text.setCharacterSize(pixel_factor * 1.5);
   text.setFillColor(sf::Color::White);
   text.setPosition(2*pixel_factor + xSize*pixel_factor + 0.9*pixel_factor, pixel_factor*17 - pixel_factor/2);
   window.draw(text);
   k = to_string(argLevel);
   text.setString(k);
   text.setCharacterSize(pixel_factor * 1.5);
   text.setFillColor(sf::Color::White);
   text.setPosition(2*pixel_factor + xSize*pixel_factor, pixel_factor*18 - pixel_factor/2);
   window.draw(text);
  /*
  for (int i = 0; i < 22; i++)
  {
      sf::Text text;
      text.setFont(font);
      string k = to_string(i);
      text.setString(k);
      text.setCharacterSize(pixel_factor * 1.5);
      text.setFillColor(sf::Color::White);
      text.setPosition(2*pixel_factor + xSize*pixel_factor, pixel_factor*i - pixel_factor/2);
      window.draw(text);
  }
  */
}