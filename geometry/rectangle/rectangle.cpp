#include "rectangle.h"

Rectangle::Rectangle() {}

Rectangle::~Rectangle() {
    if (_TempRectangle != nullptr) {
        delete _TempRectangle;
    }
}

void Rectangle::TempRectangleCreate(){
    if (!_TempRectangle)
        _TempRectangle = new TempRectangle();
}

bool Rectangle::RenderZoneExist(){
    if (_TempRectangle == nullptr)
        return false;

    return !_TempRectangle->RenderZoneIsEmpty();
}

void Rectangle::RenderZoneUpdate(const QVector<Rectangle*> &vrz) {
    if (_TempRectangle == nullptr)
        return ;

    _TempRectangle->RenderZone_update(vrz);
}

Rectangle* Rectangle::RenderZoneGet() {
    if (_TempRectangle == nullptr)
        return nullptr;

    return _TempRectangle->getRenderZone();
}

void Rectangle::TempRectangleDelete(){
    if (_TempRectangle) {
        delete _TempRectangle;
        _TempRectangle = nullptr;
    }
}
