#ifndef NETWORK_UTILITIES_COMPLEMENT
#define NETWORK_UTILITIES_COMPLEMENT

#include "network_declarations.inl"

Packet& operator <<(sf::Packet& packet, player_info& argPlayerInfo)
{
    return packet << argPlayerInfo.address.toString()
        << argPlayerInfo.ready
        << argPlayerInfo.name
        << argPlayerInfo.score;
}

Packet& operator >>(sf::Packet& packet, player_info& argPlayerInfo)
{
    string player_address;
    packet >> player_address;
    argPlayerInfo.address = player_address;

    return packet >> argPlayerInfo.ready
        >> argPlayerInfo.name
        >> argPlayerInfo.score;
}

#endif