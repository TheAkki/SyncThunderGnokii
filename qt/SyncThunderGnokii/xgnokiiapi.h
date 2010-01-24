#ifndef XGNOKIIAPI_H
#define XGNOKIIAPI_H

#include <QtGlobal>
#include <QString>
#include <QVector>

#include <gnokii.h>
#include <gnokii/statemachine.h>
#include "Global.h"

/*
#define SZ_IMEI         100
#define SZ_MANUF        100
#define SZ_MODEL        100
#define SZ_PROD_NAME    100
#define SZ_REVISION     20
typedef struct
{
    uchar   c_IMEI[SZ_IMEI];
    uchar   c_Manufacturer[SZ_MANUF];
    uchar   c_Model[SZ_MODEL];
    uchar   c_Product[SZ_PROD_NAME];
    uchar   c_Revision[SZ_REVISION];
}rHANDY_INFO;
*/

typedef struct
{
    QString strIMEI;
    QString strManufacturer;
    QString strModel;
    QString strProduct;
    QString strRevision;
}rHANDY_INFO;

typedef struct
{
    QString             strConnection;
    quint16             ui16Number;
}rSUPPORTED_CONNECTIONS;

class xGnokiiApi
{
    public:
        typedef QVector<rSUPPORTED_CONNECTIONS> vecConnectionType;

    public:
        xGnokiiApi();
        ~xGnokiiApi();

        STG_ERRORCODE::tErrorcode eDoInit();
        STG_ERRORCODE::tErrorcode eDoInit(QString strConfigFile,
                                          QString strConfigEntry);
        STG_ERRORCODE::tErrorcode eDoReadHandyInfo(rHANDY_INFO& rHInfo);
        STG_ERRORCODE::tErrorcode eGetSupportedConnections(vecConnectionType&
                        ConnType);
        quint32 ui32GetUsedGnokiiApi();

        // Print functions
        void vPrintSupportedConnections(vecConnectionType& ConnType) const;
        void vPrintHandyInfo(const rHANDY_INFO& rHInfo);
        void vPrintUsedGnokiiApi(quint32 uiVersionGnokiiApi);

    private: // functions
        void v_CommonConstructor();
        STG_ERRORCODE::tErrorcode e_TerminateBus();

        // DEBUG
        void v_PrintGnokiiState(const gn_statemachine* statemachine);

    private: //var
        STG_ERRORCODE::tErrorcode c_Status;

        //rHandyInfo c_HandyInfo; //
        gn_statemachine*    c_pApiState;

};

#endif // XGNOKIIAPI_H
