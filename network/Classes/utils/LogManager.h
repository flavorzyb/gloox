//
//  LogManager.h
//  CGame
//
//  Created by admin on 14-10-20.
//
//

#ifndef __CGAME__LOGMANAGER__
#define __CGAME__LOGMANAGER__
#include <string>

class LogManager
{
    private:
        LogManager();
        LogManager(const LogManager & lm);
        LogManager & operator = (const LogManager & lm);
    
    public:
        virtual ~LogManager();
        static LogManager * getInstance();
        void sendCrashLog(const std::string & msg);
        void sendErrorLog(const char * msg);
        void setLogUrl(const char * url);
    
        inline const char * getLogUrl() const { return m_logUrl.c_str();}
    
    private:
        static LogManager * m_instance;
        std::string m_logUrl;
};

#endif /* defined(__CGAME__LOGMANAGER__) */
