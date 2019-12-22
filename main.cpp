#include <QApplication>
#include "MainWindow.h"

int main( int argc, char** argv ) {
    QApplication app( argc, argv );
    app.setOrganizationName( "Lemz" );
    app.setApplicationName( "grinbar" );
    app.setApplicationVersion( "v0.1" );
    MainWindow gui;
    gui.show();
    return app.exec();
}
