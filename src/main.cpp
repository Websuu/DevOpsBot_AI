#include <QApplication>
#include "MainWindow.h"

#ifdef Q_OS_WIN
#include <Windows.h>
#endif

int main(int argc, char* argv[]) {
#ifdef Q_OS_WIN
    // Tworzy konsolę dla aplikacji WIN32
    AllocConsole();
    // Przekierowuje standardowe strumienie do nowej konsoli
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);
#endif

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}