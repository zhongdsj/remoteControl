#include "MyUdp.h"
#include "ikcp.h"

MyUdp::MyUdp(unsigned int conv, User* user)
{
    this->m_kcp = ikcp_create(conv, user);
}

MyUdp::~MyUdp()
{
    ikcp_release(this->m_kcp);
}

User* MyUdp::user() const
{
    return static_cast<User*>(this->m_kcp->user);
}

void MyUdp::setOutput(int(* output)(const char* buf, int len, IKCPCB* ikcpcb, void* user)) const
{
    ikcp_setoutput(this->m_kcp, output);
}

int MyUdp::rcv(char* buffer, int len) const
{
    return ikcp_recv(this->m_kcp, buffer, len);
}

int MyUdp::send(const char* buffer, int len) const
{
    return ikcp_send(this->m_kcp, buffer, len);
}

void MyUdp::update(unsigned int current) const
{
    ikcp_update(this->m_kcp, current);
}

int MyUdp::peekSize() const
{
    return ikcp_peeksize(this->m_kcp);
}

int MyUdp::input(const char* buffer, int len) const
{
    return ikcp_input(this->m_kcp, buffer, len);
}




