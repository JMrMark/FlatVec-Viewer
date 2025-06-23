#include <QtTest/QTest>
#include "../geometry/rectangle/curvedrectangle.h"

class TestCurvedRectangle : public QObject {
    Q_OBJECT

private slots:
    void testConstruction();
    void testSomeBehavior(); // заміни на актуальні методи
};

void TestCurvedRectangle::testConstruction() {
    CurvedRectangle r;
    QVERIFY(true); // заміни на щось змістовне
}

void TestCurvedRectangle::testSomeBehavior() {
    CurvedRectangle r;
    // QVERIFY(...);
}

QTEST_MAIN(TestCurvedRectangle)
#include "test_curvedrectangle.moc"
