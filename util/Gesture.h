/*
 * Modified version of Nokia Corporation Wiki.
 */
#ifndef GESTURE_H
#define GESTURE_H

// INCLUDES
#include <QObject>
#include <QList>
#include <QPoint>

// FORWARD DECLARATIONS

// CLASS DECLARATION
class Gesture: public QObject
{
Q_OBJECT

public:
    Gesture(QObject *parent = 0);
    ~Gesture();

public:
    bool isDraggingLeft(QList<QPoint>& points, int total = 10);
    bool isDraggingRight(QList<QPoint>& points, int total = 10);
    bool isDraggingTop(QList<QPoint>& points, int total = 10);
    bool isDraggingBottom(QList<QPoint>& points, int total = 10);
private:
    // Data

};

#endif //GESTURE_H
