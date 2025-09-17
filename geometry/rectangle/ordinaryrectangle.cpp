#include "ordinaryrectangle.h"
#include "curvedrectangle.h"
#include "../../styles/stylemanager.h"

namespace {
    constexpr qreal kHandleRadius  = 5.0;  // твій кружечок 5x5
    constexpr qreal kHandlePadding = 2.0;  // невеличкий запас
    constexpr qreal kMaxPenWidth   = 3.0;  // найбільша товщина контуру у твоїх стилях
    inline qreal extraMargin() {
        return kHandleRadius + kHandlePadding + kMaxPenWidth * 0.5;
    }
}

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

bool OrdinaryRectangle::includesPoint(const QPointF &point, float overSize) const {
    if (X - overSize <= point.x() &&
        point.x() <= X + Width + 2 * overSize &&
        Y - overSize <= point.y() &&
        point.y() <= Y + Length + 2 * overSize)
    {
        return true;
    }
    return false;
}

bool OrdinaryRectangle::collides(const Rectangle& other, float overSize) const {
    return other.collidesWithOrdinaryRectangle(*this, overSize);
}

bool OrdinaryRectangle::collidesWithCurvedRectangle(const CurvedRectangle& a, float overSize) const {
    return a.collidesWithOrdinaryRectangle(*this, overSize);
}

bool OrdinaryRectangle::collidesWithOrdinaryRectangle(const OrdinaryRectangle& b, float overSize) const {
    return !(X + Width + overSize <= b.X || X - overSize >= b.X + b.Width || Y + Length + overSize <= b.Y || Y - overSize >= b.Y + b.Length);
    // Task Coll-Or->Or
}

bool OrdinaryRectangle::collidesWithSlantedRectangle(const SlantedRectangle& c, float overSize) const {

    // Task Coll-Or->Sr

    return true;
}

QRectF OrdinaryRectangle::boundingRect() const {
    QRectF r(X, Y, Width, Length);
    r = r.normalized();
    const qreal m = extraMargin();
    r.adjust(-m, -m, +m, +m);
    return r;
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

    if (CurrentState == 1) {
        painter->setBrush(Qt::red);
        painter->setPen(Qt::NoPen);
        for (const QPointF &h : handles()) {
            painter->drawEllipse(h, 5, 5); // маленькі червоні кружечки-ручки
        }
    }
}

void OrdinaryRectangle::setDimensions(float width, float length){
    Width = width;
    Length = length;
}

void OrdinaryRectangle::setPosition(float x, float y){
    X = x;
    Y = y;
}

void OrdinaryRectangle::setSize(float width, float length){
    prepareGeometryChange();
    if (width < 25 || length < 25){
        Width = 25;
        Length = 25;
    }
    else {
        Width = width;
        Length = length;
    }
    normalizeRect();
    update();
}

QVector<QPointF> OrdinaryRectangle::handles() const{
    QVector<QPointF> result;

    QRectF rect(X, Y, Width, Length);

    result << rect.topLeft()                                   // TopLeft
           << QPointF(rect.center().x(), rect.top())           // TopMiddle
           << rect.topRight()                                  // TopRight
           << QPointF(rect.left(), rect.center().y())          // MiddleLeft
           << rect.center()                                    // Center
           << QPointF(rect.right(), rect.center().y())         // MiddleRight
           << rect.bottomLeft()                                // BottomLeft
           << QPointF(rect.center().x(), rect.bottom())        // BottomMiddle
           << rect.bottomRight();                              // BottomRight

    return result;
}

OrdinaryRectangle::HandleType OrdinaryRectangle::hitHandle(const QPointF &point, qreal radius) const {
    QVector<QPointF> hs = handles();
    for (int i = 0; i < hs.size(); ++i) {
        if (QLineF(point, hs[i]).length() <= radius) {
            return static_cast<HandleType>(i);
        }
    }
    return HandleType::None;
}

Rectangle::Action OrdinaryRectangle::createAction(HandleType handle) {
    prepareGeometryChange();
    switch (handle) {
    case HandleType::TopLeft:
        return [this](const QPointF &pos) {
            Width += (X - pos.x());
            Length += (Y - pos.y());
            X = pos.x();
            Y = pos.y();
            normalizeRect();
            update();
        };

    case HandleType::TopMiddle:
        return [this](const QPointF &pos) {
            Length += (Y - pos.y());
            Y = pos.y();
            normalizeRect();
            update();
        };

    case HandleType::TopRight:
        return [this](const QPointF &pos) {
            Width = pos.x() - X;
            Length += (Y - pos.y());
            Y = pos.y();
            normalizeRect();
            update();
        };

    case HandleType::MiddleLeft:
        return [this](const QPointF &pos) {
            Width += (X - pos.x());
            X = pos.x();
            normalizeRect();
            update();
        };

    case HandleType::Center:
        return [this](const QPointF &pos) {
            // зсув всього прямокутника
            X = pos.x() - Width / 2.0;
            Y = pos.y() - Length / 2.0;
            normalizeRect();
            update();
        };

    case HandleType::MiddleRight:
        return [this](const QPointF &pos) {
            Width = pos.x() - X;
            normalizeRect();
            update();
        };

    case HandleType::BottomLeft:
        return [this](const QPointF &pos) {
            Width += (X - pos.x());
            X = pos.x();
            Length = pos.y() - Y;
            normalizeRect();
            update();
        };

    case HandleType::BottomMiddle:
        return [this](const QPointF &pos) {
            Length = pos.y() - Y;
            normalizeRect();
            update();
        };

    case HandleType::BottomRight:
        return [this](const QPointF &pos) {
            Width  = pos.x() - X;
            Length = pos.y() - Y;
            normalizeRect();
            update();
        };

    default:
        return nullptr;
    }
}

void OrdinaryRectangle::normalizeRect() {
    if (Width < 0) {
        X += Width;
        Width = -Width;
    }
    if (Length < 0) {
        Y += Length;
        Length = -Length;
    }

    if (Width < 25) Width = 25;
    if (Length < 25) Length = 25;
}
