#include "miner.h"

namespace Essi {

Miner::Miner(const std::shared_ptr<Essi::GameEventHandler>& eventhandler,
             const std::shared_ptr<Teemu::ObjectManager>& objectmanager,
             const std::shared_ptr<Course::PlayerBase>& owner,
             const int& tilespaces,
             const Course::ResourceMap& cost,
             const Course::ResourceMapDouble& efficiency):
    WorkerBase(
        eventhandler,
        objectmanager,
        owner,
        tilespaces,
        cost,
        efficiency)
{

}

std::string Miner::getType() const
{
    return "Miner";
}

bool Miner::canBePlacedOnTile(const std::shared_ptr<Course::TileBase> &target) const
{
    bool hasMine = false;

    // Check that the player owns the target tile
    if ( getOwner() == target->getOwner() ) {

        // Check that the tile has a mine and space for another worker
        for ( auto building : target->getBuildings() ) {
            if ( building->getType() == "Mine" and
                 target->hasSpaceForWorkers(1) ) {
                hasMine = true;
            }
        }
    }
    return hasMine;
}

const Course::ResourceMapDouble Miner::tileWorkAction()
{
    Course::ResourceMapDouble productEfficiency;
    double satisfaction = 0;

    // Check if worker agrees to work
    if ( lockEventHandler()->modifyResource(getOwner(),
                                            Course::BasicResource::FOOD, -1)
         and lockEventHandler()->modifyResource(getOwner(),
                                                Course::BasicResource::MONEY,
                                                -2) ) {
        satisfaction = 1;
    }

    for ( auto resource : Teemu::MINER_WORKER_EFFICIENCY ) {
        productEfficiency[resource.first] = resource.second * satisfaction;
    }

    return productEfficiency;
}

void Miner::doSpecialAction()
{
    lockEventHandler()->modifyResource(getOwner(), Course::BasicResource::ORE,
                                       8);
}

}
