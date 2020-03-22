#include "PacketMgr.hpp"

PacketMgr::PacketMgr()
{
    AddInUse(Payload(Query::Empty, nullptr, 0));
    m_fallback = m_inUse.front();
}

std::shared_ptr<Payload> PacketMgr::PopFront()
{
    if (m_incoming.size() > 0)
    {
        AddInUse(m_incoming.front().payload);
        m_incoming.pop_front();
        return m_inUse.back();
    }
    else
    {
        return m_fallback;
    }
}

std::shared_ptr<Payload> PacketMgr::PopBack()
{
    if (m_incoming.size() > 0)
    {
        AddInUse(m_incoming.back().payload);
        m_incoming.pop_back();
        return m_inUse.back();
    }
    else
    {
        return m_fallback;
    }
}

void PacketMgr::ReceivePackage(Connection &connection)
{
    sf::Packet incoming;

    auto socket = connection.socket;
#ifdef DEBUG
    auto ip = socket->getRemoteAddress();
    auto port = socket->getRemotePort();
#endif
    if (socket->receive(incoming) == sf::Socket::Done)
    {
        char *raw_msg = (char *)incoming.getData();

        char token_raw[TOKEN_SIZE];
        memcpy(token_raw, raw_msg, sizeof(char) * TOKEN_SIZE);
        std::string token = std::string(token_raw).substr(0, TOKEN_SIZE);
        if (token != connection.token && connection.token != "")
        {
#ifdef DEBUG
            std::cout << "Info: Wrong token! Throwing packet... IP: " << ip << " Port: " << port << std::endl;
            std::cout << "Expected: " << connection.token << " Got: " << token << std::endl;
#endif
        }
        else
        {
            char header_raw[HEADER_SIZE];
            memcpy(header_raw, raw_msg + TOKEN_SIZE, sizeof(char) * HEADER_SIZE);
            std::string header = std::string(header_raw).substr(0, HEADER_SIZE);
            header.erase(std::remove_if(header.begin(), header.end(), isspace), header.end());
            int dataSize = std::stoi(header);

            char query_raw[QUERY_SIZE];
            memcpy(query_raw, raw_msg + TOKEN_SIZE + HEADER_SIZE, sizeof(char) * QUERY_SIZE);
            std::string query_str = std::string(query_raw).substr(0, QUERY_SIZE);
            query_str.erase(std::remove_if(query_str.begin(), query_str.end(), isspace), query_str.end());
            Query query = (Query)std::stoi(query_str);

            void *data = malloc(dataSize);
            memcpy(data, raw_msg + TOKEN_SIZE + HEADER_SIZE + QUERY_SIZE, sizeof(char) * dataSize);

#ifdef DEBUG
            std::cout << "Info: Successfully received packet from IP: " << ip << " Port: " << port << " Query: " << query << std::endl;
#endif
            AddIncoming(Packet(connection, Payload(query, data, dataSize)));
        }
    }
    else
    {
#ifdef DEBUG
        std::cerr << "Error: Failed to receive packet from IP: " << socket->getRemoteAddress() << " Port: " << port << std::endl;
#endif
    }
}

void PacketMgr::SendAllPackages()
{
    for (auto outgoing = m_outgoing.begin(); outgoing != m_outgoing.end(); outgoing++)
    {
        SendPackage(*outgoing);
    }

    auto sent = std::remove_if(m_outgoing.begin(), m_outgoing.end(), [](auto packet) { return packet.sent; });
    std::for_each(sent, m_outgoing.end(), [](auto packet) { free(packet.payload.data); });
    m_outgoing.erase(sent, m_outgoing.end());
}

void PacketMgr::CleanUnusedPackages()
{
    auto non_used = std::remove_if(m_inUse.begin(), m_inUse.end(), [](auto payload) {
        return payload.use_count() == 2;
    });
    std::for_each(non_used, m_inUse.end(), [](auto payload) {
        free(payload->data);
    });
    m_inUse.erase(non_used, m_inUse.end());
}

void PacketMgr::SendPackage(Packet &packet)
{

    auto &socket = packet.connection.socket;
    auto &token = packet.connection.token;
#ifdef DEBUG
    auto ip = socket->getRemoteAddress();
    auto port = socket->getLocalPort();
#endif

    if (token == "")
        return;

    sf::Packet outgoing = PreparePacket(packet);

    if (socket->send(outgoing) == sf::Socket::Done)
    {
#ifdef DEBUG
        std::cout << "Info: Successfully sent packet to IP: " << ip << " Port: " << port << std::endl;
#endif
        packet.sent = true;
    }
    else
    {
#ifdef DEBUG
        std::cerr << "Error: Failed to send packet to IP: " << ip << " Port: " << port << std::endl;
#endif
    }
}

sf::Packet PacketMgr::PreparePacket(Packet const &packet)
{
    sf::Packet outgoing;

    std::stringstream ss;
    ss << std::left << std::setw(TOKEN_SIZE) << packet.connection.token;
    std::string token_formatted = ss.str();
    ss.str("");
    ss.clear();
    ss << std::left << std::setw(HEADER_SIZE) << packet.payload.dataSize;
    std::string header_formatted = ss.str();
    ss.str("");
    ss.clear();
    ss << std::left << std::setw(QUERY_SIZE) << packet.payload.query;
    std::string query_formatted = ss.str();
    ss.str("");
    ss.clear();

    outgoing.append((void *)token_formatted.c_str(), TOKEN_SIZE * sizeof(char));
    outgoing.append((void *)header_formatted.c_str(), HEADER_SIZE * sizeof(char));
    outgoing.append((void *)query_formatted.c_str(), QUERY_SIZE * sizeof(char));
    outgoing.append((void *)packet.payload.data, packet.payload.dataSize);

    return outgoing;
}