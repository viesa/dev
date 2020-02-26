#pragma once

#include "Core/GUI.hpp"
#include "Core/Server.hpp"
#include "Core/Client.hpp"

class Chat
{
public:
    enum Phase
    {
        LoginServerClient,
        LoginEnterCredHost,
        LoginEnterCredClient,
        RoomMain,
        NoPhase,
        PhaseCount
    };

public:
    Chat(GUI &gui);
    ~Chat();

    int SetupServer(int port);
    int SetupClient(sf::IpAddress ip, int port);

    void Update(sf::Time dt);

    void ChangePhase(Phase newPhase);

    std::vector<sfg::Window::Ptr> GetWindows();
    Client *GetClient() { return m_client; };
    Server *GetServer() { return m_server; };

private:
    void EnterPhase();
    void ExitPhase();

    void FetchAndDisplay();
    void CollectAndBroadcast();

    sf::Vector2f GetRelativeMid(sfg::Widget::Ptr widget);

private:
    GUI &m_gui;

    Phase m_phase;
    Phase m_newPhase;
    bool m_updatedPhase;

    std::vector<sfg::Window::Ptr> m_windows;
    sfg::Label::Ptr m_chatHistoryContainer;
    std::string m_chatHistory;

    Server *m_server;
    Client *m_client;
};