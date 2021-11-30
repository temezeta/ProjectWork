#include "gamescene.h"
#include "graphicsitem.h"

#include <QEvent>
#include <QGraphicsSceneMouseEvent>

#include <math.h>

namespace Teemu {

GameScene::GameScene(QWidget* parent, int width, int height, int scale):
    QGraphicsScene(parent),
    mapBoundRect(nullptr),
    sceneWidth_(10),
    sceneHeight_(10),
    sceneScale_(50),
    gameobject_(nullptr),
    maxScale(0)
{
    setSize(width, height);
    setScale(scale);
}

void GameScene::setSize(int width, int height)
{
    sceneWidth_ = width;
    sceneHeight_ = height;
    resize();
}

void GameScene::setScale(int scale)
{
    sceneScale_ = scale;
    resize();
}

void GameScene::resize()
{
    if ( mapBoundRect != nullptr ){
            QGraphicsScene::removeItem(mapBoundRect);
        }
    //Top left is (0,0) for the coordinates
    QRect rect = QRect( 0, 0, sceneWidth_ * sceneScale_, sceneHeight_ * sceneScale_);
    addRect(rect, QPen(Qt::black));
    setSceneRect(rect);
    mapBoundRect = itemAt(rect.topLeft(), QTransform());
    mapBoundRect->setZValue(-1);
}

void GameScene::drawItem(std::shared_ptr<Course::TileBase> obj)
{
    GraphicsItem* newItem = new GraphicsItem(obj, sceneScale_);
    addItem(newItem);
}

bool GameScene::event(QEvent *event)
{
    if(event->type() == QEvent::GraphicsSceneMousePress)
    {
        QGraphicsSceneMouseEvent* mouse_event =
                dynamic_cast<QGraphicsSceneMouseEvent*>(event);

        if ( sceneRect().contains(mouse_event->scenePos())){

            QPointF point = mouse_event->scenePos() / sceneScale_;

            point.rx() = floor(point.rx());
            point.ry() = floor(point.ry());

            QGraphicsItem* pressed = itemAt(point * sceneScale_, QTransform());
            //Send selected tile to mainwindow if it is custom graphicsitem
            if(dynamic_cast<GraphicsItem*>(pressed)){
                gameobject_ = static_cast<GraphicsItem*>(pressed)->getObject();
                emit sendSelected(gameobject_);
            }
            return true;

        }
    }
    else if(event->type() == QEvent::GraphicsSceneWheel){
        QGraphicsSceneWheelEvent* wheel_event =
                dynamic_cast<QGraphicsSceneWheelEvent*>(event);
        int direction = wheel_event->delta();
        if(direction > 0){
            zoomIn();
        }
        else{
            zoomOut();
        }
        emit sendWheelEvent();
        return true;
    }

    return QGraphicsScene::event(event);
}


void GameScene::setSelectedNull()
{
    gameobject_ = nullptr;
}

void GameScene::zoomIn()
{
    if(maxScale < 5){
        for(auto item: items()){
            Teemu::GraphicsItem* tile = dynamic_cast<Teemu::GraphicsItem*>(item);
            if(tile){
                tile->zoomIn();
            }
        }
        setScale(sceneScale_ + 10);
        maxScale++;
    }
}

void GameScene::zoomOut()
{
    if(maxScale > 0){
        for(auto item: items()){
            Teemu::GraphicsItem* tile = dynamic_cast<Teemu::GraphicsItem*>(item);
            if(tile){
                tile->zoomOut();
            }
        }
        setScale(sceneScale_ - 10);
        maxScale--;
    }
}

QPointF GameScene::returnCenter()
{
    QPointF center = QPointF(sceneWidth_*sceneScale_/2
                             ,sceneHeight_*sceneScale_/2);
    return center;
}


}
