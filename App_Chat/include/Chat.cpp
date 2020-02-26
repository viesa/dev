#include "Chat.hpp"

#include "Core/Graphics.hpp"

#include <iostream>

Chat::Chat(GUI &gui)
    : m_gui(gui),
      m_phase(NoPhase),
      m_newPhase(NoPhase),
      m_updatedPhase(false),
      m_server(nullptr),
      m_client(nullptr)
{
    ChangePhase(LoginServerClient);
}

Chat::~Chat()
{
    if (m_client)
    {
        delete m_client;
        m_client = nullptr;
    }
    if (m_server)
    {
        delete m_server;
        m_server = nullptr;
    }
}

int Chat::SetupServer(int port)
{
    m_server = new Server(port);
    if (m_server->AcceptConnections())
    {
        std::cout << "Server is up" << std::endl;
        return 1;
    }
    else
    {
        std::cout << "Server is NOT up" << std::endl;
        return 0;
    }
}

int Chat::SetupClient(sf::IpAddress ip, int port)
{
    m_client = new Client(ip, port);
    if (m_client->Connect())
    {
        std::cout << "Connected" << std::endl;
        return 1;
    }
    else
    {
        std::cout << "NOT Connceted" << std::endl;
        return 0;
    }
}

void Chat::Update(sf::Time dt)
{
    if (m_updatedPhase)
    {
        ExitPhase();
        m_phase = m_newPhase;
        EnterPhase();
        m_updatedPhase = false;
    }

    if (m_phase == RoomMain)
        FetchAndDisplay();

    if (m_server)
        CollectAndBroadcast();
}

void Chat::ChangePhase(Phase newPhase)
{
    m_newPhase = newPhase;
    m_updatedPhase = true;
}

std::vector<sfg::Window::Ptr> Chat::GetWindows()
{
    return m_windows;
}

void Chat::EnterPhase()
{
    m_windows.push_back(sfg::Window::Create());
    switch (m_phase)
    {
    case LoginServerClient:
    {
        auto box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 10.0f);
        auto buttonCreate = sfg::Button::Create("Create");
        auto buttonJoin = sfg::Button::Create("Join");

        buttonCreate->GetSignal(sfg::Widget::OnLeftClick).Connect([this] {
            ChangePhase(LoginEnterCredHost);
        });
        buttonJoin->GetSignal(sfg::Widget::OnLeftClick).Connect([this] {
            ChangePhase(LoginEnterCredClient);
        });

        box->Pack(buttonCreate);
        box->Pack(buttonJoin);

        m_windows[0]->Add(box);
        break;
    }
    case LoginEnterCredHost:
    {
        auto box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 10.0f);
        auto labelPort = sfg::Label::Create("Port");
        auto labelName = sfg::Label::Create("Name");
        auto entryPort = sfg::Entry::Create();
        auto entryName = sfg::Entry::Create();
        auto buttonNext = sfg::Button::Create("Next");

        entryPort->SetRequisition(sf::Vector2f(150.0f, 0.0f));
        entryName->SetRequisition(sf::Vector2f(150.0f, 0.0f));
        buttonNext->SetRequisition(sf::Vector2f(150.0f, 0.0f));

        buttonNext->GetSignal(sfg::Widget::OnLeftClick).Connect([this, entryPort, entryName] {
            int c_server = SetupServer(std::stoi(entryPort->GetText().toAnsiString()));
            int c_client = SetupClient("127.0.0.1", std::stoi(entryPort->GetText().toAnsiString()));
            if (c_server && c_client)
            {
                ChangePhase(RoomMain);
            }
            else if (!c_server || !c_client)
            {
                GetWindows()[0]->SetTitle("Failed to start host");
            }
        });

        box->Pack(labelPort);
        box->Pack(entryPort);
        box->Pack(labelName);
        box->Pack(entryName);
        box->Pack(buttonNext);

        m_windows[0]->Add(box);
        break;
    }
    case LoginEnterCredClient:
    {
        auto box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 10.0f);
        auto labelIP = sfg::Label::Create("IP-adress");
        auto labelPort = sfg::Label::Create("Port");
        auto labelName = sfg::Label::Create("Name");
        auto entryIP = sfg::Entry::Create();
        auto entryPort = sfg::Entry::Create();
        auto entryName = sfg::Entry::Create();
        auto buttonNext = sfg::Button::Create("Next");

        entryIP->SetRequisition(sf::Vector2f(150.0f, 0.0f));
        entryPort->SetRequisition(sf::Vector2f(150.0f, 0.0f));
        entryName->SetRequisition(sf::Vector2f(150.0f, 0.0f));
        buttonNext->SetRequisition(sf::Vector2f(150.0f, 0.0f));

        buttonNext->GetSignal(sfg::Widget::OnLeftClick).Connect([this, entryIP, entryPort, entryName] {
            if (SetupClient(entryIP->GetText().toAnsiString(), std::stoi(entryPort->GetText().toAnsiString())))
            {
                ChangePhase(RoomMain);
            }
            else
            {
                GetWindows()[0]->SetTitle("Failed to connect");
            }
        });

        box->Pack(labelIP);
        box->Pack(entryIP);
        box->Pack(labelPort);
        box->Pack(entryPort);
        box->Pack(labelName);
        box->Pack(entryName);
        box->Pack(buttonNext);

        m_windows[0]->Add(box);
        break;
    }
    case RoomMain:
    {
        auto box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 10.0f);
        m_chatHistoryContainer = sfg::Label::Create();

        auto boxInput = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, 4.0f);
        auto input = sfg::Entry::Create();
        auto send = sfg::Button::Create("Send");

        m_chatHistoryContainer->SetRequisition(sf::Vector2f(200.0f, 200.0f));
        input->SetRequisition(sf::Vector2f(165.0f, 20.0f));
        send->SetRequisition(sf::Vector2f(30.0f, 20.0f));

        send->GetSignal(sfg::Widget::OnLeftClick).Connect([this, input] {
            GetClient()->Send(input->GetText().toAnsiString());
            input->SetText("");
        });

        box->Pack(m_chatHistoryContainer);
        box->Pack(boxInput);
        boxInput->Pack(input);
        boxInput->Pack(send);

        m_windows[0]->Add(box);
        break;
    }
    default:
        break;
    }

    m_windows[0]->SetPosition(GetRelativeMid(m_windows[0]));

    for (auto &window : m_windows)
    {
        m_gui.AddWindow(window);
    }
}

void Chat::ExitPhase()
{
    for (auto &window : m_windows)
    {
        window->RemoveAll();
        m_gui.RemoveWindow(window);
    }
    m_windows.clear();
    m_phase = NoPhase;
}

void Chat::FetchAndDisplay()
{
    for (sf::Packet packet = m_client->PopFront(); *(char *)(packet.getData()); packet = m_client->PopFront())
    {
        std::string _new((char *)(packet.getData()));
        _new.push_back('\n');
        m_chatHistory += _new;
    };
    m_chatHistoryContainer->SetText(m_chatHistory);
}

void Chat::CollectAndBroadcast()
{
    for (sf::Packet packet = m_server->PopFront(); *(char *)(packet.getData()); packet = m_server->PopFront())
    {
        m_server->BroadCast(packet);
    };
}

sf::Vector2f Chat::GetRelativeMid(sfg::Widget::Ptr widget)
{
    float x = Graphics::ScreenWidth / 2.0f - widget->GetAllocation().width / 2.0f;
    float y = Graphics::ScreenHeight / 2.0f - widget->GetAllocation().height / 2.0f;
    return {x, y};
}