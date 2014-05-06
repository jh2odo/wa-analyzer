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
#include "notificationlistviewdelegate.h"

NotificationListViewDelegate::NotificationListViewDelegate()
    {
    // TODO Auto-generated constructor stub

    }

NotificationListViewDelegate::~NotificationListViewDelegate()
    {
    // TODO Auto-generated destructor stub
    }

//alocate each item size in listview.
QSize NotificationListViewDelegate::sizeHint(const QStyleOptionViewItem &  option ,
                              const QModelIndex & index) const
{
    QSize iconsize = QSize(20,20);
    QFont font = QApplication::font();
    QFontMetrics fm(font);

    return(QSize(iconsize.width(),iconsize.height()+fm.height() + 8));
}
void NotificationListViewDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                           const QModelIndex &index) const
 {
    QStyledItemDelegate::paint(painter,option,index);

    painter->save();

    QFont font = QApplication::font();
    QFont SubFont = QApplication::font();
    //font.setPixelSize(font.weight()+);
    font.setBold(true);
    font.setPointSize(7);
    SubFont.setWeight(font.weight() - 15);
    SubFont.setPointSize(6);
    QFontMetrics fm(font);

    QIcon icon = qvariant_cast<QIcon>(index.data(IconRoleType));
    QIcon icon2 = qvariant_cast<QIcon>(index.data(IconRoleSource));
    QString headerTextTmp = qvariant_cast<QString>(index.data(TextRoleContact));

    QString subText = qvariant_cast<QString>(index.data(TextRoleDate));
    int origin = qvariant_cast<int>(index.data(NumberRoleOrigin));

    QSize iconsize = QSize(20,20);

    QRect headerRect = option.rect;
    QRect subheaderRect = option.rect;
    QRect iconRect = option.rect;

    iconRect.setRight(iconsize.width()+30);
    iconRect.setTop(iconRect.top());


    headerRect.setLeft(iconRect.right());
    subheaderRect.setLeft(iconRect.right());
    headerRect.setTop(headerRect.top()+2);
    headerRect.setBottom(headerRect.top()+fm.height());

    QStringList headerText;
    bool group;
    if(headerTextTmp.indexOf(":::") != -1){
        QStringList textTmp = headerTextTmp.split(":::");
        if(textTmp.size() == 2){
            group = true;
            QString cont = textTmp.at(0);
            QString grp = textTmp.at(1);
            headerText.append(cont);
            headerText.append(grp);
        }else{
            headerText.append(headerTextTmp);
            group = false;
        }
    }else{
        group = false;
        headerText.append(headerTextTmp);
    }

    if(group){
        headerRect.setRight(190);
    }




    subheaderRect.setTop(headerRect.bottom()+2);


    //painter->drawPixmap(QPoint(iconRect.right()/2,iconRect.top()/2),icon.pixmap(iconsize.width(),iconsize.height()));
    if(origin == 1){
        painter->setBrush(QBrush(QColor(111,117,255)));
        painter->setPen(QColor(111,117,255));
    }else{
        painter->setBrush(QBrush(QColor(138,255,255)));
        painter->setPen(QColor(138,255,255));
    }

    painter->drawRect(QRect(QPoint(iconRect.left()+2,iconRect.top()+5),QPoint(iconRect.left()+8,iconRect.top() + (iconsize.height() * 2) + 4)));

    painter->drawPixmap(QPoint(iconRect.left()+iconsize.width()/2 +5,iconRect.top() + 4),icon.pixmap(iconsize.width(),iconsize.height()));
    painter->drawPixmap(QPoint(iconRect.left()+iconsize.width()/2 +5,iconRect.top()+iconsize.height() + 7),icon2.pixmap(iconsize.width(),iconsize.height()));

    painter->setPen(Qt::yellow);
    painter->setFont(font);
    painter->drawText(headerRect,headerText.at(0));

    if(group){
        painter->setPen(Qt::white);
        font.setBold(false);
        painter->setFont(font);
        headerRect.setLeft(headerRect.right()+5);
        headerRect.setRight(subheaderRect.right());
        painter->drawText(headerRect,headerText.at(1));
    }

    painter->setPen(Qt::darkGray);
    painter->setFont(SubFont);
    painter->drawText(subheaderRect.left(),subheaderRect.top()+17,subText);

    painter->restore();

 }
