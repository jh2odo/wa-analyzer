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
#include "graph.h"

Graph::Graph(QWidget *parent) :
    QWidget(parent)
{
    QSizePolicy policy(QSizePolicy::Preferred, QSizePolicy::Preferred );
    setSizePolicy( policy );
    setVisible(true);
    typeChart = Graph::Pie;
}

Graph::~Graph()
{
}

//Histogramm or Pie
void Graph::setType(Graph::type type){
    this->typeChart = type;
}

void Graph::setTitle(QString text)
{
    this->title = text;
}

void Graph::setData(QMap<QString,QColor> data){
    this->data = data;
}

void Graph::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    qDebug() << width() << " " << height();

    QPainter *painter = new QPainter();
    painter->begin(this);

    Charts PieChart;

    PieChart.setTitle(title);
    PieChart.setTitleCords(10,10);

    if(typeChart == Graph::Pie){
        PieChart.setType(Charts::Pie);//{Pie};
        PieChart.setCords(20,40,260,160);
        PieChart.setLegendCords(50,210);
        PieChart.setShadows(true);
    }else if(typeChart == Graph::Histogramm){
        PieChart.setType(Charts::Histogramm);//{Histogramm};
        PieChart.setCords(50,40,250,160);
        PieChart.setLegendCords(50,210);
        PieChart.setShadows(true);
    }else if(typeChart == Graph::Graphic){
        PieChart.setType(Charts::Graphic);//{Graphic};
        PieChart.setCords(50,40,250,160);
        PieChart.setLegendCords(50,210);
        PieChart.setShadows(false);
    }

    foreach (QString key, data.keys()) {
        QStringList text = key.split(":");
        QString item = text.at(0)+" ("+text.at(2)+")";
        QString percent = text.at(1);
        QColor color = data.value(key);
        PieChart.addPiece(item,color,percent.toInt());
    }

    PieChart.draw(painter);

    painter->end();
}

void Graph::setSize(QSize size)
{
    setFixedSize(size);
}
