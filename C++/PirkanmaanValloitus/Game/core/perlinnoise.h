#ifndef PERLINNOISE_H
#define PERLINNOISE_H

#include <functional>
#include <map>
#include <memory>

#include "core/gameeventhandler.hh"
#include "core/objectmanager.h"
#include "tiles/tilebase.h"
#include <math.h>

namespace Teemu {

using TileConstructorPointer = std::function<std::shared_ptr<Course::TileBase>(
    Course::Coordinate,
    std::shared_ptr<Essi::GameEventHandler>,
    std::shared_ptr<ObjectManager>)>;
/**
 * @brief The PerlinNoise class is a singleton for generating tiles for the game
 *
 * 1. Get Instance
 * 2. Add constructor
 * 3. Generate map
 */
class PerlinNoise
{
public:
    /**
     * @brief Used to get a reference to the Singleton instance.
     * @return Reference to the Singleton instance.
     * @post Exception guarantee: No-throw
     */
    static PerlinNoise& getInstance();

    //Prevent copy and move
    PerlinNoise(const PerlinNoise&) = delete;
    PerlinNoise& operator=(const PerlinNoise&) = delete;
    PerlinNoise(PerlinNoise&&) = delete;
    PerlinNoise& operator=(PerlinNoise&&) = delete;

    template<typename T>
    /**
     * @brief addType add tiles constructor
     * @param weight
     */
    void addType(unsigned int weight){
        TileConstructorPointer ctor = std::make_shared<T, Course::Coordinate,
                std::shared_ptr<Essi::GameEventHandler>,
                std::shared_ptr<ObjectManager> >;
        m_ctors.insert(std::make_pair(weight, ctor));
    }

    /**
     * @brief Generates Tile-objects and sends them to ObjectManager.
     * @param sizeX is the horizontal size of the map area.
     * @param sizeY is the vertical size of the map area.
     * @param seed is the seed-value used in the generation.
     * @param objectmanager points to the ObjectManager that receives the
     * generated Tiles.
     * @param eventhandler points to the student's GameEventHandler.
     * @post Exception guarantee: No-throw
     */
    void generateMap(unsigned int sizeX,
                     unsigned int sizeY,
                     unsigned int seed,
                     const std::shared_ptr<ObjectManager>& objectmanager,
                     const std::shared_ptr<Essi::GameEventHandler>& eventhandler);
private:
    /**
     * @brief Default constructor.
     */
    PerlinNoise() = default;

    /**
     * @brief Default destructor.
     */
    ~PerlinNoise() = default;
    /**
     * @brief Find the Tile ctor matching the random number.
     * @param random is the random number being matched to a Tile.
     * @return The constructor matching the random number.
     */
    TileConstructorPointer findNoiseCtor(int noise) const;

    std::multimap<unsigned int, TileConstructorPointer> m_ctors;
};


}
#endif // PERLINNOISE_H
