#ifndef SABOTEUR_H
#define SABOTEUR_H

#include "../Course/CourseLib/workers/workerbase.h"
#include "../Game/core/gameeventhandler.hh"
#include "../Game/core/objectmanager.h"
#include "../Game/core/misc.h"

namespace Essi {

/**
 * @brief The Saboteur class represents a saboteur worker type
 *
 * Saboteur destroys one building (not HeadQuarters) and one worker on another
 * player's tile.
 * It can only be placed on the tile in case the tile is owned by another
 * player.
 *
 * Saboteur always has a production-efficiency of 0 and is deleted directly
 * another player's building and worker are destroyed.
 */

class Saboteur : public Course::WorkerBase
{
public:
    /**
     * @brief Disabled parameterless constructor.
     */
    Saboteur() = delete;

    /**
     * @brief Constructor for the Saboteur class.
     *
     * @param eventHandler points to the GameEventHandler.
     * @param objectManager points to the objectManager.
     * @param owner points to the player who owns the Saboteur.
     */
    Saboteur(const std::shared_ptr<Essi::GameEventHandler>& eventHandler,
             const std::shared_ptr<Teemu::ObjectManager>& objectManager,
             const std::shared_ptr<Course::PlayerBase>& owner,
             const int& tilespaces = 0,
             const Course::ResourceMap& cost = Teemu::SABOTEUR_RECRUITMENT_COST,
             const Course::ResourceMapDouble& efficiency
             = Teemu::SABOTEUR_WORKER_EFFICIENCY
            );

    /**
     * @brief Default destructor.
     */
    virtual ~Saboteur() = default;

    /**
     * @brief tileWorkAction returns Saboteur's production efficiency, which is
     * always 0.
     */
    virtual const Course::ResourceMapDouble tileWorkAction() override;

    /**
     * @brief getType returns the worker type
     * @return worker type "Saboteur"
     */
    virtual std::string getType() const override;

    /**
     * @brief canBePlacedOnTile checks if Saboteur can be placed on a tile. It
     * can only be done if the tile has an owner and it is owned by someone
     * else.
     * @return true if placement is successful
     */
    virtual bool canBePlacedOnTile(
            const std::shared_ptr<Course::TileBase> &target) const override;

    /**
     * @brief doSpecialAction destroys one building and one worker on another
     * player's tile.
     */
    virtual void doSpecialAction() override;
};

}
#endif // SABOTEUR_H
