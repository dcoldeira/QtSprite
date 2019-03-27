#include "Game.h"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsSvgItem>

#include <KeyEvent.h>

Game::Game()
{
    KeyEvent *capy = new KeyEvent();
    capy->setPixmap(QPixmap(":/ima/Img/ChiquiOriginal2.svg"));
    capy->setPos(329.5 + 107, 360);
    capy->setFlag(QGraphicsSvgItem::ItemIsFocusable);
    capy->setFocus();

    scene = new QGraphicsScene();
    scene->setSceneRect(0,0,1000,800);
    scene->addItem(capy);

    QGraphicsView * view = new QGraphicsView(scene);
    view->show();

}
