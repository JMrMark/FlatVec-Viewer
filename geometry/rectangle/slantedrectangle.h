#ifndef SLANTEDRECTANGLE_H
#define SLANTEDRECTANGLE_H
#include "rectangle.h"

class SlantedRectangle: public Rectangle
{

    friend CurvedRectangle;
    friend OrdinaryRectangle;

public:
    SlantedRectangle();

    virtual bool collides(const Rectangle& other) const override;

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    bool includesPoint(const QPointF &point) const override;


protected:

    virtual bool collidesWithCurvedRectangle(const CurvedRectangle& a) const override;
    virtual bool collidesWithOrdinaryRectangle(const OrdinaryRectangle& b) const override;
    virtual bool collidesWithSlantedRectangle(const SlantedRectangle& c) const override;
};

#endif // SLANTEDRECTANGLE_H
