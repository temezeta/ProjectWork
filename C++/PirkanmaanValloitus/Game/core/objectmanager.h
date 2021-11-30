#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H

#include "../Course/CourseLib/interfaces/iobjectmanager.h"
#include "../Course/CourseLib/tiles/tilebase.h"
#include "../Course/CourseLib/core/playerbase.h"
#include "../Course/CourseLib/tiles/forest.h"
#include "../Course/CourseLib/tiles/grassland.h"
#include "tiles/water.h"
#include "tiles/mountain.h"
#include "tiles/village.hh"

namespace Teemu {
/**
 * @brief The ObjectManager is a class for managing gameobject
 */
class ObjectManager : public Course::iObjectManager
{
public:
    /**
     * @brief ObjectManager constructor
     */
    ObjectManager();
    /**
      * @brief default destructor
      */
    virtual ~ObjectManager() = default;
    /**
     * @brief addTiles add tiles to the object manager
     * @param tiles is vector of tiles to be added
     */
    virtual void addTiles(const std::vector<std::shared_ptr<Course::TileBase>>& tiles);
    /**
     * @brief getTile get tile object by coordinate
     * @param coordinate
     * @return shared pointer to selected tile or null if no tile found
     */
    virtual std::shared_ptr<Course::TileBase> getTile(const Course::Coordinate& coordinate);
    /**
     * @brief getTile get tile object by ID
     * @param id
     * @return shared pointer to selected tile or null if no tile found
     */
    virtual std::shared_ptr<Course::TileBase> getTile(const Course::ObjectId& id);
    /**
     * @brief getTiles get tiles by a vector of coordinates
     * @param coordinates
     * @return vector of tiles found with given coordinates
     */
    virtual std::vector<std::shared_ptr<Course::TileBase>> getTiles(const std::vector<Course::Coordinate>& coordinates);
    /**
     * @brief addPlayer adds player to players_
     * @param player
     */
    void addPlayer(std::shared_ptr<Course::PlayerBase> player);
    /**
     * @brief removePlayer remove player from the game
     * @param player
     */
    void removePlayer(std::shared_ptr<Course::PlayerBase> player);
    /**
     * @brief getPlayer returns player with given index
     * @param playerID
     */
    std::shared_ptr<Course::PlayerBase> getPlayer(unsigned int playerIndex); 
    /**
     * @brief addBuilding adds building to objectmanager
     * @param building
     */
    bool addBuilding(std::shared_ptr<Course::BuildingBase> building);
    /**
     * @brief removeBuilding removes building from objectmanager
     * @param building
     */
    bool removeBuilding(std::shared_ptr<Course::BuildingBase> building);
    /**
     * @brief addWorker adds worker to objectmanager
     * @param worker
     */
    bool addWorker(std::shared_ptr<Course::WorkerBase> worker);
    /**
     * @brief removeWorker removes worker from objectmanager
     * @param worker
     */
    bool removeWorker(std::shared_ptr<Course::WorkerBase> worker);
private:
    /**
    * @brief tiles_ stores tiles present on the gamemap
    */
    std::vector<std::shared_ptr<Course::TileBase>> tiles_;
    /**
     * @brief players_ stores players
     */
    std::vector<std::shared_ptr<Course::PlayerBase>> players_;
    /**
     * @brief buildings_ stores buildings
     */
    std::vector<std::shared_ptr<Course::BuildingBase>> buildings_;
    /**
     * @brief workers_ stores workers
     */
    std::vector<std::shared_ptr<Course::WorkerBase>> workers_;

};

}
#endif // OBJECTMANAGER_H
