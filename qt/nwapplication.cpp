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

    client.receiveWithPthread();

    std::string data = "i am ok";
    while(client.isConnect())
    {
        client.send(data.c_str(), data.length());
        sleep(1);
    }
}
