#include "nwapplication.h"

int main(int argc, char *argv[])
{
    NWApplication a(argc, argv);
    a.run();
    return a.exec();
}
