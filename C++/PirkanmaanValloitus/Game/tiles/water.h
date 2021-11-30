#ifndef WATER_H
#define WATER_H
#include "tiles/tilebase.h"
#include "core/gameeventhandler.hh"
#include "core/objectmanager.h"
#include "core/misc.h"

namespace Teemu {

/**
 * @brief The Water class represents lake or a pond in the gameworld
 *
 * Water has BasicProduction: \n
 * * Money = 2
 * * Food = 5
 * * Wood = 0
 * * Stone = 2
 * * Ore = 0
 *
 * Water can't be built in, but it supports 2 workers if ownership is gained
 * outpost or HQ
 */
class Water : public Course::TileBase
{
public:
    /**
     * @brief Disabled parameterless constructor.
     */
    Water() = delete;
    /**
     * @brief Water constructor
     * @param location  Coordinate of the tile in game
     * @param eventhandler points to created GameEventHandler
     * @param objectmanager points to Objectmanager
     */
    Water(const Course::Coordinate& location,
           const std::shared_ptr<Course::iGameEventHandler>& eventhandler,
           const std::shared_ptr<Course::iObjectManager>& objectmanager,
           const unsigned int& max_build = 0,
           const unsigned int& max_work = 2,
           const Course::ResourceMap& production = WATER_BP);
    /**
     * @brief Default destructor.
     */
    virtual ~Water() = default;
    /**
     * @brief getType returns tile type
     * @return "Water"
     */
    virtual std::string getType() const override;
};

}
#endif // WATER_H
