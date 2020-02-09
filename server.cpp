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

    while(true){
        //Check if there is a message:
        if (socket.receive(packet_recv, sender, port) == Socket::Done)
        {
            Packet packet_send, all_packet_send;
            Uint32 infotype_value;
            //Get the buffer information:
            packet_recv >> infotype_value;            
            switch (infotype_value)
            {
            //Check if the message is a request for information:
            case c2s_info_request:
                packet_send << s2c_info;
                packet_send << creator_name;
                packet_send << players_connected;                  
                //Send server information to client 
                if (socket.send(packet_send, sender, client_port) != sf::Socket::Done) cout << "Server: Send error" << endl;
                break;
            case c2s_connection_request:
                //Check if I can receive more players:
                if(players.size() == 4){
                    //Buffer filling with error message:
                    packet_send << s2c_connection_error;
                } else {
                    //Adding the new client to the client list:
                    string player_name;
                    packet_recv >> player_name;
                    player_info player_data{sender, false, player_name};
                    if(find(players.begin(), players.end(), player_data) == players.end())
                    {
                        cout << "player added" << endl;
                        players.emplace_back(player_data);
                    }
                    //Buffer filling with success message
                    packet_send << s2c_connection_sucess;
                    packet_send << (Uint32)players.size();

                    all_packet_send << new_player_info;
                   
                    for(unsigned i = 0; i < players.size(); i ++){
                        packet_send << players[i].name;
                        packet_send << players[i].ready;

                        all_packet_send << players[i].name;
                        all_packet_send << players[i].ready;
                    }                    
                }
                //Send information to client
                if (socket.send(packet_send, sender, client_port) != sf::Socket::Done)
                    cout << "Server: Send error" << endl;
                
                if(all_packet_send.getDataSize() > 0){
                    for(unsigned i = 0; i < players.size(); i ++){
                        if(players[i].address != sender && players[i].address != local_ip_address)
                            if (socket.send(all_packet_send, players[i].address, client_port) != sf::Socket::Done)
                                cout << "Server: Send error" << endl;
                    }
                    argStatus = changed;
                    cout << "New client " << sender << endl;
                }               
                break;
            default:
                break;
            }
        }
    }
}

Server::~Server()
{
}
