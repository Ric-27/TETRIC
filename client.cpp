#include "client.hpp"

Client::Client(/* args */)
{
}

void Client::Connect()
{
    if (socket.bind(client_port) != sf::Socket::Done)
    {
        cout << "Server: Connection error" << endl;
        return;
    }
}

vector<server_info> Client::Get_server_list()
{
    return server_list;
}

void Client::Set_Name(string argName)
{
    name = argName;
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

Client::~Client()
{
}
