#include <SFML/Graphics.hpp>
#include <iostream>
#include "logic.hpp"
#include <chrono>
#include <thread>


int const xSize = 10;
int const ySize = 22;
int frames = 20;
bool gameStarted = false;
const int pixel_factor = 40;
const int window_width = pixel_factor * xSize;
const int window_height = pixel_factor * (ySize);

void draw_game(vector<string>);

sf::RenderWindow window(sf::VideoMode(window_width, window_height), "TETRIS");

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
            if (event.key.code == sf::Keyboard::N)
            {
               game.newTetromino();
               gameStarted = true;
               //cout << "n" << endl;
            }
            if (event.key.code == sf::Keyboard::D)
            {
               game.moveTetromino('d');
            }         
         }
      }
   
      window.clear();
      if (gameStarted == true)
      {
         game.updateMatrix();
         draw_game(game.getMatrix());
         game.moveTetromino('d');
         for (int i = 0; i < frames; i++)
         {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
         }
      }     
      window.display();      
   }
   return 0;
}

void draw_game(vector<string> argMatrix){
   for (int j = 0; j < ySize; j++)
   {
      for (int i = 0; i < xSize; i++)
      {
         sf::RectangleShape pixel(sf::Vector2f(pixel_factor, pixel_factor));
         pixel.setOutlineThickness(-1.f);
         pixel.setOutlineColor(sf::Color::Black);
         pixel.setPosition(i*pixel_factor, j*pixel_factor);
         string k = argMatrix[i + xSize * j];
         if (k.length() > 0)
         {
            int posp1 = k.find('.');
            int posp2 = k.find('.',posp1 + 1);
            
            int r = stoi(k.substr(1,posp1));
            int g = stoi(k.substr(posp1 + 1,  posp2 - posp1 - 1));
            int b = stoi(k.substr(posp2 + 1,  k.length() - posp2 - 1));
            
            //cout << posp1 << " - " << posp2 << endl;
            //cout << k.substr(0,posp1) << " * " << k.substr(posp1 + 1, posp2 - posp1 - 1) << " * " << k.substr(posp2 + 1, k.length() - posp2 - 1) << endl;
            pixel.setFillColor(sf::Color(r,g,b,255));  
         } else {
            pixel.setFillColor(sf::Color(69,69,69,255));
         }             
         window.draw(pixel);            
      }        
   }
}