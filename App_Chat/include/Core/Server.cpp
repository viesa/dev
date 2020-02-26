#include "Server.hpp"

#include <iostream>

Server::Server()
    : Server(4000)
{
}

Server::Server(int const &port)
    : m_port(port),
      m_active(true),
      m_accepting(false),
      m_receiving(true),
      m_sending(true),
      m_worker(&Server::Mgr, this)
{
}

Server::~Server()
{
    m_active = false;
    m_accepting = false;
    m_receiving = false;
    m_sending = false;

    for (auto &client : m_clients)
        client->disconnect();

    m_socketSelector.clear();

    if (m_worker.joinable())
        m_worker.join();

    for (auto &client : m_clients)
    {
        delete client;
        client = nullptr;
    }
}

int Server::AcceptConnections()
{
    if (m_listener.listen(m_port) == sf::Socket::Done)
    {
#ifdef DEBUG
        std::cout << "Info: Successfully started listener on server! Port: " << m_port << std::endl;
#endif
        m_socketSelector.add(m_listener);
        m_accepting = true;
        return 1;
    }
    else
    {
#ifdef DEBUG
        std::cerr << "Error: Failed to start listener on server! Port: " << m_port << std::endl;
#endif
        return 0;
    }
}

void Server::RejectConnections()
{
    m_listener.close();
    m_accepting = false;
}

void Server::Send(sf::Packet const &packet, sf::TcpSocket *client)
{
    m_pendingSending.push_back(std::make_pair(client, packet));
}

void Server::Send(void *data, size_t size, sf::TcpSocket *client)
{
    sf::Packet packet;
    packet.append(data, size);
    Send(packet, client);
}

void Server::Send(std::string const &string, sf::TcpSocket *client)
{
    sf::Packet packet;
    packet.append((void *)&string, string.length() + 1);
    Send(packet, client);
}

void Server::BroadCast(sf::Packet const &packet)
{
    for (auto &client : m_clients)
        Send(packet, client);
}

void Server::BroadCast(void *data, size_t size)
{
    for (auto &client : m_clients)
        Send(data, size, client);
}

void Server::BroadCast(std::string const &string)
{
    for (auto &client : m_clients)
        Send(string, client);
}

sf::Packet Server::PopFront()
{
    if (m_pendingReceiving.size() > 0)
    {
        sf::Packet ret = m_pendingReceiving.front().second;
        m_pendingReceiving.pop_front();
        return ret;
    }
    else
    {
        sf::Packet ret;
        ret.append("", 1);
        return ret;
    }
}

sf::Packet Server::PopBack()
{
    if (m_pendingReceiving.size() > 0)
    {
        sf::Packet ret = m_pendingReceiving.back().second;
        m_pendingReceiving.pop_back();
        return ret;
    }
    else
    {
        sf::Packet ret;
        ret.append("", 1);
        return ret;
    }
}

void Server::Mgr()
{
    while (m_active)
    {
        if (m_socketSelector.wait(sf::milliseconds(10)))
        {
            if (m_accepting && m_socketSelector.isReady(m_listener))
            {
                sf::TcpSocket *client = new sf::TcpSocket;
                if (m_listener.accept(*client) == sf::Socket::Done)
                {
#ifdef DEBUG
                    std::cout << "Info: Successfully accepted connection to client! IP: " << client->getRemoteAddress() << " Port: " << m_port << std::endl;
#endif
                    m_clients.push_back(client);
                    m_socketSelector.add(*client);
                }
                else
                {
#ifdef DEBUG
                    std::cerr << "Error: Failed to accept connection to client! IP: " << client->getRemoteAddress() << " Port: " << m_port << std::endl;
#endif
                    delete client;
                }
            }
            else if (m_receiving)
            {
                for (auto &client : m_clients)
                {
                    if (m_socketSelector.isReady(*client))
                    {
                        sf::Packet packet;
                        if (client->receive(packet) == sf::Socket::Done)
                        {

#ifdef DEBUG
                            std::cout << "Info: Successfully received packet from client! IP: " << client->getRemoteAddress() << " Port: " << m_port << std::endl;
#endif
                            m_pendingReceiving.push_back(std::make_pair(client, packet));
                        }
                        else
                        {
#ifdef DEBUG
                            std::cerr << "Error: Failed to receive packet from client! Did the client disconnect? IP: " << client->getRemoteAddress() << " Port: " << m_port << std::endl;
#endif
                        }
                    }
                }
            }
        }
        if (m_sending)
            while (m_pendingSending.size() > 0)
            {
                sf::TcpSocket *client = m_pendingSending.front().first;
                sf::Packet *packet = &m_pendingSending.front().second;

                if (client->send(*packet) == sf::Socket::Done)
                {
#ifdef DEBUG
                    std::cout << "Info: Successfully sent packet to server! IP: " << client->getRemoteAddress() << " Port: " << m_port << std::endl;
#endif
                    m_pendingSending.pop_front();
                }
                else
                {
#ifdef DEBUG
                    std::cerr << "Error: Failed to send packet to server! IP: " << client->getRemoteAddress() << " Port: " << m_port << std::endl;
#endif
                }
            }
    }
}