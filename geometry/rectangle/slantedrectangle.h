#ifndef SLANTEDRECTANGLE_H
#define SLANTEDRECTANGLE_H
#include "rectangle.h"

class SlantedRectangle: public Rectangle
{

    friend CurvedRectangle;
    friend OrdinaryRectangle;

public:
    SlantedRectangle();

    float getWidth() const override {return 0;};
    float getLength() const override {return 0;};
    void setSize(float width, float length) override {};

    virtual bool collides(const Rectangle& other, float overSize = 0) const override;

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    bool includesPoint(const QPointF &point) const override;
    bool includesPoint(const QPointF &point, float overSize) const override;

    QVector<QPointF> handles() const override;
    HandleType hitHandle(const QPointF &point, qreal radius = 8.0) const override;

    ActionRender RenderZone(const HandleType handle) const override;

    Action createAction(HandleType handle) override;
    void refuseAction() override;

    bool renderZoneUpdateValidation() const override;

    void normalizeRect() override;

    void TempRectangleFill() override;
    void TempRectangleNormalize() override;

protected:

    virtual bool collidesWithCurvedRectangle(const CurvedRectangle& a, float overSize = 0) const override;
    virtual bool collidesWithOrdinaryRectangle(const OrdinaryRectangle& b, float overSize = 0) const override;
    virtual bool collidesWithSlantedRectangle(const SlantedRectangle& c, float overSize = 0) const override;
};

#endif // SLANTEDRECTANGLE_H
