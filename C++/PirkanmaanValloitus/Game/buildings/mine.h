#ifndef MINE_H
#define MINE_H

#include "../CourseLib/buildings/buildingbase.h"
#include "../Game/core/gameeventhandler.hh"
#include "../Game/core/objectmanager.h"
#include "../Game/core/misc.h"

namespace Essi {
/**
 * @brief The Mine class represents a mine building in the game
 * Increases production of money, stone and ore
 * Can only be placed on Mountains
 */
class Mine : public Course::BuildingBase
{
public:
    /**
     * @brief Disabled parameterless constructor.
     */
    Mine() = delete;

    /**
     * @brief Mine Constructor for the Mine class
     * @param eventhandler points to the eventhandler
     * @param objectmanager points to the objectmanager
     * @param owner owns the building
     */
    explicit Mine(const std::shared_ptr<Essi::GameEventHandler>& eventhandler,
         const std::shared_ptr<Teemu::ObjectManager>& objectmanager,
         const std::shared_ptr<Course::PlayerBase>& owner,
         const int& tilespaces = 1,
         const Course::ResourceMap& buildcost = Teemu::MINE_BUILD_COST,
         const Course::ResourceMap& production = Teemu::MINE_PRODUCTION
            );

    /**
     * @brief Default destructor.
     */
    virtual ~Mine() = default;

    /**
     * @brief returns building type
     * @return "Mine"
     */
    virtual std::string getType() const override;

    /**
     * @brief Return's mine's production as a ResourceMap.
     */
    virtual Course::ResourceMap getProduction() override;

    /**
     * @brief canBePlacedOnTile checks if Mine can be placed on tile
     * @param target pointer to the tile building is being placed
     * @return true, if placement is successful
     */
    virtual bool canBePlacedOnTile(
            const std::shared_ptr<Course::TileBase> &target) const override;
};

}

#endif // MINE_H
