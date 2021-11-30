#include "perlinnoise.h"

namespace Teemu {
/**
 * @brief Noise generate noise based on seed, x and y
 * @param x
 * @param y
 * @param seed
 * @return
 */
int  Noise(unsigned int x,unsigned int y, unsigned int seed, unsigned int weight){
    return 2*weight*pow(cos(sqrt(x+x/(y+1))),2) + weight*pow(cos(sqrt(y+x)),2) +seed;
}

PerlinNoise &PerlinNoise::getInstance()
{
    static PerlinNoise instance;
    return instance;
}

void PerlinNoise::generateMap(unsigned int sizeX,
                    unsigned int sizeY,
                    unsigned int seed,
                    const std::shared_ptr<ObjectManager> &objectmanager,
                    const std::shared_ptr<Essi::GameEventHandler> &eventhandler)
{
    unsigned int totalWeight = 0;
    for (const auto& ctor : m_ctors)
    {
        totalWeight += ctor.first;
    }
    std::vector<std::shared_ptr<Course::TileBase>> tiles;
    for (unsigned int x = 0; x < sizeX; ++x)
        {
            for (unsigned int y = 0; y < sizeY; ++y)
            {
                auto ctor = findNoiseCtor(Noise(x,y,seed,totalWeight) % totalWeight);
                tiles.push_back(ctor(Course::Coordinate(x, y), eventhandler, objectmanager));
            }
        }

        objectmanager->addTiles(tiles);

}

TileConstructorPointer PerlinNoise::findNoiseCtor(int noise) const
{
    while (noise >= 0)
    {
        for (const auto& ctor : m_ctors)
        {
            noise -= ctor.first;
            if (noise < 0)
            {
                return ctor.second;
            }
        }
    }
    return {};
}

}
