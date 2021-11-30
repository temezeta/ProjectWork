#include <QtTest>

#include "core/objectmanager.h"
#include "core/gameeventhandler.hh"
#include "core/worldgenerator.h"
#include "buildings/headquarters.h"
#include "workers/basicworker.h"
#include "core/basicresources.h"

class ObjectManager_tests : public QObject
{
    Q_OBJECT

public:
    ObjectManager_tests();
    ~ObjectManager_tests();

private Q_SLOTS:
    void addTiles();
    /**
     * @brief getTileByCoordinate Unit tests for tiles that exist with
     * coordinates search
     */
    void getTileByCoordinate();
    /**
     * @brief getTileByCoordinateNotFound Unit tests for tiles that do not exist
     * with coordinates search
     */
    void getTileByCoordinateNotFound();
    /**
     * @brief getTileByID Unit tests for tiles that exist with ID search
     */
    void getTileByID();
    /**
     * @brief getTileByIDNotFound Unit tests for tiles that do not exist with
     * ID search
     */
    void getTileByIDNotFound();
    /**
     * @brief getTilesByCoordinates Unit tests for vector of tiles found by
     * a vector of coordinates which exist
     */
    void getTilesByCoordinates();
    /**
     * @brief getTilesByCoordinatesNotFound test getTiles function with wrong
     * coordinates, empty tiles vector or no coordinates given
     */
    void getTilesByCoordinatesNotFound();
    /**
     * @brief addPlayer Test that add player function adds TESTPLAYER to
     * objectmanagers players_
     */
    void addPlayer();
    /**
     * @brief getPlayer tests that if given valid playerIndex it returns the
     * actual player pointer and not nullptr
     */
    void getPlayer();
    /**
     * @brief getPlayerNotFound tests that the correct error is thrown if
     * player is not found
     */
    void getPlayerNotFound();
    /**
     * @brief removePlayer tests removing a player
     */
    void removePlayer();
    /**
     * @brief removePlayerNotFound tests removing a player which doesn't exist
     */
    void removePlayerNotFound();
    /**
     * @brief addBuilding tests adding a building
     */
    void addBuilding();
    /**
     * @brief removeBuilding tests removing a building
     */
    void removeBuilding();
    /**
     * @brief removeBuildingNotFound tests for removing a building which doesn't
     * exist
     */
    void removeBuildingNotFound();

private:
    const unsigned int DEFAULTSEED = 100;
    const unsigned int PLAYERNUMBER = 2;
    const Course::Coordinate COORDINATE = Course::Coordinate(2,0);
    const Course::ObjectId ID = Course::ObjectId(20);
    const std::vector<Course::Coordinate> COORDINATESVECTOR ={
        Course::Coordinate(2,0),
        Course::Coordinate(2,1),
        Course::Coordinate(2,2),
        Course::Coordinate(2,3)
    };
    const std::vector<Course::Coordinate> DUPLICATECOORDINATESVECTOR ={
        Course::Coordinate(2,0),
        Course::Coordinate(2,0),
        Course::Coordinate(2,0),
        Course::Coordinate(2,0)
    };
    const std::vector<Course::Coordinate> INVALIDCOORDINATESVECTOR ={
        Course::Coordinate(9,66),
        Course::Coordinate(9,66),
        Course::Coordinate(78,5),
        Course::Coordinate(996,55)
    };
    const
    std::vector<std::shared_ptr<Course::TileBase>> EMPTY = {};
    const std::shared_ptr<Essi::GameEventHandler> DEFAULTEVENTHANDLER = nullptr;

    const std::shared_ptr<Teemu::ObjectManager> EMPTYOBJECTMANAGER =
            std::make_shared<Teemu::ObjectManager>();

    const std::shared_ptr<Course::PlayerBase> TESTPLAYER
            = std::make_shared<Course::PlayerBase>("TESTI");
    const std::shared_ptr<Course::HeadQuarters> TESTBUILDING =
            std::make_shared<Course::HeadQuarters>(nullptr,
                                                   objectmanager,
                                                   TESTPLAYER);
    const std::shared_ptr<Course::BasicWorker> TESTWORKER =
            std::make_shared<Course::BasicWorker>(nullptr,
                                                  objectmanager,
                                                  TESTPLAYER);
    std::shared_ptr<Teemu::ObjectManager> objectmanager;
    std::shared_ptr<Essi::GameEventHandler> eventhandler;

};

ObjectManager_tests::ObjectManager_tests()
{
    objectmanager = std::make_shared<Teemu::ObjectManager>();
    Course::WorldGenerator * World = &Course::WorldGenerator::getInstance();
    World->addConstructor<Course::Forest> (30);
    World->addConstructor<Course::Grassland> (25);
    World->addConstructor<Teemu::Mountain> (15);
    World->addConstructor<Teemu::Water> (25);
    World->addConstructor<Teemu::Village> (5);
    World->generateMap(5*PLAYERNUMBER,5*PLAYERNUMBER,DEFAULTSEED,objectmanager
                       ,DEFAULTEVENTHANDLER);

}

ObjectManager_tests::~ObjectManager_tests()
{
}

void ObjectManager_tests::addTiles()
{
    //WorldGenerator succesfully added tiles to object generator if you get X
    //number of tiles where X = 25 * PLAYERNUMBER * PLAYERNUMBER
    std::vector<Course::Coordinate> coordinates;
    for (unsigned int i = 0; i < 10; i++){
        for(unsigned int j = 0; j < 10; j++){
            Course::Coordinate newCoordinate = Course::Coordinate(i,j);
            coordinates.push_back(newCoordinate);
        }
    }
    //Test that each coordinate is actually a tile and not a nullptr
    for(auto coordinate: coordinates){
        auto tile = objectmanager->getTile(coordinate);
        QVERIFY(tile != nullptr);
    }
    //Test that all the tiles are there
    auto tiles = objectmanager->getTiles(coordinates);
    QVERIFY(tiles.size() == 100);
}

void ObjectManager_tests::getTileByCoordinate()
{
    //Check that  tile return with valid coordinate is not nullptr
    QVERIFY(objectmanager->getTile(COORDINATE) != nullptr);
    //Check that tile got by coordinates is same as tile returned by ID
    auto getID = objectmanager->getTile(COORDINATE)->ID;
    QVERIFY(objectmanager->getTile(COORDINATE) == objectmanager->getTile(getID));
    //Test that different ID returns different tile
    QVERIFY(objectmanager->getTile(COORDINATE) !=
            objectmanager->getTile(Course::Coordinate(2,1)));
}

void ObjectManager_tests::getTileByCoordinateNotFound()
{
    //Tile with given coordinate does not exist (tiles vector is empty)
    QVERIFY(EMPTYOBJECTMANAGER->getTile(COORDINATE) == nullptr);
    //Tile with given coordinate does not exist
    QVERIFY(objectmanager->getTile(Course::Coordinate(1000,1000)) == nullptr);
}

void ObjectManager_tests::getTileByID()
{
    //Check that  tile return with valid ID is not nullptr
    QVERIFY(objectmanager->getTile(ID) != nullptr);
    //Check that tile got by ID is same as tile returned by coordinates
    auto coordinate = objectmanager->getTile(ID)->getCoordinate();
    QVERIFY(objectmanager->getTile(ID) == objectmanager->getTile(coordinate));
    //Test that different ID returns different tile
    QVERIFY(objectmanager->getTile(ID) !=
            objectmanager->getTile(21));
}

void ObjectManager_tests::getTileByIDNotFound()
{
    //Tile with given ID does not exist (tiles vector empty)
    QVERIFY(EMPTYOBJECTMANAGER->getTile(ID) == nullptr);
    //Tile with given ID does not exist
    QVERIFY(objectmanager->getTile(100000) == nullptr);
}

void ObjectManager_tests::getTilesByCoordinates()
{
    //Vector contains unique coordinates
    //Found tiles is not an empty vector if coordinates were right
    auto tilesByVector = objectmanager->getTiles(COORDINATESVECTOR);
    QVERIFY(tilesByVector != EMPTY);
    for (auto tile: tilesByVector){
        //Each tile actually points to a tile
        QVERIFY(tile != nullptr);
    }
    //Same amount of tiles is returned as unique coordinates given
    QVERIFY(tilesByVector.size() == COORDINATESVECTOR.size());

    //Vector contain duplicate but valid coordinates
    auto tilesByVector2 = objectmanager->getTiles(DUPLICATECOORDINATESVECTOR);
    QVERIFY(tilesByVector2 != EMPTY);
    for (auto tile: tilesByVector2){
        QVERIFY(tile != nullptr);
    }
    //No duplicate tiles returned
    QVERIFY(tilesByVector2.size() != COORDINATESVECTOR.size());
    //There was one unique coordinate given in the vector
    QVERIFY(tilesByVector2.size() == 1);
}

void ObjectManager_tests::getTilesByCoordinatesNotFound()
{
    //Coordinate given were invalid
    auto tilesByVector = objectmanager->getTiles(INVALIDCOORDINATESVECTOR);
    QVERIFY(tilesByVector == EMPTY);
    //No coordinates were given
    auto tilesByVector2 = objectmanager->getTiles({});
    QVERIFY(tilesByVector2 == EMPTY);
    //Trying to search for tiles in an empty tiles vector
    auto tilesVectorEmpty = EMPTYOBJECTMANAGER->getTiles(COORDINATESVECTOR);
    QVERIFY(tilesVectorEmpty == EMPTY);
}

void ObjectManager_tests::addPlayer()
{
    objectmanager->addPlayer(TESTPLAYER);
    //Player was added to objectmanager and added player points to the same player
    QCOMPARE(objectmanager->getPlayer(0), TESTPLAYER);

}

void ObjectManager_tests::getPlayer()
{
    //Added player is testplayer
    QVERIFY(objectmanager->getPlayer(0) == TESTPLAYER);
    //Added player is not a nullptr
    QVERIFY(objectmanager->getPlayer(0) != nullptr);

}

void ObjectManager_tests::getPlayerNotFound()
{
    //Player with given index doesnt exist
    QVERIFY_EXCEPTION_THROWN(objectmanager->getPlayer(1),std::out_of_range);
}

void ObjectManager_tests::removePlayer()
{
    objectmanager->removePlayer(TESTPLAYER);
    //If testplayer was removed, players_ vector should now be empty
    QVERIFY_EXCEPTION_THROWN(objectmanager->getPlayer(0),std::out_of_range);
}

void ObjectManager_tests::removePlayerNotFound()
{
    objectmanager->addPlayer(TESTPLAYER);
    std::shared_ptr<Course::PlayerBase> newPlayer =
            std::make_shared<Course::PlayerBase>("New");
    objectmanager->removePlayer(newPlayer);
    //No player was removed if player to remove isn't found
    QCOMPARE(objectmanager->getPlayer(0),TESTPLAYER);
}

void ObjectManager_tests::addBuilding()
{
    //Add building to objectmanager
    QVERIFY(objectmanager->addBuilding(TESTBUILDING) == true);
}

void ObjectManager_tests::removeBuilding()
{
    //Test if building added can be removed
    QVERIFY(objectmanager->removeBuilding(TESTBUILDING) == true);
}

void ObjectManager_tests::removeBuildingNotFound()
{
    QVERIFY(objectmanager->removeBuilding(TESTBUILDING) == false);
}


QTEST_APPLESS_MAIN(ObjectManager_tests)

#include "objectmanager_tests.moc"
