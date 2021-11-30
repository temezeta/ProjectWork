#ifndef SOLDIER_H
#define SOLDIER_H
#include "workers/workerbase.h"
#include "core/misc.h"
#include "core/gameeventhandler.hh"
#include "core/objectmanager.h"

namespace Teemu {
/**
 * @brief The Soldier class represents a soldier in the game
 * Soldier does not produce anything and cannot be placed on tiles which
 * * Have no owner
 * * You are the owner of
 * * Have no HeadQuarters
 *
 * Soldier is instantly removed from the tile and removes owner of the tile from
 * the game permanently
 */
class Soldier : public Course::WorkerBase
{
public:
    /**
     * @brief Soldier Disabled parameterless constructor
     */
    Soldier() = delete;
    /**
     * @brief Soldier
     * @param eventhandler points to gameeventhandler
     * @param objectmanager points to objectmanager
     * @param owner  owner of the worker (who assigned it)
     */
    Soldier(const std::shared_ptr<Essi::GameEventHandler>& eventhandler,
            const std::shared_ptr<Teemu::ObjectManager>& objectmanager,
            const std::shared_ptr<Course::PlayerBase>& owner,
            const int& tilespaces = 0,
            const Course::ResourceMap& cost =
            SOLDIER_RECRUITMENT_COST,
            const Course::ResourceMapDouble& efficiency =
            SOLDIER_WORKER_EFFICIENCY
            );
    /**
      * @brief Default destructor
      */
    virtual ~Soldier() = default;
    /**
     * @brief getType returns type of the worker
     * @return type of the worker
     */
    virtual std::string getType() const override;
    /**
     * @brief canBePlacedOnTile Check if worker can be placed on tile
     * Rules:
     * *Tile must have an owner
     * *Tiles owner must be someone else than you
     * *Tile must have a HeadQuarters
     * * Player must own a Barracks
     * Note:: Space check redundant because worker takes no tilespaces but
     * checked anyways to keep consistency
     * @param target Tile that worker is placed on
     * @return True  if above conditions check true, else false
     */
    virtual bool canBePlacedOnTile(
            const std::shared_ptr<Course::TileBase> &target) const override;
    /**
     * @brief doSpecialAction perform special action
     */
    virtual void doSpecialAction() override;
    /**
     * @brief tileWorkAction returns Soldiers efficiency at resource production
     * Soldier consumes no resources and as such it resource production is
     * always 0
     * @return resourceproduction
     */
    virtual const Course::ResourceMapDouble tileWorkAction() override;
};

}
#endif // SOLDIER_H
