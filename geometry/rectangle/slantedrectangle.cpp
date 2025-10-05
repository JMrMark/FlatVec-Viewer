#include "slantedrectangle.h"
#include "ordinaryrectangle.h"
#include "curvedrectangle.h"

SlantedRectangle::SlantedRectangle() {}

bool SlantedRectangle::includesPoint(const QPointF &point) const {

    // Task Incl-Sr

    return true;
}

bool SlantedRectangle::includesPoint(const QPointF &point, float overSize) const {

    // Task Incl-Sr
    // Збільшення прямокутника на overSize для взаємодії з червоними точками

    return true;
}

void SlantedRectangle::TempRectangleFill() {
    // Just put your data into TR data
}

void SlantedRectangle::TempRectangleNormalize() {
    // Just normalize TR data
}

QVector<QPointF> SlantedRectangle::handles() const{
    // None for now
    QVector<QPointF> p;
    return p;
}

Rectangle::HandleType SlantedRectangle::hitHandle(const QPointF &point, qreal radius) const{
    return HandleType::None;
}

Rectangle::ActionRender SlantedRectangle::RenderZone(const HandleType handle) const {
    return [this]() {
        qDebug() << "Nothing";
        QVector<Rectangle*> vec;
        return vec;
    };
}

Rectangle::Action SlantedRectangle::createAction(HandleType handle){
    return [this](const QPointF &pos) {
        qDebug() << "Nothing";
    };
}

void SlantedRectangle::refuseAction() {
    // Just put TR data to its own data
}

bool SlantedRectangle::renderZoneUpdateValidation() const {

    // перевірка чи прямокутник не вийшов за межі RenderUpdateZone

    return false;
}

void SlantedRectangle::normalizeRect() {
    // Bark
}

bool SlantedRectangle::collides(const Rectangle& other, float overSize) const {
    return other.collidesWithSlantedRectangle(*this, overSize);
}

bool SlantedRectangle::collidesWithCurvedRectangle(const CurvedRectangle& a, float overSize) const {
    return a.collidesWithSlantedRectangle(*this, overSize);
}

bool SlantedRectangle::collidesWithOrdinaryRectangle(const OrdinaryRectangle& b, float overSize) const {
    return b.collidesWithSlantedRectangle(*this, overSize);
}

bool SlantedRectangle::collidesWithSlantedRectangle(const SlantedRectangle& c, float overSize) const {

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
