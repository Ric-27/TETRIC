#include "server.hpp"

Server::Server():
    creator_name(""),
    players_connected(1)
{
    local_ip_address = IpAddress::getLocalAddress();
    players.emplace_back(player_info{local_ip_address, true});
}

void Server::Connect()
{
    if (socket.bind(server_port) != sf::Socket::Done)
    {
        cout << "Server: Connection error" << endl;
        return;
    }
}

void Server::Set_Creator_name(string argName)
{
    creator_name = argName;
    players[0].name = argName;
    players[0].ready = true;
}

vector<player_info> Server::Get_Players()
{
    return players;
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
                    //Check if I can receive more players:
                    if(players.size() == 4){
                        //Buffer filling with error message:
                        packet_send << s2c_connection_error;
                    } else {
                        players_connected++;
                        //Adding the new client to the client list:
                        player_name = "";
                        packet_recv >> player_name;
                        player_info player_data{sender, false, player_name};
                        if(find(players.begin(), players.end(), player_data) == players.end())
                        {
                            players.emplace_back(player_data);
                        }
                        //Buffer filling with success message
                        packet_send << s2c_connection_sucess;
                        packet_send << (Uint32)players.size();

                        all_packet_send << s2c_new_player_info;
                        all_packet_send << (Uint32)players.size();
                    
                        for(unsigned i = 0; i < players.size(); i ++){
                            packet_send << players[i].name;
                            packet_send << players[i].ready;

                            all_packet_send << players[i];
                        }                    
                    }
                    //Send information to client
                    if (socket.send(packet_send, sender, client_port) != sf::Socket::Done)
                        cout << "Server: Send error" << endl;
                    
                    if(all_packet_send.getDataSize() > 0){
                        for(unsigned i = 0; i < players.size(); i ++){
                            if(players[i].address != sender && players[i].name != creator_name)
                                if (socket.send(all_packet_send, players[i].address, client_port) != sf::Socket::Done)
                                    cout << "Server: Send error" << endl;
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

                    for(unsigned i = 0; i < players.size(); i++){
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
                    
                    for(unsigned i = 0; i < players.size(); i++){
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

Server::~Server()
{
}
