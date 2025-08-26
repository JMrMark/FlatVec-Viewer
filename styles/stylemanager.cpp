#include "stylemanager.h"
#include <QFile>
#include <QTextStream>
#include <QRegularExpression>


QMap<QString, Style> StyleManager::styles = {
    { "StdStyleRect",      { QBrush(QColor("#a9a9a9")), QPen(Qt::black, 1) } },
    { "ActivatedStyleRect",{ QBrush(QColor("#a9a9a9")), QPen(Qt::black, 1, Qt::DashLine) } },
    { "SelectedStyleRect", { QBrush(QColor("#90c8c8")), QPen(Qt::black, 1) } },
    { "ErrorStyleRect",    { QBrush(QColor("#fb4444")), QPen(Qt::black, 1) } }
};

void StyleManager::loadStyles(const QString& qssPath) {
    QFile file(qssPath);
    if (!file.open(QFile::ReadOnly | QFile::Text)) return;

    QString qss = QTextStream(&file).readAll();

    // --- Знаходимо селектори через RegExp ---
    QRegularExpression blockRegex(R"(#(\w+)\s*\{([^}]*)\})");
    auto it = blockRegex.globalMatch(qss);

    while (it.hasNext()) {
        auto match = it.next();
        QString name = match.captured(1);      // наприклад: StdStyleRect
        QString body = match.captured(2);      // вміст {}

        QColor bgColor = Qt::transparent;
        QPen pen(Qt::black);

        // --- Парсимо background-color ---
        QRegularExpression bgRegex(R"(background-color\s*:\s*([^;]+);)");
        auto bgMatch = bgRegex.match(body);
        if (bgMatch.hasMatch()) {
            bgColor = QColor(bgMatch.captured(1).trimmed());
        }

        // --- Парсимо border (1px solid black) ---
        QRegularExpression borderRegex(R"(border\s*:\s*(\d+)px\s+(\w+)\s+([^;]+);)");
        auto borderMatch = borderRegex.match(body);
        if (borderMatch.hasMatch()) {
            int width = borderMatch.captured(1).toInt();
            QString style = borderMatch.captured(2).toLower();
            QColor color(borderMatch.captured(3).trimmed());

            pen.setWidth(width);
            pen.setColor(color);

            if (style == "solid") pen.setStyle(Qt::SolidLine);
            else if (style == "dash") pen.setStyle(Qt::DashLine);
            else if (style == "dot") pen.setStyle(Qt::DotLine);
            else pen.setStyle(Qt::SolidLine);
        }

        Style st;
        st.brush = QBrush(bgColor);
        st.pen = pen;
        styles.insert(name, st);
    }
}

Style StyleManager::getStyle(const QString& name) {
    if (styles.contains(name)) {
        return styles[name];
    }
    return { QColor(Qt::gray), QPen(Qt::black, 1) };
}
