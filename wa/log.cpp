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
#include "log.h"

Log::Log()
{
}

Log::Log(QDate date, QString filePath){
    this->date = date;
    this->filePath = filePath;
    lines = QList<Line>();
    version = "2.8.22";
}

void Log::setFilePath(QString filePath){
    this->filePath = filePath;
}

QString Log::getFilePath(){
    return this->filePath;
}

void Log::addLine(Line line){
    lines.append(line);
}

void Log::setDate(QDate date){
    this->date = date;
}

void Log::setLines(QList<Line> lines){
    this->lines = lines;
}

void Log::bubbleSortDescendingLines(){
    Line tmpLine;
    for (int j = 1; j <= lines.size(); j++)
    {
         for (int i = 0; i < (lines.size() -1); i++)
         {
             Line line1 = lines.at(i);
             Line line2 = lines.at(i+1);
             if (line1.getTime() < line2.getTime()) //Si Cambiamos el operador < por > entonces ordenaremos de menor a mayor
             {
               tmpLine = lines.at(i);
               lines.replace(i,lines.at(i+1));
               lines.replace(i+1,tmpLine);
             }
         }
    }
}

bool Log::existLine(Line line)
{
    foreach (Line l, this->lines) {
        //qDebug() << QString::number(l.getTime().secsTo(line.getTime()));
        if((l.getTime().secsTo(line.getTime()) == 0) && (l.getType().contains(line.getType())) && (l.getText() == line.getText())){
            return true;
        }
    }
    return false;
}

void Log::reverseLines(){
    QList<Line> tmp = QList<Line>();
    while(!this->lines.isEmpty()) {
         tmp.append(this->lines.last());
         this->lines.pop_back();
    }
    //this->lines.clear();
    this->lines = tmp;
}

QDate Log::getDate(){
    return this->date;
}

QList<Line> Log::getLines(){
    return this->lines;
}


QDataStream & operator<< (QDataStream& stream, const Log& log){
    stream << log.date << log.filePath << log.lines;
    return stream;
}

QDataStream & operator>> (QDataStream& stream, Log& log){
    QDate date;
    QString filePath;
    QList<Line> lines;
    stream >> date >> filePath >> lines;
    log.date = date;
    log.filePath = filePath;
    log.lines = lines;
    return stream;
}
