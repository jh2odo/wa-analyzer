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
#ifndef GRAPH_H
#define GRAPH_H

#include <QWidget>
#include <QString>
#include <QPainter>
#include <QFontMetrics>
#include <QDebug>
#include <QMap>
#include "../util/charts.h"

class Graph : public QWidget
{
    Q_OBJECT
public:
    explicit Graph(QWidget *parent = 0);
    ~Graph();
    enum type { Histogramm , Pie, Graphic };
    void setSize(QSize size);
    void setType(Graph::type type);
    void setTitle(QString text);
    void setData(QMap<QString, QColor> data);
signals:
    
public slots:

private:
    void paintEvent(QPaintEvent *event);
    int typeChart;
    QString title;
    QMap<QString,QColor> data;

};

#endif // GRAPH_H
