#include "Client.hpp"

using namespace std::chrono_literals;

Client::Client()
    : Client(sf::IpAddress("127.0.0.1"), 4000)
{
}

Client::Client(sf::IpAddress const &ip, int const &port)
    : m_ip(ip),
      m_port(port),
      m_server(Connection(new sf::TcpSocket, "")),
      m_active(true),
      m_receiving(true),
      m_sending(true),
      m_connected(false),
      m_worker(&Client::Mgr, this),
      counter(0)
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

    delete m_server.socket;
}

int Client::Connect()
{
    if (!m_connected)
    {
        if (m_server.socket->connect(m_ip, m_port) == sf::Socket::Status::Done)
        {
            m_socketSelector.add(*m_server.socket);
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
        m_server.socket->disconnect();
        m_connected = false;
    }
}

void Client::Mgr()
{
    while (m_active)
    {
        if (m_socketSelector.wait(sf::milliseconds(100)))
        {
            if (m_receiving && m_socketSelector.isReady(*m_server.socket))
            {
                ReceivePackage(m_server);
            }
        }
        if (m_sending)
            SendAllPackages();

        counter > 50 ? CleanUnusedPackages(), counter = 0 : counter++;
    }
}