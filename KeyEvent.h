#ifndef KEYEVENT_H
#define KEYEVENT_H
#include <QGraphicsPixmapItem>

class KeyEvent: public QGraphicsPixmapItem
{
public:
    void keyPressEvent(QKeyEvent * event);
};

#endif // KEYEVENT_H
