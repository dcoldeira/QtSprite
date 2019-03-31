#ifndef BACKGROUNDITEAM_H
#define BACKGROUNDITEAM_H

#include <QGraphicsPixmapItem>


class BackgroundItem : public QGraphicsPixmapItem
{
public:
    explicit BackgroundItem(const QPixmap &pixmap, QGraphicsItem *parent = nullptr);

public:
    virtual QPainterPath shape() const;
};

#endif // BACKGROUNDITEAM_H
