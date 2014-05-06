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
#include "ui_mainwindow.h"

#include <QtCore/QCoreApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    draggingPoints->clear();
    delete draggingPoints;
    delete ui;
}

void MainWindow::setOrientation(ScreenOrientation orientation)
{
#if defined(Q_OS_SYMBIAN)
    // If the version of Qt on the device is < 4.7.2, that attribute won't work
    if (orientation != ScreenOrientationAuto) {
        const QStringList v = QString::fromAscii(qVersion()).split(QLatin1Char('.'));
        if (v.count() == 3 && (v.at(0).toInt() << 16 | v.at(1).toInt() << 8 | v.at(2).toInt()) < 0x040702) {
            qWarning("Screen orientation locking only supported with Qt 4.7.2 and above");
            return;
        }
    }
#endif // Q_OS_SYMBIAN

    Qt::WidgetAttribute attribute;
    switch (orientation) {
#if QT_VERSION < 0x040702
    // Qt < 4.7.2 does not yet have the Qt::WA_*Orientation attributes
    case ScreenOrientationLockPortrait:
        attribute = static_cast<Qt::WidgetAttribute>(128);
        break;
    case ScreenOrientationLockLandscape:
        attribute = static_cast<Qt::WidgetAttribute>(129);
        break;
    default:
    case ScreenOrientationAuto:
        attribute = static_cast<Qt::WidgetAttribute>(130);
        break;
#else // QT_VERSION < 0x040702
    case ScreenOrientationLockPortrait:
        attribute = Qt::WA_LockPortraitOrientation;
        break;
    case ScreenOrientationLockLandscape:
        attribute = Qt::WA_LockLandscapeOrientation;
        break;
    default:
    case ScreenOrientationAuto:
        attribute = Qt::WA_AutoOrientation;
        break;
#endif // QT_VERSION < 0x040702
    };
    setAttribute(attribute, true);
}

void MainWindow::showExpanded()
{
#if defined(Q_OS_SYMBIAN) || defined(Q_WS_SIMULATOR)
    showFullScreen();
#elif defined(Q_WS_MAEMO_5)
    showMaximized();
#else
    show();
#endif
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
   qDebug() << "resizeEvent size:" << event->size();
    QSize size = event->size();
    bool isLandscape = size.width() > size.height();

    qDebug() << "Size: " << size;

    if (!isLandscape){
        qDebug() << "Orientation: portrait";
    }else{
        qDebug() << "Orientation: landscape";
    }

   // landscapeWidget->setFixedSize(size);
   // size.transpose();
   // portraitWidget->setFixedSize(size);

   // landscapeWidget->setVisible(isLandscape);
    // portraitWidget->setVisible(!isLandscape);
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    qDebug() << "paintEvent";
}

void MainWindow::timerEvent(QTimerEvent *event)
 {
    qDebug() << "timerEvent : Timer ID:" << event->timerId();
 }



void MainWindow::mousePressEvent(QMouseEvent* event)
{
    qDebug() << "mousePressEvent pos: " << event->pos();
    draggingPoints->clear();
    draggingPoints->append(event->pos());
}

void MainWindow::mouseMoveEvent(QMouseEvent* event)
{
   qDebug() << "mouseMoveEvent pos: " << event->pos();
   draggingPoints->append(event->pos());
}

void MainWindow::mouseReleaseEvent(QMouseEvent* event)
{
   qDebug() << "mouseReleaseEvent pos: " << event->pos();
   draggingPoints->append(event->pos());

   int currentPage = ui->stackedWidgetCentral->currentIndex();

   QPoint point = event->pos();
   if(point.y() < 60 && (gesture->isDraggingLeft(*draggingPoints) ||  gesture->isDraggingRight(*draggingPoints))) {
       // Pasamos en los menus principales
       qDebug() << "Gesture menu principal";

       if(gesture->isDraggingLeft(*draggingPoints)){
           currentPage--;
           if(currentPage <= 0){
               currentPage = 0;
           }
           setCurrentStackedWidgetPage(currentPage);
       }else if(gesture->isDraggingRight(*draggingPoints)){
           currentPage++;
           if(currentPage >= 4){
               currentPage = 4;
           }
           setCurrentStackedWidgetPage(currentPage);
       }
      return;
   }

    currentPage = ui->stackedWidgetCentral->currentIndex();

   if(currentPage == 0){
        if(point.y() > 520 && (gesture->isDraggingLeft(*draggingPoints) ||  gesture->isDraggingRight(*draggingPoints))) {
            toggleSubBars(currentPage);
        }
   }else if(currentPage == 1){
       if(point.y() > 520 && (gesture->isDraggingLeft(*draggingPoints) ||  gesture->isDraggingRight(*draggingPoints))) {
           toggleSubBars(currentPage);
       }
   }
}


void MainWindow::init()
{
    languages.insert("es","español");
    languages.insert("en","english");
    languages.insert("fr","français");
    languages.insert("de","deutsch");
    languages.insert("pt","português");
    languages.insert("it","italiano");
    languages.insert("ru","???????");

    if(QLocale::system().language() == QLocale::Spanish){
        setLanguage("es");
        ui->comboBoxLanguage->setCurrentIndex(0);
    }else if(QLocale::system().language() == QLocale::French){
        setLanguage("fr");
        ui->comboBoxLanguage->setCurrentIndex(2);
    }else if(QLocale::system().language() == QLocale::German){
        setLanguage("de");
        ui->comboBoxLanguage->setCurrentIndex(3);
    }else if(QLocale::system().language() == QLocale::Portuguese){
        setLanguage("pt");
        ui->comboBoxLanguage->setCurrentIndex(4);
    }else if(QLocale::system().language() == QLocale::Italian){
        setLanguage("it");
        ui->comboBoxLanguage->setCurrentIndex(5);
    }else if(QLocale::system().language() == QLocale::Russian){
        setLanguage("ru");
        ui->comboBoxLanguage->setCurrentIndex(6);
    }else{
        setLanguage("en");
        ui->comboBoxLanguage->setCurrentIndex(1);
    }

    notificationsFilters.insert("text",true);
    notificationsFilters.insert("audio",true);
    notificationsFilters.insert("image",true);
    notificationsFilters.insert("video",true);
    notificationsFilters.insert("private",true);
    notificationsFilters.insert("group",false);
    notificationsFilters.insert("send",false);
    notificationsFilters.insert("recv",true);


    // Detectar directorio
    QString pathWALogsDetected = "unknown";
    QStringList dirsWALogs;
                dirsWALogs.append("c:/WALogs/");
                dirsWALogs.append("d:/WALogs/");
                dirsWALogs.append("e:/WALogs/");
                dirsWALogs.append("f:/WALogs/");

    for (int i = 0; i < dirsWALogs.size(); i++) {
        QString dirWALogs = dirsWALogs.at(i);
        QDir dir;
        dir.setFilter(QDir::Dirs | QDir::Hidden | QDir::NoSymLinks);
        dir.setPath(dirWALogs);
        if(dir.exists(dirWALogs)){
            pathWALogsDetected = dirWALogs;
            break;
        }
    }

    pathWALogs = pathWALogsDetected;

    qDebug() << "Directory WALogs: " << pathWALogs;

    // Detectar directorio Images
    QString pathImagesDetected = "unknown";
    QStringList dirsImages;
                dirsImages.append("e:/Images/");
                dirsImages.append("d:/Images/");
                dirsImages.append("c:/Images/");

    for (int i = 0; i < dirsImages.size(); i++) {
        QString dirImages = dirsImages.at(i);
        QDir dir;
        dir.setFilter(QDir::Dirs | QDir::Hidden | QDir::NoSymLinks);
        dir.setPath(dirImages);
        if(dir.exists(dirImages)){
            pathImagesDetected = dirImages + "WAAnalyzer/";
            if(!QDir(pathImagesDetected).exists()){
                QDir().mkdir(pathImagesDetected);
            }
            break;
        }
    }

    pathImages = pathImagesDetected;

    qDebug() << "Directory Images: " << pathImages;


    // GUI

    if(pathWALogs.contains("unknown")){
        ui->comboBoxDirectoryWALogs->addItem(tr("unknown"));
    }else{
        ui->comboBoxDirectoryWALogs->addItem(pathWALogs);
    }
    ui->comboBoxDirectoryWALogs->setCurrentIndex(0);
    ui->comboBoxDirectoryWALogs->addItem(tr("Select other directory..."));

    if(pathImages.contains("unknown")){
        ui->comboBoxDirectoryScreenshot->addItem(tr("unknown"));
    }else{
        ui->comboBoxDirectoryScreenshot->addItem(pathImages);
    }
    ui->comboBoxDirectoryScreenshot->setCurrentIndex(0);
    ui->comboBoxDirectoryScreenshot->addItem(tr("Select other directory..."));

    //

    contactsActiveWhatsApp = QList<Contact>();

    notificationsFiltersContact = 0;
    ui->comboBoxNotificationsFilterContacts->addItem(tr("All contacts"));
    ui->comboBoxNotificationsFilterContacts->setCurrentIndex(notificationsFiltersContact);

    analysisFiltersContact = 0;
    ui->comboBoxAnalysisFilterContacts->addItem(tr("All contacts"));
    ui->comboBoxAnalysisFilterContacts->setCurrentIndex(analysisFiltersContact);

    ui->stackedWidgetCentral->setCurrentIndex(0);

    //Create a QMapper to map the values of the toolbuttons with the indexes of the stacked widget
    mapperMenuBarMain = new QSignalMapper(this);

    //Connect the clicked signal with the QSignalMapper
    connect(ui->pushButtonMenuBarNotifications, SIGNAL(clicked()), mapperMenuBarMain, SLOT(map()));
    connect(ui->pushButtonMenuBarAnaysis, SIGNAL(clicked()), mapperMenuBarMain, SLOT(map()));
    connect(ui->pushButtonMenuBarTools, SIGNAL(clicked()), mapperMenuBarMain, SLOT(map()));
    connect(ui->pushButtonMenuBarSettings, SIGNAL(clicked()), mapperMenuBarMain, SLOT(map()));
    connect(ui->pushButtonMenuBarInfo, SIGNAL(clicked()), mapperMenuBarMain, SLOT(map()));

    //Set a value for each button
    mapperMenuBarMain->setMapping(ui->pushButtonMenuBarNotifications, 0);
    mapperMenuBarMain->setMapping(ui->pushButtonMenuBarAnaysis, 1);
    mapperMenuBarMain->setMapping(ui->pushButtonMenuBarTools, 2);
    mapperMenuBarMain->setMapping(ui->pushButtonMenuBarSettings, 3);
    mapperMenuBarMain->setMapping(ui->pushButtonMenuBarInfo, 4);

    //Connect the mapper to the central widget
    //The mapper will set a value to each button and set that value to the central widget
    connect(mapperMenuBarMain, SIGNAL(mapped(int)), this, SLOT(setCurrentStackedWidgetPage(int)));

    //
    connect(ui->pushButtonStatusBarExit, SIGNAL(clicked()), this, SLOT(exitApp()));
    connect(ui->pushButtonStatusBarCaptureScreen, SIGNAL(clicked()), this, SLOT(captureScreen()));

    //connect(ui->listViewNotifications, SIGNAL(clicked(const QModelIndex &)), this, SLOT(demoSlot(const QModelIndex &)));

    connect(ui->pushButtonNotificationsUpdate, SIGNAL(clicked()), this, SLOT(notificationsUpdate()));


    mapperMenuBarAnalysisDays = new QSignalMapper(this);

    //Connect the clicked signal with the QSignalMapper
    connect(ui->pushButtonAnalysisUpdateToday, SIGNAL(clicked()), mapperMenuBarAnalysisDays, SLOT(map()));
    connect(ui->pushButtonAnalysisUpdateLastThreeDays, SIGNAL(clicked()), mapperMenuBarAnalysisDays, SLOT(map()));
    connect(ui->pushButtonAnalysisUpdateLastSevenDays, SIGNAL(clicked()), mapperMenuBarAnalysisDays, SLOT(map()));
    connect(ui->pushButtonAnalysisUpdateMonthDays, SIGNAL(clicked()), mapperMenuBarAnalysisDays, SLOT(map()));

    //Set a value for each button
    mapperMenuBarAnalysisDays->setMapping(ui->pushButtonAnalysisUpdateToday, 0);
    mapperMenuBarAnalysisDays->setMapping(ui->pushButtonAnalysisUpdateLastThreeDays, 1);
    mapperMenuBarAnalysisDays->setMapping(ui->pushButtonAnalysisUpdateLastSevenDays, 2);
    mapperMenuBarAnalysisDays->setMapping(ui->pushButtonAnalysisUpdateMonthDays, 3);

    //Connect the mapper to the central widget
    //The mapper will set a value to each button and set that value to the central widget
    connect(mapperMenuBarAnalysisDays, SIGNAL(mapped(int)), this, SLOT(setCurrentAnalysisDays(int)));


    mapperMenuBarAnalysisCharts = new QSignalMapper(this);

    //Connect the clicked signal with the QSignalMapper
    connect(ui->pushButtonAnalysisFilterPie, SIGNAL(clicked()), mapperMenuBarAnalysisCharts, SLOT(map()));
    connect(ui->pushButtonAnalysisFilterHistogramm, SIGNAL(clicked()), mapperMenuBarAnalysisCharts, SLOT(map()));
    connect(ui->pushButtonAnalysisFilterGraphic, SIGNAL(clicked()), mapperMenuBarAnalysisCharts, SLOT(map()));

    //Set a value for each button
    mapperMenuBarAnalysisCharts->setMapping(ui->pushButtonAnalysisFilterPie, 0);
    mapperMenuBarAnalysisCharts->setMapping(ui->pushButtonAnalysisFilterHistogramm, 1);
    mapperMenuBarAnalysisCharts->setMapping(ui->pushButtonAnalysisFilterGraphic, 2);

    //Connect the mapper to the central widget
    //The mapper will set a value to each button and set that value to the central widget
    connect(mapperMenuBarAnalysisCharts, SIGNAL(mapped(int)), this, SLOT(setCurrentAnalysisCharts(int)));


    mapperMenuBarNotificationsFilters = new QSignalMapper(this);

    //Connect the clicked signal with the QSignalMapper
    connect(ui->pushButtonNotificationsFilterText, SIGNAL(clicked()), mapperMenuBarNotificationsFilters, SLOT(map()));
    connect(ui->pushButtonNotificationsFilterImage, SIGNAL(clicked()), mapperMenuBarNotificationsFilters, SLOT(map()));
    connect(ui->pushButtonNotificationsFilterVideo, SIGNAL(clicked()), mapperMenuBarNotificationsFilters, SLOT(map()));
    connect(ui->pushButtonNotificationsFilterAudio, SIGNAL(clicked()), mapperMenuBarNotificationsFilters, SLOT(map()));
    connect(ui->pushButtonNotificationsFilterPrivate, SIGNAL(clicked()), mapperMenuBarNotificationsFilters, SLOT(map()));
    connect(ui->pushButtonNotificationsFilterGroup, SIGNAL(clicked()), mapperMenuBarNotificationsFilters, SLOT(map()));
    connect(ui->pushButtonNotificationsFilterReceived, SIGNAL(clicked()), mapperMenuBarNotificationsFilters, SLOT(map()));
    connect(ui->pushButtonNotificationsFilterSent, SIGNAL(clicked()), mapperMenuBarNotificationsFilters, SLOT(map()));



    //Set a value for each button
    mapperMenuBarNotificationsFilters->setMapping(ui->pushButtonNotificationsFilterText, 0);
    mapperMenuBarNotificationsFilters->setMapping(ui->pushButtonNotificationsFilterImage, 1);
    mapperMenuBarNotificationsFilters->setMapping(ui->pushButtonNotificationsFilterVideo, 2);
    mapperMenuBarNotificationsFilters->setMapping(ui->pushButtonNotificationsFilterAudio, 3);
    mapperMenuBarNotificationsFilters->setMapping(ui->pushButtonNotificationsFilterPrivate, 4);
    mapperMenuBarNotificationsFilters->setMapping(ui->pushButtonNotificationsFilterGroup, 5);
    mapperMenuBarNotificationsFilters->setMapping(ui->pushButtonNotificationsFilterReceived, 6);
    mapperMenuBarNotificationsFilters->setMapping(ui->pushButtonNotificationsFilterSent, 7);

    //Connect the mapper to the central widget
    //The mapper will set a value to each button and set that value to the central widget
    connect(mapperMenuBarNotificationsFilters, SIGNAL(mapped(int)), this, SLOT(setNotificationsFilter(int)));


    QStandardItemModel *model;
    NotificationListViewDelegate *listdelegate;

    model = new QStandardItemModel();
    listdelegate = new NotificationListViewDelegate();

    ui->listViewNotifications->setItemDelegate(listdelegate); //connect the delegate to view
    ui->listViewNotifications->setModel(model);//connect the model to view.

    QsKineticScroller *kinecticScroller = new QsKineticScroller(this);
    kinecticScroller->enableKineticScrollFor(ui->listViewNotifications);

    QsKineticScroller *kinecticScrollerAnalysis = new QsKineticScroller(this);
    kinecticScrollerAnalysis->enableKineticScrollFor(ui->scrollAreaAnalysis);

    QsKineticScroller *kinecticScrollerSettings = new QsKineticScroller(this);
    kinecticScrollerSettings->enableKineticScrollFor(ui->scrollAreaSettings);

    QsKineticScroller *kinecticScrollerInfo = new QsKineticScroller(this);
    kinecticScrollerInfo->enableKineticScrollFor(ui->scrollAreaInfo);

    //startTimer(10000);   // 10-second time


    initAnalyzer();

    retranslateUi();


    // take ownership of toolbar created with Qt Designer
    // and set it up

    subBarNotifications = true;
    subBarAnalysis = true;

    // define toolbar y movement positions for animation
    SUBBAR_N_X_SHOWN = 5;
    SUBBAR_N_X_HIDDEN = - ui->pageNotifications->width();
    SUBBAR_A_X_SHOWN = 0;
    SUBBAR_A_X_HIDDEN = - ui->pageAnalysis->width();

    // create animation class for transition
    transitionNotifications = new QPropertyAnimation(ui->pageNotifications, "pos", this);
    animationNotifications = new QPropertyAnimation(ui->widgetNotificationsFilter, "pos", this);
    subBarNotificationsHeight = ui->listViewNotifications->height();

    transitionAnalysis = new QPropertyAnimation(ui->pageAnalysis, "pos", this);
    animationAnalysis = new QPropertyAnimation(ui->widgetAnalysisFilter, "pos", this);
    subBarAnalysisHeight = 351;

    qDebug() << "subBarAnalysisHeight" << subBarAnalysisHeight;

    // Very basic gesture for dragging left/right
    gesture = new Gesture(this);

    draggingPoints = new QList<QPoint> ();
}

void MainWindow::initAnalyzer(){

    ui->labelStatusBarLoadingStatus->setVisible(true);

    analyzer = new Analyzer();
    analyzer->setPathWALogs(pathWALogs);
    analyzer->setContactsFromPhone();
    days = 0;

    if(pathWALogs.contains("unknown")){
        QMessageBox::warning(0,tr("No found WhatsApp Logs"),tr("It is not detected WhatsApp logs (Directory: WALogs)"),QMessageBox::Ok);
        qWarning() << "No detected WALogs";
    }else{
        int totalNewLogs = analyzer->getTotalNewLogs(10);
        if(totalNewLogs > 0){
           /*
            int result = QMessageBox::question(0, tr("New Logs"), tr("There are new logs in the last 7 days.\n\nDo you want to process?") , QMessageBox::No, QMessageBox::Yes);

            if(QMessageBox::Yes == result)
            {
                analyzer->updateLastByDays(7);
            }else{
                analyzer->updateLastByDays(days);
            }
           */
            analyzer->updateLastByDays(days);
        }
        qDebug() << "First Notifications Update";
        updateNotificationsData();
        updateNotificationsFilter();
        analysisDays = 0;
        analysisChart = 1;
        updateAnalysisData(); // Los stast Today
    }

    ui->labelStatusBarLoadingStatus->setVisible(false);
}

void MainWindow::setNotificationsFilter(int index){

    QFeedbackEffect::playThemeEffect(QFeedbackEffect::ThemeBasicButton);
    ui->labelStatusBarLoadingStatus->setVisible(true);

    QString filter;
    if(index == 0){
        filter = "text";
    }else if(index == 1){
         filter = "image";
    }else if(index == 2){
         filter = "video";
    }else if(index == 3){
         filter = "audio";
    }else if(index == 4){
         filter = "private";
    }else if(index == 5){
        filter = "group";
    }else if(index == 6){
        filter = "recv";
    }else if(index == 7){
        filter = "send";
    }

    if(!filter.isNull()){
        QPushButton *pushButton = qobject_cast<QPushButton *>(mapperMenuBarNotificationsFilters->mapping(index));

        if(pushButton->isChecked()){
            notificationsFilters.insert(filter,true);
        }else{
            notificationsFilters.insert(filter,false);
        }
        updateNotificationsFilter();
        qDebug() << "Filter: " << filter << notificationsFilters.value(filter);
    }

    ui->labelStatusBarLoadingStatus->setVisible(false);

}

void MainWindow::setCurrentAnalysisCharts(int index){
   QFeedbackEffect::playThemeEffect(QFeedbackEffect::ThemeBasicButton);

   qDebug() << "sAa h: " << ui->scrollAreaAnalysis->height();
   analysisChart = index;

   ui->labelStatusBarLoadingStatus->setVisible(true);

   for (int i = 0; i < 3; i++) {
         QPushButton *pushButton = qobject_cast<QPushButton *>(mapperMenuBarAnalysisCharts->mapping(i));
         if(pushButton->isChecked()){
             pushButton->setChecked(false);
         }
   }

   updateAnalysisData();

   QPushButton *pushButton = qobject_cast<QPushButton *>(mapperMenuBarAnalysisCharts->mapping(index));
   pushButton->setChecked(true);

   ui->labelStatusBarLoadingStatus->setVisible(false);
}


void MainWindow::setCurrentAnalysisDays(int index){
   QFeedbackEffect::playThemeEffect(QFeedbackEffect::ThemeBasicButton);

   ui->labelStatusBarLoadingStatus->setVisible(true);

   if(index == 0){
        analysisDays = 0;
   }else if(index == 1){
        analysisDays = 2;
   }else if(index == 2){
        analysisDays = 6;
   }else if(index == 3){
        analysisDays = 29;
   }

   for (int i = 0; i < 4; i++) {
         QPushButton *pushButton = qobject_cast<QPushButton *>(mapperMenuBarAnalysisDays->mapping(i));
         if(pushButton->isChecked()){
             pushButton->setChecked(false);
         }
   }

   updateAnalysisData();
   QPushButton *pushButton = qobject_cast<QPushButton *>(mapperMenuBarAnalysisDays->mapping(index));
   pushButton->setChecked(true);

   ui->labelStatusBarLoadingStatus->setVisible(false);
}

void  MainWindow::setCurrentStackedWidgetPage(int index){

     QFeedbackEffect::playThemeEffect(QFeedbackEffect::ThemeBasicButton);

    int current = ui->stackedWidgetCentral->currentIndex();
    if(current == 5){
        current = 0; // Notification Details TO Notifications
    }

    QPushButton *pushButton = qobject_cast<QPushButton *>(mapperMenuBarMain->mapping(current));
    pushButton->setChecked(false);
    ui->stackedWidgetCentral->setCurrentIndex(index);
    pushButton = qobject_cast<QPushButton *>(mapperMenuBarMain->mapping(index));
    pushButton->setChecked(true);

    if(index == 0){
        demo();
    }
}

void MainWindow::exitApp()
{
    QFeedbackEffect::playThemeEffect(QFeedbackEffect::ThemePopUp);
    int result = QMessageBox::question(0, tr("Exit"), tr("Do you want to exit?") , QMessageBox::No, QMessageBox::Yes);
    if(QMessageBox::Yes == result)
    {
        close();
    }
}

void MainWindow::updateAnalysisData()
{

    qDebug() << "updateAnalysisData Days: " << analysisDays;
    qDebug() << "updateAnalysisData Chart: " << analysisChart;

    int scrollValue = ui->scrollAreaAnalysis->verticalScrollBar()->value();
    qDebug() << "Scroll Analysis: " << scrollValue;

        QObjectList list = ui->scrollAreaWidgetContentsAnalysis->children();
        qDebug() << list.size();
        //QLayout *layout = ui->scrollAreaWidgetContentsTools->layout();
        for (int index = 1; index < list.size(); index++) {
            QWidget *widget = qobject_cast<QWidget *>(list.at(index));
            ui->verticalLayoutContentsAnalysis->removeWidget(widget);
            delete widget;
        }

       //ui->scrollAreaWidgetContentsAnalysis->setVisible(false);


        QString contactNameFilter = ui->comboBoxAnalysisFilterContacts->currentText();
        qDebug() << "Actual: " << contactNameFilter;
        ui->comboBoxAnalysisFilterContacts->clear();
        ui->comboBoxAnalysisFilterContacts->addItem(tr("All contacts"));
        foreach (Contact contact, contactsActiveWhatsApp) {
            ui->comboBoxAnalysisFilterContacts->addItem(contact.getName());
        }

        if(!contactNameFilter.contains(tr("All contacts"))){

            int indexContact = ui->comboBoxAnalysisFilterContacts->findText(contactNameFilter);
            if(indexContact == -1){
                 indexContact = 0;
                 QMessageBox::information(0,tr("Information"),tr("For selected days, there are no data for the contact") +": \n\n"+ contactNameFilter,QMessageBox::Ok);
            }
            ui->comboBoxAnalysisFilterContacts->setCurrentIndex(indexContact);
            analysisFiltersContact = indexContact;

        }else{
            analysisFiltersContact = 0;
        }


        // Estadisticas por contacto
        if(analysisFiltersContact != 0){
            Contact contact = contactsActiveWhatsApp.at(analysisFiltersContact - 1);
            qDebug() << "Filtro contacto: individual";
            qDebug() << "Number: " << contact.getNumber();
        // Estadisticas generales
        }else{
            qDebug() << "Filtro contacto: todos";
        }



       QList<Stats> stats = analyzer->getStatsTotalLastByDays(analysisDays);

       if(stats.size() > 0){

           Stats stat1 = stats.at(0);
           Stats stat2 = stats.at(1);
           Stats stat3 = stats.at(2);

           int width = 300;

           if(analysisChart == 0){
               Graph *graph = new Graph(ui->scrollAreaWidgetContentsAnalysis);
               graph->setTitle(tr("Total - Sent / Received"));
               graph->setData(stat1.getData());
               graph->setSize(QSize(width,370));
               graph->setType(Graph::Pie);
               ui->verticalLayoutContentsAnalysis->addWidget(graph);

               graph = new Graph(ui->scrollAreaWidgetContentsAnalysis);
               graph->setTitle(tr("Total - Privates / Groups"));
               graph->setData(stat2.getData());
               graph->setSize(QSize(width,370));
               graph->setType(Graph::Pie);
               ui->verticalLayoutContentsAnalysis->addWidget(graph);

               graph = new Graph(ui->scrollAreaWidgetContentsAnalysis);
               graph->setTitle(tr("Total - Types"));
               graph->setData(stat3.getData());
               graph->setSize(QSize(width,460));
               graph->setType(Graph::Pie);
               ui->verticalLayoutContentsAnalysis->addWidget(graph);
           }else if(analysisChart == 1){
               Graph *graph = new Graph(ui->scrollAreaWidgetContentsAnalysis);
               graph->setTitle(tr("Total - Sent / Received"));
               graph->setData(stat1.getData());
               graph->setSize(QSize(width,370));
               graph->setType(Graph::Histogramm);

               ui->verticalLayoutContentsAnalysis->addWidget(graph);
               graph = new Graph(ui->scrollAreaWidgetContentsAnalysis);
               graph->setTitle(tr("Total - Privates / Groups"));
               graph->setData(stat2.getData());
               graph->setSize(QSize(width,370));
               graph->setType(Graph::Histogramm);
               ui->verticalLayoutContentsAnalysis->addWidget(graph);

               graph = new Graph(ui->scrollAreaWidgetContentsAnalysis);
               graph->setTitle(tr("Total - Types"));
               graph->setData(stat3.getData());
               graph->setSize(QSize(width,450));
               graph->setType(Graph::Histogramm);
               ui->verticalLayoutContentsAnalysis->addWidget(graph);
           }else if(analysisChart == 2){
               Graph *graph = new Graph(ui->scrollAreaWidgetContentsAnalysis);
               graph->setTitle(tr("Not enabled"));
               graph->setData(QMap<QString,QColor>());
               graph->setSize(QSize(width,370));
               graph->setType(Graph::Graphic);
               ui->verticalLayoutContentsAnalysis->addWidget(graph);
           }

       }else{
           QMessageBox::information(0,tr("Information"),tr("No data"),QMessageBox::Ok);
       }

      //QCoreApplication::processEvents();
      // ui->scrollAreaWidgetContentsAnalysis->setVisible(true);
       ui->labelStatusBarLoadingStatus->setVisible(false);

       // Para posicionar el mismo sitio tras las filtros
      ui->scrollAreaAnalysis->verticalScrollBar()->setValue(scrollValue+1); // Para generar un movimiento y
      ui->scrollAreaAnalysis->verticalScrollBar()->setValue(scrollValue); // y solucionar una mala vision
}


void MainWindow::demoSlot(const QModelIndex &index)
{
    QFeedbackEffect::playThemeEffect(QFeedbackEffect::ThemeBasicItem);

    ui->stackedWidgetCentral->setCurrentIndex(5);
    int tmp = index.row();
    QVariant contact = index.data(NotificationListViewDelegate::TextRoleContact);
    QVariant date = index.data(NotificationListViewDelegate::TextRoleDate);

    ui->labelDemo->setText("" + QString::number(tmp + 1) + "\nContact: " + contact.toString() + "\nDate: " + date.toString());
    qDebug() << "demoSlot" << tmp;
}

void MainWindow::demo()
{

}

void MainWindow::updateNotificationsFilter()
{
    qDebug() << "updateNotificationsFilter";

    ui->labelStatusBarLoadingStatus->setVisible(true);

    ui->listViewNotifications->scrollToTop();

    QStandardItemModel *model = qobject_cast<QStandardItemModel *>(ui->listViewNotifications->model());

    qDebug() << "Total Rows: " << QString::number(model->rowCount());
    model->clear();
    qDebug() << "Total Rows: " << QString::number(model->rowCount());

    //ui->comboBoxNotificationsFilterContacts->clear();
    //for (int index = 0; index < contacts.size(); index++) {
    //    ui->comboBoxNotificationsFilterContacts->addItem(contacts.at(index));
    //}

    QString contactNameFilter = ui->comboBoxNotificationsFilterContacts->currentText();
    qDebug() << "ACtual: " << contactNameFilter;
    ui->comboBoxNotificationsFilterContacts->clear();
    ui->comboBoxNotificationsFilterContacts->addItem(tr("All contacts"));
    foreach (Contact contact, contactsActiveWhatsApp) {
        ui->comboBoxNotificationsFilterContacts->addItem(contact.getName());
    }

    if(!contactNameFilter.contains(tr("All contacts"))){

        int indexContact = ui->comboBoxNotificationsFilterContacts->findText(contactNameFilter);
        if(indexContact == -1){
             indexContact = 0;
             QMessageBox::information(0,tr("Information"),tr("For selected days, there are no notifications for contact") +": \n\n"+ contactNameFilter,QMessageBox::Ok);
        }
        ui->comboBoxNotificationsFilterContacts->setCurrentIndex(indexContact);
        notificationsFiltersContact = indexContact;

    }else{
        notificationsFiltersContact = 0;
    }

    QCoreApplication::processEvents();

    bool success = true;
    if(!notificationsFilters.value("recv") &&  !notificationsFilters.value("send")){
        QMessageBox::information(0,tr("Notifications Filter"),tr("Select a display filter")+": \n\n"+tr("Sent / Received"),QMessageBox::Ok);
        success = false;
    }

    if(!notificationsFilters.value("group") &&  !notificationsFilters.value("private")){
         QMessageBox::information(0,tr("Notifications Filter"),tr("Select a display filter")+": \n\n"+tr("Group / Private"),QMessageBox::Ok);
         success = false;
    }

    if(!notificationsFilters.value("text") && !notificationsFilters.value("image") &&  !notificationsFilters.value("video") &&  !notificationsFilters.value("audio")){
         QMessageBox::information(0,tr("Notifications Filter"),tr("Select a display filter")+": \n\n"+tr("Text / Image / Video / Audio"),QMessageBox::Ok);
         success = false;
    }

    if(success){

        int number = 0;
        for (int index = 0; index < notifications.size(); index++) {
            Notification notification = notifications.at(index);

            if(   !notification.getMessage().getType().contains("Text")
               && !notification.getMessage().getType().contains("Image")
               && !notification.getMessage().getType().contains("Audio")
               && !notification.getMessage().getType().contains("Video")){
                continue;
            }

            if(!notification.getType().contains("recv-message") && !notification.getType().contains("send-message")){
                continue;
            }

            if(!notificationsFilters.value("recv") && notification.getType().contains("recv-message")){
                continue;
            }

            if(!notificationsFilters.value("send") && notification.getType().contains("send-message")){
                continue;
            }

            if(!notificationsFilters.value("text") && notification.getMessage().getType().contains("Text")){
                continue;
            }

            if(!notificationsFilters.value("image") && notification.getMessage().getType().contains("Image")){
                continue;
            }

            if(!notificationsFilters.value("audio") && notification.getMessage().getType().contains("Audio")){
                continue;
            }

            if(!notificationsFilters.value("video") && notification.getMessage().getType().contains("Video")){
                continue;
            }

            if(!notificationsFilters.value("group") && notification.getMessage().isGroup()){
                continue;
            }

            if(!notificationsFilters.value("private") && !notification.getMessage().isGroup()){
                continue;
            }

            if(notificationsFiltersContact != 0){
                Contact contactNumber = contactsActiveWhatsApp.at(notificationsFiltersContact - 1);
                if(!notification.getMessage().getContact().getNumber().contains(contactNumber.getNumber())){
                    continue;
                }
            }

            bool group = false;
            if(notification.getMessage().isGroup()){
                group = true;
            }

            int type = 1;
            if(!notification.getType().contains("recv-message")){
                type = 2;
            }

            QStandardItem *item = new QStandardItem();

            QString date = notification.getDate().toString("dd-MM-yyyy")+" "+notification.getTime().toString("hh:mm:ss");
            date = date + " : " + QString::number(number + 1);
            QString contact =  notification.getMessage().getContact().getName();
            if(group){
                contact = contact.mid(0,15) + ":::" +  notification.getMessage().getGroup().getContact().getName().mid(0,12);
            }
            /*
            QString contact =  notification.getMessage().getContact().getNumber();
            qDebug() << "---------- " << notification.getMessage().getContact().getNumber() << " " << notification.getMessage().getContact().getName();
            //qDebug() << "indexeddd"  << contactsActiveWhatsApp.size();
            foreach (Contact contactSearch, contactsActiveWhatsApp) {
                if(notification.getMessage().getContact().getNumber().contains(contactSearch.getNumber())){
                    contact = contactSearch.getName();
                    break;
                }
            }
            */

            QIcon iconType = QIcon(":/images/private_on");
            if(group){
                iconType.addFile(":/images/group_on");
            }

            QIcon iconSource = QIcon(":/images/text_on");
            if(notification.getMessage().getType().contains("Image")){
                iconSource.addFile(":/images/picture_on");
            }else if(notification.getMessage().getType().contains("Video")){
                iconSource.addFile(":/images/movie_on");
            }else if(notification.getMessage().getType().contains("Audio")){
                iconSource.addFile(":/images/music_on");
            }else{
                iconSource.addFile(":/images/text_on");
            }

            item->setData(contact,NotificationListViewDelegate::TextRoleContact);
            item->setData(date,NotificationListViewDelegate::TextRoleDate);
            item->setData(iconType,NotificationListViewDelegate::IconRoleType);
            item->setData(iconSource,NotificationListViewDelegate::IconRoleSource);
            item->setData(type,NotificationListViewDelegate::NumberRoleOrigin);
            item->setEditable(false);
            model->appendRow(item);

            number++;

        }

        if(notifications.size() == 0){
            QMessageBox::information(0,tr("Information"),tr("No data"),QMessageBox::Ok);
        }

    }

    ui->labelStatusBarLoadingStatus->setVisible(false);
    ui->labelNotificationsTotal->setText(QString::number(model->rowCount()));

  //  qDebug() << "Total Rows: " << QString::number(model->rowCount());
  //  qDebug() << "Total Rows: " << QString::number(ui->listViewNotifications->model()->rowCount());
}

void MainWindow::setLanguage(QString language){

    if(languages.contains(language)){
        this->language = language;
    }else{
        this->language = "en";
    }

   qDebug() << "Set Language: " << this->language;
   //adding localization

   QTranslator* translator = new QTranslator(0);
   if(!translator->load(":/translations/"+this->language+".qm")){
       qWarning() << "Load language: Unable";
   }else{
       qDebug() << "Load language: OK";
   }

   qApp->installTranslator(translator);  //translator installed
}

void MainWindow::notificationsUpdate()
{
    QFeedbackEffect::playThemeEffect(QFeedbackEffect::ThemeBasicItem);
    ui->labelStatusBarLoadingStatus->setVisible(true);
    ui->pushButtonNotificationsUpdate->setChecked(true);
    repaint();
    updateAnalyzer();
    updateNotificationsData();
    updateNotificationsFilter();
    ui->labelStatusBarLoadingStatus->setVisible(false);
    ui->pushButtonNotificationsUpdate->setChecked(false);
}

void MainWindow::updateAnalyzer(){
    analyzer->updateLastByDays(days);
}

void MainWindow::updateNotificationsData(){
    qDebug() << "updateNotificationsData";
    notifications = analyzer->getNotificationsLastByDays(days);
    contactsActiveWhatsApp = analyzer->getContactsWhatsApp();
    foreach (Contact contact, contactsActiveWhatsApp) {
        qDebug() << contact.getNumber() << " : " << contact.getName();
    }
}

void MainWindow::on_horizontalSliderNotificationsDays_valueChanged(int value)
{
    QFeedbackEffect::playThemeEffect(QFeedbackEffect::ThemeBasicSlider);
    days = value - 1;
    ui->labelNotificationsDays->setText(QString::number(value));
    qDebug() << "Days: " << QString::number(days);
}


void MainWindow::captureScreen()
{
     //QDesktopServices::openUrl(QUrl("http://jh2odo.es/"));
    qDebug() << QDesktopServices::displayName(QDesktopServices::PicturesLocation);

    ui->pushButtonStatusBarCaptureScreen->setChecked(true);
    qApp->beep();
    QString path = pathImages;
    if(!QDir(path).exists()){
        QDir().mkdir(path);
    }
    QPixmap pixmap(this->size());
    this->render(&pixmap);

    QString name = "000.png";
    if(QDir(path).exists()){
        QDir dir = QDir(path);
        dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
        QStringList filters;
            filters << "*.png";
            dir.setNameFilters(filters);
        dir.setSorting(QDir::Name);
        QFileInfoList dirs = dir.entryInfoList();
        if(dirs.size() > 0){
            name = QString::number(dirs.last().baseName().toInt() + 1);
            if(name.length() == 1){
                name = "00"+name;
            }else if(name.length() == 2){
                name = "0"+name;
            }
            qDebug() << name;
            name = name + ".png";
        }
    }

    path = path + name;
    if(pixmap.save(path)){
        QMessageBox::information(0,tr("Save Image"),tr("The image has been saved successfully in") + ": \n\n" +path,QMessageBox::Ok);
    }else{
        QMessageBox::critical(0,tr("Error"),tr("The image has not been saved in") + ": \n\n" +path,QMessageBox::Ok);
    }
    ui->pushButtonStatusBarCaptureScreen->setChecked(false);
}

void MainWindow::on_comboBoxNotificationsFilterContacts_activated(int index)
{
    updateNotificationsFilter();
    qDebug() << "Filter Contact 2: " << QString::number(index);
}

void MainWindow::on_comboBoxDirectoryWALogs_activated(int index)
{
    if(index == 1){
        pathWALogs = QFileDialog::getExistingDirectory();
        qDebug() << "Set Directory WALogs: " << pathWALogs;
        ui->comboBoxDirectoryWALogs->setItemText(0,pathWALogs);
        ui->comboBoxDirectoryWALogs->setCurrentIndex(0);
        analyzer->setPathWALogs(pathWALogs);
    }
}

void MainWindow::on_comboBoxDirectoryScreenshot_activated(int index)
{
    if(index == 1){
        pathImages = QFileDialog::getExistingDirectory();
        qDebug() << "Set Directory Images: " << pathImages;
        ui->comboBoxDirectoryScreenshot->setItemText(0,pathImages);
        ui->comboBoxDirectoryScreenshot->setCurrentIndex(0);
    }
}

void MainWindow::on_comboBoxLanguage_activated(int index)
{
    QString language;
    if(index == 0){
        language = "es";
        ui->comboBoxLanguage->setCurrentIndex(0);
    }else if(index == 2){
        language = "fr";
        ui->comboBoxLanguage->setCurrentIndex(2);
    }else if(index == 3){
        language = "de";
        ui->comboBoxLanguage->setCurrentIndex(3);
    }else if(index == 4){
        language = "pt";
        ui->comboBoxLanguage->setCurrentIndex(4);
    }else if(index == 5){
        language = "it";
        ui->comboBoxLanguage->setCurrentIndex(5);
    }else if(index == 6){
        language = "ru";
        ui->comboBoxLanguage->setCurrentIndex(6);
    }else{
        language = "en";
        ui->comboBoxLanguage->setCurrentIndex(1);
    }

   setLanguage(language);

   retranslateUi();
}

void MainWindow::on_pushButtonInfoSupportTwitter_clicked()
{
    QFeedbackEffect::playThemeEffect(QFeedbackEffect::ThemeBasicButton);
    bool res = QDesktopServices::openUrl(QUrl("https://twitter.com/jh2odo"));
}

void MainWindow::on_pushButtonInfoSupportWeb_clicked()
{
    QFeedbackEffect::playThemeEffect(QFeedbackEffect::ThemeBasicButton);
    bool res = QDesktopServices::openUrl(QUrl("http://joseaguado.com"));
}


void MainWindow::retranslateUi(){
    qDebug() << "retranslateUi";

    ui->labelNotifications->setText(tr("Notifications"));
    ui->labelNotificationsDaysName->setText(tr("Days")+":");
    ui->comboBoxNotificationsFilterContacts->setItemData(0,tr("All contacts"));

    ui->labelNotificationsDetails->setText(tr("Notification Details"));

    ui->labelAnalysis->setText(tr("Analysis - Stats"));
    ui->comboBoxAnalysisFilterContacts->setItemData(0,tr("All contacts"));

    ui->labelTools->setText(tr("Tools"));
    ui->labelToolDisable->setText(tr("Not enabled"));

    ui->labelSettings->setText(tr("Settings"));
    ui->labelLanguage->setText(tr("Language"));
    ui->labelLanguageDescription->setText(tr("Select the application language"));
    ui->labelDirectoryWALogs->setText(tr("Directory WALogs"));
    ui->labelDirectoryWALogsDescription->setText(tr("Select the directory WhatsApp logs"));
    ui->labelDirectoryScreenshot->setText(tr("Directory Screenshot"));
    ui->labelDirectoryScreenshotDescription->setText(tr("Selected images directory to save"));
    ui->labelSettingsCache->setText(tr("Cache data"));
    ui->labelSettingsCacheDeleteDescription->setText(tr("All stored data will be lost. It will only be possible to reprocess the last 7 days, if there still"));
    ui->pushButtonSettingsCacheDelete->setText(tr("Delete"));
    ui->comboBoxDirectoryWALogs->setItemData(1,tr("Select other directory..."));
    ui->comboBoxDirectoryScreenshot->setItemData(1,tr("Select other directory..."));

    ui->labelInfo->setText(tr("About - Help"));
    ui->labelInfoAbout->setText(tr("About"));
    ui->labelInfoDescription->setText(tr("Analyze logs files of WhatsApp App to display and export the data of the communications in lists, reports and statistics."));

    ui->labelInfoSupport->setText(tr("Support"));
    ui->labelInfoAuthor->setText(tr("Author"));
    ui->labelInfoName->setText(tr("Name"));
    ui->labelInfoWeb->setText(tr("Web"));

    ui->labelInfoQuickGuide->setText(tr("Quick Guide"));
    ui->labelInfoQuickGuideScrollMainMenu->setText(tr("To scroll through the main menu swipe left or right on top of the application"));

    ui->labelInfoNotificationsDetails->setText(tr("Notifications"));
    ui->labelInfoNotificationsSwip->setText(tr("Show / hide the filter menu swiping left or right on the main menu"));
    ui->labelInfoNotificationsDescription->setText(tr("To process and display using filters"));
    ui->labelInfoNotificationsUpdateDaysText->setText(tr("Update days"));
    ui->labelInfoNotificationsFilterTextText->setText(tr("Text filter"));
    ui->labelInfoNotificationsFilterImageText->setText(tr("Image filter"));
    ui->labelInfoNotificationsFilterVideoText->setText(tr("Video filter"));
    ui->labelInfoNotificationsFilterAudioText->setText(tr("Audio filter"));
    ui->labelInfoNotificationsFilterPrivateText->setText(tr("Private filter"));
    ui->labelInfoNotificationsFilterGroupText->setText(tr("Group filter"));
    ui->labelInfoNotificationsFilterReceivedText->setText(tr("Received filter"));
    ui->labelInfoNotificationsFilterSentText->setText(tr("Sent filter"));
    ui->labelInfoNotificationsFilterContactsText->setText(tr("Contacts filter"));

    ui->labelInfoAnalysisDetails->setText(tr("Analysis - Stats"));
    ui->labelInfoAnalysisSwip->setText(tr("Show / hide the filter menu swiping left or right on the main menu"));
    ui->labelInfoAnalysisDescription->setText(tr("To analyzer and display the statistics using charts"));
    ui->labelInfoAnalysisPiesText->setText(tr("Pies"));
    ui->labelInfoAnalysisHistogramsText->setText(tr("Histograms"));
    ui->labelInfoAnalysisGraphsText->setText(tr("Graphs"));
    ui->labelInfoAnalysisFilter1daysText->setText(tr("1 day filter"));
    ui->labelInfoAnalysisFilter3daysText->setText(tr("3 days filter"));
    ui->labelInfoAnalysisFilter7daysText->setText(tr("7 days filter"));
    ui->labelInfoAnalysisFilter30daysText->setText(tr("30 days filter"));
    ui->labelInfoAnalysisFilterContactsText->setText(tr("Contacts filter"));

    ui->labelInfoToolsDetails->setText(tr("Tools"));
    ui->labelInfoToolsDescription->setText(tr("To export the data to text files or images"));

    ui->labelInfoSettingsDetails->setText(tr("Settings"));
    ui->labelInfoSettingsDescription->setText(tr("To configure the application"));

    ui->labelInfoInformationDetails->setText(tr("Information"));
    ui->labelInfoInformationDescription->setText(tr("About, Help and Support"));

    ui->labelInfoExitDetails->setText(tr("Exit"));
    ui->labelInfoCaptureDetails->setText(tr("Capture Screen"));
    ui->labelInfoLoadDetails->setText(tr("App Loading"));
}

void MainWindow::on_comboBoxAnalysisFilterContacts_activated(int index)
{
    updateAnalysisData();
}

void MainWindow::toggleSubBars(int bar){


    if(bar == 0){
        animationNotifications->setDuration(400);
        if(subBarNotifications)
        {
            animationNotifications->setEndValue(QPoint(SUBBAR_N_X_HIDDEN, ui->widgetNotificationsFilter->pos().y()));
            animationNotifications->setEasingCurve(QEasingCurve::InBack);
            ui->listViewNotifications->setFixedHeight(subBarNotificationsHeight + ui->widgetNotificationsFilter->height());
            subBarNotifications = false;
        }
        else
        {
            animationNotifications->setEndValue(QPoint(SUBBAR_N_X_SHOWN, ui->widgetNotificationsFilter->pos().y()));
            animationNotifications->setEasingCurve(QEasingCurve::OutBack);
            ui->listViewNotifications->setFixedHeight(subBarNotificationsHeight);
            subBarNotifications = true;
        }
        animationNotifications->start();
    }else if(bar == 1){
        animationAnalysis->setDuration(400);
        if(subBarAnalysis)
        {
            animationAnalysis->setEndValue(QPoint(SUBBAR_A_X_HIDDEN, ui->widgetAnalysisFilter->pos().y()));
            animationAnalysis->setEasingCurve(QEasingCurve::InBack);
            ui->scrollAreaAnalysis->setFixedHeight(subBarAnalysisHeight + ui->widgetAnalysisFilter->height());
            subBarAnalysis = false;
        }
        else
        {
            animationAnalysis->setEndValue(QPoint(SUBBAR_A_X_SHOWN, ui->widgetAnalysisFilter->pos().y()));
            animationAnalysis->setEasingCurve(QEasingCurve::OutBack);
            ui->scrollAreaAnalysis->setFixedHeight(subBarAnalysisHeight);
            subBarAnalysis = true;
        }
        animationAnalysis->start();
    }
}

void MainWindow::on_pushButtonStatusSubBarsMode_clicked()
{
    int currentPage = ui->stackedWidgetCentral->currentIndex();
    if((currentPage == 0) || (currentPage == 1)){
        toggleSubBars(currentPage);
    }
}

void MainWindow::on_pushButtonSettingsCacheDelete_clicked()
{
    QFeedbackEffect::playThemeEffect(QFeedbackEffect::ThemeBasicButton);
    int result = QMessageBox::question(0, tr("Delete"), tr("Do you want to delete the cache data?") , QMessageBox::No, QMessageBox::Yes);
    if(QMessageBox::Yes == result)
    {
        int result = analyzer->removeCache();
        if(result){
            QMessageBox::information(0,tr("Delete"),tr("The cache data has been deleted successfully"),QMessageBox::Ok);
        }else{
            QMessageBox::critical(0,tr("Error"),tr("The cache data has not been deleted"),QMessageBox::Ok);
        }
    }
}

