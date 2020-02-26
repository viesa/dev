#pragma once

#define DEBUG

#include <SFML/Network.hpp>
#include <deque>
#include <vector>
#include <map>
#include <thread>

class Server
{
public:
    Server();
    Server(int const &port);
    ~Server();

    int AcceptConnections();
    void RejectConnections();

    void Send(sf::Packet const &packet, sf::TcpSocket *client);
    void Send(void *data, size_t size, sf::TcpSocket *client);
    void Send(std::string const &string, sf::TcpSocket *client);
    void BroadCast(sf::Packet const &packet);
    void BroadCast(void *data, size_t size);
    void BroadCast(std::string const &string);

    sf::Packet PopFront();
    sf::Packet PopBack();

private:
    void Mgr();

private:
    int m_port;

    sf::SocketSelector m_socketSelector;
    sf::TcpListener m_listener;
    std::vector<sf::TcpSocket *> m_clients;
    std::deque<std::pair<sf::TcpSocket *, sf::Packet>> m_pendingReceiving;
    std::deque<std::pair<sf::TcpSocket *, sf::Packet>> m_pendingSending;

    bool m_active;
    bool m_accepting;
    bool m_receiving;
    bool m_sending;

    std::thread m_worker;
};