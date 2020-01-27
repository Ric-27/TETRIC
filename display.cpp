#include <SFML/Graphics.hpp>
#include <iostream>
#include "logic.hpp"
#include <chrono>
#include <thread>


int const xSize = 10;
int const ySize = 22;
int counterGravityMax = 400;
int counterGravity = 0;
bool gameStarted = false;

const int pixel_factor = 30;

const int window_width = 39;
const int window_height = 22;

void DrawGame(vector<string>);
void DrawWindow();
void DrawNext(vector<string>);

sf::RenderWindow window(sf::VideoMode(window_width * pixel_factor, window_height * pixel_factor), "RIC -- TETRIS");

int main()
{
   
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
            if (event.key.code == sf::Keyboard::N && gameStarted == false)
            {
               //game.newTetromino();
               gameStarted = true;
            }
            if (event.key.code == sf::Keyboard::Down)
            {
               game.moveTetromino('d');
            }
            if (event.key.code == sf::Keyboard::Right) //move right
            {
               game.moveTetromino('r');
            }
            if (event.key.code == sf::Keyboard::Left) //move left
            {
               game.moveTetromino('l');
            }
            if (event.key.code == sf::Keyboard::Up) //turning clockwise
            {
               game.rotateTetromino();
            }
            if (event.key.code == sf::Keyboard::Q)
            {
               return 0;
            }        
         }
      }
   
      //window.clear();

      if (gameStarted == true)
      {
         counterGravity++;

         DrawGame(game.getMatrix());
         if (game.getPieceDied())
         {
            DrawNext(game.getNext());
         }
         
         

         if (counterGravity > counterGravityMax) 
         {
            counterGravity = 0;
            game.moveTetromino('d');
         }

      } else {
         DrawWindow();
      }
      window.display();      
   }
   return 0;
}

void DrawGame(vector<string> argMatrix){
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
   text.setPosition(2*pixel_factor + xSize*pixel_factor + 1.5*pixel_factor, pixel_factor / 2 - 1);
   // inside the main loop, between window.clear() and window.display()
   window.draw(text);
   for (int j = 0; j < 4; j++)
   {
      for (int i = 0; i < 6; i++)
      {
         sf::RectangleShape pixel(sf::Vector2f(pixel_factor, pixel_factor));
         pixel.setOutlineThickness(-1.f);
         pixel.setOutlineColor(sf::Color(20,20,20,255));
         pixel.setPosition((i + 2 + xSize) *pixel_factor, (j + 2)*pixel_factor);
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