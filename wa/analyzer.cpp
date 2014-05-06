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
#include "analyzer.h"

Analyzer::Analyzer(QObject *parent) :
    QObject(parent)
{
    logger = new Logger("");
    contactsPhone = false;

    Contacts * phoneContacts = new Contacts();
    contacts = phoneContacts->getContactsFromPhone();
    phoneContacts->bubbleSortContacts(&contacts);
    connect(logger, SIGNAL(valueProcessChanged(int)), this, SLOT(loggerProcessed(int)));

    colors.insert("recv",QColor(111,117,255));
    colors.insert("send",QColor(138,255,255));
    colors.insert("text",QColor(214,214,0));
    colors.insert("audio",QColor(0,208,0));
    colors.insert("video",QColor(220,0,0));
    colors.insert("image",QColor(124,174,209));
    colors.insert("private",QColor(243,185,60));
    colors.insert("group",QColor(219,149,210));
}

void Analyzer::loggerProcessed(int value){
    qDebug() << "Logger Processed " << value;
}

void Analyzer::updateLastByDays(int days)
{
    QDate last = QDate::currentDate();
    last = last.addDays(days * -1);
    logger->processWALogs(last,QDate::currentDate());
}

/*
void Analyzer::processStats(QDate dateStart, QDate dateEnd){

}
*/

QList<Stats> Analyzer::getStatsTotalLastByDays(int days){

    QList<Notification> notifications = getNotificationsLastByDays(days);

    if(notifications.size() == 0){
        return QList<Stats>();
    }

    int total = 0;

    int totalIn = 0;
    int totalOut = 0;

    int totalText = 0;
    int totalImage = 0;
    int totalVideo = 0;
    int totalAudio = 0;

    int totalPrivate = 0;
    int totalGroup = 0;

    QMap<QString,int> totalContactsByNumber;

    for (int index = 0; index < notifications.size(); index++) {
        Notification notification = notifications.at(index);

        if(   !notification.getMessage().getType().contains("Text")
           && !notification.getMessage().getType().contains("Image")
           && !notification.getMessage().getType().contains("Audio")
           && !notification.getMessage().getType().contains("Video")){
            continue;
        }

        if(     !notification.getType().contains("recv-message")
             && !notification.getType().contains("send-message")){
            continue;
        }

        if(notification.getType().contains("recv-message")){
            totalIn++;
        }

        if(notification.getType().contains("send-message")){
            totalOut++;
        }

        if(notification.getMessage().getType().contains("Text")){
            totalText++;
        }

        if(notification.getMessage().getType().contains("Image")){
            totalImage++;
        }

        if(notification.getMessage().getType().contains("Audio")){
            totalAudio++;
        }

        if(notification.getMessage().getType().contains("Video")){
            totalVideo++;
        }

        if(notification.getMessage().isGroup()){
            totalGroup++;
        }

        if(!notification.getMessage().isGroup()){
            totalPrivate++;
        }


        Contact contact = notification.getMessage().getContact();
        if(contactsWhatsApp.contains(contact.getNumber())){
            if(totalContactsByNumber.contains(contact.getNumber())){
                totalContactsByNumber.insert(contact.getNumber(),(totalContactsByNumber.value(contact.getNumber()) + 1));
            }else{
                totalContactsByNumber.insert(contact.getNumber(),1);
            }
        }

        total++;
    }

    QMap<int, QString> totalContactsByTotal;
    foreach( QString key, totalContactsByNumber.keys() )
    {
        totalContactsByTotal.insertMulti(totalContactsByNumber.value(key),key);
        qDebug() << key << " : " << totalContactsByNumber.value(key);
    }

    QList<int> list = totalContactsByNumber.values();
    QSet<int> setUnique = list.toSet();
    list = setUnique.values();
    qSort(list.begin(), list.end());


    for (int index = list.size() - 1; index >= 0; index--) {
        QMap<int, QString>::const_iterator i = totalContactsByTotal.find(list.at(index));
        while (i != totalContactsByTotal.end() && i.key() == list.at(index)) {
            qDebug() << i.value() << i.key();
            ++i;
        }
        //qDebug() << total << " : " << totalContactsByTotal.value(total);
    }

    // Para que no reviente
    if(total == 0){
        total++;
    }

    QList<Stats> stats;


    QMap<QString,QColor> data;
    data.insert("In:"+QString::number((totalIn * 100) / total)+":"+QString::number(totalIn),colors.value("recv"));
    data.insert("Out:"+QString::number((totalOut * 100) / total)+":"+QString::number(totalOut),colors.value("send"));
    stats.append(data);

    data.clear();
    data.insert("Private:"+QString::number((totalPrivate * 100) / total)+":"+QString::number(totalPrivate),colors.value("private"));
    data.insert("Group:"+QString::number((totalGroup * 100) / total)+":"+QString::number(totalGroup),colors.value("group"));
    stats.append(data);

    data.clear();
    data.insert("Text:"+QString::number((totalText * 100) / total)+":"+QString::number(totalText),colors.value("text"));
    data.insert("Image:"+QString::number((totalImage * 100) / total)+":"+QString::number(totalImage),colors.value("image"));
    data.insert("Video:"+QString::number((totalVideo * 100) / total)+":"+QString::number(totalVideo),colors.value("video"));
    data.insert("Audio:"+QString::number((totalAudio * 100) / total)+":"+QString::number(totalAudio),colors.value("audio"));
    stats.append(data);

    return stats;
}

bool Analyzer::removeCache()
{
   return logger->removeLogs();
}

QList<Notification> Analyzer::getNotificationsLastByDays(int days)
{
    contactsWhatsApp.clear(); // Reset para no acumular contact

    QList<Notification> notifications = QList<Notification>();

    QList<Log> logs = logger->getLogsLastByDays(days);
    for (int index = 0; index < logs.size(); index++) {
        Log log = logs.at(index);
        QList<Line> lines = log.getLines();
        int total = lines.size();
        for (int index2 = 0; index2 < total; index2++) {
            Line line = lines.at(index2);
            QMap<QString,QString> textList = line.getText();
            Notification notification = Notification();
            notification.setDate(log.getDate());
            notification.setTime(line.getTime());
            notification.setType(line.getType());

            Message message = Message();
            Contact contact = Contact("",textList.value("contact"));
            if(contactsPhone){
                //qDebug() << "---------- " << getContactName(contact.getNumber());
                contact.setName(getContactName(contact.getNumber()));
            }
            message.setContact(contact);
            message.setType(textList.value("type"));
            message.setLength(textList.value("length").toInt());
            message.setId(textList.value("id"));

            if(textList.contains("group")){
                contact = Contact("",textList.value("group"));
                if(contactsPhone){
                    contact.setName(getContactName(contact.getNumber()));
                }
                message.setGroup(Group(contact,textList.value("id")));
            }

            notification.setMessage(message);
            notifications.append(notification);
        }
    }

    return notifications;
}

void Analyzer::setContactsFromPhone()
{
    contactsPhone = true;
}

QList<Contact> Analyzer::getContactsWhatsApp(){
    QList<Contact> contacts = QList<Contact>();
    foreach( QString key, contactsWhatsApp.keys() )
    {
        contacts.append(Contact(contactsWhatsApp.value(key),key));
    }
    Contacts * phoneContacts = new Contacts();
    phoneContacts->bubbleSortContacts(&contacts);
    return contacts;
}

QList<Contact> Analyzer::getContactsPhone(){
    return contacts;
}

QString Analyzer::getContactName(QString number)
{
    if(contactsWhatsApp.contains(number)){
        return contactsWhatsApp.value(number);
    }

    foreach (Contact contact, contacts) {
        if(number.contains(contact.getNumber())){
            contactsWhatsApp.insert(number,contact.getName());
            return contactsWhatsApp.value(number);
        }
    }
    contactsWhatsApp.insert(number,number);
    return number;
}

/*t> Analyzer::getContacts(QList<Contact> contacts, QList<Notification> notifications)
{
    QList<Contact> contactsTmp = QList<Contact>();

    foreach (Notification notification, notifications) {
        bool found = false;
        foreach (Contact contact, contactsTmp) {
            if(notification.getMessage().getContact().getNumber().contains(contact.getNumber())){
                found = true;
                break;
            }
        }
        if(!found){
            foreach (Contact contact, contacts) {
                if(notification.getMessage().getContact().getNumber().contains(contact.getNumber())){
                    found = true;
                    contactsTmp.append(contact);
                    break;
                }
            }
        }
    }
    return contactsTmp;
}
*/

int Analyzer::getTotalNewLogs(int days)
{
    QDate last = QDate::currentDate();
    last = last.addDays(days * -1);
    return logger->getTotalWALogsNotProcessed(last,QDate::currentDate());
}

void Analyzer::setPathWALogs(QString pathWALogs)
{
    logger->setPathWALogs(pathWALogs);
}
