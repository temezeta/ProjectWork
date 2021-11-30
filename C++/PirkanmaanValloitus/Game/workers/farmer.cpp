#include "farmer.h"

namespace Essi {

Farmer::Farmer(const std::shared_ptr<Essi::GameEventHandler>& eventhandler,
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

std::string Farmer::getType() const
{
    return "Farmer";
}

bool Farmer::canBePlacedOnTile(const std::shared_ptr<Course::TileBase> &target) const
{
    bool hasFarm = false;

    // Check that player owns the tile
    if ( getOwner() == target->getOwner() ) {

        // Check that the tile has a farm and can fit another worker
        for ( auto building : target->getBuildings() ) {
            if ( building->getType() == "Farm" and
                 target->hasSpaceForWorkers(1) ) {
                hasFarm = true;
            }
        }
    }

    return hasFarm;
}

const Course::ResourceMapDouble Farmer::tileWorkAction()
{
    Course::ResourceMapDouble productEfficiency;
    double satisfaction = 0;

    if ( lockEventHandler()->modifyResource(getOwner(),
                                            Course::BasicResource::FOOD, -1) ) {
        satisfaction = 0.5;
        if ( lockEventHandler()->modifyResource(getOwner(),
                                                Course::BasicResource::MONEY,
                                                -2) ) {
            satisfaction = 1;
        }
    }

    for ( auto resource : Teemu::FARMER_WORKER_EFFICIENCY ) {
        if ( resource.first == getResourceFocus() and satisfaction != 0 ) {
            productEfficiency[resource.first] = resource.second
                    * (satisfaction + 0.3);
        } else {
            productEfficiency[resource.first] = resource.second * satisfaction;
        }
    }

    return productEfficiency;
}

void Farmer::doSpecialAction()
{
    lockEventHandler()->modifyResource(getOwner(), Course::BasicResource::FOOD, 8);
}

}
