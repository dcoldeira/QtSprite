#include "Coin.h"

#include <QParallelAnimationGroup>
#include <QPen>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>

Coin::Coin(QGraphicsItem *parent) :
    QGraphicsEllipseItem(parent)
  , m_explosion(false)
{
    setPen(QPen(QColor(0, 3, 16), 1));
    setBrush(QColor(12, 73, 31));
    setRect(-10, -10, 15, 15);
}

int Coin::type() const
{
    return Type;
}

void Coin::explode()
{
    if (m_explosion) {
        return;
    }

    m_explosion = true;
    QParallelAnimationGroup *group = new QParallelAnimationGroup(this);
    //QSequentialAnimationGroup *group = new QSequentialAnimationGroup(this);

    QPropertyAnimation *scaleAnimation = new QPropertyAnimation(this, "rect");
    scaleAnimation->setDuration(700);
    QRectF r = rect();
    scaleAnimation->setStartValue(r);
    scaleAnimation->setEndValue(QRectF(r.topLeft() - r.bottomRight(), r.size() * 2));
    scaleAnimation->setEasingCurve(QEasingCurve::OutQuart);
    group->addAnimation(scaleAnimation);

    QPropertyAnimation *fadeAnimation = new QPropertyAnimation(this, "opacity");
    fadeAnimation->setDuration(700);
    fadeAnimation->setStartValue(1);
    fadeAnimation->setEndValue(0);
    fadeAnimation->setEasingCurve(QEasingCurve::OutQuart);
    group->addAnimation(fadeAnimation);

    connect(group, &QParallelAnimationGroup::finished,
            this, &Coin::deleteLater);
    group->start();
}
