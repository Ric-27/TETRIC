#ifndef SERVER
#define SERVER

#include "network_declarations.inl"
#include "config.inl"

#include "logic.hpp"

class Server 
{
private:
    string creator_name;
    Uint32 players_connected;
    UdpSocket socket;
    vector<player_info> players;
    IpAddress local_ip_address;
    bool start_able;
public:
    Server();
    vector<player_info> Get_Players();
    void Clients_Communication(status_type&);
    void Connect();
    Uint32 GetPlayers_Connected();
    void Set_Creator_name(string, Logic&);
    void Start();
    bool Check_Start_ability();
    void Send_Games(Logic&);
    void Game_Communication(status_type&,Logic&);
    ~Server();
};

#endif