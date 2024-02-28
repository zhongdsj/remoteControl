#pragma once
#include "Client.h"
#pragma comment(lib, "ws2_32.lib")

bool init()
{
    WSADATA  wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cout << "WSAStartup error:" << GetLastError() << std::endl;
        return false;
    }
    return true;
}

void close(Client* client)
{
    client->close();
    WSACleanup();
}

//定义宏
#ifdef IMPORT_DLL
#else
#define IMPORT_DLL extern "C" _declspec(dllimport) //指的是允许将其给外部调用
#endif

IMPORT_DLL Client* createClient(int port);
IMPORT_DLL void connectT(Client* client, unsigned int conv,const char* ip, unsigned short port);
IMPORT_DLL void destroyClient(Client* client);
IMPORT_DLL void sendMessage(Client* client, unsigned int conv, const char* key, const char* message);
IMPORT_DLL void setOnMessageCallbackFunction(Client* client, OnMessageCallbackFunction fun);
IMPORT_DLL void setOnConnectedCallbackFunction(Client* client, OnConnectedCallbackFunction fun);
IMPORT_DLL void setOnClosedCallbackFunction(Client* client, OnClosedCallbackFunction fun);

Client* createClient(int port) {
    if (!init()) return nullptr;
    Client* client = new Client((User*)0, port, 120);
    client->start();
	return client;
}

void destroyClient(Client* client) {
    close(client);
	delete client;
}

void sendMessage(Client* client, unsigned int conv, const char* key, const char* message)
{
    client->sendMessage(conv, key, message);
}

void setOnMessageCallbackFunction(Client* client, OnMessageCallbackFunction fun/*void * fun*/) {
    client->setOnMessageCallbackFunction(fun);
}

void setOnConnectedCallbackFunction(Client* client, OnConnectedCallbackFunction fun/*void * fun*/) {
    client->setOnConnectedCallbackFunction(fun);
}

void setOnClosedCallbackFunction(Client* client, OnClosedCallbackFunction fun/*void * fun*/) {
    client->setOnClosedCallbackFunction(fun);
}

void connectT(Client* client, unsigned int conv, const char* ip, unsigned short port) {
    client->connect(conv, ip, port, true);
}
