/********************************************************************************
** Form generated from reading UI file 'charts.ui'
**
** Created: Sun 16. Dec 20:55:34 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHARTS_H
#define UI_CHARTS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_charts
{
public:

    void setupUi(QWidget *charts)
    {
        if (charts->objectName().isEmpty())
            charts->setObjectName(QString::fromUtf8("charts"));
        charts->resize(362, 316);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(charts->sizePolicy().hasHeightForWidth());
        charts->setSizePolicy(sizePolicy);

        retranslateUi(charts);

        QMetaObject::connectSlotsByName(charts);
    } // setupUi

    void retranslateUi(QWidget *charts)
    {
        charts->setWindowTitle(QApplication::translate("charts", "Form", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class charts: public Ui_charts {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHARTS_H
