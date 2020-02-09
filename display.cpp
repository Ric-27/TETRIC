#include "display.hpp"

Display::Display(): 
   xSize(width),
   ySize(height + amount_of_pixels),
   pixel_factor(px_factor),
   window_width(screen_width),
   window_height(screen_height),
   window(sf::VideoMode(window_width * pixel_factor, window_height * pixel_factor), "TETRIC"),
   game(xSize, ySize)
{
   networking = nullptr;
   thread_runing = false;
   level = game.getLevel();
   sleepCont = 0;
   colorChanging = 0;
   colorAug = true;
   counterGravity = 0;
   player_Name = "";
   server_choosen = 0;
   reload = false;
   display_ready = false;
   my_status = not_changed;
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
            //Draw_Multiplayer_Join_Screen();
            //Draw_Loading();
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
         case 4:
            Draw_Multiplayer_Name_Screen();
            break;
         case 5:
            Draw_Multiplayer_Option_Screen();
            break;
         case 6: //lobby server
            if (!thread_runing)
            {
               host.Set_Creator_name(player_Name);
               host.Connect();
               networking = new Thread([&] () { host.Clients_Communication(my_status);});
               networking->launch();
               thread_runing = true;
            }
            Draw_Multiplayer_Create_Screen();
            break;
         case 7: //server list
            if (!thread_runing)
            {
               player.Set_Name(player_Name);
               player.Connect();
               networking = new Thread([&] () {player.Fill_server_list();});
               networking->launch();
               thread_runing = true;
            }
            if (reload)
            {
               networking->terminate();
               networking->launch();
               reload = false;
            }            
            Draw_Multiplayer_Join_Screen();
            break;
         case 8:
            Draw_Lobby();   
            break;
         default:
            break;
      }
      window.display();
   }
}
void Display::Events()
{
   while (window.pollEvent(event))
   {
      if (event.type == sf::Event::Closed) window.close();
      if (event.type == sf::Event::KeyPressed) {
         switch (game.getStatus())
         {
            case 0: //tittle screen
               switch (event.key.code)
               {
               case sf::Keyboard::S: //Start Game
                  game.setStatus(1);
                  break;
               case sf::Keyboard::Q: //Quit
                  window.close();
                  break;
               default:
                  break;
               }
               break;

            case 1: //Running
               switch (event.key.code)
               {
                  case sf::Keyboard::Q: //Quit
                     window.close();
                     break;
                  case sf::Keyboard::P: //Pause
                     game.setStatus(2);
                     break;
                  case sf::Keyboard::M: //Multiplayer
                     game.setStatus(4);
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
               break;

            case 2: //Pause
               switch (event.key.code)
               {
                  case sf::Keyboard::P: //Pause
                     game.setStatus(1);
                     break;
                  case sf::Keyboard::Q: //Quit
                     window.close();
                     break;
                  default:
                     break;
               }
               break;

            case 3: //gameover
               switch (event.key.code)
               {
                  case sf::Keyboard::R: //Restart
                     level = 0;
                     game.CleanUp();              
                     break;

                  case sf::Keyboard::Q: //Quit
                     window.close();
                     break;
                  
                  default:
                     break;
               }
               break;

            case 4: //writing name
               Writing();
               break;
            
            case 5: //multiplayer screen
               switch (event.key.code)
               {
                  case sf::Keyboard::C: //Create Server
                     game.setStatus(6);
                     break;

                  case sf::Keyboard::J: //Join Server
                     game.setStatus(7);
                     break;

                  case sf::Keyboard::Q: //Quit
                     window.close();
                     break;

                  case sf::Keyboard::Escape: 
                     game.setStatus(4);
                     break;

                  default:
                     break;
               }
               break;
            
            case 6: //host server screen
               switch (event.key.code)
                  {
                  case sf::Keyboard::Q: //Quit
                     window.close();
                     break;

                  case sf::Keyboard::Escape: 
                     game.setStatus(5);
                     break;

                  default:
                     break;
                  }
               break;

            case 7: //join server screen
               switch (event.key.code)
               {
                  case sf::Keyboard::Q: //Quit
                     window.close();
                     break;

                  case sf::Keyboard::Escape: //back
                     game.setStatus(5);
                     break;

                  case sf::Keyboard::Return: //back
                     if(server_choosen != 0)
                     {
                        window.clear();
                        Draw_Loading();
                        window.display();
                        networking->terminate();
                        player.Selected_Server(server_choosen,my_status);
                        game.setStatus(8);
                     }
                     break;

                  case sf::Keyboard::R: //reload 
                     reload = true;           
                     break;

                  case sf::Keyboard::Num1: 
                     if(player.Get_server_list().size() >= 1) server_choosen = 1;
                     break;
                  
                  case sf::Keyboard::Num2: 
                     if(player.Get_server_list().size() >= 2) server_choosen = 2;
                     break;

                  case sf::Keyboard::Num3: 
                     if(player.Get_server_list().size() >= 3) server_choosen = 3;
                     break;

                  case sf::Keyboard::Num4: 
                     if(player.Get_server_list().size() >= 4) server_choosen = 4;
                     break;

                  case sf::Keyboard::Num5: 
                     if(player.Get_server_list().size() >= 5) server_choosen = 5;
                     break;

                  case sf::Keyboard::Num6: 
                     if(player.Get_server_list().size() >=6) server_choosen = 6;
                     break;

                  case sf::Keyboard::Num7: 
                     if(player.Get_server_list().size() >= 7) server_choosen = 7;
                     break;

                  case sf::Keyboard::Num8: 
                     if(player.Get_server_list().size() >= 8) server_choosen = 8;
                     break;

                  default:
                     break;
               }
               break;
            case 8: //Lobby
               switch (event.key.code)
               {
                  case sf::Keyboard::Escape: //back
                     reload = true;
                     game.setStatus(7);
                     break;
                  
                  case sf::Keyboard::Q: //Quit
                     window.close();
                     break;
                  
                  case sf::Keyboard::Return: //change status
                  {
                     display_ready = !display_ready;
                     break;
                  }
                  default:
                     break;
               }
               break;
            default:
               break;
         }
      }
   } 
}
void Display::Writing()
{
   if (event.type == sf::Event::KeyPressed && player_Name.length() < 10) {
      switch (event.key.code)
      {
      case sf::Keyboard::Q:
         player_Name += "Q";
         break;
      case sf::Keyboard::W:
         player_Name += "W";
         break;
      case sf::Keyboard::E:
         player_Name += "E";
         break;
      case sf::Keyboard::R:
         player_Name += "R";
         break;
      case sf::Keyboard::T:
         player_Name += "T";
         break;
      case sf::Keyboard::Y:
         player_Name += "Y";
         break;
      case sf::Keyboard::U:
         player_Name += "U";
         break;
      case sf::Keyboard::I:
         player_Name += "I";
         break;
      case sf::Keyboard::O:
         player_Name += "O";
         break;
      case sf::Keyboard::P:
         player_Name += "P";
         break;
      case sf::Keyboard::A:
         player_Name += "A";
         break;
      case sf::Keyboard::S:
         player_Name += "S";
         break;
      case sf::Keyboard::D:
         player_Name += "D";
         break;
      case sf::Keyboard::F:
         player_Name += "F";
         break;
      case sf::Keyboard::G:
         player_Name += "G";
         break;
      case sf::Keyboard::H:
         player_Name += "H";
         break;
      case sf::Keyboard::J:
         player_Name += "J";
         break;
      case sf::Keyboard::K:
         player_Name += "K";
         break;
      case sf::Keyboard::L:
         player_Name += "L";
         break;
      case sf::Keyboard::Z:
         player_Name += "Z";
         break;
      case sf::Keyboard::X:
         player_Name += "X";
         break;
      case sf::Keyboard::C:
         player_Name += "C";
         break;
      case sf::Keyboard::V:
         player_Name += "V";
         break;
      case sf::Keyboard::B:
         player_Name += "B";
         break;
      case sf::Keyboard::N:
         player_Name += "N";
         break;
      case sf::Keyboard::M:
         player_Name += "M";
         break;
      default:
         break;
      }
   }
   if (event.type == sf::Event::KeyPressed) {
      switch (event.key.code)
      {
      case sf::Keyboard::BackSpace:
         if (player_Name.length() > 0)
         {
            player_Name.erase(player_Name.end()-1);
         }
         break;

      case sf::Keyboard::Escape:
         game.setStatus(1);
         break;
      case sf::Keyboard::Return:
         if (player_Name.length() > 0)
         {
            game.setStatus(5);
         }
         break;
      default:
         break;
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
void Display::Draw_Loading()
{
   sf::Font font;
   if (!font.loadFromFile("font.ttf"))
   {
      cout << "font load error..." << endl;
   }
   sf::Text text;
   text.setFont(font);
   text.setString("Loading");
   text.setCharacterSize(pixel_factor * 7);
   text.setFillColor(sf::Color(legend_text_color,legend_text_color,legend_text_color,255));
   text.setPosition((window_width/2 * pixel_factor) - (12.5*pixel_factor), (5*pixel_factor));
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
   for (int j = amount_of_pixels; j < ySize; j++)
   {
      for (int i = 0; i < xSize; i++)
      {
         sf::RectangleShape pixel(sf::Vector2f(pixel_factor, pixel_factor));
         pixel.setOutlineThickness(-1.f);
         pixel.setOutlineColor(sf::Color(outline_background_color,outline_background_color,outline_background_color,255));
         pixel.setPosition((i + 1) *pixel_factor, (j - (amount_of_pixels - 1))*pixel_factor);
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
            pixel.setFillColor(sf::Color(background_game_color,background_game_color,background_game_color,255));
         }             
         window.draw(pixel);            
      }        
   }
}
void Display::DrawPlayers(vector<string> argMatrixP1,int argScoreP1,vector<string> argMatrixP2,int argScoreP2,vector<string> argMatrixP3,int argScoreP3)
{
   int local_PF = pixel_factor / 2.5;
   for (int j = amount_of_pixels; j < ySize; j++)
   {
      for (int i = 0; i < xSize; i++)
      {
         sf::RectangleShape pixel(sf::Vector2f(local_PF, local_PF));
         pixel.setPosition(((i + 1) *local_PF)+20.6*pixel_factor, ((j - amount_of_pixels)*local_PF)+1*pixel_factor);
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
            pixel.setFillColor(sf::Color(background_game_color,background_game_color,background_game_color,255));
         }             
         window.draw(pixel);            
      }        
   }
   for (int j = amount_of_pixels; j < ySize; j++)
   {
      for (int i = 0; i < xSize; i++)
      {
         sf::RectangleShape pixel(sf::Vector2f(local_PF, local_PF));
         pixel.setPosition(((i + 1) *local_PF)+26.6*pixel_factor, ((j - amount_of_pixels)*local_PF)+1*pixel_factor);
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
            pixel.setFillColor(sf::Color(background_game_color,background_game_color,background_game_color,255));
         }             
         window.draw(pixel);            
      }    
   }
   for (int j = amount_of_pixels; j < ySize; j++)
   {
      for (int i = 0; i < xSize; i++)
      {
         sf::RectangleShape pixel(sf::Vector2f(local_PF, local_PF));
         pixel.setPosition(((i + 1) *local_PF)+32.6*pixel_factor, ((j - amount_of_pixels)*local_PF)+1*pixel_factor);
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
            pixel.setFillColor(sf::Color(background_game_color,background_game_color,background_game_color,255));
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
   text.setFillColor(sf::Color::White);

   text.setString("TETRIC");
   text.setCharacterSize(pixel_factor * 10);
   text.setPosition((window_width/2 * pixel_factor) - (15.5*pixel_factor), (-2*pixel_factor));
   window.draw(text);

   text.setString("Tetris   by   Ricardo  Rico");
   text.setCharacterSize(pixel_factor*2);
   text.setPosition((7.8*pixel_factor), (8.2*pixel_factor));
   window.draw(text);

   text.setString("IN204     ENSTA PARIS");
   text.setCharacterSize(pixel_factor*1.5);
   text.setPosition((12.5*pixel_factor),(10.5*pixel_factor));
   window.draw(text);

   text.setString("2020");
   text.setCharacterSize(pixel_factor*1.5);
   text.setPosition((17.5*pixel_factor),(12.5*pixel_factor));
   window.draw(text);

   text.setString("Press    S    to  play");
   text.setCharacterSize(pixel_factor*4);
   text.setFillColor(sf::Color(255,255,255,colorChanging));
   text.setPosition((3.9*pixel_factor),(17.2*pixel_factor));
   window.draw(text);
}
void Display::Draw_Multiplayer_Name_Screen()
{
   ChangeColor();   
   sf::Font font;
   if (!font.loadFromFile("font.ttf"))
   {
      cout << "font load error..." << endl;
   }
   sf::Text text;
   text.setFont(font);

   text.setString("MULTIPLAYER");
   text.setCharacterSize(pixel_factor * 6);
   text.setFillColor(sf::Color(legend_text_color,legend_text_color,legend_text_color,255));
   text.setPosition((1.5 * pixel_factor), (-1.5*pixel_factor));
   window.draw(text);
   if (player_Name == "")
   {
      text.setString("Your name");
      text.setPosition((7.5 * pixel_factor), (6*pixel_factor));
   } else {
      text.setString(player_Name);
      text.setPosition(((window_width/2 - 2 - player_Name.length()) * pixel_factor), (6*pixel_factor));
   }   
   text.setCharacterSize(pixel_factor * 5);
   text.setFillColor(sf::Color(legend_text_color,legend_text_color,legend_text_color,colorChanging));
   window.draw(text);

   text.setCharacterSize(pixel_factor * 1.5);
   text.setFillColor(sf::Color(legend_text_color,legend_text_color,legend_text_color,255));
   
   text.setString("esc     back");
   text.setPosition((0.5* pixel_factor), ((window_height - 2)*pixel_factor));
   window.draw(text);

   text.setString("enter     ok");
   text.setPosition(((window_width - 7.5)* pixel_factor), ((window_height - 2)*pixel_factor));
   window.draw(text);
}
void Display::Draw_Multiplayer_Create_Screen()
{
   ChangeColor();   
   sf::Font font;
   if (!font.loadFromFile("font.ttf"))
   {
      cout << "font load error..." << endl;
   }
   sf::Text text;
   text.setFont(font);

   text.setString("MULTIPLAYER");
   text.setCharacterSize(pixel_factor * 6);
   text.setFillColor(sf::Color(legend_text_color,legend_text_color,legend_text_color,colorChanging));
   text.setPosition((1.5 * pixel_factor), (-1.5*pixel_factor));
   window.draw(text);

   text.setCharacterSize(pixel_factor * 2);
   text.setFillColor(sf::Color(legend_text_color,legend_text_color,legend_text_color,255));
   
   text.setString("Player  Name");
   text.setPosition((2 * pixel_factor), (6*pixel_factor));
   window.draw(text);
   
   text.setString("status");
   text.setPosition((20 * pixel_factor), (6*pixel_factor));
   window.draw(text);

   text.setCharacterSize(pixel_factor * 1.5);

   for (unsigned i = 0; i < host.Get_Players().size(); i++)
   {
      text.setString(host.Get_Players()[i].name);
      text.setPosition((2 * pixel_factor), ((8 + (i * 1.5))*pixel_factor));
      window.draw(text);

      if (host.Get_Players()[i].ready == true)
      {
         text.setString("ready");
      } else {
         text.setString("readynt");
      }     
      text.setPosition((20 * pixel_factor), ((8 + (i * 1.5))*pixel_factor));
      window.draw(text);
   }

   text.setCharacterSize(pixel_factor * 1.5);
   text.setFillColor(sf::Color(legend_text_color,legend_text_color,legend_text_color,255));
   
   text.setString("esc     back");
   text.setPosition((0.5* pixel_factor), ((window_height - 2)*pixel_factor));   
   window.draw(text);

   text.setString("enter     start");
   text.setPosition(((window_width - 10.5)* pixel_factor), ((window_height - 2)*pixel_factor));
   window.draw(text);
}
void Display::Draw_Lobby()
{
   ChangeColor();   
   sf::Font font;
   if (!font.loadFromFile("font.ttf"))
   {
      cout << "font load error..." << endl;
   }
   sf::Text text;
   text.setFont(font);

   text.setString("MULTIPLAYER");
   text.setCharacterSize(pixel_factor * 6);
   text.setFillColor(sf::Color(legend_text_color,legend_text_color,legend_text_color,colorChanging));
   text.setPosition((1.5 * pixel_factor), (-1.5*pixel_factor));
   window.draw(text);

   text.setCharacterSize(pixel_factor * 2);
   text.setFillColor(sf::Color(legend_text_color,legend_text_color,legend_text_color,255));
   
   text.setString("Player  Name");
   text.setPosition((2 * pixel_factor), (6*pixel_factor));
   window.draw(text);
   
   text.setString("status");
   text.setPosition((20 * pixel_factor), (6*pixel_factor));
   window.draw(text);

   text.setCharacterSize(pixel_factor * 1.5);

   for (unsigned i = 0; i < player.Get_playing_server().players.size(); i++)
   {
      text.setString(player.Get_playing_server().players[i].name);
      text.setPosition((2 * pixel_factor), ((8 + (i * 1.5))*pixel_factor));
      window.draw(text);

      if (player.Get_playing_server().players[i].ready == true)
      {
         text.setString("ready");
      } else {
         text.setString("readynt");
      }     
      text.setPosition((20 * pixel_factor), ((8 + (i * 1.5))*pixel_factor));
      window.draw(text);
   }
   
   text.setCharacterSize(pixel_factor * 1.5);
   text.setFillColor(sf::Color(legend_text_color,legend_text_color,legend_text_color,255));
   
   text.setString("esc     back");
   text.setPosition((0.5* pixel_factor), ((window_height - 2)*pixel_factor));   
   window.draw(text);

   if (!display_ready)
   {
      text.setString("enter     ready");
      text.setPosition(((window_width - 10.5)* pixel_factor), ((window_height - 2)*pixel_factor));
   } else {
      text.setString("enter     readynt");
      text.setPosition(((window_width - 12.5)* pixel_factor), ((window_height - 2)*pixel_factor));
   }   
   window.draw(text);
}
void Display::Draw_Multiplayer_Join_Screen()
{
   ChangeColor();   
   sf::Font font;
   if (!font.loadFromFile("font.ttf"))
   {
      cout << "font load error..." << endl;
   }
   sf::Text text;
   text.setFont(font);

   text.setString("MULTIPLAYER");
   text.setCharacterSize(pixel_factor * 6);
   text.setFillColor(sf::Color(legend_text_color,legend_text_color,legend_text_color,colorChanging));
   text.setPosition((1.5 * pixel_factor), (-1.5*pixel_factor));
   window.draw(text);
   
   text.setFillColor(sf::Color(legend_text_color,legend_text_color,legend_text_color,255));
   
   text.setCharacterSize(pixel_factor * 1);

   text.setString("press   to");
   text.setPosition((1 * pixel_factor), (6.4*pixel_factor));
   window.draw(text);
   text.setString("select");
   text.setPosition((1.5 * pixel_factor), (7*pixel_factor));
   window.draw(text);
   
   text.setCharacterSize(pixel_factor * 2);

   text.setString("Server  Name");
   text.setPosition((7 * pixel_factor), (6*pixel_factor));
   window.draw(text);
   
   text.setString("players");
   text.setPosition((28 * pixel_factor), (6*pixel_factor));
   window.draw(text);

   if(server_choosen > 0)
   {
      sf::RectangleShape pixel(sf::Vector2f(35 * pixel_factor, 1.5 * pixel_factor));
      pixel.setOutlineThickness(-5.f);
      pixel.setOutlineColor(sf::Color(legend_text_color,legend_text_color,legend_text_color,colorChanging));
      pixel.setPosition(1*pixel_factor, (8.2 + (server_choosen - 1)*1.5)*pixel_factor);
      pixel.setFillColor(sf::Color(background_game_color,background_game_color,background_game_color,255));  
      window.draw(pixel);
   }

   text.setCharacterSize(pixel_factor * 1.5);

   for (unsigned i = 0; i < player.Get_server_list().size(); i++)
   {
      text.setString(to_string(i + 1));
      text.setPosition((2.5 * pixel_factor), ((8 + (i * 1.5))*pixel_factor));
      window.draw(text);

      text.setString("server   of   " + player.Get_server_list()[i].creator_name);
      text.setPosition((7 * pixel_factor), ((8 + (i * 1.5))*pixel_factor));
      window.draw(text);

      text.setString(to_string(player.Get_server_list()[i].number_of_players_connected) + " of 4");
      text.setPosition((28 * pixel_factor), ((8 + (i * 1.5))*pixel_factor));
      window.draw(text);

      if(i == 8) break;
   }

   text.setCharacterSize(pixel_factor * 1.5);
   text.setFillColor(sf::Color(legend_text_color,legend_text_color,legend_text_color,255));
   
   text.setString("esc     back");
   text.setPosition((0.5* pixel_factor), ((window_height - 2)*pixel_factor));
   window.draw(text);

   text.setString("R     Reload");
   text.setPosition(((window_width/2 - 4.5)* pixel_factor), ((window_height - 2)*pixel_factor));
   window.draw(text);

   text.setString("enter     select");
   text.setPosition(((window_width - 10.5)* pixel_factor), ((window_height - 2)*pixel_factor));
   window.draw(text);
}
void Display::Draw_Multiplayer_Option_Screen()
{
   ChangeColor();
   sf::Font font;
   if (!font.loadFromFile("font.ttf"))
   {
      cout << "font load error..." << endl;
   }
   sf::Text text;
   text.setFont(font);

   text.setString("MULTIPLAYER");
   text.setCharacterSize(pixel_factor * 6);
   text.setFillColor(sf::Color(legend_text_color,legend_text_color,legend_text_color,colorChanging));
   text.setPosition((1.5 * pixel_factor), (-1.5*pixel_factor));
   window.draw(text);

   text.setCharacterSize(pixel_factor * 3);
   text.setFillColor(sf::Color(legend_text_color,legend_text_color,legend_text_color,255));
   
   text.setString("C      create  server");
   text.setPosition((8 * pixel_factor), (8*pixel_factor));
   window.draw(text);

   text.setString("J      join  server");
   text.setPosition((8 * pixel_factor), (11*pixel_factor));
   window.draw(text);

   text.setCharacterSize(pixel_factor * 1.5);
   text.setPosition((0.5* pixel_factor), ((window_height - 2)*pixel_factor));
   
   text.setString("esc     back");
   text.setFillColor(sf::Color(legend_text_color,legend_text_color,legend_text_color,255));
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
         pixel.setOutlineColor(sf::Color(outline_background_color,outline_background_color,outline_background_color,255));
         pixel.setPosition(i*pixel_factor, j*pixel_factor);
         pixel.setFillColor(sf::Color(background_color,background_color,background_color,255));
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
   text.setFillColor(sf::Color(legend_text_color,legend_text_color,legend_text_color,255));

   text.setString("Press      to");   
   text.setPosition((21*pixel_factor), (11*pixel_factor));
   window.draw(text);

   text.setCharacterSize(pixel_factor * 1.5);

   text.setString("right                        move  right");   
   text.setPosition((21*pixel_factor), (13.5*pixel_factor));
   window.draw(text);

   text.setString("left                            move  left");   
   text.setPosition((21*pixel_factor), (14.5*pixel_factor));
   window.draw(text);

   text.setString("up                                    rotate");   
   text.setPosition((21*pixel_factor), (15.5*pixel_factor));
   window.draw(text);

   text.setString("down                           soft drop");   
   text.setPosition((21*pixel_factor), (16.5*pixel_factor));
   window.draw(text);

   text.setString("space                       hard drop");   
   text.setPosition((21*pixel_factor), (17.5*pixel_factor));
   window.draw(text);

   text.setString("p                                        pause");   
   text.setPosition((21*pixel_factor), (18.5*pixel_factor));
   window.draw(text);

   text.setString("M                                        Multiplayer");   
   text.setPosition((21*pixel_factor), (19.5*pixel_factor));
   window.draw(text);

   text.setString("Q                                        quit");   
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
   text.setCharacterSize(pixel_factor * 1.5);
   text.setString("NEXT");
   text.setPosition(2*pixel_factor + xSize*pixel_factor + 1.4*pixel_factor, 1.5*pixel_factor);
   window.draw(text);
   for (int j = 0; j < 4; j++)
   {
      for (int i = 0; i < 6; i++)
      {
         sf::RectangleShape pixel(sf::Vector2f(pixel_factor, pixel_factor));
         pixel.setOutlineThickness(-1.f);
         pixel.setOutlineColor(sf::Color(outline_game_color,outline_game_color,outline_game_color,255));
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
            pixel.setFillColor(sf::Color(background_game_color,background_game_color,background_game_color,255));
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
      pixel.setOutlineColor(sf::Color(outline_background_color,outline_background_color,outline_background_color,255));
      pixel.setPosition((i + 2 + xSize) *pixel_factor, (10)*pixel_factor);
      pixel.setFillColor(sf::Color(background_game_color,background_game_color,background_game_color,255));            
      window.draw(pixel);            
   }
   for (int i = 0; i < 6; i++)
   {
      sf::RectangleShape pixel(sf::Vector2f(pixel_factor, pixel_factor));
      pixel.setOutlineThickness(-1.f);
      pixel.setOutlineColor(sf::Color(outline_background_color,outline_background_color,outline_background_color,255));
      pixel.setPosition((i + 2 + xSize) *pixel_factor, (14)*pixel_factor);
      pixel.setFillColor(sf::Color(background_game_color,background_game_color,background_game_color,255));            
      window.draw(pixel);            
   }
   for (int i = 0; i < 6; i++)
   {
      sf::RectangleShape pixel(sf::Vector2f(pixel_factor, pixel_factor));
      pixel.setOutlineThickness(-1.f);
      pixel.setOutlineColor(sf::Color(outline_background_color,outline_background_color,outline_background_color,255));
      pixel.setPosition((i + 2 + xSize) *pixel_factor, (18)*pixel_factor);
      pixel.setFillColor(sf::Color(background_game_color,background_game_color,background_game_color,255));            
      window.draw(pixel);            
   }
   sf::Font font;
   if (!font.loadFromFile("font.ttf"))
   {
      cout << "font load error..." << endl;
   }
   sf::Text text;
   text.setFont(font);
   text.setCharacterSize(pixel_factor * 1.5);
   text.setFillColor(sf::Color(score_text_color,score_text_color,score_text_color,255));

   text.setString("SCORE");
   text.setPosition(2*pixel_factor + xSize*pixel_factor + 0.9*pixel_factor, pixel_factor*9 - pixel_factor/2);
   window.draw(text);
   string k = to_string(argScore);
   text.setString(k);
   text.setPosition(2*pixel_factor + xSize*pixel_factor, pixel_factor*10 - pixel_factor/2);
   window.draw(text);

   text.setString("ROWS");
   text.setPosition(2*pixel_factor + xSize*pixel_factor + 1.3*pixel_factor, pixel_factor*13 - pixel_factor/2);
   window.draw(text);
   k = to_string(argRows);
   text.setString(k + "  of  " + to_string((level+1)*rows_to_level));
   text.setPosition(2*pixel_factor + xSize*pixel_factor, pixel_factor*14 - pixel_factor/2);
   window.draw(text);

   text.setString("LEVEL");
   text.setPosition(2*pixel_factor + xSize*pixel_factor + 0.9*pixel_factor, pixel_factor*17 - pixel_factor/2);
   window.draw(text);
   k = to_string(argLevel);
   text.setString(k);
   text.setPosition(2*pixel_factor + xSize*pixel_factor, pixel_factor*18 - pixel_factor/2);
   window.draw(text);
}
Display::~Display()
{
}