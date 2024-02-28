#pragma once
#include <WS2tcpip.h>
#include <string>
#include <thread>
#include <shared_mutex>
#include <map>
#include "Serialize.h"

class MyUdp;
class User;
class Timer;

typedef bool(*OnMessageCallbackFunction)(char* _key, char* _payload, char* _res_key, char* _response);
typedef bool(*OnConnectedCallbackFunction)(unsigned int conv);
typedef bool(*OnClosedCallbackFunction)(unsigned int conv);

namespace zdsj
{
    class Serialize;
}

class _declspec(dllimport) Client
{
public:
    Client(User* user, u_short port, u_short fps);
    ~Client();
    int sendMessage(unsigned int conv, std::string key, std::string message);
    MyUdp* connect(unsigned int conv, PCSTR ip, u_short port, boolean sendHand = false);
    void close();
    void start();
    void setOnMessageCallbackFunction(OnMessageCallbackFunction fun);
    void setOnConnectedCallbackFunction(OnConnectedCallbackFunction fun);
    void setOnClosedCallbackFunction(OnClosedCallbackFunction fun);
private:
    void update();
    void rcv();
    void removeZdp(u_int conv, MyUdp* zdp);
    void clear();
    MyUdp* getZdpByConv(int conv);
    void sendHeader(unsigned int head, MyUdp* zdp, zdsj::Serialize& _stream);
    void parseHeader(unsigned int conv, MyUdp* zdp, char* data, int size, bool& isMessage, User* user, char*& _key, char*& _payload);
    bool checkAndSendFind(unsigned int conv, MyUdp* zdp, User* user, unsigned int currentTime);
    int send(MyUdp* zdp, const char* buffer, int size);
    bool parse(char* _key, char* _payload, char* _res_key, char* _response);

    SOCKET m_udp;
    User* m_localUser;
    Timer* m_timer;
    u_short m_fps;
    BOOL m_alive = true;
    std::thread m_updateThread;
    std::thread m_rcvThread;
    std::map<unsigned int, MyUdp*> m_connected;
    zdsj::Serialize m_stream;
    zdsj::Serialize m_rcvStream;
    std::shared_mutex m_mutex;
    std::mutex m_stream_mutex;

    OnMessageCallbackFunction m_onMessage;
    OnConnectedCallbackFunction m_onConnected;
    OnClosedCallbackFunction m_onClosed;
};
