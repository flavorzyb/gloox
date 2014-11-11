//
//  MessageQueue.h
//  CGame
//
//  Created by admin on 14-11-11.
//
//

#ifndef __NET__MESSAGEQUEUE_H_
#define __NET__MESSAGEQUEUE_H_

namespace net
{
    class MessageQueue
    {
    protected:
        // 每帧处理的消息数量
        static const int MAX_MESSAGE_PER_FRAME_RATE;
        MessageQueue();
        
        static MessageQueue* s_instance;
        
    public:
        virtual ~MessageQueue();
        static MessageQueue * getInstance();
        void init();
        void add(char * data, int size);
        void dispatcherAllMessage();
        
    private:
        bool m_isInit;
    };
}
#endif /* defined(__NET__MESSAGEQUEUE_H_) */
