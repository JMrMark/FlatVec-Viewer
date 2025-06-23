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
