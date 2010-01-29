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
        std::cerr << "Open phone failed with errorcode: " << result << "("
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
  @brief Function to read memory usage of one memory type
  @param[in] eMemory Type of Memory which want to read
  @param[out] Structure to write data in
  @return OK: STG_ERRORCODE::S_OK, NOK: STG_ERRORCODE::E_CANNOT_EXECUTE
*/
STG_ERRORCODE::tErrorcode xGnokiiApi::eDoReadMemoryUsage(
    eMEMORY_TYPE eMemory,
    rMEMORY_USAGE& Data
)
{
    if( MA_STS_FAILED(c_Status ) )
    {
        return c_Status;
    }

    Data.eMemory = eMemory;
    gn_error result = gn_lib_addressbook_memstat(
            c_pApiState,
            (gn_memory_type) eMemory,
            &Data.i32Used,
            &Data.i32Free
    );

    // check if ok
    if( result != GN_ERR_NONE)
    {
        Data.i32Free = -1;
        Data.i32Used = -1;
        std::cerr   << "xGnokiiApi::eDoReadMemoryUsage: Can't read ("
                    << result << ") " << gn_error_print(result) << "\n";
        return STG_ERRORCODE::E_CANNOT_EXECUTE;
    }

    return c_Status;
}


/**
  @brief This function read the memory usage of all available types of memory
  @param[out] MemUsage Vector with usage informations
  @return OK: STG_ERRORCODE::S_OK, NOK: STG_ERRORCODE::E_CANNOT_EXECUTE
  */
STG_ERRORCODE::tErrorcode xGnokiiApi::eDoReadMemoryUsage(vecMemUsage& MemUsage)
{
    MemUsage.clear();
    STG_ERRORCODE::tErrorcode result = STG_ERRORCODE::E_NOK;
    for(quint16 ui16__Loop = MT_InternalMemory;
        ui16__Loop < MT_Invalid;
        ui16__Loop++)
    {
        if( ui16__Loop == MT_IntMemorySim) continue;

        rMEMORY_USAGE r__Temp;
        result = eDoReadMemoryUsage(static_cast<eMEMORY_TYPE>(ui16__Loop),
                                    r__Temp);
        if( MA_STS_SUCCESS(result) )
        {
            MemUsage.push_back( r__Temp );
        }
        else
        {
            return result;
        }
    }
    return STG_ERRORCODE::S_OK;
}


STG_ERRORCODE::tErrorcode xGnokiiApi::eDoReadPhonebookEntry
(
    eMEMORY_TYPE eMemory,
    qint32 Slot,
    rPHONEBOOK_ENTRY& Entry
)
{
    STG_ERRORCODE::tErrorcode result = STG_ERRORCODE::E_NOK;

    Entry.rHandy.eMemType   = eMemory;
    Entry.rHandy.i32Slot    = Slot;

    int i__IsEmpty = gn_lib_phonebook_entry_isempty(
                c_pApiState,
                (gn_memory_type) eMemory,
                (int) Slot
            );

    if( i__IsEmpty == 0)
    {
        Entry.rHandy.bHasData = TRUE;
        gn_error gn__result = GN_ERR_FAILED;
        gn__result = gn_lib_phonebook_read_entry(
                        c_pApiState,
                        (gn_memory_type) eMemory,
                        (int) Slot
                    );

        // check if i can load phonebook entry in internal gnokii struct
        if( gn__result != GN_ERR_NONE )
        {
            std::cerr   << "xGnokiiApi::eDoReadPhonebookEntry(): Can't read "
                        << "phonebook entry (" << gn__result << ") "
                        << gn_error_print(gn__result) << "\n";
            result = STG_ERRORCODE::E_CANNOT_EXECUTE;
            return result;
        }

        gn_phonebook_entry* po_Entry = &(c_pApiState->u.pb_entry);

        // Get Name
        Entry.strName = QString( po_Entry->name );
        Entry.strMainNumber = QString( po_Entry->number );

        vecPhoneBookSubE vecSubEntry;

        // Read SubEntrys
        for(quint16 ui16__Loop = 0;
            ui16__Loop < po_Entry->subentries_count;
            ui16__Loop++
        )
        {
            rPHONEBOOK_SUBENTRY tempSubE;
            tempSubE.eSubType = (xGnokiiApi::ePHONEBOOK_SUBENTRY_TYPE)
                                po_Entry->subentries[ui16__Loop].entry_type;

            // work depend on SubEntryType
            switch( tempSubE.eSubType )
            {
                default:
                    tempSubE.eNumType = (xGnokiiApi::ePHONEBOOK_NUMBER_TYPE)
                        po_Entry->subentries[ui16__Loop].number_type;
                    tempSubE.strValue =
                        QString( po_Entry->subentries[ui16__Loop].data.number );

            }

            // Add Entry to Vector
            vecSubEntry.push_back( tempSubE );
        }

        // add Vector to Struct
        Entry.vSubEntrys = vecSubEntry;

    }
    result = STG_ERRORCODE::S_OK;

    return result;
}


STG_ERRORCODE::tErrorcode xGnokiiApi::eDoReadPhonebookEntry(
    eMEMORY_TYPE eMemory,
    vecPhoneBookEntry& Entrys,
    bool boClearVector
)
{
    STG_ERRORCODE::tErrorcode result = STG_ERRORCODE::E_NOK;
    rMEMORY_USAGE rMem;
    eDoReadMemoryUsage(eMemory, rMem);

    // clear Vector
    if(boClearVector == TRUE)
    {
        Entrys.clear();
    }

    // read all Entrys
    quint32 ui32_SumEntry = rMem.i32Used + rMem.i32Free;
    for(quint32 ui32__Loop = 1; ui32__Loop <= ui32_SumEntry; ui32__Loop++)
    {
        // read if entry is empty
        bool bo__IsEmpty = gn_lib_phonebook_entry_isempty(
                                c_pApiState,
                                (gn_memory_type)eMemory,
                                ui32__Loop);

        // skipp if empty
        if( TRUE == bo__IsEmpty) { continue; }

        rPHONEBOOK_ENTRY rTemp;
        result = eDoReadPhonebookEntry(eMemory,ui32__Loop, rTemp);

        // check if can be read
        if( MA_STS_SUCCESS(result) )
        {
            Entrys.push_back( rTemp );
        }
        else
        {
            std::cerr   << "xGnokiiApi::eDoReadPhonebookEntry(): "
                        << "Single Entry (" << ui32__Loop
                        << ") can not read. ErrorCode: "
                        << result << "\n";
        }
    }

    return result;
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
    std::cout   << "Handy Infos\n"
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


/**
  @brief This function print a vector with informations about memory usage
*/
void xGnokiiApi::vPrintMemoryUsage(vecMemUsage& MemUsage) const
{
    std::cout   << "Memory Usage\n"
                << "============\n";
    for(vecMemUsage::iterator itLoop = MemUsage.begin();
        itLoop != MemUsage.end();
        itLoop++)
    {
        if((itLoop->i32Used == -1) || (itLoop->i32Free == -1) )
        {
            // Error Case
            std::cout   << strMemoryType(itLoop->eMemory).toStdString()
                        << ":\t" << "Invalid" << "\n";
        }
        else
        {
            qreal d__Ratio = 0.0;
            qint32 i32__sum = itLoop->i32Used + itLoop->i32Free;
            d__Ratio = (qreal)itLoop->i32Used / (qreal)(i32__sum) * 100.0;

            std::cout   << strMemoryType( itLoop->eMemory ).toStdString()
                        << ":\t" << itLoop->i32Used << " of "
                        << i32__sum << " = " << d__Ratio << "%\n";
        }
    }
    std::cout << "\n";
}


void xGnokiiApi::vPrintPhoneBook
(
    vecPhoneBookEntry& Entrys,
    bool Verbose
) const
{
    std::cout   << "PhoneBookEntrys:\n"
                << "================\n";

    for(vecPhoneBookEntry::iterator it_Loop = Entrys.begin();
        it_Loop != Entrys.end();
        it_Loop++)
    {
        rPHONEBOOK_ENTRY rTempEntry = *it_Loop;

        // print name and default number
        std::cout   << rTempEntry.strName.toStdString() << "\n";

        // check if entry has only a main number
        if(rTempEntry.vSubEntrys.size() == 0)
        {
            std::cout << rTempEntry.strMainNumber.toStdString() << "\n";
        }
        // is a big entry -> has sub entrys (with main number)
        else
        {
            vecPhoneBookSubE::iterator it_Sub;
            for(it_Sub = rTempEntry.vSubEntrys.begin();
                it_Sub != rTempEntry.vSubEntrys.end();
                it_Sub++)
            {
                rPHONEBOOK_SUBENTRY rSub = *it_Sub;
                switch(rSub.eSubType)
                {
                    case EntTypeNote:
                        std::cout << "Note: " << rSub.strValue.toStdString() << "\n";
                        break;

                    case EntTypeEmail:
                        std::cout << "Email: " << rSub.strValue.toStdString() << "\n";
                        break;

                    case EntTypeUrl:
                        std::cout << "URL: " << rSub.strValue.toStdString() << "\n";

                    case EntTypeNumber:
                        switch(rSub.eNumType)
                        {
                            case NumTypeCommon:
                                std::cout << "Common: ";
                                break;

                            case NumTypeMobile:
                                std::cout << "Mobile: ";
                                break;

                            case NumTypeHome:
                                std::cout << "Home: ";
                                break;

                            case NumTypeFax:
                                std::cout << "Fax: ";
                                break;

                            case NumTypeWork:
                                std::cout << "Work: ";
                                break;

                            case NumTypeGeneral:
                                std::cout << "General: ";
                                break;

                            case NumTypeNone:
                                std::cout << "None Type: ";
                                break;

                            default:
                                std::cout << "Unknown N-Type: ";
                        }
                        std::cout << rSub.strValue.toStdString() << "\n";
                        break;

                    default:
                        std::cout << "Unknown Type:" << rSub.strValue.toStdString() << "\n";
                }
            }
        }
        std::cout << "\n";

    }
    std::cout << "\n";
}
