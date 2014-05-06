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
#include "nbu.h"

Nbu::Nbu()
{
    this->filePath = "";
}

Nbu::Nbu(QString filePath)
{
     this->filePath = filePath;
}


void Nbu::setFilePath(QString filePath){
    this->filePath = filePath;
}

// File .nbu
QList<Contact> Nbu::getContacts(){
    QList<Contact> contacts;

    QFile file(filePath);
    file.open(QIODevice::ReadOnly | QIODevice::Text);

    QTextStream stream(&file);

    QString lineLast;
    while(!stream.atEnd()){
        QString line = stream.readLine();

        if(line.indexOf("N") == 0){
            lineLast = line;
        }

        if(line.contains("CELL")){

            lineLast = lineLast.replace("N:","");
            lineLast = lineLast.replace("N;ENCODING=QUOTED-PRINTABLE;CHARSET=utf-8:","");
            lineLast = lineLast.replace("=20"," ");
            lineLast = lineLast.replace("=C3=A1","á");
            lineLast = lineLast.replace("=C3=A9","é");
            lineLast = lineLast.replace("=C3=B3","ó");
            lineLast = lineLast.replace("=C3=AD","í");
            lineLast = lineLast.replace("=C3=BA","ú");
            lineLast = lineLast.replace("=C3=B1","ñ");
            lineLast = lineLast.replace(";"," ");
            lineLast = lineLast.simplified();

            QString number = line.mid(line.length() - 9, 9);
            number = "34" + number.replace("PREF:","");
            Contact contact(lineLast,number);
            contacts.append(contact);
        }
    }

    file.close();

    // Habria que serializar aqui

    return contacts;
}
