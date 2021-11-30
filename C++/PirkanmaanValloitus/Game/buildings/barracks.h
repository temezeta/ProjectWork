#ifndef BARRACKS_H
#define BARRACKS_H

#include "buildings/buildingbase.h"
#include "core/gameeventhandler.hh"
#include "core/objectmanager.h"
#include "core/misc.h"

namespace Teemu {

/**
 * @brief The Barracks class represents a barracks building in the gameworlds
 * Required to assign soldiers
 * Reclaims land around it emptying it and assigning it to nobody
 */
class Barracks : public Course::BuildingBase
{
public:
    /**
     * @brief Disabled parameterless constructor.
     */
    Barracks() = delete;
    /**
     * @brief Barracks Constructor for Barracks
     * @param eventhandler points to gameeventhandler
     * @param objectmanager points to objectmanager
     * @param owner owner of the building
     */
    explicit Barracks(
            const std::shared_ptr<Essi::GameEventHandler>& eventhandler,
            const std::shared_ptr<Teemu::ObjectManager>& objectmanager,
            const std::shared_ptr<Course::PlayerBase>& owner,
            const int& tilespaces = 1,
            const Course::ResourceMap& buildcost = Teemu::BARRACKS_BUILD_COST,
            const Course::ResourceMap& production = Teemu::BARRACKS_PRODUCTION
            );
    /**
     * @brief Default destructor.
     */
    virtual ~Barracks() = default;
    /**
     * @brief getType returns building type
     * @return "Barracks"
     */
    virtual std::string getType() const override;
    /**
     * @brief onBuildAction sets neighbouring tiles ownership to null
     * if the owner of this building doesnt own them or they have HQ or tiles isnt
     * owned by anyone and
     * removes all buildings and workers from them
     */
    virtual void onBuildAction() override;
    /**
     * @brief getProduction
     * @return production of the building
     */
    virtual Course::ResourceMap getProduction() override;
};

}
#endif // BARRACKS_H
