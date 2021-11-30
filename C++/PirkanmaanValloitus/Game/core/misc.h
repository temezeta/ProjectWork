#ifndef MISC_H
#define MISC_H

#include "core/basicresources.h"
#include <QString>
namespace Teemu {
//Miscellaneous functions
/**
 * @brief negativeResourceMap returns negative version of the input resourcemap
 * @param resourcemap
 * @return resourcemap * -1 for each resource
 */
Course::ResourceMap negativeResourceMap(Course::ResourceMap resourcemap);

//Production maps
const Course::ResourceMap MOUNTAIN_BP = {
        {Course::MONEY, 4},
        {Course::FOOD, 0},
        {Course::WOOD, 0},
        {Course::STONE, 5},
        {Course::ORE, 3}

};

const Course::ResourceMap VILLAGE_BP = {
    {Course::MONEY, 0},
    {Course::FOOD, 0},
    {Course::WOOD, 0},
    {Course::STONE, 0},
    {Course::ORE, 0}
};

const Course::ResourceMap WATER_BP = {
    {Course::MONEY, 2},
    {Course::FOOD, 5},
    {Course::WOOD, 0},
    {Course::STONE, 2},
    {Course::ORE, 0}
};

//Buildings
//Mine
const Course::ResourceMap MINE_BUILD_COST ={
    {Course::MONEY, 100},
    {Course::WOOD, 100},
    {Course::STONE, 50},
    {Course::ORE, 50}
};
const Course::ResourceMap MINE_PRODUCTION ={
    {Course::MONEY,2},
    {Course::STONE,5},
    {Course::ORE,3}
};

//Barracks
const Course::ResourceMap BARRACKS_BUILD_COST ={
    {Course::MONEY, 500},
    {Course::WOOD, 200},
    {Course::STONE, 200},
    {Course::ORE, 100}
};

const Course::ResourceMap BARRACKS_PRODUCTION ={
    {Course::MONEY,-10},
    {Course::FOOD,-4},
    {Course::WOOD,-2}
};

//Workers
//Farmer
const Course::ResourceMap FARMER_RECRUITMENT_COST ={
    {Course::MONEY,50},
    {Course::FOOD,40}
};

const Course::ResourceMapDouble FARMER_WORKER_EFFICIENCY ={
    {Course::MONEY, 0.20},
    {Course::FOOD,1.5},
    {Course::WOOD, 1},
    {Course::STONE, 0.1},
    {Course::ORE, 0.1}
};

//Miner
const Course::ResourceMap MINER_RECRUITMENT_COST ={
    {Course::MONEY,50},
    {Course::FOOD,40}
};

const Course::ResourceMapDouble MINER_WORKER_EFFICIENCY ={
    {Course::MONEY, 0.20},
    {Course::FOOD, 0.1},
    {Course::WOOD, 0.1},
    {Course::STONE, 1.5},
    {Course::ORE, 1}
};

//Saboteur
const Course::ResourceMap SABOTEUR_RECRUITMENT_COST ={
    {Course::MONEY,200},
    {Course::FOOD,50},
    {Course::STONE,50},
    {Course::ORE,50}
};

const Course::ResourceMapDouble SABOTEUR_WORKER_EFFICIENCY ={
    {Course::MONEY, 0.0},
    {Course::FOOD, 0.0},
    {Course::WOOD, 0.0},
    {Course::STONE, 0.0},
    {Course::ORE, 0.0}
};

//Soldier
const Course::ResourceMap SOLDIER_RECRUITMENT_COST ={
    {Course::MONEY,500},
    {Course::FOOD,200},
    {Course::STONE,50},
    {Course::ORE,50}
};

const Course::ResourceMapDouble SOLDIER_WORKER_EFFICIENCY ={
    {Course::MONEY, 0.0},
    {Course::FOOD, 0.0},
    {Course::WOOD, 0.0},
    {Course::STONE, 0.0},
    {Course::ORE, 0.0}
};

//Other

const Course::ResourceMap VILLAGE_STOCK = {
    {Course::MONEY, 0},
    {Course::FOOD, 100},
    {Course::WOOD, 100},
    {Course::STONE, 0},
    {Course::ORE, 0}
};

const std::map<std::string, QString> IMAGE_LOCATIONS = {
    {"Forest", ":/Images/forest.png"},
    {"Mountain", ":/Images/mountain.png"},
    {"Grassland", ":/Images/grassland.png"},
    {"Village", ":/Images/village.png"},
    {"Water", ":/Images/water.png"},
    {"House", ":/Images/house.png"},
    {"Person", ":/Images/person.png"}
};

}

#endif // MISC_H
