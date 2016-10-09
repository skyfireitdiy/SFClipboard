//SingleApplication.cpp
#include "SingleApplication.h"
SingleApplication::SingleApplication(int &argc, char *argv[], const QString uniqueKey) :
    QApplication(argc, argv), _uniqueKey(uniqueKey)

{
    sharedMemory.setKey(_uniqueKey);
    if (sharedMemory.attach())
        _isRunning = true;
    else
    {
        _isRunning = false;
        if (!sharedMemory.create(1))
        {
            return;
        }
    }
}

bool SingleApplication::isRunning()
{
    return _isRunning;
}
