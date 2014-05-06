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
#include "line.h"

Line::Line()
{
}

Line::Line(QTime time,QString type,QMap<QString,QString> text){
    this->time = time;
    this->type = type;
    this->text = text;
    raw = "";
}

QString Line::getType(){
    return type;
}

QMap<QString,QString> Line::getText(){
    return text;
}

QTime Line::getTime(){
    return time;
}

bool Line::isNull()
{
   return time.isNull();
}

void Line::setType(QString type){
    this->type = type;
}

void Line::setText(QMap<QString,QString> text){
    this->text = text;
}

void Line::setTime(QTime time){
    this->time = time;
}

void Line::setRaw(QString raw)
{
    this->raw = raw;
}

QString Line::getRaw()
{
    return raw;
}

QDataStream & operator<< (QDataStream& stream, const Line& line){
    stream << line.time << line.type << line.text;
    return stream;
}

QDataStream & operator>> (QDataStream& stream, Line& line){
    QTime time;
    QString type;
    QMap<QString,QString> text;
    stream >> time >> type >> text;
    line.time = time;
    line.type = type;
    line.text = text;
    return stream;
}

