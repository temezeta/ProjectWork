#ifndef GAMEEVENTHANDLER_HH
#define GAMEEVENTHANDLER_HH

#include "../../Course/CourseLib/interfaces/igameeventhandler.h"
#include <map>
#include "core/playerbase.h"

namespace Essi {

/**
 * @brief The GameEventHandler class for managing the game
 */
class GameEventHandler : public Course::iGameEventHandler {

public:
    /**
     * @brief GameEventHandler constructor for the class
     */
    GameEventHandler();
    /**
     * @brief Default destructor
     */
    virtual ~GameEventHandler() = default;
    /**
     * @brief modifyResource
     * @param player whose resource to modify
     * @param resource which resource to modify
     * @param amount how much to modify
     * @return
     */
    bool modifyResource(std::shared_ptr<Course::PlayerBase> player,
                                Course::BasicResource resource,
                                int amount);
    /**
     * @brief modifyResources
     * @param player whose resource to modify
     * @param resources which resources to modify and how much
     * @return
     */
    bool modifyResources(std::shared_ptr<Course::PlayerBase> player,
                                 Course::ResourceMap resources);
    /**
     * @brief addPlayer Add a new player and create their resources
     * @param player pointer to the new player
     */
    void addPlayer(std::shared_ptr<Course::PlayerBase> player);

    /**
     * @brief getResource Returns the amount of a resource player has
     * @param player pointer to the player
     * @param resource the resource wanted
     * @return amount of the resource
     */
    int getResource(std::shared_ptr<Course::PlayerBase> player,
                    Course::BasicResource resource);

    /**
     * @brief setHQstatus Define whether player has headquarters on the map
     * or not
     * @param player pointer to the player
     * @param status true if headquarters exist, false if not
     */
    void setHQstatus(std::shared_ptr<Course::PlayerBase> player, bool status);

    /**
     * @brief getHQstatus Return the headquarters status
     * @param player pointer to the player
     * @return true if headquarters exist, false if not
     */
    bool getHQstatus(std::shared_ptr<Course::PlayerBase> player);
    /**
     * @brief getResources
     * @param player player whose resources to return
     * @return that players current resources
     */
    Course::ResourceMap getResources(std::shared_ptr<Course::PlayerBase> player);
private:
    /**
     * @brief playerResources_ stores information about player resources
     */
    std::map<std::shared_ptr<Course::PlayerBase>, Course::ResourceMap> \
    playerResources_ = {};
    /**
     * @brief HQStatus_ true if a player has headquarters set on the game scene
     */
    std::map<std::shared_ptr<Course::PlayerBase>, bool> HQStatus_;
};

}

#endif // GAMEEVENTHANDLER_HH
