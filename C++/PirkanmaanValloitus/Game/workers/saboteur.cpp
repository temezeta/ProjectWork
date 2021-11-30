#include "saboteur.h"
#include "stdio.h"
#include "stdlib.h"
#include "time.h"

namespace Essi {

Saboteur::Saboteur(const std::shared_ptr<Essi::GameEventHandler>& eventhandler,
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

const Course::ResourceMapDouble Saboteur::tileWorkAction()
{
    return Teemu::SABOTEUR_WORKER_EFFICIENCY;
}

std::string Saboteur::getType() const
{
    return "Saboteur";
}

bool Saboteur::canBePlacedOnTile(
        const std::shared_ptr<Course::TileBase> &target) const
{
    if ( getOwner() != target->getOwner() and target->getOwner()!= nullptr ) {
        return true;
    } else {
        return false;
    }
}

void Saboteur::doSpecialAction()
{
    std::shared_ptr<Teemu::ObjectManager> objectmanager=
            std::static_pointer_cast<Teemu::ObjectManager>(lockObjectManager());

    srand(time(0));

    if ( currentLocationTile()->getBuildingCount() > 0 ) {
        std::shared_ptr<Course::BuildingBase> buildingToDestroy;
        int randomBuildingToDestroy = rand() %
                    currentLocationTile()->getBuildingCount();

        if ( currentLocationTile()->getBuildingCount() != 1 ) {
            // HeadQuarters cannot be destroyed, so new random number for a
            // building to destroy is generated
            while ( currentLocationTile()->getBuildings()
                    .at(randomBuildingToDestroy)
                    ->getType() == "HeadQuarters" ) {
                randomBuildingToDestroy = rand() %
                            currentLocationTile()->getBuildings().size();
            }
            buildingToDestroy = currentLocationTile()->getBuildings()
                    .at(randomBuildingToDestroy);
            currentLocationTile()->removeBuilding(buildingToDestroy);
            objectmanager->removeBuilding(buildingToDestroy);

        } else if ( currentLocationTile()->getBuildings().at(0)->getType()
                    != "HeadQuarters" ) {
            buildingToDestroy = currentLocationTile()->getBuildings().at(0);
            currentLocationTile()->removeBuilding(buildingToDestroy);
            objectmanager->removeBuilding(buildingToDestroy);

        }

    }

    if ( currentLocationTile()->getWorkerCount() > 0) {
        int randomWorkerToDestroy = rand() %
                currentLocationTile()->getWorkerCount();
        std::shared_ptr<Course::WorkerBase> workerToDestroy =
                currentLocationTile()->getWorkers()
                .at(randomWorkerToDestroy);

        currentLocationTile()->removeWorker(workerToDestroy);
        objectmanager->removeWorker(workerToDestroy);
    }
}

}
