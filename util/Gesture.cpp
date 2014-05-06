/*
 * Modified version of Nokia Corporation Wiki.
 */
#include "Gesture.h"

Gesture::Gesture(QObject *parent) :
    QObject(parent)
{
}

Gesture::~Gesture()
{
}

bool Gesture::isDraggingLeft(QList<QPoint>& points, int total)
{
    QPoint start = points.first();
    QPoint end = points.last();
    if (points.count() < total)
        return false;

    if (start.x() > end.x())
        return true;
    else
        return false;
}

bool Gesture::isDraggingRight(QList<QPoint>& points, int total)
{
    QPoint start = points.first();
    QPoint end = points.last();
    if (points.count() < total)
        return false;

    if (start.x() < end.x())
        return true;
    else
        return false;
    return false;
}

bool Gesture::isDraggingTop(QList<QPoint> &points, int total)
{
    QPoint start = points.first();
    QPoint end = points.last();
    if (points.count() < total)
        return false;

    if (start.y() < end.y())
        return true;
    else
        return false;
}

bool Gesture::isDraggingBottom(QList<QPoint> &points, int total)
{
    QPoint start = points.first();
    QPoint end = points.last();
    if (points.count() < total)
        return false;

    if (start.y() > end.y())
        return true;
    else
        return false;
}
