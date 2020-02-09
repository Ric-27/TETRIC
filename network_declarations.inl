#ifndef NETWORK_UTILITIES
#define NETWORK_UTILITIES

#include <SFML/Network.hpp>

using namespace sf;
using namespace std;

const unsigned short server_port = 2000;
const unsigned short client_port = 2100;

const unsigned short search_time = 10;
const unsigned short connection_time = 5;

enum info_type{
    c2s_info_request,
    c2s_connection_request,
    s2c_info,
    s2c_connection_error,
    s2c_connection_sucess
};

enum status_type {
    error,
    readynt,
    connected,
    not_connected,
    new_player_info,
    changed,
    not_changed
};

struct player_info {
    IpAddress address;
    bool ready;
    string name;
    Uint32 score;
    vector<string> game;
    bool operator == (player_info other_player){
       return name == other_player.name;
    };
};
struct server_info{
    IpAddress address;
    string creator_name;
    Uint32 number_of_players_connected;
    vector<player_info> players;
    bool operator == (server_info other_server){
        return address == other_server.address;
    };
};

#endif