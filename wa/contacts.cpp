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
#include "contacts.h"

Contacts::Contacts()
{

}

QList<Contact> Contacts::getContactsFromPhone(){
   QPointer<QContactManager> contactManager = new QContactManager();
   // QStringList QContactManager::supportedContactTypes ()
   QList<QContact> contactsPhone = contactManager->contacts();
   QList<Contact> contacts;
   foreach (const QContact& contact, contactsPhone) {
       QContactPhoneNumber phoneNumber = contact.detail<QContactPhoneNumber>();
       //  QList<QContactPhoneNumber> phoneNumbers = contact.details<QContactPhoneNumber>();
       if(!phoneNumber.isEmpty()){
           QContactName name = contact.detail<QContactName>();
           QString number = phoneNumber.number();
           number = number.replace("+","");
           QString nameFinal = name.firstName()+" "+name.lastName();
           contacts.append(Contact(nameFinal.trimmed(),number));
       }
   }
   return contacts;
}

QList<Contact> Contacts::getContactsFromNokiaBackup(QString filePath){
    Nbu contactsNbu = Nbu(filePath);
    QList<Contact> contacts = contactsNbu.getContacts();
    return contacts;
}

void Contacts::bubbleSortContacts(QList<Contact> *contacts)
{
    Contact tmp;
    for (int j = 1; j <= contacts->size(); j++)
    {
         for (int i = 0; i < (contacts->size() -1); i++)
         {
             Contact contactA = contacts->at(i);
             Contact contactB = contacts->at(i+1);
             if (contactA.getName() > contactB.getName()) //Si Cambiamos el operador < por > entonces ordenaremos de menor a mayor
             {
               tmp = contacts->at(i);
               contacts->replace(i,contacts->at(i+1));
               contacts->replace(i+1,tmp);
             }
         }
    }
}
