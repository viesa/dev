#include "NetworkMgr.hpp"

NetworkMgr::NetworkMgr()
    : m_client(nullptr),
      m_server(nullptr)
{
}

void NetworkMgr::PollAll()
{
    if (m_client)
    {
        for (auto packet = m_client->PopFront(); packet->query != Query::Empty; packet = m_client->PopFront())
        {
            switch (packet->query)
            {
            case Query::Connect:
                m_client->SetToken(std::string((char *)(packet->data)));

                break;
            case Query::Test:
                std::cout << std::string((char *)(packet->data)) << std::endl;
                break;
            default:
                break;
            }
        }
    }
    if (m_server)
    {
        for (auto packet = m_server->PopFront(); packet->query != Query::Empty; packet = m_server->PopFront())
        {
            switch (packet->query)
            {
            case Query::Test:
                std::cout << std::string((char *)(packet->data)) << std::endl;
                break;
            default:
                break;
            }
        }
    }
}