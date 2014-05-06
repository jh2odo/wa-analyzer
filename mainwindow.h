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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QString>
#include <QMessageBox>
#include <QTranslator>
#include <QDebug>
#include <QResizeEvent>
#include <QSignalMapper>
#include <QFeedbackEffect>
#include <QDesktopServices>
#include <QPropertyAnimation>

#include "wa/analyzer.h"
#include "wa/graph.h"
#include "wa/contacts.h"
#include "wa/stats.h"
#include "wa/notificationlistviewdelegate.h"

#include "util/QsKineticScroller.h"
#include "util/Gesture.h"

using namespace QtMobility;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    enum ScreenOrientation {
        ScreenOrientationLockPortrait,
        ScreenOrientationLockLandscape,
        ScreenOrientationAuto
    };

    explicit MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();

    // Note that this will only have an effect on Symbian and Fremantle.
    void setOrientation(ScreenOrientation orientation);
    void showExpanded();
    void init();

    void demo();
private slots:

    void on_horizontalSliderNotificationsDays_valueChanged(int value);

    void setCurrentStackedWidgetPage(int index);
    void exitApp();
    void captureScreen();

    void notificationsUpdate();

    void setCurrentAnalysisDays(int index);
    void setCurrentAnalysisCharts(int index);
    void setNotificationsFilter(int index);

    void demoSlot(const QModelIndex &index);

    void on_comboBoxNotificationsFilterContacts_activated(int index);

    void on_comboBoxDirectoryWALogs_activated(int index);

    void on_comboBoxDirectoryScreenshot_activated(int index);

    void on_comboBoxLanguage_activated(int index);

    void initAnalyzer();

    void on_pushButtonInfoSupportTwitter_clicked();
    void on_pushButtonInfoSupportWeb_clicked();

    void on_comboBoxAnalysisFilterContacts_activated(int index);

    void toggleSubBars(int bar);

    void on_pushButtonStatusSubBarsMode_clicked();

    void on_pushButtonSettingsCacheDelete_clicked();

protected:
    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent *event);
    void timerEvent(QTimerEvent *event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private:
    Ui::MainWindow *ui;
    QString language;

    QMap<QString,QString> languages;

    void setLanguage(QString language);
    void retranslateUi();

    QString pathWALogs;
    QString pathImages;

    Analyzer *analyzer;
    QList<Notification> notifications;
    int days;

    QList<Contact> contacts;
    QList<Contact> contactsActiveWhatsApp;

    QMap<QString,bool> notificationsFilters;
    int notificationsFiltersContact;

    QSignalMapper *mapperMenuBarMain;
    QSignalMapper *mapperMenuBarAnalysisDays;
    QSignalMapper *mapperMenuBarAnalysisCharts;
    QSignalMapper *mapperMenuBarNotificationsFilters;

    void updateNotificationsData();
    void updateNotificationsFilter();
    void updateAnalyzer();

    int analysisDays;
    int analysisChart;
    void updateAnalysisData();
    int analysisFiltersContact;

    bool subBarNotifications;
    bool subBarAnalysis;
    int subBarNotificationsHeight;
    int subBarAnalysisHeight;
    int SUBBAR_N_X_HIDDEN;
    int SUBBAR_N_X_SHOWN;
    int SUBBAR_A_X_HIDDEN;
    int SUBBAR_A_X_SHOWN;
    QPropertyAnimation* animationNotifications;
    QPropertyAnimation* transitionNotifications;
    QPropertyAnimation* animationAnalysis;
    QPropertyAnimation* transitionAnalysis;

    Gesture* gesture;
    QList<QPoint>* draggingPoints;

};

#endif // MAINWINDOW_H
