#include <pthread.h>
#include <unistd.h>

#include "net/client.h"
#include "net/MessageQueue.h"
#include "utils/CmdUtils.h"

#include "cocos2d.h"

using namespace gloox;
using namespace std;
using namespace utils;
using namespace neo_engine;

USING_NS_CC;

namespace net
{
    class _ClientFrameScript : public CCObject
    {
    public:
        explicit _ClientFrameScript(LUA_FUNCTION callback);
        virtual ~_ClientFrameScript();
        
        virtual void framescript(float t);
        static _ClientFrameScript * create(LUA_FUNCTION callback);
        inline LUA_FUNCTION getCallback() const {return m_callback;}
    private:
        _ClientFrameScript(const _ClientFrameScript & fs);
        _ClientFrameScript & operator= (const _ClientFrameScript & fs);
        
    private:
        
        LUA_FUNCTION m_callback;
    };
    
    class _ClientDisconnectFrameScript : public _ClientFrameScript
    {
    protected:
        explicit _ClientDisconnectFrameScript(LUA_FUNCTION callback, int reason, Client * pClient);

    public:
        virtual ~_ClientDisconnectFrameScript();
        static _ClientDisconnectFrameScript * create(LUA_FUNCTION callback, int reason, Client * pClient);
        inline int getReason() const {return m_reason;}
        virtual void framescript(float t);
        
    private:
        int m_reason;
        Client * m_client;
    };
    
    _ClientFrameScript::_ClientFrameScript(LUA_FUNCTION callback):
      CCObject()
    , m_callback(callback)
    {
    }
    
    _ClientFrameScript::~_ClientFrameScript()
    {
    }
    
    void _ClientFrameScript::framescript(float t)
    {
        CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(_ClientFrameScript::framescript), this);
        if (m_callback > 0)
        {
            CCLuaStack *ls = LuaUtils::getSharedLuaStack();
            ls->executeFunctionByHandler(m_callback, 0);
        }
    }
    
    _ClientFrameScript * _ClientFrameScript::create(LUA_FUNCTION callback)
    {
        _ClientFrameScript * result = new _ClientFrameScript(callback);
        result->autorelease();
        
        return result;
    }
    
    _ClientDisconnectFrameScript::_ClientDisconnectFrameScript(LUA_FUNCTION callback, int reason, Client * pClient):
      _ClientFrameScript(callback)
    , m_reason(reason)
    , m_client(pClient)
    {
    }
    
    _ClientDisconnectFrameScript::~_ClientDisconnectFrameScript()
    {
        
    }
    
    _ClientDisconnectFrameScript * _ClientDisconnectFrameScript::create(LUA_FUNCTION callback, int reason, Client * pClient)
    {
        _ClientDisconnectFrameScript * result = new _ClientDisconnectFrameScript(callback, reason, pClient);
        result->autorelease();
        return result;
    }
    
    void _ClientDisconnectFrameScript::framescript(float t)
    {
        CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(_ClientDisconnectFrameScript::framescript), this);
        if (getCallback() > 0)
        {
            CCLuaStack *ls = LuaUtils::getSharedLuaStack();
            ls->pushInt(m_reason);
            ls->executeFunctionByHandler(getCallback(), 1);
        }
        
        if (m_client != NULL)
        {
            m_client->setIsDisconnectSchedule(false);
        }
    }
    
    static int S_MAX_RETRY_TIMES = 3;
    Client::Client():
        m_server("")
      , m_port(0)
      , m_client(NULL)
      , m_offset(0)
      , m_disconnectCallback(0)
      , m_onRecvHandler(0)
      , m_isDisconnectSchedule(false)
      , m_isConnecting(false)
    {
    }

    Client::~Client()
    {
        close();
        
        delete m_client;
        unregisterDisconnectHandler();
        unregisterOnRecvHandler();
    }

    void Client::init(const char * server, int port)
    {
        if (server == NULL)
        {
            return ;
        }
        
        m_server = server;
        m_port = port;
    }

    bool Client::connect(const char * server, int port)
    {
        if (server == NULL)
        {
            return false;
        }
        
        m_server = server;
        m_port = port;
        
        return connect();
    }
    
    struct _onConnectStruct
    {
        LUA_FUNCTION m_succ;
        Client * m_client;
    };
    
    
    static void * _onConnect(void * argv)
    {
        struct _onConnectStruct * cs = static_cast<struct _onConnectStruct *>(argv);
        if (NULL!=cs)
        {
            LUA_FUNCTION succ = cs->m_succ;
            Client * pClient = cs->m_client;
            
            if (pClient != NULL)
            {
                if (pClient->connect() && (succ > 0))
                {
                    _ClientFrameScript * frameScript = _ClientFrameScript::create(succ);
                    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(_ClientFrameScript::framescript), frameScript, 0, false);
                }
            }
        }
        
        delete cs;
        return 0;
    }
    
    void Client::connectWithPthread(const char * server, int port, LUA_FUNCTION succ)
    {
        init(server, port);
        struct _onConnectStruct *cs = new struct _onConnectStruct;
        cs->m_succ = succ;
        cs->m_client = this;
        pthread_t pthread;
        pthread_create(&pthread, NULL, _onConnect, cs);
    }
    
    void Client::close()
    {
        if (m_client != NULL)
        {
            m_client->cleanup();
        }
        
        m_offset = 0;
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

    bool Client::reconnect()
    {
        return connect();
    }
    
    static void * _onReconnect(void * argv)
    {
        struct _onConnectStruct *cs = static_cast<struct _onConnectStruct *>(argv);
        
        if (cs != NULL)
        {
            Client *pClient = cs->m_client;
            LUA_FUNCTION succ = cs->m_succ;
        
            if (NULL!=pClient)
            {
                if (pClient->reconnect() && (succ > 0))
                {
                    _ClientFrameScript * frameScript = _ClientFrameScript::create(succ);
                    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(_ClientFrameScript::framescript), frameScript, 0, false);
                }
            }
        }
        
        delete cs;
        
        return 0;
    }
    void Client::reconnectWithPthread(LUA_FUNCTION succ)
    {
        struct _onConnectStruct *cs = new struct _onConnectStruct;
        cs->m_succ = succ;
        cs->m_client = this;
        pthread_t pthread;
        pthread_create(&pthread, NULL, _onReconnect, cs);
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
            char  * buff = new char[size+2+1];
            buff[size+2] = '\0';
            memcpy(buff, &size, sizeof(unsigned short));
            CmdUtils::orderData(buff, 2);
            memcpy(buff + 2, data, size);
            string str(buff, size + 2);
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
    
    bool Client::isConnecting()
    {
        return m_isConnecting;
    }

    bool Client::connect()
    {
        close();

        if (m_client == NULL)
        {
            m_client = new ConnectionTCPClient(this, m_log, m_server, m_port);
        }
        else
        {
            m_client->setServer(m_server, m_port);
        }

        ConnectionError error;
        m_isConnecting = true;
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
            usleep(500000);
    #endif
        }
        
        if (error != ConnNoError)
        {
            _ClientDisconnectFrameScript * frameScript = _ClientDisconnectFrameScript::create(m_disconnectCallback, error, this);
            CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(_ClientDisconnectFrameScript::framescript), frameScript, 0, false);
        }
        m_isConnecting = false;
        
        return (error==ConnNoError);
    }

    static char S_STR_LEN[utils::CmdUtils::CMD_HEAD_SIZE] = {0};
    void Client::handleReceivedData( const ConnectionBase* connection, const std::string& data )
    {
        if (connection != m_client)
        {
            return ;
        }
        
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
                MessageQueue::getInstance()->add(pData+offset + utils::CmdUtils::CMD_HEAD_SIZE, len, m_onRecvHandler);
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
        close();

        if (m_disconnectCallback > 0 && (m_isDisconnectSchedule == false) && (isConnecting() == false))
        {

            m_isDisconnectSchedule = true;
            _ClientDisconnectFrameScript * frameScript = _ClientDisconnectFrameScript::create(m_disconnectCallback, reason, this);
            CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(_ClientDisconnectFrameScript::framescript), frameScript, 0, false);
        }
    }
    
    void Client::registerOnRecvHandler(LUA_FUNCTION handler)
    {
        if (m_onRecvHandler != handler)
        {
            unregisterOnRecvHandler();
            m_onRecvHandler = handler;
        }
    }
    
    void Client::unregisterOnRecvHandler()
    {
        LuaUtils::removeScriptHandler(m_onRecvHandler);
        m_onRecvHandler = 0;
    }
    
    void Client::release()
    {
        unregisterDisconnectHandler();
        if (m_client != NULL)
        {
            while (isConnect())
            {
                close();
                if (isConnect())
                {
#if defined( _WIN32 )
                    Sleep(1000);
#else
                    usleep(500000);
#endif
                }
            }
        }
        
        delete this;
    }
    
    Client * Client::create()
    {
        Client * result = new Client();

        return result;
    }
}
