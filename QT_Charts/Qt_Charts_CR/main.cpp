#include <QtCharts>
#include "mainwindow.h"

QT_CHARTS_USE_NAMESPACE

void manageSegFailure(int signalCode)
{
    int userResult = QMessageBox::critical(nullptr, "Error", "Problema interno all'applicazione: segnalazione creata", QMessageBox::Ok);
    if (userResult == QMessageBox::Yes) {
        QString dt = "LogFile";
        dt += QDateTime::currentDateTime().toString("ddMMyyyy hhmmss");
        dt += ".log";
    }
    signal(signalCode, SIG_DFL);
    QApplication::exit(3);
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    signal(SIGSEGV, manageSegFailure);

    a.setWindowIcon(QIcon(":/icons/chart_icon.png"));
    QPixmap pixmap(":/icons/qt_charts_splash.jpg");
    QSplashScreen splash(pixmap, Qt::WindowStaysOnTopHint);
    splash.show();

    MainWindow *t=new MainWindow();
    t->setWindowTitle(QApplication::translate("qtcharts", "QT Charts"));
    t->setMinimumSize(800, 800);
    t->readPositionSettings();
    t->centralWidget()->adjustSize();

    t->show();
    splash.finish(*&t);

    return a.exec();
}


