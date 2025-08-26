#include "curvedrectangle.h"

CurvedRectangle::CurvedRectangle() {}

bool CurvedRectangle::collides(const Rectangle& other) const {
    return other.collidesWithCurvedRectangle(*this);
}

bool CurvedRectangle::includesPoint(const QPointF &point) const {

    // Task Incl-Cr

    return true;
}

bool CurvedRectangle::collidesWithCurvedRectangle(const CurvedRectangle& a) const {

    // Task Coll-Cr->Cr

    return true;
}

bool CurvedRectangle::collidesWithOrdinaryRectangle(const OrdinaryRectangle& b) const {

    // Task Coll-Cr->Or

    return true;
}

bool CurvedRectangle::collidesWithSlantedRectangle(const SlantedRectangle& c) const {

    // Task Coll-Cr->Sr

    return true;
}


// Тимчасово!
QRectF CurvedRectangle::boundingRect() const {
    return QRectF(10, 10, 10, 10);
}
void CurvedRectangle::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    painter->setBrush(Qt::blue);
    painter->drawRect(10, 10, 10, 10);
}
// Тимчасово!
