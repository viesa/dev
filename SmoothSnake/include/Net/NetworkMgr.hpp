#pragma once

#include "Client.hpp"
#include "Server.hpp"

class NetworkMgr
{
public:
    NetworkMgr();

    void PollAll();

    void SetClient(Client *client) { m_client = client; };
    void SetServer(Server *server) { m_server = server; };

private:
    Client *m_client;
    Server *m_server;
};