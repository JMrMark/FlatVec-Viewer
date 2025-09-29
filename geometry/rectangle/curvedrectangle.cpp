#include "curvedrectangle.h"

CurvedRectangle::CurvedRectangle() {}

bool CurvedRectangle::collides(const Rectangle& other, float overSize) const {
    return other.collidesWithCurvedRectangle(*this, overSize);
}

bool CurvedRectangle::includesPoint(const QPointF &point) const {

    // Task Incl-Cr

    return true;
}

bool CurvedRectangle::includesPoint(const QPointF &point, float overSize) const {

    // Task Incl-Cr
    // Збільшення прямокутника на overSize для взаємодії з червоними точками

    return true;
}

QVector<QPointF> CurvedRectangle::handles() const{
    // None for now
    QVector<QPointF> p;
    return p;
}

Rectangle::HandleType CurvedRectangle::hitHandle(const QPointF &point, qreal radius) const{
    return HandleType::None;
}

Rectangle* CurvedRectangle::handleZone(HandleType handle) const {
    return nullptr;
}

Rectangle::ActionRender CurvedRectangle::RenderZone(const HandleType handle) const {
    return [this]() {
        qDebug() << "Nothing";
        QVector<Rectangle*> vec;
        return vec;
    };
}

Rectangle::Action CurvedRectangle::createAction(HandleType handle){
    return [this](const QPointF &pos) {
        qDebug() << "Nothing";
    };
}

void CurvedRectangle::refuseAction() {
    // Just put TR data to its own data
}

void CurvedRectangle::normalizeRect() {
    // Mya
}

bool CurvedRectangle::collidesWithCurvedRectangle(const CurvedRectangle& a, float overSize) const {

    // Task Coll-Cr->Cr

    return true;
}

bool CurvedRectangle::collidesWithOrdinaryRectangle(const OrdinaryRectangle& b, float overSize) const {

    // Task Coll-Cr->Or

    return true;
}

bool CurvedRectangle::collidesWithSlantedRectangle(const SlantedRectangle& c, float overSize) const {

    // Task Coll-Cr->Sr

    return true;
}

void CurvedRectangle::TempRectangleFill() {
    // Just put your data into TR data
}

void CurvedRectangle::TempRectangleNormalize() {
    // Just normalize TR data
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
