#pragma once
#include <memory>
#include <WS2tcpip.h>

class User
{
public:
    User(SOCKET* udp, SOCKADDR_IN* remoteAddress, UINT32 currentTime);
    ~User();
    SOCKET getSocket();
    SOCKADDR_IN* getRemoteAddress();
    void setLastMessageTime(UINT32 currentTime);
    UINT32 getLastMassageTime();
    void setFindTimes(int find);
    int getFindTimes();
    std::unique_ptr<char> getAddress() const;
    void sendHanded();
    bool getSendHand();
private:
    SOCKET* m_udp;
    SOCKADDR_IN* m_remoteAddress;
    UINT32 m_lastMessageTime; // 最近接收到信息的时刻
    int m_findTimes = 1;
    bool m_sendHand = false;
};
