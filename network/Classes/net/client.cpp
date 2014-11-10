#include "net/client.h"
#include <pthread.h>
#include <unistd.h>

using namespace gloox;
using namespace std;

Client::Client():
    m_server("")
  , m_port(0)
  , m_dataHandler(NULL)
  , m_client(NULL)
{
    m_dataHandler = new DataHandler();
}

Client::~Client()
{
    delete m_client;
    delete m_dataHandler;
}

bool Client::connect(const string &server, int port, LUA_FUNCTION errorFunc)
{
    m_server = server;
    m_port = port;

    close();

    m_dataHandler->registerDisconnectHandler(errorFunc);
    m_client = new ConnectionTCPClient(m_dataHandler, m_log, m_server, m_port);
    ConnectionError error;
    for (int i = 0; i < 5; i++)
    {
        error = m_client->connect();
        if (error == ConnNoError)
        {
            break;
        }

#if defined( _WIN32 )
        Sleep(100);
#else
        usleep(500000);
#endif
    }

    return (error==ConnNoError);
}

void Client::close()
{
    m_dataHandler->unregisterDisconnectHandler();
    if (m_client != NULL)
    {
        m_client->cleanup();
        delete m_client;
    }

    m_client = NULL;
}

void Client::receive()
{

}

void Client::receiveWithPthread()
{
}

bool Client::reconnect()
{
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
        m_client->send(str);
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
