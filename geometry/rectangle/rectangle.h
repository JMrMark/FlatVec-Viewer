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

    // ---= Взаємодія прямокутника з точкою =--- //
    virtual bool includesPoint(const QPointF &point) const = 0;
    virtual bool includesPoint(const QPointF &point, float overSize) const = 0;

    // ---= Стан прямокутника =--- //
    inline void setActivatedOff()   {CurrentState = 0; update();}
    inline void setActivatedOn()    {CurrentState = 1; update();}
    inline void setSelectedOn()     {CurrentState = 2; update();}
    inline void setErrorOn()        {CurrentState = 3; update();}

    // Стан потрібен для підвантаження стилю
    // 0 - StdStyleRect
    // 1 - ActivatedStyleRect
    // 2 - SelectedStyleRect
    // 3 - ErrorStyleRect
    unsigned short int CurrentState = 0;

    // ---= Модифікація прямокутника =--- //
    enum HandleType {
        TopLeft,    TopMiddle,    TopRight,
        MiddleLeft, Center,       MiddleRight,
        BottomLeft, BottomMiddle, BottomRight,
        None
    };

    virtual QVector<QPointF> handles() const = 0;
    virtual HandleType hitHandle(const QPointF &point, qreal radius = 8.0) const = 0;

    using Action = std::function<void(const QPointF &mousePos)>;
    virtual Action createAction(HandleType handle) = 0;

    virtual void normalizeRect() = 0;

    // ---= Взаємодія прямокутників =--- //
    virtual bool collides(const Rectangle& other) const = 0;

    virtual bool collidesWithCurvedRectangle(const CurvedRectangle& a) const = 0;
    virtual bool collidesWithOrdinaryRectangle(const OrdinaryRectangle& b) const = 0;
    virtual bool collidesWithSlantedRectangle(const SlantedRectangle& c) const = 0;

    virtual ~Rectangle() = default;

};

#endif // RECTANGLE_H
