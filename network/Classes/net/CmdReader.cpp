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
    CmdReader::CmdReader(char * pData, unsigned int len):
      m_len(len)
    , m_data(pData)
    {
    }
    
    CmdReader::~CmdReader()
    {
    }
    
    char * CmdReader::getData()
    {
        return m_data;
    }
    
    unsigned int CmdReader::getLength() const
    {
        return m_len;
    }
}
