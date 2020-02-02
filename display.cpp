#include "display.hpp"

Display::Display(): 
   xSize(10),
   ySize(24),
   pixel_factor(30),
   window_width(39),
   window_height(22),
   window(sf::VideoMode(window_width * pixel_factor, window_height * pixel_factor), "TETRIC"),
   game(xSize, ySize)
{
   level = game.getLevel();
   sleepCont = 0;
   colorChanging = 0;
   colorAug = true;
   counterGravity = 0;
}
void Display::Run()
{
   while (window.isOpen())
   {
      Events();
      window.clear();      
      switch (game.getStatus())
      {
         case 0:
            DrawTitleScreen();
            break;         

         case 1:         
            counterGravity++;
            DrawBackground();
            DrawGame(game.getMatrix());
            DrawPlayers(game.getMatrix(),game.getScore(),game.getMatrix(),game.getScore(),game.getMatrix(),game.getScore());
            DrawNext(game.getNext());
            DrawPoints(game.getScore(),game.getRows(),game.getLevel());
            if (level != game.getLevel())
            {
               sleepCont++;
               DrawLevelUp();
            }           
            if (counterGravity > game.GetSlowness()) 
            {
               counterGravity = 0;
               game.moveTetromino('d');
            }            
            break;

         case 2:
            DrawBackground();
            DrawGame(game.getMatrix());
            DrawPlayers(game.getMatrix(),game.getScore(),game.getMatrix(),game.getScore(),game.getMatrix(),game.getScore());
            DrawNext(game.getNext());
            DrawPoints(game.getScore(),game.getRows(),game.getLevel());
            DrawPause();
            break;

         case 3:
            DrawGameOver(game.getScore(),game.getRows(),game.getLevel());
            break;
      }
      window.display();
   }
}
void Display::Events()
{
    while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            window.close();
            if (event.type == sf::Event::KeyPressed)
            {
            switch (event.key.code)
            {
            case sf::Keyboard::S: //Start Game
               if (game.getStatus() == 0) game.setStatus(1);
               break;
            case sf::Keyboard::Q: //Quit
               window.close();
               break;
            case sf::Keyboard::P: //Pause
               if (game.getStatus() == 1)
                  game.setStatus(2);
               else if (game.getStatus() == 2) 
                  game.setStatus(1);
               break;
            case sf::Keyboard::C: //Connection
               
               break;
            case sf::Keyboard::R: //Restart
               if (game.getStatus() == 3){
                  level = 0;
                  game.CleanUp();
               }              
               break;
            case sf::Keyboard::Up: // Rotate
            game.rotateTetromino();
               break;
            case sf::Keyboard::Right: //Move Right
               game.moveTetromino('r');
               break;
            case sf::Keyboard::Left: //Move Left
               game.moveTetromino('l');
               break;
            case sf::Keyboard::Down: //SoftDrop
               game.moveTetromino('d');
               break;
            case sf::Keyboard::Space: //HardDrop
               game.ActivateHardDrop();
               break;
            default:
               break;
            }      
        }
    } 
}
void Display::ChangeColor()
{
   if (colorAug)
   {
      colorChanging+=2;
      colorAug = (colorChanging > 250) ? false : true;
   } else {
      colorChanging-=2;
      colorAug = (colorChanging < 20) ? true : false;
   } 
}
void Display::DrawPause()
{
   ChangeColor();
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
void Display::DrawLevelUp()
{
   sf::Font font;
   if (!font.loadFromFile("font.ttf"))
   {
      cout << "font load error..." << endl;
   }
   sf::Text text;
   text.setFont(font);
   text.setString("Level up");
   text.setCharacterSize(pixel_factor * 2);
   text.setFillColor(sf::Color(255,255,255,150));
   text.setPosition((2*pixel_factor), 9*pixel_factor); 
   window.draw(text);
   
   if (sleepCont > 50)
   {
      level = game.getLevel();
      sleepCont = 0;
   }
}
void Display::DrawGame(vector<string> argMatrix)
{
   for (int j = 4; j < ySize; j++)
   {
      for (int i = 0; i < xSize; i++)
      {
         sf::RectangleShape pixel(sf::Vector2f(pixel_factor, pixel_factor));
         pixel.setOutlineThickness(-1.f);
         pixel.setOutlineColor(sf::Color(20,20,20,255));
         pixel.setPosition((i + 1) *pixel_factor, (j - 3)*pixel_factor);
         string k = argMatrix[i + xSize * j];
         if (k.length() > 0)
         {
            int posp1 = k.find('.');
            int posp2 = k.find('.',posp1 + 1);
            
            short r = stoi(k.substr(0,posp1));
            short g = stoi(k.substr(posp1 + 1,  posp2 - posp1 - 1));
            short b = stoi(k.substr(posp2 + 1,  k.length() - posp2 - 1));
            pixel.setFillColor(sf::Color(r,g,b,255));  
         } else {
            pixel.setFillColor(sf::Color::Black);
         }             
         window.draw(pixel);            
      }        
   }
}
void Display::DrawPlayers(vector<string> argMatrixP1,int argScoreP1,vector<string> argMatrixP2,int argScoreP2,vector<string> argMatrixP3,int argScoreP3)
{
   int local_PF = pixel_factor / 2.5;
   for (int j = 4; j < ySize; j++)
   {
      for (int i = 0; i < xSize; i++)
      {
         sf::RectangleShape pixel(sf::Vector2f(local_PF, local_PF));
         pixel.setPosition(((i + 1) *local_PF)+20.6*pixel_factor, ((j - 4)*local_PF)+1*pixel_factor);
         string k = argMatrixP1[i + xSize * j];
         if (k.length() > 0)
         {
            int posp1 = k.find('.');
            int posp2 = k.find('.',posp1 + 1);
            
            short r = stoi(k.substr(0,posp1));
            short g = stoi(k.substr(posp1 + 1,  posp2 - posp1 - 1));
            short b = stoi(k.substr(posp2 + 1,  k.length() - posp2 - 1));
            pixel.setFillColor(sf::Color(r,g,b,255));
         } else {
            pixel.setFillColor(sf::Color::Black);
         }             
         window.draw(pixel);            
      }        
   }
   for (int j = 4; j < ySize; j++)
   {
      for (int i = 0; i < xSize; i++)
      {
         sf::RectangleShape pixel(sf::Vector2f(local_PF, local_PF));
         pixel.setPosition(((i + 1) *local_PF)+26.6*pixel_factor, ((j - 4)*local_PF)+1*pixel_factor);
         string k = argMatrixP2[i + xSize * j];
         if (k.length() > 0)
         {
            int posp1 = k.find('.');
            int posp2 = k.find('.',posp1 + 1);
            
            short r = stoi(k.substr(0,posp1));
            short g = stoi(k.substr(posp1 + 1,  posp2 - posp1 - 1));
            short b = stoi(k.substr(posp2 + 1,  k.length() - posp2 - 1));
            pixel.setFillColor(sf::Color(r,g,b,255)); 
         } else {
            pixel.setFillColor(sf::Color::Black);
         }             
         window.draw(pixel);            
      }    
   }
   for (int j = 4; j < ySize; j++)
   {
      for (int i = 0; i < xSize; i++)
      {
         sf::RectangleShape pixel(sf::Vector2f(local_PF, local_PF));
         pixel.setPosition(((i + 1) *local_PF)+32.6*pixel_factor, ((j - 4)*local_PF)+1*pixel_factor);
         string k = argMatrixP3[i + xSize * j];
         if (k.length() > 0)
         {
            int posp1 = k.find('.');
            int posp2 = k.find('.',posp1 + 1);
            
            short r = stoi(k.substr(0,posp1));
            short g = stoi(k.substr(posp1 + 1,  posp2 - posp1 - 1));
            short b = stoi(k.substr(posp2 + 1,  k.length() - posp2 - 1));
            pixel.setFillColor(sf::Color(r,g,b,255)); 
         } else {
            pixel.setFillColor(sf::Color::Black);
         }             
         window.draw(pixel);            
      }        
   }

   sf::RectangleShape pixel(sf::Vector2f(4*pixel_factor, 2*pixel_factor));
   pixel.setPosition(21*pixel_factor, 9*pixel_factor);
   pixel.setFillColor(sf::Color(20,20,20,255));            
   window.draw(pixel);

   pixel.setPosition(27*pixel_factor, 9*pixel_factor);
   pixel.setFillColor(sf::Color(20,20,20,255));            
   window.draw(pixel);

   pixel.setPosition(33*pixel_factor, 9*pixel_factor);
   pixel.setFillColor(sf::Color(20,20,20,255));            
   window.draw(pixel);

   sf::Font font;
   if (!font.loadFromFile("font.ttf"))
   {
      cout << "font load error..." << endl;
   }
   sf::Text text;
   text.setFont(font);
   text.setCharacterSize(local_PF * 2.5);
   text.setFillColor(sf::Color::White);
   string k;

   text.setString("SCORE");   
   text.setPosition(21.5*pixel_factor, 8.8 * pixel_factor);
   window.draw(text);
   k = to_string(argScoreP1);
   text.setString(k);   
   text.setPosition(21.1*pixel_factor, 9.8 * pixel_factor);
   window.draw(text);

   text.setString("SCORE");   
   text.setPosition(27.5*pixel_factor, 8.8 * pixel_factor);
   window.draw(text);
   k = to_string(argScoreP2);
   text.setString(k);   
   text.setPosition(27.1*pixel_factor, 9.8 * pixel_factor);
   window.draw(text);
  
   text.setString("SCORE");   
   text.setPosition(33.5*pixel_factor, 8.8 * pixel_factor);
   window.draw(text);
   k = to_string(argScoreP3);
   text.setString(k);   
   text.setPosition(33.1*pixel_factor, 9.8 * pixel_factor);
   window.draw(text);
}
void Display::DrawTitleScreen()
{
   ChangeColor();   
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
   text.setFillColor(sf::Color(255,255,255,colorChanging));
   text.setPosition((3.9*pixel_factor),(17.2*pixel_factor));
   window.draw(text);
}
void Display::DrawGameOver(int argScore, int argRows, int argLevel)
{
   ChangeColor();   
   sf::Font font;
   if (!font.loadFromFile("font.ttf"))
   {
      cout << "font load error..." << endl;
   }
   sf::Text text;
   text.setFont(font);
   text.setString("GAME  OVER");
   text.setCharacterSize(pixel_factor * 8);
   text.setFillColor(sf::Color::White);
   text.setPosition((0.5*pixel_factor), (-2*pixel_factor));
   window.draw(text);
   string k = to_string(argScore);
   text.setString("SCORE  " + k);
   text.setCharacterSize(pixel_factor*2);
   text.setFillColor(sf::Color::White);
   text.setPosition((13.5*pixel_factor), (8.5*pixel_factor));
   window.draw(text);
   k = to_string(argRows);
   text.setString("ROWS  " + k);
   text.setCharacterSize(pixel_factor*2);
   text.setFillColor(sf::Color::White);
   text.setPosition((13.5*pixel_factor),(10.5*pixel_factor));
   window.draw(text);
   k = to_string(argLevel);
   text.setString("LEVEL  " + k);
   text.setCharacterSize(pixel_factor*2);
   text.setFillColor(sf::Color::White);
   text.setPosition((13.5*pixel_factor),(12.5*pixel_factor));
   window.draw(text);

   text.setString("Press    R    to  restart");
   text.setCharacterSize(pixel_factor*4);
   text.setFillColor(sf::Color(255,255,255,colorChanging));
   text.setPosition((0.5*pixel_factor),(17.2*pixel_factor));
   window.draw(text);
}
void Display::DrawBackground()
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
   sf::Font font;
   if (!font.loadFromFile("font.ttf"))
   {
      cout << "font load error..." << endl;
   }
   sf::Text text;
   text.setFont(font);
   text.setCharacterSize(pixel_factor * 2.5);
   text.setFillColor(sf::Color(200,200,200,255));

   text.setString("Press         to");   
   text.setPosition((21*pixel_factor), (11*pixel_factor));
   window.draw(text);

   text.setCharacterSize(pixel_factor * 1.5);

   text.setString("right                             move  right");   
   text.setPosition((21*pixel_factor), (13.5*pixel_factor));
   window.draw(text);

   text.setString("left                                 move  left");   
   text.setPosition((21*pixel_factor), (14.5*pixel_factor));
   window.draw(text);

   text.setString("up                                          rotate");   
   text.setPosition((21*pixel_factor), (15.5*pixel_factor));
   window.draw(text);

   text.setString("down                                 soft drop");   
   text.setPosition((21*pixel_factor), (16.5*pixel_factor));
   window.draw(text);

   text.setString("space                             hard drop");   
   text.setPosition((21*pixel_factor), (17.5*pixel_factor));
   window.draw(text);

   text.setString("p                                              pause");   
   text.setPosition((21*pixel_factor), (18.5*pixel_factor));
   window.draw(text);

   text.setString("c                                              connect");   
   text.setPosition((21*pixel_factor), (19.5*pixel_factor));
   window.draw(text);

   text.setString("Q                                              quit");   
   text.setPosition((21*pixel_factor), (20.5*pixel_factor));
   window.draw(text);
}
void Display::DrawNext(vector<string> argMatrix)
{
   sf::Font font;
   if (!font.loadFromFile("font.ttf"))
   {
      cout << "font load error..." << endl;
   }
   sf::Text text;
   text.setFont(font);
   text.setString("NEXT");
   text.setCharacterSize(pixel_factor * 1.5);
   text.setFillColor(sf::Color::White);
   text.setPosition(2*pixel_factor + xSize*pixel_factor + 1.4*pixel_factor, 1.5*pixel_factor);
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
         if (k.length() > 0)
         {
            int posp1 = k.find('.');
            int posp2 = k.find('.',posp1 + 1);
            
            short r = stoi(k.substr(0,posp1));
            short g = stoi(k.substr(posp1 + 1,  posp2 - posp1 - 1));
            short b = stoi(k.substr(posp2 + 1,  k.length() - posp2 - 1));
            pixel.setFillColor(sf::Color(r,g,b,255));
         } else {
            pixel.setFillColor(sf::Color::Black);
         }             
         window.draw(pixel);            
      }        
   }
}
void Display::DrawPoints(int argScore, int argRows, int argLevel)
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
   text.setString(k + "  of  " + to_string((level+1)*game.getRows2LU()));
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
}
Display::~Display()
{
}