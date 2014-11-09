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

void DataHandler::handleReceivedData(const ConnectionBase *connection, char *data, int size)
{

}

void DataHandler::handleConnect(const ConnectionBase *connection)
{
}

void DataHandler::registerDisconnectHandler(LUA_FUNCTION func)
{

}

void DataHandler::unregisterDisconnectHandler()
{

}

void DataHandler::handleDisconnect(ConnectionBase *connection, ConnectionError reason)
{
}
