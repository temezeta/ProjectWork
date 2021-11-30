#include "objectmanager.h"

namespace Teemu{

ObjectManager::ObjectManager():
    tiles_{},
    players_{},
    buildings_{},
    workers_{}
{
}


void ObjectManager::addTiles(
        const std::vector<std::shared_ptr<Course::TileBase> > &tiles)
{
    tiles_ = tiles;
}

std::shared_ptr<Course::TileBase> ObjectManager::getTile(
        const Course::Coordinate &coordinate)
{
    for (auto tile: tiles_){
        if (tile->getCoordinate().x() == coordinate.x()
                && tile->getCoordinate().y() == coordinate.y()){
            return tile;
        }
    }
    return nullptr;
}

std::shared_ptr<Course::TileBase> ObjectManager::getTile(
        const Course::ObjectId &id)
{
    for (auto tile: tiles_){
        if (tile->ID == id){
            return tile;
        }
    }
    return nullptr;
}

std::vector<std::shared_ptr<Course::TileBase> >
ObjectManager::getTiles(const std::vector<Course::Coordinate> &coordinates)
{
    std::vector<std::shared_ptr<Course::TileBase> > tiles = {};
    for (auto tile: tiles_){
        for (auto coordinate: coordinates){
            if(tile->getCoordinate().x() == coordinate.x()
                    && tile->getCoordinate().y() == coordinate.y()){
                //Only return tile with same coordinate once
                if(std::find(tiles.begin(),tiles.end(),tile) == tiles.end()){
                    tiles.push_back(tile);
                }
            }
        }
    }
    return tiles;
}

void ObjectManager::addPlayer(std::shared_ptr<Course::PlayerBase> player)
{
    players_.push_back(player);
}

void ObjectManager::removePlayer(std::shared_ptr<Course::PlayerBase> player)
{
    auto foundPlayer = std::find(players_.begin(),players_.end(),player);
    if(foundPlayer != players_.end()){
        players_.erase(foundPlayer);
    }
}

std::shared_ptr<Course::PlayerBase> ObjectManager::getPlayer(
        unsigned int playerIndex)
{
    return players_.at(playerIndex);
}

bool ObjectManager::addBuilding(
        std::shared_ptr<Course::BuildingBase> building)
{
    buildings_.push_back(building);
    return true;
}

bool ObjectManager::removeBuilding(
        std::shared_ptr<Course::BuildingBase> building)
{
    auto foundBuilding = std::find(buildings_.begin(),buildings_.end(),building);
    if(foundBuilding != buildings_.end()){
        buildings_.erase(foundBuilding);
        return true;
    }
    return false;
}

bool ObjectManager::addWorker(std::shared_ptr<Course::WorkerBase> worker)
{
    workers_.push_back(worker);
    return true;
}

bool ObjectManager::removeWorker(std::shared_ptr<Course::WorkerBase> worker)
{
    auto foundWorker = std::find(workers_.begin(),workers_.end(),worker);
    if(foundWorker != workers_.end()){
        workers_.erase(foundWorker);
        return true;
    }
    return false;
}


}
