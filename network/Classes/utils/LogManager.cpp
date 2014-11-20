//
//  LogManager.cpp
//  CGame
//
//  Created by admin on 14-10-20.
//
//
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pthread.h>
#include <unistd.h>
#include <curl/curl.h>
#include <curl/easy.h>

#include "LogManager.h"
using namespace std;

struct LogData
{
    string url;
    string log;
};

static struct LogData sLogData;

static void * _sendErrorLogWithPthread(void * argv)
{
    CURL * curl=curl_easy_init();
    if (curl)
    {
        bool result = (CURLE_OK == curl_easy_setopt(curl, CURLOPT_URL, sLogData.url.c_str())) &&
        (CURLE_OK == curl_easy_setopt(curl, CURLOPT_POSTFIELDS, sLogData.log.c_str())) &&
        (CURLE_OK == curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, sLogData.log.length())) &&
        (CURLE_OK == curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L)) &&
        (CURLE_OK == curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L)) &&
        (CURLE_OK == curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 5));
        if (result)
        {
            curl_easy_perform(curl);
        }
        curl_easy_cleanup(curl);
    }
    
    return 0;
}

LogManager * LogManager::m_instance = NULL;

LogManager::LogManager():
    m_logUrl("")
{
    
}

LogManager::~LogManager()
{
}

LogManager * LogManager::getInstance()
{
    if (m_instance == NULL)
    {
        m_instance = new LogManager();
    }
    
    return m_instance;
}

void LogManager::setLogUrl(const char * url)
{
    if (url != NULL)
    {
        m_logUrl = url;
    }
}

void LogManager::sendCrashLog(const string &msg)
{
    if ((msg.length() > 0) && (m_logUrl.length() > 0))
    {
        sLogData.log = "server=1&log=";
        sLogData.log += msg;
        sLogData.url = m_logUrl;
        
        pthread_t pthread;
        pthread_create(&pthread, NULL, _sendErrorLogWithPthread, NULL);
    }
}

void LogManager::sendErrorLog(const char * msg)
{
    if (msg != NULL)
    {
        sLogData.log = "server=1&error_log=";
        sLogData.log += msg;
        sLogData.url = m_logUrl;
        
        pthread_t pthread;
        pthread_create(&pthread, NULL, _sendErrorLogWithPthread, NULL);
    }
}
