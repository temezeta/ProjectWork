#ifndef MOUNTAIN_H
#define MOUNTAIN_H
#include "tiles/tilebase.h"
#include "core/gameeventhandler.hh"
#include "core/objectmanager.h"
#include "core/misc.h"

namespace Teemu {
/**
 * @brief The Mountain class represents Mountain in the gameworld
 *
 * Mountain has BasicProduction: \n
 * * Money = 4
 * * Food = 0
 * * Wood = 0
 * * Stone = 5
 * * Ore = 3
 *
 * Building in mountain takes time so buildins get extra 2 hold markers.
 *
 * Tile supports 2 buildings.
 */
class Mountain : public Course::TileBase
{
public:
    /**
     * @brief Disabled parameterless constructor.
     */
    Mountain() = delete;

    /**
     * @brief Mountain constructor
     * @param location  Coordinate of the tile in game
     * @param eventhandler points to created GameEventHandler
     * @param objectmanager points to Objectmanager
     */
    Mountain(const Course::Coordinate& location,
           const std::shared_ptr<Course::iGameEventHandler>& eventhandler,
           const std::shared_ptr<Course::iObjectManager>& objectmanager,
           const unsigned int& max_build = 2,
           const unsigned int& max_work = 3,
           const Course::ResourceMap& production = MOUNTAIN_BP);

    /**
     * @brief Default destructor.
     */
    virtual ~Mountain() = default;

    /**
     * @brief getType returns tile type
     * @return "Mountain"
     */
    virtual std::string getType() const override;

    /**
     * @brief Adds a new building-object to the tile. Building in mountain adds
     * three (3) hold-marker to the building.
     *
     * Phases: \n
     * 1. Check that there is space for the building. \n
     * 2. Call parent's addBuilding \n
     * 3. Add HoldMarkers for the building. \n
     */
    void addBuilding(const std::shared_ptr<Course::BuildingBase>& building) override;

};

}
#endif // MOUNTAIN_H
