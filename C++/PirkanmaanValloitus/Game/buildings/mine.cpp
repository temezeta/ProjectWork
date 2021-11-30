#include "mine.h"

namespace Essi {

Mine::Mine(const std::shared_ptr<Essi::GameEventHandler>& eventhandler,
           const std::shared_ptr<Teemu::ObjectManager>& objectmanager,
           const std::shared_ptr<Course::PlayerBase>& owner,
           const int& tilespaces,
           const Course::ResourceMap& buildcost,
           const Course::ResourceMap& production):

    Course::BuildingBase(eventhandler,
                         objectmanager,
                         owner,
                         tilespaces,
                         buildcost,
                         production)
{

}

std::string Mine::getType() const
{
    return "Mine";
}

Course::ResourceMap Mine::getProduction()
{
    return Course::BuildingBase::getProduction();
}

bool Mine::canBePlacedOnTile(const std::shared_ptr<Course::TileBase> &target) const
{
    bool canBePlaced = false;

    if ( target->getType() == "Mountain" and target->hasSpaceForBuildings(1) ) {
        if ( target->getOwner() == nullptr or target->getOwner() ==
             getOwner() ) {
            canBePlaced = true;
        }
    }

    return canBePlaced;
}

}
