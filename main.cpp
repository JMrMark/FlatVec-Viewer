#include "mainwindow.h"

#include <QApplication>
#include <QSettings>
#include <QFile>
#include <QTextStream>
#include <QStyleHints>

static QString loadStyle(const QString &resPath) {
    QFile f(resPath);
    if (!f.open(QFile::ReadOnly)) return {};
    return QString::fromUtf8(f.readAll());
}

// Qt6.5+: визначаємо системну тему
static bool isSystemDark() {
#if QT_VERSION >= QT_VERSION_CHECK(6,5,0)
    return qApp->styleHints()->colorScheme() == Qt::ColorScheme::Dark;
#else
    // Фолбек для старших версій
    return false;
#endif
}

static void applyTheme(const QString &themeName) {
    QString qss;
    if (themeName == "dark") {
        qss = loadStyle(":/styles/darkTheme.qss");
    } else if (themeName == "light") {
        qss = loadStyle(":/styles/lightTheme.qss");
    } else { // "system"
        qss = isSystemDark() ? loadStyle(":/styles/darkTheme.qss")
                             : loadStyle(":/styles/lightTheme.qss");
    }
    qApp->setStyleSheet(qss);

    // зберігаємо вибір користувача (dark/light/system)
    QSettings().setValue("theme", themeName);
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("JMRCorp");
    QCoreApplication::setOrganizationDomain("JMRCorp.com");
    QCoreApplication::setApplicationName("FlatVecViewer");

    const QString saved = QSettings().value("theme", "system").toString();

    applyTheme(saved);

    MainWindow w;
    w.show();
    return a.exec();
}
