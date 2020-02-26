#pragma once

#define DEBUG

#include <SFML/Network.hpp>

#include <deque>
#include <thread>

class Client
{
public:
    Client();
    Client(sf::IpAddress const &hostIp, int const &port);
    ~Client();

    int Connect();
    void Disconnect();

    void Send(sf::Packet const &packet);
    void Send(void *data, size_t size);
    void Send(std::string const &string);

    sf::Packet PopFront();
    sf::Packet PopBack();

private:
    void Mgr();

private:
    sf::IpAddress m_ip;
    int m_port;

    sf::SocketSelector m_socketSelector;
    sf::TcpSocket *m_socket;

    std::deque<sf::Packet> m_pendingReceiving;
    std::deque<sf::Packet> m_pendingSending;

    bool m_active;
    bool m_receiving;
    bool m_sending;
    bool m_connected;

    std::thread m_worker;
};