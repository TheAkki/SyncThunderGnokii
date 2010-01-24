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

    rHANDY_INFO rHandyInfo;

    myGnokii->eDoReadHandyInfo( rHandyInfo );
    myGnokii->vPrintHandyInfo( rHandyInfo );

    /*
    xGnokiiApi::vecConnectionType myConnections;
    myGnokii->eGetSupportedConnections(myConnections);
    myGnokii->vPrintSupportedConnections(myConnections);
    */

    delete myGnokii;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Doing();

    // return a.exec();
}
