#include "village.hh"

namespace Teemu {

Village::Village(const Course::Coordinate &location,
                 const std::shared_ptr<Course::iGameEventHandler> &eventhandler,
                 const std::shared_ptr<Course::iObjectManager> &objectmanager,
                 const unsigned int &max_build,
                 const unsigned int &max_work,
                 const Course::ResourceMap &production):
    Course::TileBase(location,
                     eventhandler,
                     objectmanager,
                     max_build,
                     max_work,
                     production),
    villageStock_(VILLAGE_STOCK)
{
}

std::string Village::getType() const
{
    return "Village";
}

Course::ResourceMap Village::returnStock()
{
    return villageStock_;
}

bool Village::modifyResources(Course::ResourceMap resources)
{
    Course::ResourceMap modifiedResources = villageStock_;
    for(auto resourceChange: resources){
        for(auto currentStock: modifiedResources){
            if(currentStock.first == resourceChange.first){
                if(currentStock.second + resourceChange.second >= 0){
                    modifiedResources.at(currentStock.first)
                            += resourceChange.second;
                }
                else{
                    return false;
                }
            }
        }
    }
    villageStock_ = modifiedResources;
    return true;
}


}

