#include "gameeventhandler.hh"

namespace Essi {

GameEventHandler::GameEventHandler()
{
}

bool GameEventHandler::modifyResources(std::shared_ptr<Course::PlayerBase> \
                                       player, Course::ResourceMap resources) {

    if ( playerResources_.find(player) != playerResources_.end() ) {

        Course::ResourceMap modified_resources = playerResources_.at(player);

        for ( auto resource_change : resources ) {
            for ( auto old_resource : modified_resources ) {
                if ( old_resource.first == resource_change.first ) {
                    if ( old_resource.second + resource_change.second >= 0 ) {
                        modified_resources.at(old_resource.first)
                                += resource_change.second;
                    } else {
                        return false;
                    }
                }
            }
        }
        playerResources_.at(player) = modified_resources;
        return true;
    }
    return false;

}

void GameEventHandler::addPlayer(std::shared_ptr<Course::PlayerBase> player)
{
    Course::ResourceMap startingResources = {
        {Course::BasicResource::MONEY, 1000},
        {Course::BasicResource::FOOD, 1000},
        {Course::BasicResource::WOOD, 1000},
        {Course::BasicResource::STONE, 1000},
        {Course::BasicResource::ORE, 1000} };

        playerResources_[player] = startingResources;
        HQStatus_[player] = false;
}

int GameEventHandler::getResource(std::shared_ptr<Course::PlayerBase> player,
                                  Course::BasicResource resource)
{
    return playerResources_.at(player).at(resource);
}

void GameEventHandler::setHQstatus(std::shared_ptr<Course::PlayerBase> player,
                                   bool status)
{
    HQStatus_.at(player) = status;
}

bool GameEventHandler::getHQstatus(std::shared_ptr<Course::PlayerBase> player)
{
    return HQStatus_.at(player);
}

Course::ResourceMap GameEventHandler::getResources(
        std::shared_ptr<Course::PlayerBase> player)
{
    return playerResources_.at(player);
}

bool GameEventHandler::modifyResource(std::shared_ptr<Course::PlayerBase> \
                                      player, Course::BasicResource resource, \
                                      int amount) {

    if ( playerResources_.find(player) != playerResources_.end() ) {
        if ( playerResources_.at(player).at(resource) + amount >= 0 ) {
            playerResources_.at(player).at(resource) += amount;
            return true;
        }
    }
    return false;

}

}
