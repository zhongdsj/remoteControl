#pragma once

struct IKCPCB;
class User;

class MyUdp
{
private:
    IKCPCB* m_kcp = nullptr;
public:
    MyUdp(unsigned int conv, User* user);

    ~MyUdp();

    User* user() const;

    void setOutput(int (*output)(const char *buf, int len, IKCPCB* ikcpcb,void *user)) const;

    int rcv(char* buffer, int len) const;

    int send(const char* buffer, int len) const;

    void update(unsigned int current) const;

    int peekSize() const;

    int input(const char* buffer, int len) const;
    // int output(const void* data, int size) const;
};
