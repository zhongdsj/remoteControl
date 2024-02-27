#pragma once

class MyQueue;

class Segment
{
public:
    Segment(int size);
    ~Segment();
    unsigned int getData(char* buffer) const;
    void setData(unsigned int begin, const char* buffer, int size) const;
    unsigned int len() const;
    void len(unsigned int _len);
    unsigned int frg() const;
    void frg(unsigned int _frg);
    unsigned int resentts() const;
    void conv(unsigned int _conv);
    unsigned int conv() const;
    void cmd(unsigned int _cmd);
    unsigned int cmd() const;
    void wnd(unsigned int _wnd);
    unsigned int wnd() const;
    void ts(unsigned int _ts);
    unsigned int ts() const;
    void una(unsigned int _una);
    unsigned int una() const;
    void fastAck(unsigned int _fastAck);
    unsigned int fastAck() const;
    MyQueue* node() const;
    void node(MyQueue* _node);
    unsigned int sn() const;
    void sn(unsigned int _sn);
    unsigned int resendts() const;
    void resendts(unsigned int _resendts);
    unsigned int rto() const;
    void rto(unsigned int _rto);
    unsigned int xmit() const;
    void xmit(unsigned int _xmit);
    char* data() const;
private:
    MyQueue* m_node = nullptr;
    unsigned int m_conv = 0;
    unsigned int m_cmd = 0;
    unsigned int m_frg = 0;
    unsigned int m_wnd = 0;
    unsigned int m_ts = 0;
    unsigned int m_sn = 0;
    unsigned int m_una = 0;
    unsigned int m_len = 0;
    unsigned int m_resendts = 0;
    unsigned int m_rto = 0;
    unsigned int m_fastack = 0;
    unsigned int m_xmit = 0;
    char* m_data = nullptr;
};
