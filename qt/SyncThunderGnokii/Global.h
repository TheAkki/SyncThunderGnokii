#ifndef GLOBAL_H
#define GLOBAL_H
#include <QtGlobal>

namespace STG_ERRORCODE
{
    typedef qint32  tErrorcode; // OK >= 0, NOK < 0
    const tErrorcode S_OK               =   0;
    const tErrorcode E_NOK              = - 1;
    const tErrorcode E_NOT_INIT         = - 2;
    const tErrorcode E_NO_MEMORY        = - 3;
    const tErrorcode E_INVALID_STATE    = - 4;
    const tErrorcode E_INVALID_ARG      = - 5;
    const tErrorcode E_NOT_IMPLEMENTET  = - 6;
    const tErrorcode E_CANNOT_EXECUTE   = - 7;
}

#define TRUE    true
#define FALSE   false


#define MA_STS_SUCCESS(a) ( ((qint32)a>=0)?TRUE:FALSE )
#define MA_STS_FAILED(a) ( ((qint32)a<0)?TRUE:FALSE )

#endif // GLOBAL_H
