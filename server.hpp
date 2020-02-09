#include "network_declarations.inl"
#include "config.inl"

class Server 
{
private:
    string creator_name;
    Uint32 players_connected;
    UdpSocket socket;
    vector<player_info> players;
    IpAddress local_ip_address;
public:
    Server();
    vector<player_info> Get_Players();
    void Clients_Communication(status_type&);
    void Connect();
    void Set_Creator_name(string);
    ~Server();
};