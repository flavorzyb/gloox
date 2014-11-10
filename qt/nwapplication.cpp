#include <QtCore>
#include "nwapplication.h"

NWApplication::NWApplication(int &argc, char **argv):
    QCoreApplication(argc, argv)
{
}

NWApplication::~NWApplication()
{

}

void NWApplication::run()
{
    m_thread.start();
}


void NWThread::run()
{
    qDebug()<<"run........";
}
