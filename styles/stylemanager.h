#ifndef STYLEMANAGER_H
#define STYLEMANAGER_H

#pragma once
#include <QString>
#include <QMap>
#include <QColor>
#include <QPen>
#include <QBrush>

struct Style {
    QBrush brush;
    QPen pen;
};

class StyleManager {
public:
    static void loadStyles(const QString& qssPath);
    static Style getStyle(const QString& objectName);

private:
    static QMap<QString, Style> styles;
};



#endif // STYLEMANAGER_H
