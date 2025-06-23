#include <QtTest/QTest>
#include "../geometry/rectangle/ordinaryrectangle.h"

class TestOrdinaryRectangle : public QObject {
    Q_OBJECT

private slots:
    void testConstruction();
    void testSomeBehavior(); // заміни на актуальні методи
};

void TestOrdinaryRectangle::testConstruction() {
    OrdinaryRectangle r(1,1,1,1);
    QVERIFY(true); // заміни на щось змістовне
}

void TestOrdinaryRectangle::testSomeBehavior() {
    //OrdinaryRectangle r;
    // QVERIFY(...);
}

QTEST_MAIN(TestOrdinaryRectangle)
#include "test_ordinaryrectangle.moc"
