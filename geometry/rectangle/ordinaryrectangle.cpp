#include "ordinaryrectangle.h"
#include "curvedrectangle.h"

OrdinaryRectangle::OrdinaryRectangle(float x, float y, float w, float l){
    X = x;
    Y = y;
    Width = w;
    Length = l;
}

bool OrdinaryRectangle::includesPoint(const QPointF &point) const {

    // Task Incl-Or

    return true;
}

bool OrdinaryRectangle::collides(const Rectangle& other) const {
    return other.collidesWithOrdinaryRectangle(*this);
}

bool OrdinaryRectangle::collidesWithCurvedRectangle(const CurvedRectangle& a) const {
    return a.collidesWithOrdinaryRectangle(*this);
}

bool OrdinaryRectangle::collidesWithOrdinaryRectangle(const OrdinaryRectangle& b) const {

    // Task Coll-Or->Or

    return true;
}

bool OrdinaryRectangle::collidesWithSlantedRectangle(const SlantedRectangle& c) const {

    // Task Coll-Or->Sr

    return true;
}
