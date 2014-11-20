//
//  MessageQueue.cpp
//  CGame
//
//  Created by admin on 14-11-11.
//
//
#include <pthread.h>
#include <unistd.h>
#include <deque>

#include "cocos2d.h"
#include "net/CmdReader.h"
#include "net/MessageQueue.h"

namespace net
{
    USING_NS_CC;
    using namespace utils;
    using namespace std;
    
    enum {QMESSAGEQUEUEMAXSIZE=409600};
    static char QMessageQueue[QMESSAGEQUEUEMAXSIZE];
    static int  QMessageQueueSize = 0;
    static int  QMessageQueueOffset = 0;
    
    static deque<CmdReader *> sCmdReaderQueue;
    
    static pthread_mutex_t      sQueueMutex;
    
    // 每帧处理的消息数量
    const int MessageQueue::MAX_MESSAGE_PER_FRAME_RATE = 3;
    MessageQueue* MessageQueue::s_instance = NULL;
    
    MessageQueue::MessageQueue():
    m_isInit(false)
    {
    }
    
    MessageQueue::~MessageQueue()
    {
    }
    
    MessageQueue * MessageQueue::getInstance()
    {
        if (s_instance == NULL)
        {
            s_instance = new MessageQueue();
        }
        
        return s_instance;
    }
    
    void MessageQueue::init()
    {
        pthread_mutex_init(&sQueueMutex, NULL);
        m_isInit = true;
    }
    
    void MessageQueue::add(const char * data, int size, LUA_FUNCTION handler)
    {
        CCAssert(m_isInit, "MessageQueue must be init before add.");
        
        do
        {
            pthread_mutex_lock(&sQueueMutex);
            if ((QMESSAGEQUEUEMAXSIZE- QMessageQueueSize) < size)
            {
                pthread_mutex_unlock(&sQueueMutex);
                
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
                Sleep(100);
#else
                usleep(100000);
#endif
            }
            else
            {
                int len = QMESSAGEQUEUEMAXSIZE - QMessageQueueOffset - QMessageQueueSize;
                if (len < size)
                {
                    memmove(QMessageQueue, QMessageQueue+QMessageQueueOffset, QMessageQueueSize);
                    QMessageQueueOffset = 0;
                }
                
                memcpy(QMessageQueue+QMessageQueueSize+QMessageQueueOffset, data, size);
                
                CmdReader * reader = new CmdReader(QMessageQueue+QMessageQueueSize+QMessageQueueOffset, size, handler);
                sCmdReaderQueue.push_back(reader);
                QMessageQueueSize +=size;
                
                pthread_mutex_unlock(&sQueueMutex);
                break;
            }
            
        } while (true);
    }
    
    void MessageQueue::dispatcherAllMessage()
    {
        if (sCmdReaderQueue.size() == 0)
        {
            return ;
        }

        int count = 0;
        
        CCLuaStack *LS = LuaUtils::getSharedLuaStack();
        
        pthread_mutex_lock(&sQueueMutex);
        do
        {
            CmdReader *reader = sCmdReaderQueue.front();

            if (reader->getHandler() > 0 )
            {
                LS->pushString(reader->getData(), reader->getLength());
                LS->pushInt(reader->getLength());
                LS->executeFunctionByHandler(reader->getHandler(), 2);
            }
            
            QMessageQueueSize -=reader->getLength();
            QMessageQueueOffset += reader->getLength();

            delete reader;
            
            sCmdReaderQueue.pop_front();
            
            count++;
            if (count >=MAX_MESSAGE_PER_FRAME_RATE)
            {
                break;
            }
        }while (sCmdReaderQueue.size() > 0);
        pthread_mutex_unlock(&sQueueMutex);
    }
}