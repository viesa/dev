#include "Client.hpp"
#include <iostream>

using namespace std::chrono_literals;

Client::Client()
    : Client(sf::IpAddress("127.0.0.1"), 4000)
{
}

Client::Client(sf::IpAddress const &ip, int const &port)
    : m_ip(ip),
      m_port(port),
      m_socket(new sf::TcpSocket),
      m_active(true),
      m_receiving(true),
      m_sending(true),
      m_connected(false),
      m_worker(&Client::Mgr, this)
{
}

Client::~Client()
{
    m_active = false;
    m_receiving = false;
    m_sending = false;

    Disconnect();

    m_socketSelector.clear();

    if (m_worker.joinable())
        m_worker.join();

    delete m_socket;
}

int Client::Connect()
{
    if (!m_connected)
    {
        if (m_socket->connect(m_ip, m_port) == sf::Socket::Status::Done)
        {
            m_socketSelector.add(*m_socket);
            m_connected = true;
            return 1;
        }
    }
    return 0;
}

void Client::Disconnect()
{
    if (m_connected)
    {
        m_socket->disconnect();
        m_connected = false;
    }
}

void Client::Send(sf::Packet const &packet)
{
    m_pendingSending.push_back(packet);
}

void Client::Send(void *data, size_t size)
{
    sf::Packet packet;
    packet.append(data, size);
    Send(packet);
}

void Client::Send(std::string const &string)
{
    sf::Packet packet;
    packet.append((void *)string.c_str(), string.length() + 1);
    Send(packet);
}

sf::Packet Client::PopFront()
{
    if (m_pendingReceiving.size() > 0)
    {
        sf::Packet ret = m_pendingReceiving.front();
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

sf::Packet Client::PopBack()
{
    if (m_pendingReceiving.size() > 0)
    {
        sf::Packet ret = m_pendingReceiving.back();
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

void Client::Mgr()
{
    while (m_active)
    {
        if (m_socketSelector.wait(sf::milliseconds(100)))
        {
            if (m_receiving && m_socketSelector.isReady(*m_socket))
            {
                sf::Packet packet;
                if (m_socket->receive(packet) == sf::Socket::Done)
                {
#ifdef DEBUG
                    std::cout << "Info: Successfully received packet from server! IP: " << m_ip << " Port: " << m_port << std::endl;
#endif
                    m_pendingReceiving.push_back(packet);
                }
                else
                {
#ifdef DEBUG
                    std::cerr << "Error: Failed to receive packet from server! IP: " << m_ip << " Port: " << m_port << std::endl;
#endif
                }
            }
        }
        if (m_sending)
            while (m_pendingSending.size() > 0)
            {
                if (m_socket->send(m_pendingSending.front()) == sf::Socket::Done)
                {
#ifdef DEBUG
                    std::cout << "Info: Successfully sent packet to server! IP: " << m_ip << " Port: " << m_port << std::endl;
#endif
                    m_pendingSending.pop_front();
                }
                else
                {
#ifdef DEBUG
                    std::cerr << "Error: Failed to send packet to server! IP: " << m_ip << " Port: " << m_port << std::endl;
#endif
                }
            }
    }
}