//
//  CmdReader.h
//  CGame
//
//  Created by admin on 14-11-11.
//
//

#ifndef __NET__CMDREADER_H_
#define __NET__CMDREADER_H_
namespace net
{
    class CmdReader
    {
        private:
            CmdReader(const CmdReader & reader);
            CmdReader & operator= (const CmdReader & reader);
        public:
            CmdReader(char * pData, unsigned int len);
            ~CmdReader();
            char * getData();
            unsigned int getLength() const;
        
        private:
            unsigned int m_len;
            char * m_data;
    };
}
#endif /* defined(__NET__CMDREADER_H_) */
