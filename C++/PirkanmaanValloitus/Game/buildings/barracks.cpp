#include "barracks.h"

namespace Teemu {

Barracks::Barracks(const std::shared_ptr<Essi::GameEventHandler> &eventhandler,
                   const std::shared_ptr<ObjectManager> &objectmanager,
                   const std::shared_ptr<Course::PlayerBase> &owner,
                   const int &tilespaces, const Course::ResourceMap &buildcost,
                   const Course::ResourceMap &production):
    Course::BuildingBase(eventhandler,
                         objectmanager,
                         owner,
                         tilespaces,
                         buildcost,
                         production)
{

}

std::string Barracks::getType() const
{
    return "Barracks";
}

void Barracks::onBuildAction()
{
    std::shared_ptr<Teemu::ObjectManager> objectmanager=
            std::static_pointer_cast<Teemu::ObjectManager>(lockObjectManager());
    std::vector< std::shared_ptr<Course::TileBase> > neighbours =
            objectmanager->getTiles(getCoordinatePtr()->neighbours(1));

    for(auto tile: neighbours){
        //Check if tile has HQ
        bool hasHQ = false;
        auto buildings = tile->getBuildings();
        for (auto building: buildings){
            if(building->getType() == "HeadQuarters"){
                hasHQ = true;
            }
        }
        //If tile doesnt have HQ and you are not the owner: remove everything
        if(not hasHQ
                && tile->getOwner()!=getOwner()
                && tile->getOwner() != nullptr){
            auto buildings = tile->getBuildings();
            for(auto building:buildings){
                tile->removeBuilding(building);
                objectmanager->removeBuilding(building);
            }
            auto workers = tile->getWorkers();
            for(auto worker:workers){
                tile->removeWorker(worker);
                objectmanager->removeWorker(worker);
            }
            tile->getOwner()->removeObject(tile);
            tile->setOwner(nullptr);
        }
    }

}

Course::ResourceMap Barracks::getProduction()
{
    return PRODUCTION_EFFECT;
}

}
