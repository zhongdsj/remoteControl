#include "Segment.h"
#include "MyQueue.h"

#include <cstring>

Segment::Segment(int size): m_node(new MyQueue(this))
{
    this->m_data = new char[size];
}

Segment::~Segment()
{
    delete this->m_node;
    delete this->m_data;
}

unsigned int Segment::getData(char* buffer) const
{
    memcpy(buffer, this->m_data, this->m_len);
    return this->m_len;
}

void Segment::setData(unsigned begin, const char* buffer, int size) const
{
    memcpy(this->m_data + begin, buffer, size);
}

unsigned int Segment::len() const
{
    return this->m_len;
}

void Segment::len(unsigned int _len)
{
    this->m_len = _len;
}

unsigned int Segment::frg() const
{
    return this->m_frg;
}

void Segment::frg(unsigned int _frg)
{
    this->m_frg = _frg;
}

unsigned int Segment::resentts() const
{
    return this->m_resendts;
}

void Segment::conv(unsigned int _conv)
{
    this->m_conv = _conv;
}

unsigned int Segment::conv() const
{
    return this->m_conv;
}

void Segment::cmd(unsigned int _cmd)
{
    this->m_cmd = _cmd;
}

unsigned int Segment::cmd() const
{
    return this->m_cmd;
}

void Segment::wnd(unsigned int _wnd)
{
    this->m_wnd = _wnd;
}

unsigned int Segment::wnd() const
{
    return this->m_wnd;
}

void Segment::ts(unsigned int _ts)
{
    this->m_ts = _ts;
}

unsigned Segment::ts() const
{
    return this->m_ts;
}

void Segment::una(unsigned int _una)
{
    this->m_una = _una;
}

unsigned int Segment::una() const
{
    return this->m_una;
}

void Segment::fastAck(unsigned int _fastAck)
{
    this->m_fastack = _fastAck;
}

unsigned Segment::fastAck() const
{
    return this->m_fastack;
}

MyQueue* Segment::node() const
{
    return this->m_node;
}

void Segment::node(MyQueue* _node)
{
    this->m_node = _node;
}

unsigned int Segment::sn() const
{
    return this->m_sn;
}

void Segment::sn(unsigned _sn)
{
    this->m_sn = _sn;
}

unsigned int Segment::resendts() const
{
    return this->m_resendts;
}

void Segment::resendts(unsigned int _resendts)
{
    this->m_resendts = _resendts;
}

unsigned Segment::rto() const
{
    return this->m_rto;
}

void Segment::rto(unsigned int _rto)
{
    this->m_rto = _rto;
}

unsigned int Segment::xmit() const
{
    return this->m_xmit;
}

void Segment::xmit(unsigned int _xmit)
{
    this->m_xmit = _xmit;
}

char* Segment::data() const
{
    return this->m_data;
}
