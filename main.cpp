#include <QApplication>
#include "ImageProcessingFactory.h"
#include "pissMainWindow.h"
#include "SystemDispatcher.h"
#include "pissBaseDeDonnee.h"
#include "Omega3.h"


//! -----------------------------------------------------------------------------------------------
//!
//! \brief main
//! \param argc
//! \param argv
//! \return
//!
int main(int argc, char *argv[])
{
    //! -------------------------------------------------------------------------------------------
    //! création de l'application
    //! -------------------------------------------------------------------------------------------
    QApplication analyser(argc, argv);
    analyser.setOrganizationName("CAS - SIAT");
    analyser.setApplicationName("piss");
    analyser.setWindowIcon(QIcon(":/images/icon.png"));

    //! -------------------------------------------------------------------------------------------
    //! le bibliothéque des algorithmes pour faire le traitement des images
    //! -------------------------------------------------------------------------------------------
    ImageProcessingFactory *imageProcessingFactory = new ImageProcessingFactory();

    //! -------------------------------------------------------------------------------------------
    //! le base de données pour enregistrer les informations qui concerne des maladies
    //! -------------------------------------------------------------------------------------------
    pissBaseDeDonnee* database = new pissBaseDeDonnee();
    database->setImageProcessingFactory(imageProcessingFactory);

    //! -------------------------------------------------------------------------------------------
    //! omega
    //! -------------------------------------------------------------------------------------------
    Omega3 *omega = new Omega3();

    //! -------------------------------------------------------------------------------------------
    //! Initialiser l'environment du réseaux interne pour contruire le système réparti
    //! -------------------------------------------------------------------------------------------
    igtCommunicationStack *communicationStack = new igtCommunicationStack(database);

    //! -------------------------------------------------------------------------------------------
    //! l'ordonnanceur du système
    //! -------------------------------------------------------------------------------------------
    SystemDispatcher* dispatcher = new SystemDispatcher();
    dispatcher->setSystemDataBase(database);
    dispatcher->setImageProcessingFactory(imageProcessingFactory);
    dispatcher->setCommunicationStack(communicationStack);
    dispatcher->setOmega3(omega);

    //! -------------------------------------------------------------------------------------------
    //! l'IHM
    //! -------------------------------------------------------------------------------------------
    pissMainWindow* mainWindow = new pissMainWindow(dispatcher);

    //! -------------------------------------------------------------------------------------------
    //! Chercher les maladies existants
    //! -------------------------------------------------------------------------------------------
    mainWindow->findPatientExisted();

    //! -------------------------------------------------------------------------------------------
    //! Lancer l'IHM principale
    //! -------------------------------------------------------------------------------------------
    mainWindow->display();

    return analyser.exec();
}
