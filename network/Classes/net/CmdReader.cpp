//
//  CmdReader.cpp
//  CGame
//
//  Created by admin on 14-11-11.
//
//

#include "net/CmdReader.h"

namespace net
{
    CmdReader::CmdReader(char * pData, unsigned int len, LUA_FUNCTION handler):
      m_len(len)
    , m_data(pData)
    , m_handler(handler)
    {
    }
    
    CmdReader::~CmdReader()
    {
    }
    
    const char * CmdReader::getData() const
    {
        return m_data;
    }
    
    unsigned int CmdReader::getLength() const
    {
        return m_len;
    }
    
    LUA_FUNCTION CmdReader::getHandler() const
    {
        return m_handler;
    }
}
