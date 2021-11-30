#ifndef VILLAGE_HH
#define VILLAGE_HH
#include "tiles/tilebase.h"
#include "core/gameeventhandler.hh"
#include "core/objectmanager.h"
#include "core/misc.h"
namespace Teemu {

/**
 * @brief The Village class represents a village in the gameworld
 *
 * Village tile can be owned but doesn't produce anything and nothing
 * can be placed on it. It is used for trading purposes
 */
class Village : public Course::TileBase
{
public:
    /**
     * @brief Disabled parameterless constructor.
     */
    Village() = delete;

    /**
     * @brief Village constructor
     * @param location  Coordinate of the tile in game
     * @param eventhandler points to created GameEventHandler
     * @param objectmanager points to Objectmanager
     */
    Village(const Course::Coordinate& location,
           const std::shared_ptr<Course::iGameEventHandler>& eventhandler,
           const std::shared_ptr<Course::iObjectManager>& objectmanager,
           const unsigned int& max_build = 0,
           const unsigned int& max_work = 0,
           const Course::ResourceMap& production = VILLAGE_BP);
    /**
     * @brief Default destructor.
     */
    virtual ~Village() = default;

    /**
     * @brief getType returns tile type
     * @return "Village"
     */
    virtual std::string getType() const override;
    /**
     * @brief returnStock
     * @return current village stock
     */
    Course::ResourceMap returnStock();
    /**
     * @brief modifyResources modifys villages stock
     * @return true if success, false otherwise
     */
    bool modifyResources(Course::ResourceMap resources);
private:
    Course::ResourceMap villageStock_;
};


}
#endif // VILLAGE_HH
