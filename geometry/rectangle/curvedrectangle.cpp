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

