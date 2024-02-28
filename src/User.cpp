#include "User.h"

#include <string>

User::User(SOCKET* udp, SOCKADDR_IN* remoteAddress, UINT32 currentTime)
{
    this->m_udp = udp;
    this->m_remoteAddress = remoteAddress;
    this->m_lastMessageTime = currentTime;
}

User::~User()
{
    free(this->m_remoteAddress);
}

SOCKET User::getSocket()
{
    return *this->m_udp;
}

SOCKADDR_IN* User::getRemoteAddress()
{
    return this->m_remoteAddress;
}

void User::setLastMessageTime(UINT32 currentTime)
{
    this->m_lastMessageTime = currentTime;
    this->m_findTimes = 1;
}

UINT32 User::getLastMassageTime()
{
    return this->m_lastMessageTime;
}

void User::setFindTimes(int find)
{
    this->m_findTimes = find;
}

int User::getFindTimes()
{
    return this->m_findTimes;
}

std::unique_ptr<char> User::getAddress() const
{
    char* buf = new char[20];
    std::unique_ptr<char> c(buf);
    inet_ntop(this->m_remoteAddress->sin_family, &this->m_remoteAddress->sin_addr, c.get(), 20);
    return c;
}

void User::sendHanded()
{
    this->m_sendHand = true;
}

bool User::getSendHand()
{
    return this->m_sendHand;
}
