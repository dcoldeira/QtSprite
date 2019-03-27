
#include <QGraphicsScene>
#include <QGraphicsView>
#include "Map.h"

Map::Map()
{
     scene = new QGraphicsScene();
     scene->setSceneRect(0,0,800,600);
     setScene(scene);
     setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
     setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
     //setFixedSize(800,600);

     show();

}



