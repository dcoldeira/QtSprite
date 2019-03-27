#ifndef MAP_H
#define MAP_H
#include <QGraphicsView>


class Map: public QGraphicsView
{
public:
    Map();
    QGraphicsScene * scene;
};

#endif // MAP_H
