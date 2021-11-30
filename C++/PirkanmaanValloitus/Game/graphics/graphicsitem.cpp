#include "graphicsitem.h"
#include <cstdlib>


namespace Teemu{

GraphicsItem::GraphicsItem(const std::shared_ptr<Course::TileBase> &obj, int size):
    gameobject_(obj),
    scenelocation_(gameobject_->getCoordinatePtr()->asQpoint()),
    size_(size)
{
}

QRectF GraphicsItem::boundingRect() const
{
    return QRectF(scenelocation_.x() * size_, scenelocation_.y() * size_,
                  size_,size_);
}

void GraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    std::string tileType = gameobject_->getType();
    Q_UNUSED( option ); Q_UNUSED( widget );
    //Draw tile types image
    QPixmap pixmap = QPixmap(IMAGE_LOCATIONS.at(tileType));
    painter->drawPixmap(scenelocation_.x()*size_,scenelocation_.y()*size_,
                        size_,size_,pixmap);
    //Add owner marker
    if(gameobject_->getOwner() != nullptr){
        auto owner = std::static_pointer_cast<Course::PlayerBase>(
                    gameobject_->getOwner());
        std::string name = owner->getName();

        //Convert name to number
        int numberFromString = 0;
        for(char character:name){
            numberFromString += abs(int(character));
        }
        int color = numberFromString%255;
        QRect rect = QRect(scenelocation_.x()*size_,scenelocation_.y()*size_,
                           size_/4,size_/4);
        painter->setBrush(QColor(80,color,color));
        painter->drawEllipse(rect);

    }
    //Draw worker if tile has workers
    if(gameobject_->getWorkerCount() != 0){
        QPixmap worker = QPixmap(IMAGE_LOCATIONS.at("Person"));
        painter->drawPixmap((scenelocation_.x()*size_)+size_*2/5,
                            (scenelocation_.y()*size_)+(size_-size_/2),
                            size_*2/5,
                            size_*2/5,
                            worker);
    }
    //Draw building if tile has buildings
    if(gameobject_->getBuildingCount() != 0){
        QPixmap building = QPixmap(IMAGE_LOCATIONS.at("House"));
        painter->drawPixmap(scenelocation_.x()*size_,
                             (scenelocation_.y()*size_)+(size_-size_/2),
                             size_*2/5,
                             size_*2/5,
                             building);
    }
}

const std::shared_ptr<Course::TileBase> &GraphicsItem::getObject()
{
    return gameobject_;
}

void GraphicsItem::zoomIn()
{
    size_ = size_ + 10 ;
}

void GraphicsItem::zoomOut()
{
    size_ = size_ - 10;
}


}
