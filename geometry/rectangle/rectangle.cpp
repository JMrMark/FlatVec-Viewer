#include "rectangle.h"

Rectangle::Rectangle() {}

void Rectangle::TempRectangleCreate(){
    if (!_TempRectangle)
        _TempRectangle = new TempRectangle();
}

void Rectangle::TempRectangleDelete(){
    if (_TempRectangle) {
        _TempRectangle->clear();
        delete _TempRectangle;
        _TempRectangle = nullptr;
    }
}
