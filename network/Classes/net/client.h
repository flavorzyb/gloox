#ifndef __NET_CLIENT_H
#define __NET_CLIENT_H
#include <string>
#include "connectiontcpclient.h"
#include "logsink.h"
#include "connectiondatahandler.h"

typedef int LUA_FUNCTION;

class Client : public gloox::ConnectionDataHandler
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

    void handleReceivedData(const gloox::ConnectionBase* connection, const std::string& data );

    void handleConnect(const gloox::ConnectionBase* connection);

    void registerDisconnectHandler(LUA_FUNCTION func);
    void unregisterDisconnectHandler();
    void handleDisconnect(const gloox::ConnectionBase* connection, gloox::ConnectionError reason);

private:
    Client(const Client & c);
    Client & operator=(const Client & c);
    bool _connect(LUA_FUNCTION errorFunc = 0);

private:
    enum{BUFF_SIZE=204800};
    std::string                   m_server;
    int                           m_port;
    gloox::ConnectionTCPClient  * m_client;
    gloox::LogSink                m_log;
    char                          m_buf[BUFF_SIZE];
    int                           m_offset;
    LUA_FUNCTION                  m_disconnectCallback;
};

#endif // __NET_CLIENT_H
