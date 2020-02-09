#include "client.hpp"

Client::Client(/* args */)
{
    name = "";
    status = false;
}

void Client::Connect()
{
    if (socket.bind(client_port) != sf::Socket::Done)
    {
        cout << "Server: Connection error" << endl;
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
        cout << "Client: Send error" << endl;
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
                    
                    cout << " address: " << server_list.back().address << ", SERVER OF " << server_list.back().creator_name << ", players connected: " << server_list.back().number_of_players_connected << endl;
                }
            }
        }
        elapsed_seconds = chrono::system_clock::now() - start;
        //Waiting for servers information for MAX_SEARCH_TIME
    } while(elapsed_seconds.count() <= search_time);
}

//Connect to a specific game server from the available list:
void Client::Selected_Server(const unsigned argServer, status_type& argStatus){
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
    //Sending server connection request: 
    if (socket.send(packet_send, server_list[argServer-1].address, server_port) != sf::Socket::Done)
    {
        argStatus = error;
        cout << "Client: Send error" << endl;
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
                    server_list.erase(server_list.begin() + argServer);
                    argStatus = error;
                    return;
                //Check if the message is a server connection response success:
                case s2c_connection_sucess:
                    playing_server.address = server_list[argServer].address;
                    packet_recv >> playing_server.number_of_players_connected;
                
                    for(unsigned i = 0; i < playing_server.number_of_players_connected; i ++){
                        string player_name;
                        bool player_status;
                        packet_recv >> player_name;
                        packet_recv >> player_status;
                        playing_server.players.emplace_back(player_info{"", player_status, player_name});
                    }
                    argStatus = connected;
                    cout << "Connected to the Server " << playing_server.address << endl;
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

Client::~Client()
{
}
