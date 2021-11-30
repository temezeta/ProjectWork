#ifndef MINER_H
#define MINER_H

#include "../Course/CourseLib/workers/workerbase.h"
#include "../Game/core/gameeventhandler.hh"
#include "../Game/core/objectmanager.h"

namespace Essi {

/**
 * @brief The Miner class represents a miner type worker that can be placed in
 * a mine
 *
 * Miner has the following production-efficiency
 * * Money - 0.20
 * * Food - 0.10
 * * Wood - 0.10
 * * Stone - 1.50
 * * Ore - 1.00
 *
 * Miners use the following resources accordingly
 * * 2 Food - or Miner refuses to work
 * * 2 Money - or Miner refuses to work
 *
 * Miner can only be placed on tiles owned by the player that have a mine and
 * can fit another player.
 *
 * As a special action adding a miner to a tile will bring the player 8 ore.
 */

class Miner : public Course::WorkerBase
{
public:
    /**
     * @brief Disabled parameterless constructor.
     */
    Miner() = delete;

    /**
     * @brief Constructor for the miner class.
     *
     * @param eventHandler points to the GameEventHandler.
     * @param objectManager points to the objectManager.
     * @param owner points to the player who owns the Miner.
     */
    Miner(const std::shared_ptr<Essi::GameEventHandler>& eventHandler,
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
    virtual ~Miner() = default;

    /**
     * @brief getType returns the workers type
     * @return worker type "Miner"
     */
    virtual std::string getType() const override;

    /**
     * @brief canBePlacedOnTile checks that the worker can be placed on a tile,
     * meaning that the player owns the tile and there is a mine building placed
     * on the tile.
     * @param target the tile where player wants the Miner to be added
     * @return whether placing is successful
     */
    virtual bool canBePlacedOnTile(
            const std::shared_ptr<Course::TileBase> &target) const override;

    /**
     * @brief tileWorkAction returns Miner's resource production efficiency.
     * @return ResourceMap of production-efficiency.
     */
    virtual const Course::ResourceMapDouble tileWorkAction();

    /**
     * @brief doSpecialAction performs Miner's special action which means
     * granting the player with 8 ore.
     */
    virtual void doSpecialAction() override;
};

}

#endif // MINER_H
