#ifndef ORDINARYRECTANGLE_H
#define ORDINARYRECTANGLE_H
#include "rectangle.h"

#include <QGraphicsItem>
#include <QPainter>

class OrdinaryRectangle: public Rectangle, public QGraphicsItem
{

    friend CurvedRectangle;
    friend SlantedRectangle;

public:
    OrdinaryRectangle(float x, float y, float w, float l);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    virtual bool collides(const Rectangle& other) const override;

    bool includesPoint(const QPointF &point) const;

    // Геттери та сеттери
    void setDimensions(float width, float length);
    void setPosition(float x, float y);

protected:

    virtual bool collidesWithCurvedRectangle(const CurvedRectangle& a) const override;
    virtual bool collidesWithOrdinaryRectangle(const OrdinaryRectangle& b) const override;
    virtual bool collidesWithSlantedRectangle(const SlantedRectangle& c) const override;

private:

    float X;
    float Y;

    float Width;
    float Length;
};

#endif // ORDINARYRECTANGLE_H
