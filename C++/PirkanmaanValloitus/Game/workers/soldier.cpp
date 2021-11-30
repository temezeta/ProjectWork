#include "soldier.h"

namespace Teemu {

Soldier::Soldier(const std::shared_ptr<Essi::GameEventHandler> &eventhandler,
                 const std::shared_ptr<ObjectManager> &objectmanager,
                 const std::shared_ptr<Course::PlayerBase> &owner,
                 const int &tilespaces,
                 const Course::ResourceMap &cost,
                 const Course::ResourceMapDouble &efficiency):
    WorkerBase(eventhandler,
               objectmanager,
               owner,
               tilespaces,
               cost,
               efficiency)
{

}

std::string Soldier::getType() const
{
    return "Soldier";
}

bool Soldier::canBePlacedOnTile(const std::shared_ptr<Course::TileBase> &target) const
{
    //Check if assigner has barracks
    bool hasBarracks =false;
    auto objects = getOwner()->getObjects();
    for(auto object : objects){
        std::shared_ptr<Course::TileBase> tile =
                std::static_pointer_cast<Course::TileBase>(object);
        for(auto building : tile->getBuildings()){
            if(building->getType() == "Barracks"){
                hasBarracks = true;
            }
        }
    }
    //Check if tile has HQ
    bool hasHQ = false;
    auto buildings = target->getBuildings();
    for (auto building: buildings){
        if(building->getType() == "HeadQuarters"){
            hasHQ = true;
        }
    }
    return hasBarracks && hasHQ && (target->getOwner() != nullptr) &&
            (target->getOwner() != getOwner()) &&
            (target->hasSpaceForBuildings(spacesInTileCapacity()));
}

void Soldier::doSpecialAction()
{
    //Special action is done in MainWindow because it modifies MainWindow
    //parameters
}

const Course::ResourceMapDouble Soldier::tileWorkAction()
{
    return SOLDIER_WORKER_EFFICIENCY;
}

}
