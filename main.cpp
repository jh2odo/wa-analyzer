/*
The MIT License (MIT)

Copyright (c) 2012 Jose Aguado (jh2odo)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE
*/

#include "mainwindow.h"

#include <QtGui/QApplication>
#include <QSplashScreen>
#include <QPixmap>
#include <QTimer>
#include <QString>

int main(int argc, char *argv[])
{
    qInstallMsgHandler(0); // Default

    QApplication app(argc, argv);

    MainWindow mainWindow;
    mainWindow.setOrientation(MainWindow::ScreenOrientationLockPortrait);
    mainWindow.showExpanded();
    mainWindow.show();

    QPixmap pixmap = QPixmap(":/images/splash");
    int widthSize = mainWindow.width();
    int heightSize = mainWindow.height();
    QSize size = QSize(widthSize,heightSize);
    QPoint point = QPoint((1000 - widthSize)/2,(1000 - heightSize)/2);
    QRect rect = QRect(point,size);
    pixmap = pixmap.copy(rect);

    QSplashScreen splash(pixmap);
    splash.show();

    Qt::Alignment bottomRight = Qt::AlignHCenter | Qt::AlignBottom;
    QString msg;
    if(QLocale::system().language() == QLocale::Spanish){
        msg = "Iniciando...";
    }else{
        msg = "Starting...";
    }

    splash.showMessage(msg,bottomRight, Qt::black);

    app.processEvents();//This is used to accept a click on the screen so that user can cancel the screen

    mainWindow.init();


    //mainWindow.show();
    QTimer::singleShot(2000, &splash, SLOT(close()));


    return app.exec();
}
