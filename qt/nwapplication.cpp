#include <QtCore>
#include <string>
#include "nwapplication.h"
#include "net/client.h"

NWApplication::NWApplication(int &argc, char **argv):
    QCoreApplication(argc, argv)
{
}

NWApplication::~NWApplication()
{

}

void NWApplication::run()
{
    connect(&m_thread, SIGNAL(finished()), this, SLOT(quit()));
    m_thread.start();
}


void NWThread::run()
{
    Client client;
    qDebug()<<"will connect server ... ...";
    if (!client.connect("zhuyanbin.com", 1111))
    {
        qDebug()<<"connect server fail.";
    }

    qDebug()<<"connect server ... ...ok";
    client.receiveWithPthread();

    std::string data = "i am ok\n";
    while(client.isConnect())
    {
        qDebug()<<"will send data ... ..."<<data.c_str();
        client.send(data.c_str(), data.length());
        sleep(5);
    }
}
