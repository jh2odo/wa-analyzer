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
#ifndef LOG_H
#define LOG_H

#include "line.h"
#include <QString>
#include <QDate>
#include <QList>
#include <QDebug>
#include <QDataStream>

class Log
{
public:
    Log();
    Log(QDate date, QString filePath);
    void addLine(Line line);
    void setDate(QDate date);
    void setLines(QList<Line> lines);
    QDate getDate();
    QList<Line> getLines();
    void reverseLines();
    void setFilePath(QString filePath);
    QString getFilePath();
    void bubbleSortDescendingLines();
    bool existLine(Line line);
    friend QDataStream &operator <<(QDataStream &stream, const Log &log);
    friend QDataStream &operator >>(QDataStream &stream, Log &log);
private:
    QDate date;
    QList<Line> lines;
    QString filePath;
    QString version;
};

#endif // LOG_H
