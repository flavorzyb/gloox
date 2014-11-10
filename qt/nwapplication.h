#ifndef NWAPPLICATION_H
#define NWAPPLICATION_H

#include <QCoreApplication>
#include <QThread>

class NWThread : public QThread
{
protected:
    void run();
};

class NWApplication : public QCoreApplication
{
public:
    explicit NWApplication(int &argc, char ** argv);
    virtual ~NWApplication();
    void run();

private:
    NWThread m_thread;
};

#endif // NWAPPLICATION_H
