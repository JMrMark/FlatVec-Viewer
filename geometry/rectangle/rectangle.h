/*
 * Клас є абстрактним
 * Він наслідується в усі похідні прямокутники,
 * Використовується для подвійної диспетчеризації (double dispatch)
*/

#ifndef RECTANGLE_H
#define RECTANGLE_H

#pragma once
//#include "curvedrectangle.h"
//#include "ordinaryrectangle.h"
//#include "slantedrectangle.h"

#include <QString>
#include <QPointF>

#include <QGraphicsItem>
#include <QPainter>

class CurvedRectangle;
class OrdinaryRectangle;
class SlantedRectangle;

class Rectangle : public QGraphicsItem
{
public:
    using QGraphicsItem::QGraphicsItem;
    Rectangle();

    virtual bool includesPoint(const QPointF &point) const = 0;

    virtual bool collides(const Rectangle& other) const = 0;

    // Змінна стилю
    inline void setActivatedOff()   {CurrentState = 0; update();}
    inline void setActivatedOn()    {CurrentState = 1; update();}
    inline void setSelectedOn()     {CurrentState = 2; update();}

    // Змінна розмірів
    //virtual bool setSize(float w, ...) const = 0;


    // Взаємодія прямокутників
    virtual bool collidesWithCurvedRectangle(const CurvedRectangle& a) const = 0;
    virtual bool collidesWithOrdinaryRectangle(const OrdinaryRectangle& b) const = 0;
    virtual bool collidesWithSlantedRectangle(const SlantedRectangle& c) const = 0;

    virtual ~Rectangle() = default;

    // Стан потрібен для підвантаження стилю
    // 0 - StdStyleRect
    // 1 - ActivatedStyleRect
    // 2 - SelectedStyleRect
    // 3 - ErrorStyleRect
    int CurrentState = 0;


};

#endif // RECTANGLE_H
