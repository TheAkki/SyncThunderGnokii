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

class xGnokiiApi
{
    public:
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
        typedef QVector<xGnokiiApi::rSUPPORTED_CONNECTIONS> vecConnectionType;

        typedef enum
        {
            MT_InternalMemory = GN_MT_ME,
            MT_SIM = GN_MT_SM,
            MT_IntMemorySim = GN_MT_MT,
            MT_Invalid          // must be the last entry
        }eMEMORY_TYPE;

        typedef struct
        {
            eMEMORY_TYPE    eMemory;
            qint32          i32Used;
            qint32          i32Free;
        }rMEMORY_USAGE;
        typedef QVector<xGnokiiApi::rMEMORY_USAGE> vecMemUsage;

        //
        // Phone Book
        //

        typedef enum
        {
            GroupFamily = GN_PHONEBOOK_GROUP_Family,
            GroupVIP = GN_PHONEBOOK_GROUP_Vips,
            GroupFriends = GN_PHONEBOOK_GROUP_Friends,
            GroupWork = GN_PHONEBOOK_GROUP_Work,
            GroupOther = GN_PHONEBOOK_GROUP_Others,
            GroupNone = GN_PHONEBOOK_GROUP_None
        }ePHONEBOOK_GROUPTYPE;

        typedef enum
        {
            NumTypeNone = GN_PHONEBOOK_NUMBER_None, // Allgemein
            NumTypeCommon = GN_PHONEBOOK_NUMBER_Common,
            NumTypeHome = GN_PHONEBOOK_NUMBER_Home,
            NumTypeMobile = GN_PHONEBOOK_NUMBER_Mobile,
            NumTypeFax = GN_PHONEBOOK_NUMBER_Fax,
            NumTypeWork = GN_PHONEBOOK_NUMBER_Work,
            NumTypeGeneral = GN_PHONEBOOK_NUMBER_General,
        }ePHONEBOOK_NUMBER_TYPE;


        /**
          @todo Rename unused Types
        */
        typedef enum
        {
            EntTypeName     = GN_PHONEBOOK_ENTRY_Name,
            EntTypeEmail    = GN_PHONEBOOK_ENTRY_Email,
            EntTypePostal   = GN_PHONEBOOK_ENTRY_Postal,
            EntTypeNote     = GN_PHONEBOOK_ENTRY_Note,
            EntTypeNumber   = GN_PHONEBOOK_ENTRY_Number,
            GN_PHONEBOOK_ENTRY_Ringtone        = 0x0c,
            GN_PHONEBOOK_ENTRY_Date            = 0x13, /* Date is used for DC,RC,etc (last calls) */
            GN_PHONEBOOK_ENTRY_Pointer         = 0x1a, /* Pointer to the other memory */
            GN_PHONEBOOK_ENTRY_Logo            = 0x1b,
            GN_PHONEBOOK_ENTRY_LogoSwitch      = 0x1c,
            GN_PHONEBOOK_ENTRY_Group           = 0x1e,
            EntTypeUrl      = GN_PHONEBOOK_ENTRY_URL,
            GN_PHONEBOOK_ENTRY_Location        = 0x2f, /* ??? */
            GN_PHONEBOOK_ENTRY_Image           = 0x33,
            GN_PHONEBOOK_ENTRY_RingtoneAdv     = 0x37,
            GN_PHONEBOOK_ENTRY_UserID          = 0x38,
            GN_PHONEBOOK_ENTRY_PTTAddress      = 0x3f, /* string */
            GN_PHONEBOOK_ENTRY_FirstName       = 0x46,
            GN_PHONEBOOK_ENTRY_LastName        = 0x47,
            GN_PHONEBOOK_ENTRY_PostalAddress   = 0x4a,
            GN_PHONEBOOK_ENTRY_ExtendedAddress = 0x4b,
            GN_PHONEBOOK_ENTRY_Street          = 0x4c,
            GN_PHONEBOOK_ENTRY_City            = 0x4d,
            GN_PHONEBOOK_ENTRY_StateProvince   = 0x4e,
            GN_PHONEBOOK_ENTRY_ZipCode         = 0x4f,
            GN_PHONEBOOK_ENTRY_Country         = 0x50,
            GN_PHONEBOOK_ENTRY_FormalName      = 0x52,
            GN_PHONEBOOK_ENTRY_JobTitle        = 0x54,
            GN_PHONEBOOK_ENTRY_Company         = 0x55,
            GN_PHONEBOOK_ENTRY_Nickname        = 0x56,
            EntTypeBirthday = GN_PHONEBOOK_ENTRY_Birthday
        }ePHONEBOOK_SUBENTRY_TYPE;

        typedef struct
        {
            eMEMORY_TYPE    eMemType;
            qint32          i32Slot;
            bool            bHasData;
        }rPHONEBOOK_INFO_HANDY;

        typedef struct
        {
            ePHONEBOOK_SUBENTRY_TYPE    eSubType;
            ePHONEBOOK_NUMBER_TYPE      eNumType;
            QString                     strValue;
        }rPHONEBOOK_SUBENTRY;
        typedef QVector<xGnokiiApi::rPHONEBOOK_SUBENTRY> vecPhoneBookSubE;

        typedef struct
        {
            rPHONEBOOK_INFO_HANDY   rHandy;
            QString                 strName;
            QString                 strMainNumber;
            vecPhoneBookSubE        vSubEntrys;
        }rPHONEBOOK_ENTRY;
        typedef QVector<xGnokiiApi::rPHONEBOOK_ENTRY> vecPhoneBookEntry;

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
        STG_ERRORCODE::tErrorcode eDoReadMemoryUsage(eMEMORY_TYPE eMemory,
                                                     rMEMORY_USAGE& Data);
        STG_ERRORCODE::tErrorcode eDoReadMemoryUsage(vecMemUsage& MemUsage);
        STG_ERRORCODE::tErrorcode eDoReadPhonebookEntry(
                                        eMEMORY_TYPE eMemory,
                                        qint32 Slot,
                                        rPHONEBOOK_ENTRY& Entry
                                    );
        STG_ERRORCODE::tErrorcode eDoReadPhonebookEntry(
                                        eMEMORY_TYPE eMemory,
                                        vecPhoneBookEntry& Entrys,
                                        bool boClearVector = TRUE
                                    );

        // Print functions
        void vPrintSupportedConnections(vecConnectionType& ConnType) const;
        void vPrintHandyInfo(const rHANDY_INFO& rHInfo);
        void vPrintUsedGnokiiApi(quint32 uiVersionGnokiiApi);
        void vPrintMemoryUsage(vecMemUsage& MemUsage) const;
        static QString strMemoryType(eMEMORY_TYPE eMemory)
                {
                    const char* pc_String = gn_memory_type2str(
                                                (gn_memory_type) eMemory
                                            );
                    return QString(pc_String);
                };
        void vPrintPhoneBook(vecPhoneBookEntry& Entrys, bool Verbose = FALSE) const;

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
