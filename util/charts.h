/*
 * Modified version of NightCharts: https://code.google.com/p/shopping-lists-organizer/source/browse/nightcharts.h
 */
#ifndef CHARTS_H
#define CHARTS_H
#define PerConst = 3.6;
#include <QObject>
#include <QString>
#include <QPainter>
#include <QVector>
#include <QDebug>
#include <QLinearGradient>
#include <math.h>

class pieceChart
{
public:
    explicit pieceChart();
    void addName(QString name);
    void setColor(Qt::GlobalColor);
    void setColor(QColor color);
    void setPerc(float Percentage);

    QString pname;
    QColor rgbColor;
    float pPerc;

private:

};

class Charts
{
public:

    explicit Charts();
    ~Charts();
    enum type { Histogramm , Pie, Graphic };
    int addPiece(QString name,Qt::GlobalColor,float Percentage);
    int addPiece(QString name,QColor, float Percentage);
    int setCords(double x, double y, double w, double h);
    int setLegendCords(double x, double y);
    int setTitleCords(double x, double y);
    int setTitle(QString text);
    int setType(Charts::type t);
    int setShadows(bool ok = true);
    int setFont(QFont f);
    int draw(QPainter *painter);
    double palpha;

private:
    double cX,cY,cW,cH,pW,lX,lY,tX,tY;
    int nPiece;
    bool shadows;
    QVector<pieceChart> pieces;
    int ctype;
    QFont font;
    //QPainter *cpainter;
    QPointF GetPoint(double angle, double R1 = 0, double R2 = 0);
    int GetQuater(double angle);
    double Angle360(double angle);
    QString title;

signals:

public slots:

};


#endif // CHARTS_H
