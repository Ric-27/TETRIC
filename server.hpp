#include "network_declarations.inl"
#include "config.inl"

class Server 
{
private:
    string creator_name;
    Uint32 players_connected;
    UdpSocket socket;
    IpAddress local_ip_address;
public:
    Server(/* args */);
    void Clients_Communication();
    void Connect();
    void Set_Creator_name(string);
    ~Server();
};