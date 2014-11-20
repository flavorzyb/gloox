//
//  CmdReader.h
//  CGame
//
//  Created by admin on 14-11-11.
//
//

#ifndef __NET__CMDREADER_H_
#define __NET__CMDREADER_H_
#include "LuaUtils.h"

namespace net
{
    class CmdReader
    {
        private:
            CmdReader(const CmdReader & reader);
            CmdReader & operator= (const CmdReader & reader);
        public:
            explicit CmdReader(char * pData, unsigned int len, LUA_FUNCTION handler);
            ~CmdReader();
            const char * getData() const;
            unsigned int getLength() const;
            LUA_FUNCTION getHandler() const;
        private:
            unsigned int      m_len;
            char            * m_data;
            LUA_FUNCTION      m_handler;
    };
}
#endif /* defined(__NET__CMDREADER_H_) */
