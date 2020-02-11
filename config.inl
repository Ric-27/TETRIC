#ifndef CONFIGURATION_PARAMETERS
#define CONFIGURATION_PARAMETERS

#include <string>
#include <vector>
#include <iostream>
#include <chrono>

using namespace std;
// TETROMINOS //
const int amount_of_pixels = 4;
const int variety = 8;
//colors
const string tetromino_o_color = "0.255.0";
const string tetromino_i_color = "255.0.0";
const string tetromino_l_color = "255.69.0";
const string tetromino_j_color = "0.128.128";
const string tetromino_s_color = "255.255.0";
const string tetromino_z_color = "0.255.127";
const string tetromino_t_color = "0.0.255";
const string tetromino_legendary = "138.43.226";
//grid
const int width = 10;
const int height = 20;

const int matrix_size = width*(height+amount_of_pixels);

// SCREEN //
//dimension
const int px_factor = 15;
const int screen_height = 22;
const int screen_width = 39;
//color
const string overlay_color = "169.169.169";
const int outline_game_color = 20;
const int background_game_color = 0;
const int background_color = 61;
const int outline_background_color = 20;
const int legend_text_color = 200;
const int score_text_color = 255;

// GAME //
//speed
const int initial_speed = 100;
const int step_speed = 15;
const int min_speed = 10;

//level-up
const int rows_to_level = 6;

//spawn special
const int count_spawn_special = 10;

#endif