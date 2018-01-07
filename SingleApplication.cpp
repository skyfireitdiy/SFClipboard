//SingleApplication.cpp
#include "SingleApplication.h"
SingleApplication::SingleApplication(const QString uniqueKey) :
    _uniqueKey(uniqueKey)

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

SingleApplication::~SingleApplication()
{
    sharedMemory.detach();
}

bool SingleApplication::isRunning()
{
    return _isRunning;
}

void SingleApplication::detach(){
    sharedMemory.detach();
}
