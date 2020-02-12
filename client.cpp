#include "client.hpp"

Client::Client(/* args */)
{
    name = "";
    status = false;
    playing_server.players.resize(max_players);
    for (unsigned i = 0; i < max_players; i++)
    {
        playing_server.players[i].game.resize(matrix_size);
        playing_server.players[i].name = "";
        playing_server.players[i].ready = false;
        playing_server.players[i].score = 0;
        for (int j = 0; j < matrix_size; j++)
        {
            playing_server.players[i].game[j] = "";
        }        
    }
}

void Client::Connect()
{
    if (socket.bind(client_port) != sf::Socket::Done)
    {
        std::cout << "Client: Connection error" << endl;
        return;
    }
}
bool Client::Get_Status()
{
    return status;
}
void Client::Set_Status(bool argStatus)
{
    status = argStatus;
}

vector<server_info> Client::Get_server_list()
{
    return server_list;
}

void Client::Set_Name(string argName)
{
    name = argName;
}

server_info Client::Get_playing_server()
{
    return playing_server;
}

void Client::Fill_server_list()
{
    //Filling send buffer:
    Packet packet_send;
    //Server information request:
    packet_send << c2s_info_request;
    socket.setBlocking(true);

    //Sending broadcast message for search available servers: 
    if (socket.send(packet_send, IpAddress::Broadcast, server_port) != sf::Socket::Done)
    {
        std::cout << "Client: Send error!!" << endl;
        return;
    }

    chrono::time_point<chrono::system_clock> start = chrono::system_clock::now();
    chrono::duration<double> elapsed_seconds;
    //Setting non blocking socket:
    socket.setBlocking(false);
    
    do{
        Packet packet_recv;
        IpAddress sender;
        unsigned short port;
        //Check if there is a message:
        if (socket.receive(packet_recv, sender, port) == Socket::Done)
        {
            Uint32 infotype_value;
            //Get the buffer information:
            packet_recv >> infotype_value;
            //Check if the message is a response of server information:
            if((info_type) infotype_value == s2c_info){
                //Adding server information to server list:
                vector<server_info>::iterator it = find(server_list.begin(), server_list.end(), server_info{sender, "", 0});

                if(it == server_list.end()){
                    server_info server_info_recv;
                    
                    server_info_recv.address = sender;
                    packet_recv >> server_info_recv.creator_name;
                    packet_recv >> server_info_recv.number_of_players_connected;

                    server_list.emplace_back(server_info_recv);
                    
                    std::cout << " address: " << server_list.back().address << ", SERVER OF " << server_list.back().creator_name << ", players connected: " << server_list.back().number_of_players_connected << endl;
                }
            }
        }
        elapsed_seconds = chrono::system_clock::now() - start;
        //Waiting for servers information for MAX_SEARCH_TIME
    } while(elapsed_seconds.count() <= search_time);
}

//Connect to a specific game server from the available list:
void Client::Selected_Server(const unsigned argServer, status_type& argStatus, Logic& argGame){
    //Checking that the selected server is in the list:
    if(argServer > server_list.size()){
        argStatus = error;
        return;
    }

    argStatus = not_connected;
    //Filling send buffer:
    Packet packet_send;
    //Server connection request:
    packet_send << c2s_connection_request;
    packet_send << name;
    for(unsigned i = 0; i < argGame.getMatrix().size(); i ++){
        packet_send << argGame.getMatrix()[i];
    }
    //Sending server connection request: 
    if (socket.send(packet_send, server_list[argServer-1].address, server_port) != sf::Socket::Done)
    {
        argStatus = error;
        std::cout << "Client: Send error" << endl;
        return;
    }

    chrono::time_point<chrono::system_clock> start = chrono::system_clock::now();
    chrono::duration<double> elapsed_seconds;
    //Setting non blocking socket:
    socket.setBlocking(false);
    do{
        Packet packet_recv;
        IpAddress sender;
        unsigned short port;
        //Check if there is a message:
        if (socket.receive(packet_recv, sender, port) == Socket::Done)
        {
            //Checking that the address of the sender 
            //is the same as that of the selected server
            if(sender == server_list[argServer - 1].address){
                Uint32 infotype_value;
                //Get the buffer information:
                packet_recv >> infotype_value;
                switch (infotype_value)
                {
                //Check if the message is a server connection response error:
                case s2c_connection_error:
                    server_list.erase(server_list.begin() + (argServer - 1));
                    argStatus = error;
                    return;
                //Check if the message is a server connection response success:
                case s2c_connection_sucess:
                    playing_server.address = server_list[argServer - 1].address;

                    packet_recv >> playing_server.number_of_players_connected;
                
                    for(unsigned i = 0; i < playing_server.number_of_players_connected; i ++){
                        string player_name;
                        bool player_status;

                        packet_recv >> player_name;

                        packet_recv >> player_status;
                        playing_server.players[i].name = player_name;
                        playing_server.players[i].ready = player_status;
                    }
                    argStatus = connected;
                    std::cout << "Connected to the Server " << playing_server.address << endl;
                    return;
                }
            }
        }
        elapsed_seconds = chrono::system_clock::now() - start;
        //Waiting for server response for MAX_CONNECTION_TIME
    } while(elapsed_seconds.count() <= connection_time);

    argStatus = error;
    return;
}

void Client::Lobby_Communication(status_type& argStatus){
    argStatus = changednt;
    socket.setBlocking(false);
    
    Packet packet_recv;;
    IpAddress sender;
    unsigned short port;

    string player_name;
    while(true){
         //Check if there is a message:
        if (socket.receive(packet_recv, sender, port) == Socket::Done)
        {
            Packet packet_send;
            Uint32 infotype_value;
            //Get the buffer information:
            packet_recv >> infotype_value;
            switch (infotype_value)
            {
            case s2c_ready_ok:
            {    
                player_name = "";
                packet_recv >> player_name;
                vector<player_info>::iterator it = find(playing_server.players.begin(), playing_server.players.end(), player_info{IpAddress::getLocalAddress(), false, player_name});
                if(it == playing_server.players.end()){
                    argStatus = ready_error;
                    break;
                }
                    
                (*it).ready = true;
            
                std::cout << "Ready " << endl;
                argStatus = ready_ok;
                
                break;
            }
            case s2c_readynt_ok:
            {    
                player_name = "";
                packet_recv >> player_name;
                vector<player_info>::iterator it = find(playing_server.players.begin(), playing_server.players.end(), player_info{IpAddress::getLocalAddress(), false, player_name});
                if(it == playing_server.players.end()){
                    argStatus = readynt_error;
                    break;
                }
                    
                (*it).ready = false;
            
                std::cout << "Ready " << endl;
                argStatus = readynt_ok;
                
                break;
            }
            //Check if the message is of new client info:
            case s2c_new_player_info:
                Uint32 number_of_players;
                packet_recv >> number_of_players;
                playing_server.number_of_players_connected = number_of_players;
                for(unsigned i = 0; i < number_of_players; i ++){
                    player_info player_data;
                    packet_recv >> player_data;
                    playing_server.players[i] = player_data;
                }
                argStatus = changed;
            break;
            //Check if the message is of update client info:
            case s2c_update_player:
            {    
                unsigned pos_client;

                packet_recv >> pos_client;
                packet_recv >> playing_server.players[pos_client];

                std::cout << "Client update " << playing_server.players[pos_client].address << playing_server.players[pos_client].ready << endl;
                argStatus = changed;
                break;
            }
            //Check if the message is of delete client info:
            /*
            case DELETE_CLIENT_INFO:
                unsigned pos_client;
                packet_recv >> pos_client;
                playing_server.players.erase(playing_server.players.begin() + pos_client);
                argStatus = CHANGED;
            break;
            case SERVER_DISCONNECTION:
                playing_server.address = IpAddress::None;
                argStatus = SERVER_DISCONNECTED;
                return;
            */
            case s2c_game_start:
                argStatus = playing;
                return;
            
            default:
                break;
            }
            std::cout << "Received bytes from " << sender << " on port " << port << std::endl;
        }
    }
}

void Client::ready(bool argReady){
    //Filling send buffer:
    Packet packet_send;
    if(argReady)
    {
        packet_send << c2s_ready;
    } else {
        packet_send << c2s_readynt;
    }
    packet_send << name;
    //Sending ready/not ready client message:
    if (socket.send(packet_send, playing_server.address, server_port) != sf::Socket::Done)
    {
        std::cout << "Client: Send error" << endl;
    }
}

void Client::Game_Communication(status_type& argStatus, Logic& argMatrix){
    Send_Game(argMatrix);
    argStatus = changednt;
    socket.setBlocking(false);
    
    Packet packet_recv;;
    IpAddress sender;
    unsigned short port;

    chrono::time_point<chrono::system_clock> start = chrono::system_clock::now();
    chrono::duration<double> elapsed_seconds;
    
    //cout << "client listening" << endl;
    while(true){
        
        elapsed_seconds = chrono::system_clock::now() - start;
        if(elapsed_seconds.count() >= update_time){
            start = chrono::system_clock::now();
            Send_Game(argMatrix);
        }
        
        //Check if there is a message:
        
        if (socket.receive(packet_recv, sender, port) == Socket::Done)
        {
            //Get the buffer information:
            Uint32 infotype_value;

            packet_recv >> infotype_value;
            
            switch ((unsigned)infotype_value)
            {
                case s2c_game_update:
                    for(unsigned i = 0; i < playing_server.number_of_players_connected; i ++){
                        packet_recv >> playing_server.players[i].name;
                        packet_recv >> playing_server.players[i].score;
                        string value;
                        for (int j = 0; j < matrix_size; j++)
                        {
                            packet_recv >> value;
                            playing_server.players[i].game[j] = value;
                        }                 
                    }
                    argStatus = changed;
                    break;
                default:
                    break;
            }
        }
    }
}

void Client::Send_Game(Logic& argGame){
    //Filling send buffer:
    Packet packet_send;
    
    packet_send << c2s_game_update; 
    packet_send << name;
    packet_send << argGame.getScore();

    for(unsigned i = 0; i < argGame.getMatrix().size(); i ++){
        packet_send << argGame.getMatrix()[i];
    }
    //Sending board data message: 
    if (socket.send(packet_send, playing_server.address, server_port) != sf::Socket::Done)
    {
        cout << "Client: Send error" << endl;
    }
}

Client::~Client()
{
}
