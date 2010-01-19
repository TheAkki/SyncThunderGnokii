#include <QtCore/QCoreApplication>

#include <iostream>

#include "Global.h"
#include "xgnokiiapi.h"

void Doing()
{
    xGnokiiApi *myGnokii = new xGnokiiApi();
    if( myGnokii )
    {
        std::cout << "Gnokii-Api created" << "\n";
    }
    else
    {
        std::cout << "Gnokki-Api not created" << "\n";
        return;
    }

    delete myGnokii;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Doing();

    return a.exec();
}
