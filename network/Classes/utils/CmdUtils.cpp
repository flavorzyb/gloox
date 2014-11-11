//
//  CmdUtils.cpp
//  CGame
//
//  Created by admin on 14-11-11.
//
//
#include <memory>
#include "ccMacros.h"
#include "CmdUtils.h"

namespace utils
{
    const bool CmdUtils::IS_BIG_ENDIAN = CC_HOST_IS_BIG_ENDIAN;
    // 消息协议头的定义存储消息长度的数据长度
    
    void CmdUtils::orderData(char *pData, unsigned int len)
    {
        if (false == IS_BIG_ENDIAN)
        {
            char *data = new char[len];
            for (int i = 0; i < len; i++)
            {
                data[i] = *(pData+len-i-1);
            }
            
            memcpy(pData, data, len);
            delete [] data;
        }
    }
    
    unsigned short CmdUtils::getDataLen(char *pData)
    {
        unsigned short result = 0;
        
        if (false==IS_BIG_ENDIAN)
        {
            char data[CMD_HEAD_SIZE] = {0};
            for (int i = 0; i < CMD_HEAD_SIZE; i++)
            {
                data[i] = *(pData+CMD_HEAD_SIZE - 1 -i);
            }
            
            memcpy(&result, data, CMD_HEAD_SIZE);
        }
        else
        {
            memcpy(&result, pData, CMD_HEAD_SIZE);
        }
        

        return result;
    }
}