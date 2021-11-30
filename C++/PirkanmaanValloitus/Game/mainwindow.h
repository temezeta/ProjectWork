#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>
#include <QScrollBar>
#include <algorithm>

#include "core/gameeventhandler.hh"
#include "core/objectmanager.h"
#include "core/worldgenerator.h"
#include "core/playerbase.h"
#include "core/misc.h"
#include "core/perlinnoise.h"

#include "graphics/gamescene.h"
#include "graphics/graphicsitem.h"

#include "tiles/forest.h"
#include "tiles/water.h"
#include "tiles/mountain.h"
#include "tiles/village.hh"
#include "tiles/grassland.h"

#include "buildings/headquarters.h"
#include "buildings/outpost.h"
#include "buildings/farm.h"
#include "buildings/barracks.h"
#include "buildings/mine.h"

#include "workers/basicworker.h"
#include "workers/soldier.h"
#include "workers/farmer.h"
#include "workers/miner.h"
#include "workers/saboteur.h"

#include "exceptions/invalidpointer.h"
#include "exceptions/keyerror.h"
#include "exceptions/ownerconflict.h"
#include "exceptions/notenoughspace.h"

namespace Ui {
class MainWindow;
}
/**
 * @brief The MainWindow class is the mainwindow of the game
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    /**
     * @brief Initializes game
     */
    void init();
public slots:
    /**
     * @brief setSeed Get seed from dialog window
     * @param seed
     */
    void setSeed(unsigned int seed);
    /**
     * @brief setPlayers Get player amount from dialog window
     * @param players
     */
    void setPlayers(unsigned int players);
    /**
     * @brief addPlayers creates new players
     * @param playerNames
     */
    void addPlayers(std::vector<std::string> playerNames);

    /**
     * @brief setCurrentlySelected: sets currently selected tile
     * @param selected: tile that was clicked
     */
    void setCurrentlySelected(std::shared_ptr<Course::TileBase> selected);
    /**
     * @brief wheelEventSlot update scene on wheel event
     */
    void wheelEventSlot();

private slots:
    /**
     * @brief on_pushButtonEndTurn_clicked End of turn actions
     */
    void on_pushButtonEndTurn_clicked();

    /**
     * @brief on_pushButtonHQ_clicked Add HQ to tile
     */
    void on_pushButtonHQ_clicked();
    /**
     * @brief on_pushButtonOutpost_clicked Add Outpost to tile
     */
    void on_pushButtonOutpost_clicked();
    /**
     * @brief on_pushButtonFarm_clicked Add Farm to tile
     */
    void on_pushButtonFarm_clicked();
    /**
     * @brief on_pushButtonMine_clicked Add Mine to tile
     */
    void on_pushButtonMine_clicked();
    /**
     * @brief on_pushButtonBarracks_clicked Add Barracks to tile
     */
    void on_pushButtonBarracks_clicked();
    /**
     * @brief on_pushButtonAssign_clicked assign a worker to tile
     */
    void on_pushButtonAssign_clicked();
    /**
     * @brief on_pushButtonFocus_clicked set resource focus for all current
     * players workers
     */
    void on_pushButtonFocus_clicked();
    /**
     * @brief on_pushButtonRemove_clicked remove selected worker from current tile
     * if said worker exist in that tile
     */
    void on_pushButtonRemove_clicked();
    /**
     * @brief on_pushButtonFoodBuy_clicked buy food from village
     */
    void on_pushButtonFoodBuy_clicked();
    /**
     * @brief on_pushButtonWoodBuy_clicked buy wood from village
     */
    void on_pushButtonWoodBuy_clicked();
    /**
     * @brief on_pushButtonStoneBuy_clicked buy stone from village
     */
    void on_pushButtonStoneBuy_clicked();
    /**
     * @brief on_pushButtonOreBuy_clicked buy ore from village
     */
    void on_pushButtonOreBuy_clicked();
    /**
     * @brief on_pushButtonFoodSell_clicked sell food to village
     */
    void on_pushButtonFoodSell_clicked();
    /**
     * @brief on_pushButtonWoodSell_clicked sell wood to village
     */
    void on_pushButtonWoodSell_clicked();
    /**
     * @brief on_pushButtonStoneSell_clicked sell stone to village
     */
    void on_pushButtonStoneSell_clicked();
    /**
     * @brief on_pushButtonOreSell_clicked sell ore to village
     */
    void on_pushButtonOreSell_clicked();
    /**
     * @brief on_pushButtonPlus_clicked zoom in gamescene
     */
    void on_pushButtonPlus_clicked();
    /**
     * @brief on_pushButtonMinus_clicked zoom out gamescene
     */
    void on_pushButtonMinus_clicked();

private:
    /**
     * @brief updateScene update gamescene
     */
    void updateScene();
    /**
     * @brief generateWorld Generate gameworld with given seed and player amount
     */
    void generateWorld();
    /**
     * @brief createObjects Add objects (tiles) to the gamescene
     */
    void createObjects();
    /**
     * @brief updateUI updates necessary UI elements at the end of turn
     */
    void updateUI();
    /**
     * @brief updateSelectedTile updates selected tile display
     */
    void updateSelectedTile();
    /**
     * @brief updateLCDNumbers updates LCD displayes to display resource changes
     */
    void updateLCDNumbers();

    /**
     * @brief removeWorker removes worker with given type from selected tile
     * if tile has selected worker type
     * @param type
     */
    void removeWorker(std::string type);
    /**
     * @brief errorMessageMaker creates errormessage for needed resources to build
     * somethings if you don't have enough to build it
     * @param resource
     * @return errormessage string
     */
    std::string errorMessageMaker(Course::ResourceMap resources);

    /**
     * @brief updateVillageUI updates village tradings UI
     */
    void updateVillageUI();

    /**
     * @brief disableUI used to disable UI until HQ is added to that player
     */
    void disableUI(bool value);

    /**
     * @brief tradeWithVillage do a trade with selected village tile
     * @param amount amount of resources to trade
     * @return true if succesful, false otherwise
     */
    bool tradeWithVillage(Course::ResourceMap amount);
    /**
     * @brief addOnBuildActionTiles adds tiles got from Outpost and HQ
     * onBuildActions to the players objects
     */
    void addOnBuildActionTiles();
    /**
     * @brief returnResources returns resources to player if worker/building
     * could not be added
     * @param resources
     */
    void returnResources(Course::ResourceMap resources);

    //Templated function definitions in header
    template<typename T>
    /**
     * @brief addBuilding adds building with typename T to selected tile
     * @param resource buildcost for building
     * @return true if building was placed, false otherwise
     */
    bool addBuilding(Course::ResourceMap resource){
        Course::ResourceMap buildcost = Teemu::negativeResourceMap(resource);
        std::shared_ptr<T> newBuilding = std::make_shared<T>(GEHandler_,
                                                            objectmanager_,
                                                            currentPlayer_);
        if(currentSelected_ != nullptr
                && GEHandler_->modifyResources(currentPlayer_,buildcost)){
                currentSelected_->addBuilding(newBuilding);
                objectmanager_->addBuilding(newBuilding);

                if(currentSelected_->getOwner() == nullptr){
                    currentSelected_->setOwner(currentPlayer_);
                    }
        }
        else{
            return false;
        }
        //Add tile to owned tiles if it isn't there already
        auto ownedTiles = currentPlayer_->getObjects();
        if(std::find(ownedTiles.begin(),ownedTiles.end(),currentSelected_)
                == ownedTiles.end()){
            currentPlayer_->addObject(currentSelected_);
        }
        newBuilding->onBuildAction();
        updateSelectedTile();
        updateLCDNumbers();
        return true;
    }

    template<typename U>
    /**
     * @brief addWorker adds any type of worker to a tile
     * @param resource recruitmentcost of worker
     * @return true if worker was placed, false otherwise
     */
    bool addWorker(Course::ResourceMap resource){
        Course::ResourceMap recruitmentcost = Teemu::negativeResourceMap(resource);
        std::shared_ptr<U> newWorker = std::make_shared<U>(GEHandler_,
                                                            objectmanager_,
                                                            currentPlayer_);
        if(currentSelected_ != nullptr
            && GEHandler_->modifyResources(currentPlayer_,recruitmentcost)){
            currentSelected_->addWorker(newWorker);
            objectmanager_->addWorker(newWorker);
        }
        else{
            return false;
        }
        newWorker->doSpecialAction();
        updateSelectedTile();
        updateLCDNumbers();
        return true;
    }

    Ui::MainWindow *ui;
    std::shared_ptr<Essi::GameEventHandler> GEHandler_;
    std::shared_ptr<Teemu::GameScene> gamescene_;

    unsigned int seed_;
    unsigned int players_;
    unsigned int round_;
    /**
     * @brief roundDivider_ used to determine which players turn it is
     */
    unsigned int roundDivider_;
    std::shared_ptr<Teemu::ObjectManager> objectmanager_;
    /**
     * @brief currentSelected_ points to currently selected tile or nullptr
     */
    std::shared_ptr<Course::TileBase> currentSelected_;
    std::shared_ptr<Course::PlayerBase> currentPlayer_;
};

#endif // MAINWINDOW_H
