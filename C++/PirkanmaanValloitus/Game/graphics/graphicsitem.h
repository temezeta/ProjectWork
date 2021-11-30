#ifndef GRAPHICSITEM_H
#define GRAPHICSITEM_H

#include <QGraphicsItem>
#include <QPainter>

#include <memory>
#include <map>

#include "core/gameobject.h"
#include "core/playerbase.h"
#include "core/misc.h"
#include "tiles/tilebase.h"

namespace Teemu {
/**
 * @brief The GraphicsItem class represents tiles in UI
 */
class GraphicsItem : public QGraphicsItem
{
public:
    /**
     * @brief GraphicsItem: Constructor for graphics item
     * @param obj: Object to create a UI element for
     * @param size: Size of the element
     */
    GraphicsItem(const std::shared_ptr<Course::TileBase> &obj, int size);
    /**
     * @brief boundingRect: Sets boundaries for object
     * @return
     */
    QRectF boundingRect() const override;
    /**
     * @brief paint: Paint the object on the scene
     * @param painter: Painter paints it on the scene
     * @param option
     * @param widget: Owner of the item
     */
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget);
    /**
     * @brief getObject
     * @return returns object bound to the UI element
     */
    const std::shared_ptr<Course::TileBase> &getObject();
    /**
     * @brief zoomIn zoom in a tile
     */
    void zoomIn();
    /**
     * @brief zoomOut zoom out a tile
     */
    void zoomOut();
private:
    const std::shared_ptr<Course::TileBase> gameobject_;
    QPoint scenelocation_;
    int size_;
};

}
#endif // GRAPHICSITEM_H
