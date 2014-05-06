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
#ifndef LOGGER_H
#define LOGGER_H

#include "log.h"
#include <zlib.h>
#include <QString>
#include <QList>
#include <QMap>
#include <QDebug>
#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QtCore/QTextStream>
#include <QStringList>
#include <QDateTime>
#include <QFileInfoList>
#include <QFileInfo>
#include <QCryptographicHash>
#include <QDebug>
#include <QObject>

class Logger : public QObject
{
    Q_OBJECT
public:
    explicit Logger(QObject *parent = 0);
    Logger(QString pathWALogs);

    QList<Log> getLogsByRange(QDate dateStart, QDate dateEnd);
    QList<Log> getLogsByDays(QList<QDate> days);
    QList<Log> getLogsLastByDays(int days = 1);
 //   QList<Log> getLogsLastByHours(int days = 6);

    void setPathWALogs(QString pathWALogs);
    QString getPathWALogs();

    void processWALogs(QList<QDate> days);
    void processWALogs(QDate dateStart, QDate dateEnd);
    int getTotalWALogsNotProcessed(QDate dateStart, QDate dateEnd);
    bool removeLogs(); // Data serialized only

private slots:
        void setProcessed(int value);

signals:
     void valueProcessChanged(int value);

private:
    QString pathWALogs;
    int MAX_LINES_PROCESS; // Limite de lineas que se pueden procesar
    int totalLinesProcessed;
    int totalLinesFound;
    QString PATH_LOGS_DATA;
    QString PATH_MD5;
    QString PATH_DAT;

    QString WHATSAPP_DEFAULT_VERSION;

    QByteArray decompressGz(QByteArray data);


    QString calculateHashMd5(QString data);
    QMap<QString,int> detectVersion(QStringList lines);

    QFileInfoList getLogsDataByRange(QDate dateStart, QDate dateEnd);
    QFileInfoList getLogsDataByDays(QList<QDate> days);
    QFileInfoList getWALogsByRange(QDate dateStart, QDate dateEnd);
    QFileInfoList getWALogsByDays(QList<QDate> days);

    void processWALogs(QFileInfoList listLogs);
    Log processWALog(QFileInfo logFile);
    Line processWALine(QString line);
    QTime getTimeWALine(QString line);

    QFileInfoList getWALogsNotProcessed(QFileInfoList listLogs);
    void setWALogProcessed(QFileInfo logFile);
    QList<Log> groupLogsByDays(QList<Log> logs);

    Log getLogDataSerialized(QFileInfo logFile);
    void setLogDataSerialized(Log log);

    QFileInfoList getListDirs(QString path);
    QDate getDateFromLogName(QFileInfo fileInfo);

    QList<Log> reverseLogs(QList<Log> logs);

    int processed;

    bool removeDir(QString dirName);

    QString lastLine;
};

#endif // LOGGER_H
