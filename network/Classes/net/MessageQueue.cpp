//
//  MessageQueue.cpp
//  CGame
//
//  Created by admin on 14-11-11.
//
//
#include <pthread.h>
#include <unistd.h>

#include "cocos2d.h"
#include "net/MessageQueue.h"

namespace net
{
    enum {QMESSAGEQUEUEMAXSIZE=409600};
    static char QMessageQueue[QMESSAGEQUEUEMAXSIZE];
    static int  QMessageQueueSize = 0;
    static int  QMessageQueueOffset = 0;
    
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
    
    void MessageQueue::add(char * data, int size)
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
                QMessageQueueSize +=size;
                pthread_mutex_unlock(&sQueueMutex);
                break;
            }
            
        } while (true);
    }
    
    void MessageQueue::dispatcherAllMessage()
    {
//        if (QMessageQueueSize < 16) return ;
//        int len = 0;
//        int count = 0;
//        ConnectionEventManager * conn = ConnectionEventManager::getInstance();
//        pthread_mutex_lock(&sQueueMutex);
//        do
//        {
//            if (QMessageQueueSize <= 0)
//            {
//                break;
//            }
//            
//            len = CmdUtils::getDataLen(QMessageQueue+12+QMessageQueueOffset);
//            CmdReader reader(QMessageQueue+QMessageQueueOffset, len);
//            conn->dispatcher(&reader);
//            QMessageQueueSize -=len;
//            QMessageQueueOffset += len;
//            if (!((reader.getCmdId()==100019) ||
//                  (reader.getCmdId()==100097) ||
//                  (reader.getCmdId()==100043) ||
//                  (reader.getCmdId()==100017)))
//            {
//                count++;
//            }
//            if (count >=MAX_MESSAGE_PER_FRAME_RATE)
//            {
//                break;
//            }
//        }while (1);
//        pthread_mutex_unlock(&sQueueMutex);
    }
}