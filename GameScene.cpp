
#include "GameScene.h"
#include "Coin.h"

#include <QKeyEvent>
#include <QPropertyAnimation>
#include <QDebug>
#include <QGraphicsView>
#include <QPen>

#include <QGraphicsPixmapItem>

#include "Player.h"
#include "Coin.h"
#include "BackgroundItem.h"
#include <QGamepadManager>
#include <QGamepad>
#include <math.h>

GameScene::GameScene(QObject *parent) :
    QGraphicsScene(parent)
  , m_velocity(6)
  , m_worldShift(0)
  , m_groundLevel(550) // qui...............................................
  , m_minX(0)
  , m_maxX(0)
  , m_jumpAnimation(new QPropertyAnimation(this))
  , m_jumpHeight(180)
  , m_fieldWidth(2000)
  , player()
  , m_sky()
  , m_trees()
  , m_grass()
  , m_coins()
  , m_horizontalInput(0)
{
    initPlayField();
    m_timer.setInterval(30);
    connect(&m_timer, &QTimer::timeout, this, &GameScene::movePlayer);

    m_jumpAnimation->setTargetObject(this);
    m_jumpAnimation->setPropertyName("jumpFactor");
    m_jumpAnimation->setStartValue(0);
    m_jumpAnimation->setKeyValueAt(0.5, 1);
    m_jumpAnimation->setEndValue(0);
    m_jumpAnimation->setDuration(800);
    m_jumpAnimation->setEasingCurve(QEasingCurve::OutInQuad);

    QList<int> gamepadIds = QGamepadManager::instance()->connectedGamepads();
    if (!gamepadIds.isEmpty()) {
        QGamepad *gamepad = new QGamepad(gamepadIds[0], this);
        connect(gamepad, &QGamepad::axisLeftXChanged,
                this, &GameScene::axisLeftXChanged);
        connect(gamepad, &QGamepad::axisLeftYChanged,
                this, &GameScene::axisLeftYChanged);
    }

}

void GameScene::keyPressEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat()) {
        return;
    }
    switch (event->key()) {
    case Qt::Key_Right:
        addHorizontalInput(1);
        break;
    case Qt::Key_Left:
        addHorizontalInput(-1);
        break;
    case Qt::Key_Space:
        jump();
        break;
    default:
        break;
    }
}

void GameScene::keyReleaseEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat()) {
        return;
    }
    switch (event->key()) {
    case Qt::Key_Right:
        addHorizontalInput(-1);
        break;
    case Qt::Key_Left:
        addHorizontalInput(1);
        break;
            case Qt::Key_Space:
                return;
                break;
    default:
        break;
    }
}



void GameScene::movePlayer()
{
    const int direction = player->direction();
    if (0 == direction) {
        return;
    }

    const int dx = direction * m_velocity;
    qreal newX = qBound(m_minX, m_currentX + dx, m_maxX);
    if (newX == m_currentX) {
        return;
    }
    m_currentX = newX;

    const int shiftBorder = 150;
    int rightShiftBorder = width() - shiftBorder;

    const int visiblePlayerPos = m_currentX - m_worldShift;
    const int newWorldShiftRight = visiblePlayerPos - rightShiftBorder;
    if (newWorldShiftRight > 0) {
        m_worldShift += newWorldShiftRight;
    }
    const int newWorldShiftLeft = shiftBorder - visiblePlayerPos;
    if (newWorldShiftLeft > 0) {
        m_worldShift -= newWorldShiftLeft;
    }
    const int maxWorldShift = m_fieldWidth - qRound(width());
    m_worldShift = qBound(0, m_worldShift, maxWorldShift);
    player->setX(m_currentX - m_worldShift);

    const qreal ratio = qreal(m_worldShift) / maxWorldShift;
    applyParallax(ratio, m_sky);
    applyParallax(ratio, m_grass);
    applyParallax(ratio, m_trees);
    applyParallax(ratio, m_coins);

    checkColliding();
}

void GameScene::applyParallax(qreal ratio, QGraphicsItem* item) {
    item->setX(-ratio * (item->boundingRect().width() - width()));
}

void GameScene::initPlayField()
{
    setSceneRect(0, 0, 1280,720);     // QUI..........................................................

    m_sky = new BackgroundItem(QPixmap(":/Img/sky.png"));
    addItem(m_sky);

    BackgroundItem *ground = new BackgroundItem(QPixmap(":/Img/ground.png"));
    addItem(ground);
    ground->setPos(0, m_groundLevel);

    m_trees = new BackgroundItem(QPixmap(":/Img/trees.png"));
    m_trees->setPos(0, m_groundLevel - m_trees->boundingRect().height());
    addItem(m_trees);

    m_grass = new BackgroundItem(QPixmap(":/Img/grass.png"));
    m_grass->setPos(0,m_groundLevel - m_grass->boundingRect().height());
    addItem(m_grass);

    player = new Player();
    m_minX = player->boundingRect().width() * 0.5;
    m_maxX = m_fieldWidth - player->boundingRect().width() * 0.5;
    player->setPos(m_minX, m_groundLevel - player->boundingRect().height() / 2);
    m_currentX = m_minX;
    addItem(player);

    // Add some coins
    m_coins = new QGraphicsRectItem(0, 0, m_fieldWidth, m_jumpHeight);
    m_coins->setPen(Qt::NoPen);
    m_coins->setPos(0, m_groundLevel - m_jumpHeight);
    const int xRange = (m_maxX - m_minX) * 0.94;
    for (int i = 0; i < 25; ++i) {
        Coin *c = new Coin(m_coins);
        c->setPos(m_minX + qrand() % xRange, qrand() % m_jumpHeight);
    }
    addItem(m_coins);
}

void GameScene::jump()
{
    if (QAbstractAnimation::Stopped == m_jumpAnimation->state()) {
        m_jumpAnimation->start();
    }
}

void GameScene::addHorizontalInput(int input)
{
    m_horizontalInput += input;
    player->setDirection(qBound(-1, m_horizontalInput, 1));
    checkTimer();
}

qreal GameScene::jumpFactor() const
{
    return m_jumpFactor;
}

void GameScene::setJumpFactor(const qreal &jumpFactor)
{
    if (m_jumpFactor == jumpFactor) {
        return;
    }

    m_jumpFactor = jumpFactor;
    emit jumpFactorChanged(m_jumpFactor);

    qreal groundY = (m_groundLevel - player->boundingRect().height() / 2);
    qreal y = groundY - m_jumpAnimation->currentValue().toReal() * m_jumpHeight;
    player->setY(y);

    checkColliding();
}

void GameScene::checkTimer()
{
    if (player->direction() == 0) {
        m_timer.stop();
    } else if (!m_timer.isActive()) {
        m_timer.start();
    }
}

void GameScene::checkColliding()
{
    for(QGraphicsItem* item: collidingItems(player)) {
        if (Coin *c = qgraphicsitem_cast<Coin*>(item)) {
            c->explode();
        }
    }
}

void GameScene::axisLeftXChanged(double value)
{
    int direction;
    if (value > 0) {
        direction = 1;
    } else if (value < 0) {
        direction = -1;
    } else {
        direction = 0;
    }
    player->setDirection(direction);
    checkTimer();
}

void GameScene::axisLeftYChanged(double value)
{
    if (value < -0.25) {
        jump();
    }
}
