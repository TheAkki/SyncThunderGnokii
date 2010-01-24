#include <QVector>

#include <xgnokiiapi.h>
#include "Global.h"
#include <gnokii.h>
#include <gnokii/error.h>
#include <gnokii/statemachine.h>
#include <memory.h>

#include <iostream>


/**
  @brief Contructor for Connection to Gnokii-Api
*/
xGnokiiApi::xGnokiiApi()
{
    v_CommonConstructor();
}


/**
  @brief Destructor
*/
xGnokiiApi::~xGnokiiApi()
{
    e_TerminateBus();
}


/**
  @brief This function terminate usb bus and close gnokii
  @return State of this class
*/
STG_ERRORCODE::tErrorcode xGnokiiApi::e_TerminateBus()
{
    gn_lib_phone_close(c_pApiState);
    gn_lib_phoneprofile_free(&c_pApiState);
    gn_lib_library_free();

    c_Status = STG_ERRORCODE::E_NOT_INIT;
    return c_Status;
}


/**
  @brief This function initialize the connection to gnokki and the configured
        phone.
  @param[in] strConfigFile  File in which the gnokii config is stored
  @param[in] strConfigEntry Entry which contain the selected phone
  @return State of this class
  @todo Clean up memory if you exit this function in error case
*/
STG_ERRORCODE::tErrorcode xGnokiiApi::eDoInit(
    QString strConfigFile,
    QString strConfigEntry
)
{
    c_Status = STG_ERRORCODE::E_NOT_INIT;
    c_pApiState = NULL;

    gn_error result = GN_ERR_FAILED;

    // Call Init
    result = gn_lib_init();
    if( result != GN_ERR_NONE )
    {
        std::cout << "Init failed with errorcode: " << result << "("
                  << gn_error_print( result ) << ")\n";
        return c_Status;
    }

    // load config from file
    int i__ConfigFileSize = strConfigFile.size() + 1;
    int i__ConfigEntrySize = strConfigEntry.size() + 1;
    char* pc__StrConfigFile = NULL;
    char* pc__StrConfigEntry = NULL;

    if(! strConfigEntry.isEmpty() )
    {
        pc__StrConfigEntry = new char[i__ConfigEntrySize];
        if( pc__StrConfigEntry == NULL)
        {
            c_Status = STG_ERRORCODE::E_NO_MEMORY;
            return c_Status;
        }
        qstrncpy(pc__StrConfigEntry, strConfigEntry.toAscii().constData(),
                 i__ConfigEntrySize);
    }

    if( strConfigFile.isEmpty() )
    {
        result = gn_lib_phoneprofile_load(pc__StrConfigEntry, &c_pApiState);
    }
    else
    {
        pc__StrConfigFile = new char[i__ConfigFileSize];
        if( pc__StrConfigFile == NULL)
        {
            c_Status = STG_ERRORCODE::E_NO_MEMORY;
            return c_Status;
        }
        qstrncpy(pc__StrConfigFile, strConfigFile.toAscii().constData(),
                 i__ConfigFileSize);

        result = gn_lib_phoneprofile_load_from_file(pc__StrConfigFile,
                                                    pc__StrConfigEntry,
                                                    &c_pApiState);
    }

    if( result != GN_ERR_NONE )
    {
        std::cout << "Load config failed with errorcode: " << result << "("
                  << gn_error_print( result ) << ")\n";
        return c_Status;
    }

    // open phone connection
    result = gn_lib_phone_open( c_pApiState );
    if( result != GN_ERR_NONE )
    {
        std::cout << "Open phone failed with errorcode: " << result << "("
                  << gn_error_print( result ) << ")\n";
        return c_Status;
    }

    // All Ok -> Set State
    c_Status = STG_ERRORCODE::S_OK;

    // clean up
    if( pc__StrConfigEntry) { delete[] pc__StrConfigEntry; }
    if( pc__StrConfigFile){ delete[] pc__StrConfigFile; }
    return c_Status;
}


/**
  @brief This function initialize the connection to gnokki and the configured
        phone.
  @return State of this class
*/
STG_ERRORCODE::tErrorcode xGnokiiApi::eDoInit()
{
    // use global options
    return eDoInit(QString(), QString());
}


/**
  @brief Common Constructor
*/
void xGnokiiApi::v_CommonConstructor()
{
    c_Status = STG_ERRORCODE::E_NOT_INIT;
}


/**
    @brief Function read informations of connected phone
    @param[out] rHInfo  In this structure the infos will be stored
    @return State of this class
*/
STG_ERRORCODE::tErrorcode xGnokiiApi::eDoReadHandyInfo(rHANDY_INFO& rHInfo)
{
    if( !MA_STS_SUCCESS( c_Status ) )
    {
        return c_Status;
    }

    // Pointer to String to Read
    const char *pc__String = NULL;

    // read IMEI
    pc__String = gn_lib_get_phone_imei(c_pApiState);
    rHInfo.strIMEI = QString( pc__String );

    // read Manufacturer
    pc__String = gn_lib_get_phone_manufacturer(c_pApiState);
    rHInfo.strManufacturer = QString( pc__String );

    // read Model
    pc__String = gn_lib_get_phone_model(c_pApiState);
    rHInfo.strModel = QString( pc__String );

    // read Product
    pc__String = gn_lib_get_phone_product_name(c_pApiState);
    rHInfo.strProduct = QString( pc__String );

    // read Revision
    pc__String = gn_lib_get_phone_revision(c_pApiState);
    rHInfo.strRevision = QString( pc__String );

    return c_Status;
}


/**
  @brief This function list all available connection modes
  @param[out] ConnType In this vector the informations will be stored.
  @return Wenn OK: STG_ERRORCODE::S_OK else State of this Class
*/
STG_ERRORCODE::tErrorcode xGnokiiApi::eGetSupportedConnections(
    vecConnectionType& ConnType
)
{
    if(  MA_STS_FAILED(c_Status) )
    {
        return c_Status;
    }

    for(int iLoop = 0; iLoop < 255; iLoop++)
    {
        const char* pc__SuppConn = gn_lib_get_supported_connection( iLoop );
        if( pc__SuppConn == NULL )
        {
            return STG_ERRORCODE::S_OK;
        }
        else
        {
            rSUPPORTED_CONNECTIONS rTemp;
            rTemp.ui16Number = (quint16) iLoop;
            rTemp.strConnection = QString( pc__SuppConn );
            ConnType.append( rTemp );
        }
    }

    // should not be reached
    return STG_ERRORCODE::E_INVALID_STATE;
}


/**
  @brief This function read the used version of libgnokii
  @return Version [7..0]Revision, [15..8] Minor, [23..16] Major
*/
quint32 xGnokiiApi::ui32GetUsedGnokiiApi()
{
    unsigned int ui__GnokiiVersion = 0;
    ui__GnokiiVersion = gn_lib_version();

    return (quint32)ui__GnokiiVersion;
}


/**
  @brief This function print the actual gnokii state as String
  @param[in] statemachine Pointer to Gnokii Statemachine
*/
void xGnokiiApi::v_PrintGnokiiState(const gn_statemachine* statemachine)
{
    std::cout << "cGnokiiApi::v_PrintGnokiiState:" << "State is: '";

    switch(statemachine->current_state)
    {
        case GN_SM_Startup:
            std::cout << "GN_SM_Startup";
            break;

        case GN_SM_Initialised:
            std::cout << "GN_SM_Initialised";
            break;

        case GN_SM_MessageSent:
            std::cout << "GN_SM_MessageSent";
            break;

        case GN_SM_WaitingForResponse:
            std::cout << "GN_SM_WaitingForResponse";
            break;

        case GN_SM_ResponseReceived:
            std::cout << "GN_SM_ResponseReceived";
            break;

        default:
            std::cout << "INVALID_STATE";
            break;
    }
    std::cout << "'\n";
}


/**
  @brief This Function print a vector of connections
  @param[in] ConnType Referece to Vector with supported Connections
            (use eGetSupportedConnections())
*/
void xGnokiiApi::vPrintSupportedConnections(vecConnectionType& ConnType) const
{
    std::cout << "Supported Connections:\n"
              << "======================\n";
    for(vecConnectionType::iterator itLoop = ConnType.begin();
        itLoop != ConnType.end();
        itLoop++)
    {
        std::cout << "Connection No: " << itLoop->ui16Number
                  << "\t" << itLoop->strConnection.toStdString() << "\n";
    }

    std::cout << "\n";
}


/**
  @brief This Function print the Version of libgnokii
  @param[in] Version of libgnokii (use ui32GetUsedGnokiiApi())
*/
void xGnokiiApi::vPrintUsedGnokiiApi(quint32 uiVersionGnokiiApi)
{
    quint32 ui32Major = (uiVersionGnokiiApi >> 16);
    quint32 ui32Minor = (uiVersionGnokiiApi & 0xF0) >> 8;
    quint32 ui32Release = (uiVersionGnokiiApi & 0xF);

    std::cout << "Used Version of Gnokii lib is: " << ui32Major << "."
              << ui32Minor << "." << ui32Release << "\n";
}


/**
  @brief This Function print structur rHANDY_INFO
  @param[in] Reference to structure
*/
void xGnokiiApi::vPrintHandyInfo(const rHANDY_INFO& rHInfo)
{
    std::cout   << "Handy Infos\n "
                << "===========\n"
                << "Manufacturer:" << "\t" <<
                    rHInfo.strManufacturer.toStdString() << "\n"
                << "Model:" << "\t\t" << rHInfo.strModel.toStdString() << "\n"
                << "Productname:" << "\t" << rHInfo.strProduct.toStdString() <<
                    "\n"
                << "Revision:" << "\t" << rHInfo.strRevision.toStdString() <<
                    "\n"
                << "IMEI:" << "\t\t" << rHInfo.strIMEI.toStdString() << "\n"
                << "\n";
}
