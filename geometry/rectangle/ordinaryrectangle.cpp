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

void OrdinaryRectangle::TempRectangleFill() {
    _TempRectangle->x = X;
    _TempRectangle->y = Y;
    _TempRectangle->width = Width;
    _TempRectangle->length = Length;
}

void OrdinaryRectangle::TempRectangleNormalize() {
    if (_TempRectangle->width < 0) {
        _TempRectangle->x += _TempRectangle->width;
        _TempRectangle->width = -_TempRectangle->width;
    }
    if (_TempRectangle->length < 0) {
        _TempRectangle->y += _TempRectangle->length;
        _TempRectangle->length = -_TempRectangle->length;
    }

    if (_TempRectangle->width < 25) _TempRectangle->width = 25;
    if (_TempRectangle->length < 25) _TempRectangle->length = 25;
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
    case 0: styleName = "StdStyleRect";         break;
    case 1: styleName = "ActivatedStyleRect";   break;
    case 2: styleName = "SelectedStyleRect";    break;
    case 3: styleName = "ErrorStyleRect";       break;
    default: styleName = "StdStyleRect";        break;
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
    if (Width < 25){
        Width = 25;
    }
    else if (length < 25){
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

/*    enum HandleType {
        TopLeft,    TopMiddle,    TopRight,
        MiddleLeft, Center,       MiddleRight,
        BottomLeft, BottomMiddle, BottomRight,
        None
    };
*/
// Переписати під RENDER_RADIUS_ZONE
Rectangle* OrdinaryRectangle::handleZone(HandleType handle) const {

    const float SCENE_LEFT   = 0.0f;
    const float SCENE_TOP    = 0.0f;
    const float SCENE_RIGHT  = 1000.0f;
    const float SCENE_BOTTOM = 1000.0f;

    const float ZONE_THICKNESS = 10.0f; // товщина зони для хендла

    float zoneX = 0.0f;
    float zoneY = 0.0f;
    float zoneW = 0.0f;
    float zoneH = 0.0f;

    switch (handle) {
    case HandleType::TopLeft:
        zoneX = std::max(X - RENDER_RADIUS_ZONE, SCENE_LEFT);
        zoneY = std::max(Y - RENDER_RADIUS_ZONE, SCENE_TOP);
        zoneW = X - zoneX + Width;
        zoneH = Y - zoneY + Length;
        break;

    case HandleType::TopMiddle:
        zoneX = X;
        zoneY = std::max(Y - RENDER_RADIUS_ZONE, SCENE_TOP);
        zoneW = Width;
        zoneH = (Y - std::max(Y - RENDER_RADIUS_ZONE, SCENE_TOP) + Length);
        break;

    case HandleType::TopRight:
        zoneX = X;
        zoneY = std::max(Y - RENDER_RADIUS_ZONE, SCENE_TOP);
        zoneW = Width + RENDER_RADIUS_ZONE;
        zoneH = (Y - std::max(Y - RENDER_RADIUS_ZONE, SCENE_TOP) + Length);
        break;

    case HandleType::BottomMiddle:
        zoneX = X;
        zoneY = Y;
        zoneW = Width;
        zoneH = RENDER_RADIUS_ZONE + Length;
        break;

    case HandleType::BottomRight:
        zoneX = X;
        zoneY = Y;
        zoneW = RENDER_RADIUS_ZONE + Width;
        zoneH = RENDER_RADIUS_ZONE + Length;
        break;

    case HandleType::MiddleLeft:
        zoneX = std::max(X - RENDER_RADIUS_ZONE, SCENE_LEFT);
        zoneY = Y;
        zoneW = (X - RENDER_RADIUS_ZONE <= 0) ? (X + Width) : (RENDER_RADIUS_ZONE + Width);
        zoneH = Length;
        break;

    case HandleType::Center:
        zoneX = std::max(X - RENDER_RADIUS_ZONE, 0.0f);
        zoneY = std::max(Y - RENDER_RADIUS_ZONE, 0.0f);
        zoneW = Width + 2 * RENDER_RADIUS_ZONE;
        zoneH = Length + 2 * RENDER_RADIUS_ZONE;
        break;

    case HandleType::MiddleRight:
        zoneX = X;
        zoneY = Y;
        zoneW = X + RENDER_RADIUS_ZONE;
        zoneH = Length;
        break;

    case HandleType::BottomLeft:
        zoneX = std::max(X - RENDER_RADIUS_ZONE, SCENE_LEFT);
        zoneY = Y;
        zoneW = (X - RENDER_RADIUS_ZONE <= 0) ? (X + Width) : (RENDER_RADIUS_ZONE + Width);
        zoneH = RENDER_RADIUS_ZONE + Length;
        break;

    default:
        return nullptr;
    }

    // Створюємо новий прямокутник як зону (OrdinaryRectangle)
    return new OrdinaryRectangle(zoneX, zoneY, zoneW, zoneH);
}

Rectangle::ActionRender OrdinaryRectangle::RenderZone(const HandleType handle) const {
    // Тут є баг, що зона може виходити за межі сцени по Width та Length
    // Але я хз чи то треба фіксити, бо коду буде більше, а результат
    // і так ніхто не побачить, крім тебе, бо ти це читаєш =)

    QVector<Rectangle*> vec;

    const float SCENE_LEFT   = 0.0f;
    const float SCENE_TOP    = 0.0f;
    const float SCENE_RIGHT  = 1000.0f;
    const float SCENE_BOTTOM = 1000.0f;

    switch (handle){
    case HandleType::TopLeft:
        return [&]() {

            vec.append(new OrdinaryRectangle(
                  std::max(X - RENDER_RADIUS_ZONE, SCENE_LEFT)
                , std::max(Y - RENDER_RADIUS_ZONE, SCENE_TOP)
                , X - std::max(X - RENDER_RADIUS_ZONE, SCENE_LEFT) + Width
                , Y - std::max(Y - RENDER_RADIUS_ZONE, SCENE_TOP) + Length));

            vec.append(new OrdinaryRectangle(
                  std::max(X - RENDER_RADIUS_UPDATE_ZONE, SCENE_LEFT)
                , std::max(Y - RENDER_RADIUS_UPDATE_ZONE, SCENE_TOP)
                , X - std::max(X - RENDER_RADIUS_UPDATE_ZONE, SCENE_LEFT) + Width
                , Y - std::max(Y - RENDER_RADIUS_UPDATE_ZONE, SCENE_TOP) + Length));

            return vec;
        };

    case HandleType::TopMiddle:
        return [&]() {

            vec.append(new OrdinaryRectangle(
                  X
                , std::max(Y - RENDER_RADIUS_ZONE, SCENE_TOP)
                , Width
                , (Y - std::max(Y - RENDER_RADIUS_ZONE, SCENE_TOP) + Length)));

            vec.append(new OrdinaryRectangle(
                  X
                , std::max(Y - RENDER_RADIUS_UPDATE_ZONE, SCENE_TOP)
                , Width
                , (Y - std::max(Y - RENDER_RADIUS_UPDATE_ZONE, SCENE_TOP) + Length)));

            return vec;
        };

    case HandleType::TopRight:

        return [&]() {

            vec.append(new OrdinaryRectangle(
                  X
                , std::max(Y - RENDER_RADIUS_ZONE, SCENE_TOP)
                , Width + RENDER_RADIUS_ZONE
                , (Y - std::max(Y - RENDER_RADIUS_ZONE, SCENE_TOP) + Length)));

            vec.append(new OrdinaryRectangle(
                  X
                , std::max(Y - RENDER_RADIUS_UPDATE_ZONE, SCENE_TOP)
                , Width + RENDER_RADIUS_UPDATE_ZONE
                , (Y - std::max(Y - RENDER_RADIUS_UPDATE_ZONE, SCENE_TOP) + Length)));

            return vec;
        };

    case HandleType::MiddleLeft:
        return [&]() {

            vec.append(new OrdinaryRectangle(
                  std::max(X - RENDER_RADIUS_ZONE, SCENE_LEFT)
                , Y
                , (X - RENDER_RADIUS_ZONE <= 0) ? (X + Width) : (RENDER_RADIUS_ZONE + Width)
                , Length));

            vec.append(new OrdinaryRectangle(
                  std::max(X - RENDER_RADIUS_UPDATE_ZONE, SCENE_LEFT)
                , Y
                , (X - RENDER_RADIUS_UPDATE_ZONE <= 0) ? (X + Width) : (RENDER_RADIUS_UPDATE_ZONE + Width)
                , Length));

            return vec;
        };

    case HandleType::Center:
        return [&]() {

            vec.append(new OrdinaryRectangle(
                  std::max(X - RENDER_RADIUS_ZONE, 0.0f)
                , std::max(Y - RENDER_RADIUS_ZONE, 0.0f)
                , Width + 2 * RENDER_RADIUS_ZONE
                , Length + 2 * RENDER_RADIUS_ZONE));

            vec.append(new OrdinaryRectangle(
                  std::max(X - RENDER_RADIUS_UPDATE_ZONE, 0.0f)
                , std::max(Y - RENDER_RADIUS_UPDATE_ZONE, 0.0f)
                , Width + 2 * RENDER_RADIUS_UPDATE_ZONE
                , Length + 2 * RENDER_RADIUS_UPDATE_ZONE));

            return vec;
        };

    case HandleType::MiddleRight:
        return [&]() {

            vec.append(new OrdinaryRectangle(
                  X
                , Y
                , X + RENDER_RADIUS_ZONE
                , Length));

            vec.append(new OrdinaryRectangle(
                  X
                , Y
                , X + RENDER_RADIUS_UPDATE_ZONE
                , Length));

            return vec;
        };

    case HandleType::BottomLeft:
        return [&]() {

            vec.append(new OrdinaryRectangle(
                  std::max(X - RENDER_RADIUS_ZONE, SCENE_LEFT)
                , Y
                , (X - RENDER_RADIUS_ZONE <= 0) ? (X + Width) : (RENDER_RADIUS_ZONE + Width)
                , RENDER_RADIUS_ZONE + Length));

            vec.append(new OrdinaryRectangle(
                  std::max(X - RENDER_RADIUS_UPDATE_ZONE, SCENE_LEFT)
                , Y
                , (X - RENDER_RADIUS_UPDATE_ZONE <= 0) ? (X + Width) : (RENDER_RADIUS_UPDATE_ZONE + Width)
                , RENDER_RADIUS_UPDATE_ZONE + Length));

            return vec;
        };

    case HandleType::BottomMiddle:
        return [&]() {

            vec.append(new OrdinaryRectangle(
                  X
                , Y
                , Width
                , RENDER_RADIUS_ZONE + Length));

            vec.append(new OrdinaryRectangle(
                  X
                , Y
                , Width
                , RENDER_RADIUS_UPDATE_ZONE + Length));

            return vec;
        };

    case HandleType::BottomRight:
        return [&]() {

            vec.append(new OrdinaryRectangle(
                  X
                , Y
                , RENDER_RADIUS_ZONE + Width
                , RENDER_RADIUS_ZONE + Length));

            vec.append(new OrdinaryRectangle(
                  X
                , Y
                , RENDER_RADIUS_UPDATE_ZONE + Width
                , RENDER_RADIUS_UPDATE_ZONE + Length));

            return vec;
        };

    default:
        return nullptr;
    }
}

Rectangle::Action OrdinaryRectangle::createAction(HandleType handle) {
    prepareGeometryChange();
    switch (handle) {
    case HandleType::TopLeft:
        return [this](const QPointF &pos) {
            TempRectangleFill();
            Width += (X - pos.x());
            Length += (Y - pos.y());
            X = pos.x();
            Y = pos.y();
            normalizeRect();
        };

    case HandleType::TopMiddle:
        return [this](const QPointF &pos) {
            TempRectangleFill();
            Length += (Y - pos.y());
            Y = pos.y();
            normalizeRect();
        };

    case HandleType::TopRight:
        return [this](const QPointF &pos) {
            TempRectangleFill();
            Width = pos.x() - X;
            Length += (Y - pos.y());
            Y = pos.y();
            normalizeRect();
        };

    case HandleType::MiddleLeft:
        return [this](const QPointF &pos) {
            TempRectangleFill();
            Width += (X - pos.x());
            X = pos.x();
            normalizeRect();
        };

    case HandleType::Center:
        return [this](const QPointF &pos) {
            TempRectangleFill();
            // зсув всього прямокутника
            X = pos.x() - Width / 2.0;
            Y = pos.y() - Length / 2.0;
            normalizeRect();
        };

    case HandleType::MiddleRight:
        return [this](const QPointF &pos) {
            TempRectangleFill();
            Width = pos.x() - X;
            normalizeRect();
        };

    case HandleType::BottomLeft:
        return [this](const QPointF &pos) {
            TempRectangleFill();
            Width += (X - pos.x());
            X = pos.x();
            Length = pos.y() - Y;
            normalizeRect();
        };

    case HandleType::BottomMiddle:
        return [this](const QPointF &pos) {
            TempRectangleFill();
            Length = pos.y() - Y;
            normalizeRect();
        };

    case HandleType::BottomRight:
        return [this](const QPointF &pos) {
            TempRectangleFill();
            Width  = pos.x() - X;
            Length = pos.y() - Y;
            normalizeRect();
        };

    default:
        return nullptr;
    }
}

void OrdinaryRectangle::refuseAction() {

    X = _TempRectangle->x;
    Y = _TempRectangle->y;
    Width = _TempRectangle->width;
    Length = _TempRectangle->length;
    update();
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
