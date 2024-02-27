#include "Client.h"
#include "../Zdp/MyUdp.h"
#include "../Timer/Timer.h"
#include "Headers.h"
#include "User.h"
#include "OutPut.h"
#include "../WindowView/Handle/Handle.h"

inline int udp_output(const char *buf, int len, IKCPCB* ikcpcb, void *user)
{
    // 用user标识目标地址以及udp套接字
    User* ptr = static_cast<User*>(user);
    int res = sendto(ptr->getSocket(), buf, len, 0, (sockaddr*)ptr->getRemoteAddress(), sizeof(SOCKADDR_IN));
    return 0;
}

inline void char_ptr_to_string_memcpy(std::string& des, char const* src , int size)
{
    
    des.reserve(size);
    des.resize(size);
    memcpy_s(const_cast<char*>(des.c_str()), size, src, size);
}

Client::Client(User* user, u_short port, u_short fps)
{
    this->m_localUser = user;
    this->m_udp = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    u_long imode = 1;
    ioctlsocket(this->m_udp, FIONBIO, &imode);
    SOCKADDR_IN addrSrv;     //定义sockSrv发送和接收数据包的地址
    inet_pton(AF_INET, "127.0.0.1", &addrSrv.sin_addr);
    addrSrv.sin_family = AF_INET;
    addrSrv.sin_port = htons(port);
    bind(this->m_udp,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));
    this->m_fps = fps;
    this->m_stream = zdsj::Serialize();
    this->m_rcvStream = zdsj::Serialize();
    this->m_timer = new Timer();
    this->m_timer->setFps(this->m_fps);
}

Client::~Client()
{
    this->clear();
}

int Client::sendMessage(unsigned conv, std::string key, std::string message)
{
    auto pair = this->m_connected.find(conv);
    std::unique_lock<std::mutex> lock(this->m_stream_mutex);
    this->m_stream << Headers::MESSAGE;
    this->m_stream << key;
    this->m_stream << message;
    std::string data = this->m_stream.toStream();
    this->m_stream.clear();
    return this->send(pair->second, data.c_str(), data.size() + 1);
}

MyUdp* Client::connect(unsigned conv, PCSTR ip, u_short port, boolean sendHand)
{
    // 构造远程地址
    SOCKADDR_IN* m_RemoteAddress = new SOCKADDR_IN;
    inet_pton(AF_INET, ip, &m_RemoteAddress->sin_addr);
    m_RemoteAddress->sin_family = AF_INET;
    m_RemoteAddress->sin_port = htons(port);
    User* user =  new User(&this->m_udp, m_RemoteAddress, this->m_timer->now());
    MyUdp* zdp = new MyUdp(conv, user);
    // 设置output  也就是底层调用sendto
    zdp->setOutput(udp_output);
    this->m_connected.insert(std::map<u_int, MyUdp*>::value_type(conv, zdp));
    if(sendHand)
    {
        this->sendHeader(Headers::HAND, zdp, this->m_stream);
        user->sendHanded();
    }
    this->m_stream.clear();
    return zdp;
}

void Client::close()
{
    this->m_alive = false;
    this->m_updateThread.join();
    this->m_rcvThread.join();
}

void Client::start()
{
    this->m_updateThread = std::thread(&Client::update, this);
    this->m_rcvThread = std::thread(&Client::rcv, this);
}

void Client::setHandle(Handle* _handle)
{
    if(this->m_handle == nullptr)
    {
        this->m_handle = _handle;
    }else
    {
        this->m_handle->add(_handle);
    }
}

void Client::update()
{
    char buffer[1500]; // mtu最大1500
    int len = -1;
    int conv = -1;
    sockaddr_in  from;
    int size = sizeof(from);
    UINT32 currentTime = 0;
    while (this->m_alive)
    {
        currentTime = this->m_timer->now();
        try
        {
            std::shared_lock<std::shared_mutex> shared_lock(this->m_mutex);
            for (const auto pair : this->m_connected)
            {
                auto zdp = pair.second;
                zdp->update(currentTime);
            }

            // len接收到的字符个数
            len = recvfrom(this->m_udp, buffer, 1500, 0, (sockaddr*)&from, &size);
            if(len > 0)
            {
                // 获取conv编号;
                conv = *reinterpret_cast<int*>(buffer);
                MyUdp* zdp = this->getZdpByConv(conv);
                if(zdp == nullptr)
                {
                    char* ip = new char[16];
                    inet_ntop(AF_INET, &from.sin_addr, ip, 16);
                    const u_short port = ntohs(from.sin_port);
                    zdp = this->connect(conv, ip, port);
                    delete[] ip;
                }
                zdp->input(buffer, len);
            }
        }
        catch (...)
        {
            std::cout << "exception" << std::endl;
        }
        this->m_timer->nextFps();
    }
}

void Client::rcv()
{
    std::chrono::duration<double> time = std::chrono::duration<double>::zero();
    while (this->m_alive)
    {
        int len = 0;
        unsigned int conv = 0;
        std::unique_ptr<char> buffer(new char[1500]);
        std::unique_ptr<char> res_key(new char[20]);
        _memccpy(res_key.get(), "message", '\0', 8);
        std::unique_ptr<char> response(new char[1500]);
        UINT32 currentTime = this->m_timer->now();
        for (auto pair : this->m_connected)
        {
            conv = pair.first;
            auto zdp = pair.second;
            memset(response.get(), '\0', 1500);
            memset(buffer.get(), '\0', 1500);
            len = zdp->rcv(buffer.get(), zdp->peekSize());
            auto user = static_cast<User*>(zdp->user());
            bool remove = this->checkAndSendFind(conv, zdp, user, currentTime);
            if(remove)
            {
                break;
            }
            if(len > 0)
            {
                user->setLastMessageTime(currentTime);
                bool isMessage = false;
                char* key = nullptr;
                char* payload = nullptr;
                this->parseHeader(conv, zdp, buffer.get(), len, isMessage, user, key, payload);
                if(isMessage)
                {
                    if(this->m_handle != nullptr)
                    {
                        const bool needResponse = this->m_handle->parse(key, payload, res_key.get(), response.get());
                        if(needResponse)
                        {
                            this->sendMessage(conv, res_key.get(), response.get());
                        }
                    }
                }
            }
        }
        time = this->m_timer->nextFps();
        // std::cout << 1 / time.count() << std::endl;
    }
}

void Client::removeZdp(u_int conv, MyUdp* zdp)
{
    std::unique_lock<std::shared_mutex> unique_lock(this->m_mutex);
    this->m_connected.erase(conv);
    delete zdp;
    onClosed(conv);
}

void Client::clear()
{
    for (auto pair : this->m_connected)
    {
        // first: key, second: value;
        MyUdp* zdp = pair.second;
        delete static_cast<User*>(zdp->user());
        delete zdp;
    }
    delete this->m_handle;
    delete this->m_timer;
    delete this->m_localUser;
    closesocket(this->m_udp);
}

MyUdp* Client::getZdpByConv(int conv)
{
    MyUdp* zdp = nullptr;
    const auto pair = this->m_connected.find(conv);
    if(pair != this->m_connected.end())
    {
        zdp = pair->second;
    }
    return zdp;
}

void Client::sendHeader(unsigned int head, MyUdp* zdp, zdsj::Serialize& _stream)
{
    _stream.clear();
    _stream << head;
    const std::string basic_string = _stream.toStream();
    this->send(zdp, basic_string.c_str(), basic_string.length() + 1);
}

void Client::parseHeader(unsigned int conv, MyUdp* zdp, char* data, int size, bool& isMessage, User* user, char*& _key,
    char*& _payload)
{
    isMessage = false;
    std::string message;
    char_ptr_to_string_memcpy(message, data, size);
    this->m_rcvStream.load(message);
    Headers head;
    this->m_rcvStream >> head;
    switch (head)
    {
    case Headers::HAND:
        if(!user->getSendHand())
        {
            head = Headers::HAND;
            user->sendHanded();
        }else
        {
            head = Headers::CONNECTED;
            onConnected(conv);
        }
        this->sendHeader(head, zdp, this->m_rcvStream);
        break;
    case Headers::CONNECTED:
        onConnected(conv);
        break;
    case Headers::CLOSE:
        // 关闭连接
        this->removeZdp(conv, zdp);
        break;
    case Headers::FIND:
        head = Headers::FINDRES;
        this->sendHeader(head, zdp, this->m_rcvStream);
        break;
    case Headers::FINDRES:
        // 收到心跳
        user->setLastMessageTime(this->m_timer->now());
        break;
    case Headers::MESSAGE:
        isMessage = true;
        char* key = nullptr;
        char* payload = nullptr;
        this->m_rcvStream >> _key;
        this->m_rcvStream >> _payload;
        break;
    }
    this->m_rcvStream.clear();
}

bool Client::checkAndSendFind(unsigned conv, MyUdp* zdp, User* user, unsigned int currentTime)
{
    // 计算时间差
    auto differenceTime  = currentTime - user->getLastMassageTime();
    int findTimes = user->getFindTimes();
    if(findTimes == 5)
    {
        this->removeZdp(conv, zdp);
        return true;
    }
    if(differenceTime > 2000*findTimes)
    {
        std::unique_lock<std::mutex> lock(this->m_stream_mutex);
        this->m_stream << Headers::FIND;
        std::string data = this->m_stream.toStream();
        this->m_stream.clear();
        this->send(zdp, data.c_str(), data.size() + 1);
        user->setFindTimes(findTimes + 1);
    }
    return false;
}

int Client::send(MyUdp* zdp, const char* buffer, int size)
{
    return zdp->send(buffer, size);
}
