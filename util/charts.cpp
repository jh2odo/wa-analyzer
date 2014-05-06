/*
 * Modified version of NightCharts: https://code.google.com/p/shopping-lists-organizer/source/browse/nightcharts.cpp
 */
#include "charts.h"
#include <qgraphicsscene>

Charts::Charts()//QPainter *painter)

{
    font.setFamily("verdana");
    font.setPointSize(7);
    //painter = painter;
    //painter->setFont(font);
    ctype = Charts::Pie;
    cX = 0;
    cY = 0;
    cW = 100;
    cH = 100;
    lX = cX+cW+20;
    lY = cY;
    shadows = true;
}
Charts::~Charts()
{
    pieces.clear();
}

int Charts::addPiece(QString name,Qt::GlobalColor color,float Percentage)
{
    this->nPiece++;

    pieceChart piece;
    piece.addName(name);
    piece.setColor(color);
    piece.setPerc(Percentage);
    pieces.append(piece);

    return 0;
}
int Charts::addPiece(QString name, QColor color, float Percentage)
{
    this->nPiece++;
    pieceChart piece;
    piece.addName(name);
    piece.setColor(color);
    piece.setPerc(Percentage);
    pieces.append(piece);

    return 0;
}
int Charts::setCords(double x, double y, double w, double h)
{
    this->cX = x;
    this->cY = y;
    this->cW = w;
    this->cH = h;
    this->lX = cX+cW+20;
    this->lY = cY;

    return 0;
}
int Charts::setLegendCords(double x, double y)
{
    this->lX = x;
    this->lY = y;

    return 0;
}

int Charts::setTitleCords(double x, double y){
    this->tX = x;
    this->tY = y;

    return 0;
}

int Charts::setTitle(QString text)
{
    this->title = text;

    return 0;
}

int Charts::setType(Charts::type t)
{
    this->ctype = t;

    return 0;
}

int Charts::setFont(QFont f)
{
    this->font = f;

    return 0;
}

int Charts::setShadows(bool ok)
{
    this->shadows = ok;

    return 0;
}

int Charts::draw(QPainter *painter)
{
    painter->setRenderHint(QPainter::Antialiasing);

    // Draw title
    painter->setPen(Qt::SolidLine);
    painter->setPen(Qt::yellow);
    painter->drawText(tX,tY + 10,title);

    painter->setPen(Qt::NoPen);


    if (this->ctype==Charts::Pie)
    {
      pW = 0;
      double pdegree = 0;

      //Options
      QLinearGradient gradient(cX+0.5*cW,cY,cX+0.5*cW,cY+cH*2.5);
      gradient.setColorAt(1,Qt::white);

      //Draw
      //pdegree = (360/100)*pieces[i].pPerc;
      if (shadows)
      {
          double sumangle = 0;
          for (int i=0;i<pieces.size();i++)
          {
              sumangle += 3.6*pieces[i].pPerc;
          }
          painter->setBrush(Qt::darkGray);
          painter->drawPie(cX,cY+pW+5,cW,cH,palpha*16,sumangle*16);
      }

      QPen pen;
      pen.setWidth(2);

      for (int i=0;i<pieces.size();i++)
      {
        gradient.setColorAt(0,pieces[i].rgbColor);
        painter->setBrush(gradient);
        pen.setColor(pieces[i].rgbColor);
        painter->setPen(pen);
        pdegree = 3.6*pieces[i].pPerc;
        painter->drawPie(cX,cY,cW,cH,palpha*16,pdegree*16);
        palpha += pdegree;
      }
    }
    else if (this->ctype==Charts::Histogramm)
    {
        double pDist = 15;
        double pW = (cW-(pieces.size())*pDist)/pieces.size();

        QLinearGradient gradient(cX+cW/2,cY,cX+cW/2,cY+cH);
        gradient.setColorAt(0,Qt::black);
        QPen pen;
        pen.setWidth(3);

        for (int i=0;i<pieces.size();i++)
        {
            if (shadows)
            {
                painter->setPen(Qt::NoPen);
                painter->setBrush(Qt::darkGray);
                painter->drawRect(cX+pDist+i*(pW + pDist)-pDist/2,cY+cH-1,pW,-cH/100*pieces[i].pPerc+pDist/2-5);
            }
            gradient.setColorAt(1,pieces[i].rgbColor);
            painter->setBrush(gradient);
            pen.setColor(pieces[i].rgbColor);
            painter->setPen(pen);
            painter->drawRect(cX+pDist+i*(pW + pDist),cY+cH,pW,-cH/100*pieces[i].pPerc-5);
            QString label = QString::number(pieces[i].pPerc)+"%";
            painter->setPen(Qt::SolidLine);
            painter->drawText(cX+pDist+i*(pW + pDist)+pW/2-painter->fontMetrics().width(label)/2,cY+cH-cH/100*pieces[i].pPerc-painter->fontMetrics().height()/2,label);
        }

        painter->setPen(Qt::SolidLine);
        painter->setBrush(Qt::white);
        pen.setColor(Qt::white);
        painter->setPen(pen);
        for (int i=1;i<10;i++)
        {
            painter->drawLine(cX-3,cY+cH/10*i,cX+3,cY+cH/10*i);
            if(i%2 == 0){
                painter->drawText(cX-30,(cY+cH/10*i)+7,QString::number((10-i)*10));
            }
        }
        painter->drawLine(cX-3,cY,cX+3,cY);
        painter->drawText(cX-40,cY+7,"100");
        //painter->drawText(cX-5, (cY+cH/100) - 10,"%");

        painter->drawLine(cX,cY+cH,cX,cY);
        //painter->drawLine(cX,cY,cX+4,cY+10);  // Flecha
        //painter->drawLine(cX,cY,cX-4,cY+10); // Flecha
        painter->drawLine(cX,cY+cH,cX+cW,cY+cH);

    }else if(this->ctype==Charts::Graphic)
    {
        QPen pen;
        pen.setWidth(3);
        painter->setPen(Qt::SolidLine);
        pen.setColor(Qt::white);
        painter->setPen(pen);
        for (int i=1;i<10;i++)
        {
            painter->drawLine(cX-3,cY+cH/10*i,cX+3,cY+cH/10*i);
           // if(i%2 == 0){
           //     painter->drawText(cX-30,(cY+cH/10*i)+7,QString::number((10-i)*10));
           // }
        }
        painter->drawLine(cX-3,cY,cX+3,cY);
        //painter->drawText(cX-40,cY+7,"100");
        //painter->drawText(cX-5, (cY+cH/100) - 10,"%");

        painter->drawLine(cX,cY+cH,cX,cY);
        //painter->drawLine(cX,cY,cX+4,cY+10);  // Flecha
        //painter->drawLine(cX,cY,cX-4,cY+10); // Flecha


        for (int i=1;i<10;i++)
        {
            painter->drawLine(cX+cW/10*i,cY+cH-3,cX+cW/10*i,cY+cH+3);
           // if(i%2 == 0){
           //     painter->drawText(cX-30,(cY+cH/10*i)+7,QString::number((10-i)*10));
           // }
        }
        painter->drawLine(cX+cW,cY+cH-3,cX+cW,cY+cH+3);

        painter->drawLine(cX,cY+cH,cX+cW,cY+cH);
    }

    // Draw Legend
    painter->setPen(Qt::SolidLine);
    painter->setPen(Qt::white);

    int dist = 5;
    //painter->drawRoundRect(cX+cW+20,cY,dist*2+200,pieces.size()*(painter->fontMetrics().height()+2*dist)+dist,15,15);
    for (int i=pieces.size()-1;i>=0;i--)
    {
        painter->setBrush(pieces[i].rgbColor);
        float x = lX+dist;
        float y = lY+dist+i*(painter->fontMetrics().height()+2*dist);
        painter->drawRect(x,y,painter->fontMetrics().height(),painter->fontMetrics().height());
        painter->drawText(x+painter->fontMetrics().height()+dist,y+painter->fontMetrics().height()/2+dist + 3,pieces[i].pname + " - " + QString::number(pieces[i].pPerc)+"%");
    }

    return 0;
}

QPointF Charts::GetPoint(double angle, double R1, double R2)
{
    if (R1 == 0 && R2 == 0)
    {
        R1 = cW;
        R2 = cH;
    }
    QPointF point;
    double x = R1/2*cos(angle*M_PI/180);
    x+=cW/2+cX;
    double y = -R2/2*sin(angle*M_PI/180);
    y+=cH/2+cY;
    point.setX(x);
    point.setY(y);
    return point;
}

int Charts::GetQuater(double angle)
{
    angle = Angle360(angle);

    if(angle>=0 && angle<90)
        return 1;
    if(angle>=90 && angle<180)
        return 2;
    if(angle>=180 && angle<270)
        return 3;
    if(angle>=270 && angle<360)
        return 4;

    return 0;
}

double Charts::Angle360(double angle)
{
    int i = (int)angle;
    double delta = angle - i;
    return (i%360 + delta);
}

pieceChart::pieceChart()
{
}
void pieceChart::addName(QString name)
{
    pname = name;
}
void pieceChart::setColor(Qt::GlobalColor color)
{
    rgbColor = color;
}
void pieceChart::setColor(QColor color)
{
    rgbColor = color;
}

void pieceChart::setPerc(float Percentage)
{
    pPerc = Percentage;
}
