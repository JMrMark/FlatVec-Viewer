#include <QtTest/QTest>
#include "../geometry/rectangle/ordinaryrectangle.h"

class TestOrdinaryRectangle : public QObject {
    Q_OBJECT

private slots:
    //void testConstruction();
    void testUpPoint();
    void testDownPoint();
    void testLeftPoint();
    void testRightPoint();
    void testInsidePoint();
};

void TestOrdinaryRectangle::testUpPoint() {
    OrdinaryRectangle r(2,2,3,2);
    QPointF pos(3, 1);
    QVERIFY(!r.includesPoint(pos));
}

void TestOrdinaryRectangle::testDownPoint() {
    OrdinaryRectangle r(2,2,3,2);
    QPointF pos(5, 1);
    QVERIFY(!r.includesPoint(pos));
}

void TestOrdinaryRectangle::testLeftPoint() {
    OrdinaryRectangle r(2,2,3,2);
    QPointF pos(1, 3);
    QVERIFY(!r.includesPoint(pos));
}

void TestOrdinaryRectangle::testRightPoint() {
    OrdinaryRectangle r(2,2,3,2);
    QPointF pos(6, 3);
    QVERIFY(!r.includesPoint(pos));
}

void TestOrdinaryRectangle::testInsidePoint() {
    OrdinaryRectangle r(2,2,3,2);
    QPointF pos(3, 3);
    QVERIFY(r.includesPoint(pos));
}

QTEST_MAIN(TestOrdinaryRectangle)
#include "test_ordinaryrectangle.moc"
