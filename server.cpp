#include "server.hpp"

Server::Server(/* args */)
{
    players_connected = 1;
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
}

void Server::Clients_Communication()
{
    socket.setBlocking(false);
    Packet packet_recv;
    IpAddress sender;
    unsigned short port;

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
            //Check if the message is a request for information:
            case c2s_info_request:
                packet_send << s2c_info;
                packet_send << creator_name;
                packet_send << players_connected;                  
                //Send server information to client 
                if (socket.send(packet_send, sender, client_port) != sf::Socket::Done) cout << "Server: Send error" << endl;
                break;
            }
        }
    }
}

Server::~Server()
{
}
