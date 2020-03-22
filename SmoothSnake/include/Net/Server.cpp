#include "Server.hpp"

#include <iostream>
#include <cstring>

#include "../Arth/Random.hpp"

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
        client.socket->disconnect();

    m_socketSelector.clear();

    if (m_worker.joinable())
        m_worker.join();

    for (auto &client : m_clients)
    {
        delete client.socket;
        client.socket = nullptr;
    }
}

int Server::AcceptConnections()
{
    m_listener.reuse();
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

void Server::Mgr()
{
    while (m_active)
    {
        if (m_socketSelector.wait(sf::milliseconds(100)))
        {
            if (m_accepting && m_socketSelector.isReady(m_listener))
            {
                TryAcceptNewClient();
            }
            else if (m_receiving)
            {
                for (auto &client : m_clients)
                {
                    if (m_socketSelector.isReady(*client.socket))
                    {
                        ReceivePackage(client);
                    }
                }
            }
        }
        if (m_sending)
            SendAllPackages();
        CleanUnusedPackages();
    }
}

void Server::TryAcceptNewClient()
{
    sf::TcpSocket *clientSocket = new sf::TcpSocket;
    if (m_listener.accept(*clientSocket) == sf::Socket::Done)
    {
#ifdef DEBUG
        std::cout << "Info: Successfully accepted connection to client! IP: " << clientSocket->getRemoteAddress() << " Port: " << m_port << std::endl;
#endif
        std::string token = GenerateToken(TOKEN_SIZE);
        m_clients.push_back(Connection(clientSocket, token));
        m_socketSelector.add(*clientSocket);
        Send<const char>(Query::Connect, token.c_str(), token.size(), m_clients.back());
    }
    else
    {
#ifdef DEBUG
        std::cerr << "Error: Failed to accept connection to client! IP: " << clientSocket->getRemoteAddress() << " Port: " << m_port << std::endl;
#endif
        delete clientSocket;
    }
}

std::string Server::GenerateToken(size_t length)
{
    std::string token = "";
    for (size_t i = 0; i < length; i++)
        token.push_back(Random::Character("ABCDEFGHIJKLMNOPQRSTUVW123456789"));
    return token;
}