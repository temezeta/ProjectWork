#ifndef FARMER_H
#define FARMER_H

#include "../Course/CourseLib/workers/workerbase.h"
#include "../Game/core/gameeventhandler.hh"
#include "../Game/core/objectmanager.h"
#include "../Course/CourseLib/core/resourcemaps.h"
#include "../Game/core/misc.h"

namespace Essi {

/**
 * @brief The Farmer class represents a farmer worker type
 *
 * Farmer has the following production-efficiency:
 * * Money - 0.20
 * * Food - 1.50
 * * Wood - 1.00
 * * Stone - 0.10
 * * Ore - 0.10
 *
 * Farmers consume food and money accordingly
 * * 1 Food - or Farmer refuses to work
 * * 2 Money - or Farmer works at 50% efficiency
 *
 * Farmer can only be placed on tiles that the player owns, have
 * a farm building type and can fit another player.
 *
 * ResourceFocus adds 30 % efficiency to the focused resource if farmer
 * does not refuse to work.
 *
 * As a special action Farmer brings 8 food with them when first placed on
 * a tile.
 */

class Farmer : public Course::WorkerBase
{
public:
    /**
     * @brief Disabled parameterless constructor.
     */
    Farmer() = delete;

    /**
     * @brief Constructor for the farmer class.
     *
     * @param eventHandler points to the GameEventHandler.
     * @param objectManager points to the objectManager.
     * @param owner points to the player who owns the Farmer.
     */
    Farmer(const std::shared_ptr<Essi::GameEventHandler>& eventHandler,
           const std::shared_ptr<Teemu::ObjectManager>& objectManager,
           const std::shared_ptr<Course::PlayerBase>& owner,
           const int& tilespaces = 1,
           const Course::ResourceMap& cost = Teemu::FARMER_RECRUITMENT_COST,
           const Course::ResourceMapDouble& efficiency
           = Teemu::FARMER_WORKER_EFFICIENCY
            );

    /**
     * @brief Default destructor.
     */
    virtual ~Farmer() = default;

    /**
     * @brief getType returns the workers type
     * @return worker type "Farmer"
     */
    virtual std::string getType() const override;

    /**
     * @brief canBePlacedOnTile checks that the worker can be placed on a tile,
     * meaning that the player owns the tile and there is a farm building placed
     * on the tile.
     * @param target the tile where player wants the Farmer to be added
     * @return if placing is successful
     */
    virtual bool canBePlacedOnTile(
            const std::shared_ptr<Course::TileBase> &target) const override;

    /**
     * @brief tileWorkAction returns Farmer's resource production efficiency.
     * @return ResourceMap of production-efficiency.
     */
    virtual const Course::ResourceMapDouble tileWorkAction();

    /**
     * @brief doSpecialAction performs Farmer's special action
     */
    virtual void doSpecialAction() override;
};

}

#endif // FARMER_H
