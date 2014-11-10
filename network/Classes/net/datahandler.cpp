#include "net/datahandler.h"

using namespace gloox;

DataHandler::DataHandler():
    ConnectionDataHandler()
  , m_offset(0)
  , m_disconnectCallback(0)
{
}

DataHandler::~DataHandler()
{
    //@todo 移除lua监听
}

void DataHandler::handleReceivedData( const ConnectionBase* connection, const std::string& data )
{
    printf("received data======= len:%d, str:%s\n", data.length(), data.c_str());
}

void DataHandler::handleConnect(const ConnectionBase *connection)
{
    printf("connect succ!\n");
}

void DataHandler::registerDisconnectHandler(LUA_FUNCTION func)
{
    m_disconnectCallback = func;
}

void DataHandler::unregisterDisconnectHandler()
{

}

void DataHandler::handleDisconnect(const ConnectionBase *connection, ConnectionError reason)
{
    printf("disconnect succ!\n");
}
