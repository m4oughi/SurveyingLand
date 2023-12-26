#include "mainwindow.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QSplashScreen>
#include <QPixmap>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);


    QPixmap pixmap(":/src/Assets/splashscreen.png");
    QSplashScreen splash(pixmap);
    splash.show();
    app.processEvents();

    MainWindow window;

    window.setWindowFlags(Qt::Window|Qt::FramelessWindowHint);

    double screenWidth = QApplication::desktop()-> width();
    double screenHeight = QApplication::desktop()-> height();

    double X = ((screenWidth-window.width())/2);
    double Y = ((screenHeight-window.height())/2);

    window.move(X, Y);

    window.setAttribute(Qt::WA_TranslucentBackground, true);

    //w.setFixedWidth(screenWidth);
    //w.setFixedHeight(screenHeight);

    window.setWindowIcon(QIcon(":/src/Icons/antena.png"));

    qDebug() << screenWidth << screenHeight;

    QTimer::singleShot(8000, &splash, SLOT(close()));

    window.show();

    //splash.finish(&window);

    return app.exec();
}
