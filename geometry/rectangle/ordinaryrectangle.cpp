#include "ordinaryrectangle.h"
#include "curvedrectangle.h"
#include "../../styles/stylemanager.h"

OrdinaryRectangle::OrdinaryRectangle(float x, float y, float w, float l){
    if (w < 0) {
        x += w;
        w = -w;
    }
    if (l < 0) {
        y += l;
        l = -l;
    }

    X = x;
    Y = y;
    Width = w;
    Length = l;
}

bool OrdinaryRectangle::includesPoint(const QPointF &point) const {
    if (X <= point.x() && point.x() <= X + Width && Y <= point.y() && point.y() <= Y + Length){
        return true;
    }
    return false;
}

bool OrdinaryRectangle::collides(const Rectangle& other) const {
    return other.collidesWithOrdinaryRectangle(*this);
}

bool OrdinaryRectangle::collidesWithCurvedRectangle(const CurvedRectangle& a) const {
    return a.collidesWithOrdinaryRectangle(*this);
}

bool OrdinaryRectangle::collidesWithOrdinaryRectangle(const OrdinaryRectangle& b) const {
    return !(X + Width <= b.X || X >= b.X + b.Width || Y + Length <= b.Y || Y >= b.Y + b.Length);
    // Task Coll-Or->Or
}

bool OrdinaryRectangle::collidesWithSlantedRectangle(const SlantedRectangle& c) const {

    // Task Coll-Or->Sr

    return true;
}

QRectF OrdinaryRectangle::boundingRect() const {
    return QRectF(X, Y, Width, Length);
}

void OrdinaryRectangle::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    QString styleName;
    switch (CurrentState) {
    case 0: styleName = "StdStyleRect"; break;
    case 1: styleName = "ActivatedStyleRect"; break;
    case 2: styleName = "SelectedStyleRect"; break;
    case 3: styleName = "ErrorStyleRect"; break;
    default: styleName = "StdStyleRect"; break;
    }

    Style st = StyleManager::getStyle(styleName);

    painter->setBrush(st.brush);
    painter->setPen(st.pen);
    painter->drawRect(X, Y, Width, Length);
}

void OrdinaryRectangle::setDimensions(float width, float length){
    Width = width;
    Length = length;
}

void OrdinaryRectangle::setPosition(float x, float y){
    X = x;
    Y = y;
}

