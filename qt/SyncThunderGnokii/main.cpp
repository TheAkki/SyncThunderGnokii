#include <QtCore/QCoreApplication>
#include <QString>

#include <iostream>

#include "Global.h"
#include "xgnokiiapi.h"

void Doing()
{
    xGnokiiApi *myGnokii = new xGnokiiApi();
    if( myGnokii ){ std::cout << "Gnokii-Api created" << "\n"; }
    else
    {
        std::cout << "Gnokki-Api not created" << "\n";
        return;
    }

    myGnokii->vPrintUsedGnokiiApi( myGnokii->ui32GetUsedGnokiiApi() );

    QString myConfigFile("/home/theakki/.gnokiirc");
    QString myConfigEntry("global");

    myGnokii->eDoInit(myConfigFile, myConfigEntry);

    xGnokiiApi::rHANDY_INFO rHandyInfo;

    myGnokii->eDoReadHandyInfo( rHandyInfo );
    myGnokii->vPrintHandyInfo( rHandyInfo );

    /*
    xGnokiiApi::vecConnectionType myConnections;
    myGnokii->eGetSupportedConnections(myConnections);
    myGnokii->vPrintSupportedConnections(myConnections);
    */

    xGnokiiApi::rMEMORY_USAGE mySimUsage;
    xGnokiiApi::rMEMORY_USAGE myInternalUsage;
    xGnokiiApi::vecMemUsage myUsageVec;
    //myGnokii->eDoReadMemoryUsage(xGnokiiApi::MT_SIM,mySimUsage);
    //myGnokii->eDoReadMemoryUsage(xGnokiiApi::MT_InternalMemory, myInternalUsage);
    //myUsageVec.push_back(mySimUsage);
    //myUsageVec.push_back(myInternalUsage);
    myGnokii->eDoReadMemoryUsage(myUsageVec);
    myGnokii->vPrintMemoryUsage(myUsageVec);

    /*
    xGnokiiApi::rPHONEBOOK_ENTRY myPhoneBookEntry;
    memset(&myPhoneBookEntry, 0, sizeof(myPhoneBookEntry));
    myGnokii->eDoReadPhonebookEntry(xGnokiiApi::MT_InternalMemory,23, myPhoneBookEntry);
    */

    xGnokiiApi::vecPhoneBookEntry myPhoneBook;
    myGnokii->eDoReadPhonebookEntry(xGnokiiApi::MT_InternalMemory, myPhoneBook);
    myGnokii->vPrintPhoneBook( myPhoneBook );

    delete myGnokii;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Doing();

    // return a.exec();
}
