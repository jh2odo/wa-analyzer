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
#ifndef ANALYZER_H
#define ANALYZER_H

#include <QObject>
#include <QMap>
#include <QColor>
#include "notification.h"
#include "contact.h"
#include "contacts.h"
#include "logger.h"
#include "stats.h"

class Analyzer : public QObject
{
    Q_OBJECT
public:
    explicit Analyzer(QObject *parent = 0);
    void updateLastByDays(int days = 1);
    QList<Notification> getNotificationsLastByDays(int days = 1);

    void setContactsFromPhone();

    //QList<Contact> getContacts(QList<Contact> contacts,QList<Notification> notifications);

    int getTotalNewLogs(int days = 10);
    void setPathWALogs(QString pathWALogs);
    QList<Contact> getContactsWhatsApp();
    QList<Contact> getContactsPhone();
    QList<Stats> getStatsTotalLastByDays(int days);

    bool removeCache();
signals:
    
private slots:
    void loggerProcessed(int value);

private:
    Logger *logger;
    bool contactsPhone;
    QList<Contact> contacts;
    QMap<QString,QString> contactsWhatsApp;

    QString getContactName(QString number);

    QMap<QString,QColor> colors;
};

#endif // ANALYZER_H
