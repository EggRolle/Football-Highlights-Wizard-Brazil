#include "mainwindow.h" // Include the main window header
#include <QApplication> // Qt Application class
#include <QStyleFactory> // To potentially set a nicer style

int main(int argc, char *argv[])
{
    // Create the Qt Application object
    QApplication a(argc, argv);

    // Optional: Try to set a nicer visual style
    QApplication::setStyle(QStyleFactory::create("Fusion"));

    // Create and show the main window
    MainWindow w;
    w.show();

    // Start the Qt event loop
    return a.exec();
}