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

class CurvedRectangle;
class OrdinaryRectangle;
class SlantedRectangle;

class Rectangle
{
public:
    Rectangle();

    virtual bool collides(const Rectangle& other) const = 0;

    virtual bool collidesWithCurvedRectangle(const CurvedRectangle& a) const = 0;
    virtual bool collidesWithOrdinaryRectangle(const OrdinaryRectangle& b) const = 0;
    virtual bool collidesWithSlantedRectangle(const SlantedRectangle& c) const = 0;
    virtual ~Rectangle() = default;


};

#endif // RECTANGLE_H
