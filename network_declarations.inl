#ifndef NETWORK_UTILITIES
#define NETWORK_UTILITIES

#include <SFML/Network.hpp>

using namespace sf;
using namespace std;

const unsigned short server_port = 2000;
const unsigned short client_port = 2100;

const unsigned short search_time = 10;

enum info_type{
    c2s_info_request,
    s2c_info
};

struct server_info{
    IpAddress address;
    string creator_name;
    Uint32 number_of_players_connected;
    bool operator == (server_info other_server){
        return address == other_server.address;
    };
};

#endif