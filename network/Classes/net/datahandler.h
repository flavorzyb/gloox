#ifndef __NET_DATAHANDLER_H
#define __NET_DATAHANDLER_H

#include "connectiondatahandler.h"

typedef int LUA_FUNCTION;

class DataHandler : public gloox::ConnectionDataHandler
{
public:
    DataHandler();
    virtual ~DataHandler();
    void handleReceivedData(const gloox::ConnectionBase* connection, const std::string& data );

    void handleConnect(const gloox::ConnectionBase* connection);

    void registerDisconnectHandler(LUA_FUNCTION func);
    void unregisterDisconnectHandler();
    void handleDisconnect(const gloox::ConnectionBase* connection, gloox::ConnectionError reason);

private:
    DataHandler(const DataHandler & dh);
    DataHandler & operator = (const DataHandler &dh);

private:
    enum{BUFF_SIZE=204800};
    char m_buf[BUFF_SIZE];
    int m_offset;
    LUA_FUNCTION m_disconnectCallback;
};

#endif // __NET_DATAHANDLER_H
