#include "server.hpp"

Server::Server():
    creator_name(""),
    players_connected(1),
    start_able(false)
{
    local_ip_address = IpAddress::getLocalAddress();
    players.resize(max_players);
    players[0] = player_info{local_ip_address, true};
    
    for (unsigned i = 0; i < max_players; i++)
    {
        players[i].game.resize(matrix_size);
        players[i].name = "";
        players[i].ready = true;
        for (int j = 0; j < matrix_size; j++)
        {
            players[i].game[j] = "";
        }
    }
}
void Server::Connect()
{
    if (socket.bind(server_port) != sf::Socket::Done)
    {
        cout << "Server: Connection error" << endl;
        return;
    }
}
void Server::Set_Creator_name(string argName, Logic& argGame)
{
    creator_name = argName;
    players[0].name = argName;
    for (unsigned i = 0; i < matrix_size; i++)
    {
        players[0].game[i] = argGame.getMatrix()[i];
    }
    players[0].score = argGame.getScore();
}
vector<player_info> Server::Get_Players()
{
    return players;
}
Uint32 Server::GetPlayers_Connected(){
    return players_connected;
}
void Server::Clients_Communication(status_type& argStatus)
{
    socket.setBlocking(false);
    Packet packet_recv;
    IpAddress sender;
    unsigned short port;
    string player_name;
    while(true){
        //Check if there is a message:
        if (socket.receive(packet_recv, sender, port) == Socket::Done)
        {
            //cout << "server recibio" << endl;
            Packet packet_send, all_packet_send;
            Uint32 infotype_value;
            //Get the buffer information:
            packet_recv >> infotype_value;            
            switch (infotype_value)
            {
                //Check if the message is a request for information:
                case c2s_info_request:
                {
                    packet_send << s2c_info;
                    packet_send << creator_name;
                    packet_send << players_connected;                  
                    //Send server information to client 
                    if (socket.send(packet_send, sender, client_port) != sf::Socket::Done) cout << "Server: Send error" << endl;
                    break;
                }
                case c2s_connection_request:
                {
                    cout << "here 1" << endl;
                    //Check if I can receive more players:
                    if(players_connected == 4){
                        //Buffer filling with error message:
                        packet_send << s2c_connection_error;
                    } else {
                        cout << "here 2" << endl;
                        //Adding the new client to the client list:
                        player_name = "";
                        packet_recv >> player_name;
                        player_info player_data{sender, false, player_name};

                        player_data.game.resize(matrix_size);

                        for(unsigned i = 0; i < matrix_size; i++){
                            string value = "";
                            packet_recv >> value;
                            player_data.game[i] = value;
                        }
                        cout << "here 3" << endl;
                        if(find(players.begin(), players.end(), player_data) == players.end())
                        {
                            cout << "player added" << endl;
                            players[players_connected] = player_data;
                            players_connected++;
                        }
                        cout << "here 4" << endl;
                        //Buffer filling with success message
                        packet_send << s2c_connection_sucess;
                        packet_send << (Uint32)players_connected;
                        cout << players_connected << endl;

                        all_packet_send << s2c_new_player_info;
                        all_packet_send << (Uint32)players_connected;
                        cout << "here 5" << endl;
                        for(unsigned i = 0; i < players_connected; i ++)
                        {
                            cout << "here 6" << endl;
                            packet_send << players[i].name;
                            cout << players[i].name << endl;
                            packet_send << players[i].ready;
                            cout << players[i].ready << endl;

                            all_packet_send << players[i];
                            /*
                            for(unsigned j = 0; j < matrix_size; j ++)
                            {
                                cout << j << " ";
                                packet_send << players[i].game[j];
                            }
                            cout << endl;
                            */
                        }
                        cout << "here 7" << endl;                   
                    }
                    //Send information to client
                    if (socket.send(packet_send, sender, client_port) != sf::Socket::Done)
                        cout << "Server: Send error" << endl;
                    
                    if(all_packet_send.getDataSize() > 0){
                        for(unsigned i = 0; i < players_connected; i ++)
                        {
                            if(players[i].name != player_name && players[i].name != creator_name)
                            {
                                cout << "entre donde no deberia" << endl;
                                if (socket.send(all_packet_send, players[i].address, client_port) != sf::Socket::Done)
                                {
                                    cout << "Server: Send error" << endl;
                                }
                            }
                        }
                        argStatus = changed;
                        cout << "New client " << sender << endl;
                    }               
                    break;
                }
                case c2s_ready:
                {    //Buffer filling with success message
                    cout << "rcv ready" << endl;
                    player_name = "";
                    packet_recv >> player_name;
                    vector<player_info>::iterator it = find(players.begin(), players.end(), player_info{sender, false, player_name});
                    if(it == players.end())
                        break;
                    (*it).ready = true;
                    packet_send << s2c_update_player;
                    packet_send << (Uint32)(it - players.begin());
                    packet_send << (*it);

                    for(unsigned i = 0; i < players_connected; i++){
                        if(players[i].name != creator_name)
                            if (socket.send(packet_send, players[i].address, client_port) != sf::Socket::Done)
                                cout << "Server: Send error" << endl;
                    }

                    packet_send.clear();
                    packet_send << s2c_ready_ok;
                    if (socket.send(packet_send, sender, client_port) != sf::Socket::Done)
                        cout << "Server: Send error" << endl;
                    
                    argStatus = changed;
                    cout << "Client " << sender << "is ready" << endl;
                    break;
                }
                case c2s_readynt:
                {
                    cout << "rcv not ready" << endl;
                    //Buffer filling with success message
                    player_name = "";
                    packet_recv >> player_name;
                    vector<player_info>::iterator it = find(players.begin(), players.end(), player_info{sender, false, player_name});
                    if(it == players.end())
                        break;
                    (*it).ready = false;
                    packet_send << s2c_update_player;
                    packet_send << (Uint32)(it - players.begin());
                    packet_send << (*it);
                    
                    for(unsigned i = 0; i < players_connected; i++){
                        if(players[i].name != creator_name)
                            if (socket.send(packet_send, players[i].address, client_port) != sf::Socket::Done)
                                cout << "Server: Send error" << endl;
                    }

                    packet_send.clear();
                    packet_send << s2c_readynt_ok;
                    if (socket.send(packet_send, sender, client_port) != sf::Socket::Done)
                        cout << "Server: Send error" << endl;
                    
                    argStatus = changed;
                    cout << "Client " << sender << "is not ready" << endl;
                    break;
                }                
                default:
                    break;
            }
        std::cout << "Received bytes from " << sender << " on port " << port << std::endl;
        }
    }
}
bool Server::Check_Start_ability(){
    bool canStart = true;
    if (players_connected == 1) canStart = false;
  
    for (unsigned i = 1; i < players_connected; i++) {
        if (!players[i].ready) {
            //std::cout << "Can't start a game, some players aren't ready!!!" << endl;
            canStart = false;
            break;
        }
    }
    return canStart;
}
void Server::Start() {
    if(Check_Start_ability())
    {
        Packet packet_send;

        packet_send << s2c_game_start;

        for (unsigned i = 0; i < players_connected; i++) 
        {
            if (players[i].name != creator_name)
                if (socket.send(packet_send, players[i].address, client_port) != sf::Socket::Done)
                    cout << "Server: Send error" << endl;
        }
    } else {
        cout << "can't start" << endl;
    }
}
void Server::Game_Communication(status_type& argStatus, Logic& argMatrix)
{
    cout << "server listening game" << endl;
    argStatus = changednt;
    socket.setBlocking(false);
    
    Packet packet_recv;
    IpAddress sender;
    unsigned short port;
    string player_name;

    chrono::time_point<chrono::system_clock> start = chrono::system_clock::now();
    chrono::duration<double> elapsed_seconds;
    
    while(true){
        elapsed_seconds = chrono::system_clock::now() - start;
        if(elapsed_seconds.count() >= update_time){
            start = chrono::system_clock::now();
            Send_Games(argMatrix);
        }
        //Check if there is a message:
        if (socket.receive(packet_recv, sender, port) == Socket::Done){
            //cout << "recibi" << endl;
            Uint32 infotype_value;
            //Get the buffer information:
            packet_recv >> infotype_value;
            
            switch ((unsigned) infotype_value)
            {
                case c2s_game_update:
                {
                    player_name = "";
                    packet_recv >> player_name;
                    //cout << player_name << endl;
                    vector<player_info>::iterator it = find(players.begin(), players.end(), player_info{sender, false, player_name});
                    if(it == players.end())
                    {
                        break;
                    }
                    packet_recv >> (*it).score;
                    cout << (*it).score << endl;
                    string value;
                    for(int i = 0; i < matrix_size; i++){
                            packet_recv >> value;
                            (*it).game[i] = value;
                    }
                }       
                    break;

                default:
                    break;
            }
        }
    }
}
void Server::Send_Games(Logic& argGame)
{
    //cout << "here 1" << endl;
    //Filling send buffer:
    Packet packet_send;
    
    packet_send << s2c_game_update;

    for(unsigned i = 0; i < players_connected; i ++){
        //cout << "here 1.1" << endl;
        if(players[i].name == creator_name)
        {
            players[i].score = argGame.getScore();
        }

        packet_send << players[i].name;
        packet_send << players[i].score;

        //cout << "here 1.2." << i << endl;
        for(int j = 0; j < matrix_size; j ++){
            if(players[i].name == creator_name)
            {
                players[i].game[j] = argGame.getMatrix()[j];
            }
            packet_send << players[i].game[j];
        }
        //cout << "here 1.3" << endl;
    }
    //cout << "here 2" << endl;
    
    for(unsigned i = 0; i < players_connected; i ++){
        if(players[i].name != creator_name)
        {
            //Sending board data message: 
            if (socket.send(packet_send, players[i].address, client_port) != sf::Socket::Done)
            {
                cout << "Client: Send error" << endl;
            }
        }   
    }
    //cout << "here 3" << endl;
}
Server::~Server()
{
}
