#ifndef CURVEDRECTANGLE_H
#define CURVEDRECTANGLE_H
#include "rectangle.h"

class CurvedRectangle: public Rectangle
{
    friend OrdinaryRectangle;
    friend SlantedRectangle;

public:
    CurvedRectangle();

    virtual bool collides(const Rectangle& other) const override;

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    bool includesPoint(const QPointF &point) const override;

protected:

    virtual bool collidesWithCurvedRectangle(const CurvedRectangle& a) const override;
    virtual bool collidesWithOrdinaryRectangle(const OrdinaryRectangle& b) const override;
    virtual bool collidesWithSlantedRectangle(const SlantedRectangle& c) const override;

};

#endif // CURVEDRECTANGLE_H
