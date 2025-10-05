#include "temprectangle.h"
#include <QDebug>

TempRectangle::TempRectangle() {
    x = y = width = length = 0;
    _renderZones.shrink_to_fit();
    _renderZones.reserve(2);
}

bool TempRectangle::RenderZone_update(const QVector<Rectangle*> &renderZones){

    deleteRenderZone();

    if (renderZones.empty()) {
        return false;
    }

    _renderZones.clear();
    _renderZones.reserve(renderZones.size());

    for (auto *r : renderZones) {
        _renderZones.push_back(r);
    }

    return true;
}

Rectangle* TempRectangle::getRenderZone(){
    return _renderZones.at(0);
}

Rectangle* TempRectangle::getRenderZoneUpdate(){
    return _renderZones.at(1);
}

void TempRectangle::deleteRenderZone(){
    for(auto *el : _renderZones){
        delete el;
    }
}

TempRectangle::~TempRectangle() {
    for(auto *el : _renderZones){
        delete el;
    }
}
