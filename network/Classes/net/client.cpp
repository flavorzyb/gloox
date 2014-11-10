#include "net/client.h"
#include <pthread.h>
#include <unistd.h>

using namespace gloox;
using namespace std;

Client::Client():
    m_server("")
  , m_port(0)
  , m_client(NULL)
  , m_offset(0)
  , m_disconnectCallback(0)
{
}

Client::~Client()
{
    delete m_client;
    //@todo 移除lua call back
}

bool Client::connect(const string &server, int port, LUA_FUNCTION errorFunc)
{
    m_server = server;
    m_port = port;

    return _connect(errorFunc);
}

void Client::close()
{
    unregisterDisconnectHandler();
    if (m_client != NULL)
    {
        m_client->cleanup();
        delete m_client;
    }

    m_client = NULL;
}

void Client::receive()
{
    if (isConnect())
    {
        m_client->receive();
    }
}

static void * _onReceive(void * argv)
{
    Client *pClient = static_cast<Client *> (argv);
    if (NULL!=pClient)
    {
        pClient->receive();
    }

    return 0;
}

void Client::receiveWithPthread()
{
    pthread_t pthread;
    pthread_create(&pthread, NULL, _onReceive, this);
}

bool Client::reconnect(LUA_FUNCTION errorFunc)
{
    return _connect(errorFunc);
}

bool Client::send(const char *data, unsigned int size)
{
    if ((data == NULL) || (size < 1) || (m_client == NULL))
    {
        return false;
    }

    bool result = false;

    if (isConnect())
    {
        string str(data, size);
        result = m_client->send(str);
    }

    return result;
}

bool Client::isConnect()
{
    if (m_client != NULL)
    {
        return (m_client->state() == StateConnected);
    }

    return false;
}

bool Client::_connect(LUA_FUNCTION errorFunc)
{
    close();
    registerDisconnectHandler(errorFunc);
    m_client = new ConnectionTCPClient(this, m_log, m_server, m_port);
    ConnectionError error;
    for (int i = 0; i < 5; i++)
    {
        error = m_client->connect();
        if (error == ConnNoError)
        {
            break;
        }

#if defined( _WIN32 )
        Sleep(1000);
#else
        usleep(1000000);
#endif
    }

    return (error==ConnNoError);
}

void Client::handleReceivedData( const ConnectionBase* connection, const std::string& data )
{
    printf("received data======= len:%lu, str:%s\n", data.length(), data.c_str());
}

void Client::handleConnect(const ConnectionBase *connection)
{
    printf("connect succ!\n");
}

void Client::registerDisconnectHandler(LUA_FUNCTION func)
{
    m_disconnectCallback = func;
}

void Client::unregisterDisconnectHandler()
{

}

void Client::handleDisconnect(const ConnectionBase *connection, ConnectionError reason)
{
    if (m_client != NULL)
    {
        m_client->cleanup();
    }

    printf("disconnect succ reason===%d!\n", reason);
}
