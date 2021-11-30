#include <QString>
#include <QtTest>

#include "../../Game/core/gameeventhandler.hh"
#include "../../Course/CourseLib/core/playerbase.h"

class GameEventHandler_tests : public QObject
{
    Q_OBJECT

public:
    GameEventHandler_tests();

private Q_SLOTS:
    /**
     * @brief addPlayer tests adding a player and creating player resources
     */
    void addPlayer();

    /**
     * @brief modifyResource tests player resource modification with a single
     * resource
     */
    void modifyResource();

    /**
     * @brief modifyResources tests player resource modification with resource
     * maps
     */
    void modifyResources();

    /**
     * @brief getResource tests that getResource returns the right resource
     */
    void getResource();

    /**
     * @brief getResources tests that getResources returns the right
     * resource map
     */
    void getResources();

    /**
     * @brief getHQStatus tests that the returned headquarters status is right
     */
    void getHQStatus();

    /**
     * @brief setHQstatus tests that the headquarters status changes
     */
    void setHQstatus();

private:
    std::shared_ptr<Essi::GameEventHandler> GEH;
    std::shared_ptr<Course::PlayerBase> TESTPLAYER;
    Course::ResourceMap DEFAULTRESOURCES = {
        {Course::BasicResource::MONEY, 1000},
        {Course::BasicResource::FOOD, 1000},
        {Course::BasicResource::WOOD, 1000},
        {Course::BasicResource::STONE, 1000},
        {Course::BasicResource::ORE, 1000}, };
    Course::ResourceMap EMPTYRESOURCES = {
        {Course::BasicResource::MONEY, 0},
        {Course::BasicResource::FOOD, 0},
        {Course::BasicResource::WOOD, 0},
        {Course::BasicResource::STONE, 0},
        {Course::BasicResource::ORE, 0}, };
    Course::ResourceMap NEGATIVERESOURCES = {
        {Course::BasicResource::MONEY, -1000},
        {Course::BasicResource::FOOD, -1000},
        {Course::BasicResource::WOOD, -1000},
        {Course::BasicResource::STONE, -1000},
        {Course::BasicResource::ORE, -1000}, };
};

GameEventHandler_tests::GameEventHandler_tests()
{
    GEH = std::make_shared<Essi::GameEventHandler>();
    TESTPLAYER = std::make_shared<Course::PlayerBase>("TP");
}

void GameEventHandler_tests::addPlayer()
{
    GEH->addPlayer(TESTPLAYER);
    QCOMPARE(QString::fromStdString(TESTPLAYER->getName()),
             QString::fromStdString("TP"));
    QCOMPARE(GEH->getResources(TESTPLAYER), DEFAULTRESOURCES);
    QVERIFY(GEH->getHQstatus(TESTPLAYER) == false);
}

void GameEventHandler_tests::modifyResource()
{
    // Player does not have enough resources to make the modification
    for ( auto resource : DEFAULTRESOURCES ) {
        // Resource modification is not successful
        QVERIFY(GEH->modifyResource(TESTPLAYER, resource.first, -2000)
                == false);
        // Check that the amount of resources do not change
        QCOMPARE(GEH->getResource(TESTPLAYER, resource.first), 1000);
    }

    for ( auto resource : DEFAULTRESOURCES ) {
        // Resource modification was successful
        QVERIFY(GEH->modifyResource(TESTPLAYER, resource.first, -1000)
                == true);
        // Player resources after modification are as expected
        QCOMPARE(GEH->getResource(TESTPLAYER, resource.first), 0);
    }

}

void GameEventHandler_tests::modifyResources()
{
    // Check that resources are not mofified in case player does not have
    // enough resources to make the modification
    QVERIFY(GEH->modifyResources(TESTPLAYER, NEGATIVERESOURCES) == false);
    QCOMPARE(GEH->getResources(TESTPLAYER), EMPTYRESOURCES);

    // Check that resources are modified accordingly when player has enough
    // resources
    QVERIFY(GEH->modifyResources(TESTPLAYER, DEFAULTRESOURCES) == true);
    QCOMPARE(GEH->getResources(TESTPLAYER), DEFAULTRESOURCES);

}

void GameEventHandler_tests::getResource()
{
    for ( auto resource : DEFAULTRESOURCES ) {
        QCOMPARE(GEH->getResource(TESTPLAYER, resource.first),
                 resource.second);
    }
}

void GameEventHandler_tests::getResources()
{
    QCOMPARE(GEH->getResources(TESTPLAYER), DEFAULTRESOURCES);
}

void GameEventHandler_tests::getHQStatus()
{
    QVERIFY(GEH->getHQstatus(TESTPLAYER) == false);
}

void GameEventHandler_tests::setHQstatus()
{
    if ( GEH->getHQstatus(TESTPLAYER) == false ) {
        GEH->setHQstatus(TESTPLAYER, true);
        QCOMPARE(GEH->getHQstatus(TESTPLAYER), true);
    } else {
        GEH->setHQstatus(TESTPLAYER, false);
        QCOMPARE(GEH->getHQstatus(TESTPLAYER), false);
    }

    GEH->setHQstatus(TESTPLAYER, false);
}

QTEST_APPLESS_MAIN(GameEventHandler_tests)

#include "gameeventhandler_tests.moc"
