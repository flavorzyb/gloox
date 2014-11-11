//
//  CmdUtils.h
//  CGame
//
//  Created by admin on 14-11-11.
//
//

#ifndef __NET__CMDUTILS_H_
#define __NET__CMDUTILS_H_
namespace utils
{
    class CmdUtils
    {
        public:
            enum{CMD_HEAD_SIZE = 2};
            static const bool IS_BIG_ENDIAN;
            static void orderData(char *pData, unsigned int len);
            static unsigned short getDataLen(char *pData);
    };
}
#endif /* defined(__NET__CMDUTILS_H_) */
