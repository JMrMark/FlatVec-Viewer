#include "slantedrectangle.h"
#include "ordinaryrectangle.h"
#include "curvedrectangle.h"

SlantedRectangle::SlantedRectangle() {}

bool SlantedRectangle::includesPoint(const QPointF &point) const {

    // Task Incl-Sr

    return true;
}

bool SlantedRectangle::collides(const Rectangle& other) const {
    return other.collidesWithSlantedRectangle(*this);
}

bool SlantedRectangle::collidesWithCurvedRectangle(const CurvedRectangle& a) const {
    return a.collidesWithSlantedRectangle(*this);
}

bool SlantedRectangle::collidesWithOrdinaryRectangle(const OrdinaryRectangle& b) const {
    return b.collidesWithSlantedRectangle(*this);
}

bool SlantedRectangle::collidesWithSlantedRectangle(const SlantedRectangle& c) const {

    // Task Coll-Sr->Sr

    return true;
}

// Тимчасово!
QRectF SlantedRectangle::boundingRect() const {
    return QRectF(10, 10, 10, 10);
}
void SlantedRectangle::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    painter->setBrush(Qt::blue);
    painter->drawRect(10, 10, 10, 10);
}
// Тимчасово!
