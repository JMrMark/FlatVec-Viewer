#include <QtTest/QTest>
#include "../geometry/rectangle/slantedrectangle.h"

class TestSlantedRectangle : public QObject {
    Q_OBJECT

private slots:
    void testConstruction();
    void testSomeBehavior(); // заміни на актуальні методи
};

void TestSlantedRectangle::testConstruction() {
    SlantedRectangle r;
    QVERIFY(true); // заміни на щось змістовне
}

void TestSlantedRectangle::testSomeBehavior() {
    //SlantedRectangle r;
    // QVERIFY(...);
}

QTEST_MAIN(TestSlantedRectangle)
#include "test_slantedrectangle.moc"
