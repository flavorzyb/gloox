#include <pthread.h>
#include <unistd.h>

#include "net/client.h"
#include "net/MessageQueue.h"
#include "utils/CmdUtils.h"

#include "cocos2d.h"

using namespace gloox;
using namespace std;
using namespace utils;

USING_NS_CC;

namespace net
{
    static int S_MAX_RETRY_TIMES = 5;
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
        if (m_client != NULL)
        {
            m_client->cleanup();
        }
        
        delete m_client;
        unregisterDisconnectHandler();
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
        for (int i = 0; i < S_MAX_RETRY_TIMES; i++)
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

    static char S_STR_LEN[utils::CmdUtils::CMD_HEAD_SIZE] = {0};
    void Client::handleReceivedData( const ConnectionBase* connection, const std::string& data )
    {
        int pSize = data.length();
        memcpy(m_buf + m_offset, data.c_str(), pSize);
        m_offset +=pSize;
        pSize = m_offset;
        char *pData = m_buf;
        
        unsigned short offset = 0;
        unsigned short len = 0;
        
        do
        {
            if (offset>= pSize)
            {
                m_offset = 0;
                break;
            }
            
            len = pSize-offset;
            if (len < utils::CmdUtils::CMD_HEAD_SIZE)
            {
                memmove(m_buf, pData+offset, len);
                m_offset = len;
                break;
            }

            
            memcpy(S_STR_LEN, pData+offset, utils::CmdUtils::CMD_HEAD_SIZE);
            len = CmdUtils::getDataLen(S_STR_LEN);

            if ((pSize-offset)>=(len + utils::CmdUtils::CMD_HEAD_SIZE))
            {
                MessageQueue::getInstance()->add(pData+offset + utils::CmdUtils::CMD_HEAD_SIZE, len);
                offset += len + utils::CmdUtils::CMD_HEAD_SIZE;
            }
            else
            {
                len = pSize-offset;
                memmove(m_buf, pData+offset, len);
                
                m_offset = len;
                break;
            }
        }while (true);
    }

    void Client::handleConnect(const ConnectionBase *connection)
    {
        //@todo do nothing
    }

    void Client::registerDisconnectHandler(LUA_FUNCTION func)
    {
        unregisterDisconnectHandler();
        m_disconnectCallback = func;
    }

    void Client::unregisterDisconnectHandler()
    {
        LuaUtils::removeScriptHandler(m_disconnectCallback);
        m_disconnectCallback = 0;
    }

    void Client::handleDisconnect(const ConnectionBase *connection, ConnectionError reason)
    {
        if (m_client != NULL)
        {
            m_client->cleanup();
        }

        m_offset = 0;
        
        CCLOG("disconnect succ reason===%d!\n", reason);
        if (m_disconnectCallback > 0)
        {
            CCLuaStack *ls = LuaUtils::getSharedLuaStack();
            ls->pushInt(reason);
            ls->executeFunctionByHandler(m_disconnectCallback, 1);
        }
    }
}
