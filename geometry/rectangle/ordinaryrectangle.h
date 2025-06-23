#ifndef ORDINARYRECTANGLE_H
#define ORDINARYRECTANGLE_H
#include "rectangle.h"

class OrdinaryRectangle: public Rectangle
{

    friend CurvedRectangle;
    friend SlantedRectangle;

public:
    OrdinaryRectangle(float x, float y, float w, float l);

    virtual bool collides(const Rectangle& other) const override;

    bool includesPoint(const QPointF &point) const;

protected:

    virtual bool collidesWithCurvedRectangle(const CurvedRectangle& a) const override;
    virtual bool collidesWithOrdinaryRectangle(const OrdinaryRectangle& b) const override;
    virtual bool collidesWithSlantedRectangle(const SlantedRectangle& c) const override;

private:

    double X;
    double Y;

    double Width;
    double Length;
};

#endif // ORDINARYRECTANGLE_H
