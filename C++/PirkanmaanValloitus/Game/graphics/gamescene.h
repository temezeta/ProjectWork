#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QGraphicsScene>
#include <QGraphicsView>

#include <memory>

#include "tiles/tilebase.h"

namespace Teemu{
/**
 * @brief The GameScene class Represents gamescene where the map is drawn in the
 * game and handles mouseclickevents
 */
class GameScene : public QGraphicsScene
{
    Q_OBJECT
public:
    /**
     * @brief GameScene: Constructor for gamescene class
     * @param qt_parent: scene parent
     * @param width: scene width
     * @param height: scene height
     * @param scale: scene scale
     */
    GameScene(QWidget* qt_parent = nullptr, int width = 20, int height = 20, int scale = 25);
    /**
     * @brief Default destructor.
     */
    ~GameScene() = default;
    /**
     * @brief setSize: setSize of the gamescene (how many tiles per row/column)
     * @param width
     * @param height
     */
    void setSize(int width, int height);
    /**
     * @brief setScale: set scaling of the gamescene (how big are the tiles)
     * @param scale
     */
    void setScale(int scale);
    /**
     * @brief resize: Resize gamescene to fit the given size and scale
     */
    void resize();
    /**
     * @brief drawItem: draw item on the scene
     * @param obj: item to be drawn
     */
    void drawItem(std::shared_ptr<Course::TileBase> obj);
    /**
     * @brief event: eventhandler for mouseclickaction
     * @param event
     * @return
     */
    virtual bool event(QEvent* event) override;
    /**
     * @brief setSelectedNull: sets currently selected tile to nullptr
     */
    void setSelectedNull();
    /**
     * @brief zoomIn zoom in the gamescene
     */
    void zoomIn();
    /**
     * @brief zoomOut zoom out the gamescene
     */
    void zoomOut();
    /**
     * @brief returnCenter
     * @return center point of the gamescene
     */
    QPointF returnCenter();

signals:
    /**
     * @brief sendSelected: send info about clicked tile to mainwindow
     * @param gameobject: clicked tile
     */
    void sendSelected(std::shared_ptr<Course::TileBase> gameobject);
    /**
     * @brief sendWheelEvent send wheel event to mainwindow
     */
    void sendWheelEvent();
private:
    QGraphicsItem* mapBoundRect;
    int sceneWidth_;
    int sceneHeight_;
    int sceneScale_;
    std::shared_ptr<Course::TileBase> gameobject_;
    int maxScale;
};
}
#endif // GAMESCENE_H
