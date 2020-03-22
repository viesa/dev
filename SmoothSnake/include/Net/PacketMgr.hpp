#pragma once

#include <deque>
#include <memory>
#include <string>
#include <sstream>
#include <iostream>
#include <cstring>
#include <iomanip>
#include <SFML/Network.hpp>

#include "../Core/Flags.hpp"

#define HEADER_SIZE 10
#define QUERY_SIZE 5
#define TOKEN_SIZE 15

enum Query : int8_t
{
    Connect = 0,
    Empty,
    Test,
    NQueries
};

class Payload
{
public:
    Payload() = default;
    Payload(Query query, void *data, int dataSize)
        : query(query),
          data(data),
          dataSize(dataSize)
    {
    }
    Query query;
    void *data;
    int dataSize;
};

class Connection
{
public:
    Connection(sf::TcpSocket *socket, std::string token)
        : socket(socket),
          token(token)
    {
    }
    sf::TcpSocket *socket;
    std::string token;
};

class Packet
{
public:
    Packet(Connection &connection, Payload payload)
        : connection(connection),
          payload(payload),
          sent(false)
    {
    }
    Packet operator=(Packet const &rhs)
    {
        return Packet(rhs.connection, rhs.payload);
    }

    Connection &connection;
    Payload payload;
    bool sent;
};

class PacketMgr
{
public:
    PacketMgr();

    template <typename T>
    void Send(Query query, T *data, size_t size, Connection &connection)
    {
#ifdef DEBUG
        std::cout << "Info: Adding packet to sending list. IP: " << connection.socket->getRemoteAddress() << std::endl;
#endif
        void *data_raw = malloc(size);
        memcpy(data_raw, data, size);
        AddOutgoing(Packet(connection, Payload(query, data_raw, size)));
    }

    std::shared_ptr<Payload> PopFront();
    std::shared_ptr<Payload> PopBack();

protected:
    void ReceivePackage(Connection &connection);
    void SendAllPackages();
    void CleanUnusedPackages();

    virtual std::string i_am() = 0;

private:
    void SendPackage(Packet &packet);
    sf::Packet PreparePacket(Packet const &packet);

    void AddOutgoing(Packet const &packet) { m_outgoing.push_back(packet); }
    void AddIncoming(Packet const &packet) { m_incoming.push_back(packet); }
    void AddInUse(Payload payload) { m_inUse.push_back(std::shared_ptr<Payload>(new Payload(payload))); }

private:
    std::deque<Packet> m_incoming;
    std::deque<Packet> m_outgoing;
    std::vector<std::shared_ptr<Payload>> m_inUse;
    std::shared_ptr<Payload> m_fallback;
};