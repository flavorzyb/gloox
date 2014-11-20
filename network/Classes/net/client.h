#ifndef __NET_CLIENT_H
#define __NET_CLIENT_H
#include <string>
#include "connectiontcpclient.h"
#include "logsink.h"
#include "connectiondatahandler.h"
#include "LuaUtils.h"
#include "LuaDelegate.h"

namespace net
{
    class Client : public gloox::ConnectionDataHandler
    {
    public:
        Client();
        virtual ~Client();
        void init(const char * server, int port);
        bool connect(const char * server, int port);
        bool connect();
        // 静默连接
        void connectWithPthread(const char * server, int port, LUA_FUNCTION succ);
        
        void close();
        void receive();
        void receiveWithPthread();
        bool reconnect();
        void reconnectWithPthread(LUA_FUNCTION succ);
        bool send(const char * data, unsigned int size);
        bool isConnect();
        bool isConnecting();

        void handleReceivedData(const gloox::ConnectionBase* connection, const std::string& data );

        void handleConnect(const gloox::ConnectionBase* connection);

        void registerDisconnectHandler(LUA_FUNCTION func);
        void unregisterDisconnectHandler();
        void handleDisconnect(const gloox::ConnectionBase* connection, gloox::ConnectionError reason);
        
        void registerOnRecvHandler(LUA_FUNCTION handler);
        void unregisterOnRecvHandler();
        
        void release();
        
        static Client * create();

        inline LUA_FUNCTION getDisconnectCallBack() const {return m_disconnectCallback;}
        inline void setIsDisconnectSchedule(bool isDisconnectSchedule) { m_isDisconnectSchedule = isDisconnectSchedule;}
    private:
        Client(const Client & c);
        Client & operator=(const Client & c);

    private:
        enum{BUFF_SIZE=204800};
        std::string                   m_server;
        int                           m_port;
        gloox::ConnectionTCPClient  * m_client;
        gloox::LogSink                m_log;
        char                          m_buf[BUFF_SIZE];
        int                           m_offset;
        LUA_FUNCTION                  m_disconnectCallback;
        LUA_FUNCTION                  m_onRecvHandler;
        bool                          m_isDisconnectSchedule;
        bool                          m_isConnecting;
    };
}
#endif // __NET_CLIENT_H
