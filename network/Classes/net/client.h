#ifndef __NET_CLIENT_H
#define __NET_CLIENT_H
#include <string>
#include "net/datahandler.h"
#include "connectiontcpclient.h"
#include "logsink.h"

class Client
{
public:
    Client();
    virtual ~Client();

    bool connect(const std::string & server, int port, LUA_FUNCTION errorFunc = 0);
    void close();
    void receive();
    void receiveWithPthread();
    bool reconnect(LUA_FUNCTION errorFunc = 0);
    bool send(const char * data, unsigned int size);
    bool isConnect();

private:
    Client(const Client & c);
    Client & operator=(const Client & c);
    bool _connect(LUA_FUNCTION errorFunc = 0);

private:
    std::string                   m_server;
    int                           m_port;
    DataHandler                 * m_dataHandler;
    gloox::ConnectionTCPClient  * m_client;
    gloox::LogSink                m_log;
};

#endif // __NET_CLIENT_H
