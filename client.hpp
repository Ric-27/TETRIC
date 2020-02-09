#include "config.inl"
#include "network_declarations.inl"

class Client
{
private:
    string name;
    bool status;
    vector<server_info> server_list;
    server_info playing_server;
    UdpSocket socket;
public:
    Client(/* args */);
    void Set_Name(string);
    bool Get_Status();
    void Set_Status(bool);
    void Fill_server_list();
    void Connect();
    server_info Get_playing_server();
    vector<server_info> Get_server_list();
    void Selected_Server(const unsigned, status_type&);
    ~Client();
};