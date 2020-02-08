#include "config.inl"
#include "network_declarations.inl"

class Client
{
private:
    string name;
    vector<server_info> server_list;
    UdpSocket socket;
public:
    Client(/* args */);
    void Set_Name(string);
    void Fill_server_list();
    void Connect();
    vector<server_info> Get_server_list();
    ~Client();
};