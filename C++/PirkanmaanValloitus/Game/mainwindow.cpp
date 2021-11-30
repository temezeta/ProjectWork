#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "graphics/graphicsitem.h"
#include "core/resourcemaps.h"

#include <math.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    GEHandler_(std::make_shared<Essi::GameEventHandler>()),
    gamescene_(new Teemu::GameScene(this)),
    seed_(0),
    players_(2),
    round_(2),
    roundDivider_(2),
    objectmanager_(std::make_shared<Teemu::ObjectManager> ()),
    currentSelected_(nullptr),
    currentPlayer_(nullptr)
{
    ui->setupUi(this);

   Teemu::GameScene* gameScenePointer = gamescene_.get();
   ui->graphicsView->setScene(dynamic_cast<QGraphicsScene*>(gameScenePointer));

   connect(gamescene_.get(),&Teemu::GameScene::sendSelected,this,
           &MainWindow::setCurrentlySelected);
   connect(gamescene_.get(),&Teemu::GameScene::sendWheelEvent,this,
           &MainWindow::wheelEventSlot);

   ui->selectedTileLabel->setScaledContents(true);
   ui->selectedTileLabel->setText("No tile selected");
   ui->frameVillage->setVisible(false);

   ui->graphicsView->setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
   ui->graphicsView->setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
   ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::generateWorld()
{
    Teemu::PerlinNoise *World = &Teemu::PerlinNoise::getInstance();
    World->addType<Course::Forest> (350);
    World->addType<Course::Grassland> (250);
    World->addType<Teemu::Mountain> (200);
    World->addType<Teemu::Water> (200);
    World->addType<Teemu::Village> (20);
    World->generateMap(10*players_,10*players_,seed_,objectmanager_,GEHandler_);
}

void MainWindow::createObjects()
{
    //Coordinate system starts at top left (0,0)
    unsigned int size = 10*players_;
    gamescene_->setSize(size,size);
    gamescene_->setScale(50/players_);

    for(unsigned int i = 0; i < size ;i++){
        for(unsigned int a = 0; a < size ;a++){
            Course::Coordinate cd(i,a);
            auto tile = objectmanager_->getTile(cd);
            gamescene_->drawItem(tile);
        }
    }
}

void MainWindow::init()
{
    round_ = players_;
    roundDivider_ = players_;
    ui->labelRound->setText(QString::number(floor(round_/roundDivider_)));
    currentPlayer_ = objectmanager_->getPlayer(0);
    disableUI(not GEHandler_->getHQstatus(currentPlayer_));
    ui->labelPlayer->setText(QString::fromStdString(currentPlayer_->getName()));
    updateUI();
    generateWorld();
    createObjects();
}

void MainWindow::setPlayers(unsigned int players)
{
    players_ = players;
}

void MainWindow::addPlayers(std::vector<std::string> playerNames)
{
    for ( unsigned int i = 0 ; i < playerNames.size() ; i++ ) {
        std::shared_ptr<Course::PlayerBase> newPlayer =
                std::make_shared<Course::PlayerBase>(playerNames.at(i));
        GEHandler_->addPlayer(newPlayer);
        objectmanager_->addPlayer(newPlayer);
    }
}

void MainWindow::updateUI()
{
    ui->labelRound->setText(QString::number(floor(round_/roundDivider_)));
    ui->labelPlayer->setText(QString::fromStdString(currentPlayer_->getName()));
    updateLCDNumbers();
}

void MainWindow::updateSelectedTile()
{
    //No tile currently selected
    if(currentSelected_ == nullptr){
        ui->selectedTileLabel->clear();
        ui->selectedTileLabel->setText("No tile selected");
        ui->typeLabel->clear();
        ui->ownerLabel->clear();
        ui->buildingsLabel->clear();
        ui->workersLabel->clear();
    }

    else{
        //Set tiletype display
        std::string type = currentSelected_->getType();
        QPixmap pixmap = QPixmap(Teemu::IMAGE_LOCATIONS.at(type));
        ui->selectedTileLabel->setPixmap(pixmap);

        //Set ownership information (owner,buildings, workers)
        if(currentSelected_->getOwner() == nullptr){
            ui->typeLabel->setText(QString::fromStdString(
                                       currentSelected_->getType()));
            ui->ownerLabel->clear();
            ui->buildingsLabel->clear();
            ui->workersLabel->clear();
        }
        //If tile is owned by someone, update its information
        else{
            ui->typeLabel->setText(QString::fromStdString(
                                       currentSelected_->getType()));
            //Update owner name
            std::string owner = currentSelected_->getOwner()->getName();
            if(owner == currentPlayer_->getName()){
                ui->ownerLabel->setText("You");
            }
            else{
                ui->ownerLabel->setText(QString::fromStdString(owner));
            }

            //Update worker display
            auto workers = currentSelected_->getWorkers();
            std::string workerString = "";
            for(auto worker : workers){
                workerString += worker->getType() + " ";
            }
            ui->workersLabel->setText(QString::fromStdString(workerString));

            //Update buildings display
            auto buildings = currentSelected_->getBuildings();
            std::string buildingsString = "";
            for(auto building: buildings){
                buildingsString += building->getType() + " ";
            }
            ui->buildingsLabel->setText(QString::fromStdString(buildingsString));
        }
    }
    updateScene();
}

void MainWindow::updateLCDNumbers()
{
    auto resources = GEHandler_->getResources(currentPlayer_);
    ui->lcdNumberMoney->display(resources.at(Course::BasicResource::MONEY));
    ui->lcdNumberFood->display(resources.at(Course::BasicResource::FOOD));
    ui->lcdNumberWood->display(resources.at(Course::BasicResource::WOOD));
    ui->lcdNumberStone->display(resources.at(Course::BasicResource::STONE));
    ui->lcdNumberOre->display(resources.at(Course::BasicResource::ORE));
}

void MainWindow::removeWorker(std::string type)
{
    std::shared_ptr<Course::WorkerBase> workerToRemove;
    bool found = false;
    if(currentSelected_ != nullptr){
        for(auto worker: currentSelected_->getWorkers()){
            if(worker->getType() == type){
                workerToRemove = worker;
                currentSelected_->removeWorker(workerToRemove);
                objectmanager_->removeWorker(workerToRemove);
                found = true;
                ui->errorLabel->setText(QString::fromStdString(type)
                                        + " removed succesfully");
                break;
            }
        }
        if(not found){
            ui->errorLabel->setText(QString::fromStdString(
                                        "No worker with type " + type
                                    + "\n"+ "found on selected tile"));
        }
    }
}

std::string MainWindow::errorMessageMaker(Course::ResourceMap resources)
{
    std::string message = "";
    for(auto resource: resources){
        if(resource.first == Course::BasicResource::MONEY){
            message += "Money ";
        }
        else if(resource.first == Course::BasicResource::FOOD){
            message += "Food ";
        }
        else if(resource.first == Course::BasicResource::WOOD){
            message += "Wood ";
        }
        else if(resource.first == Course::BasicResource::STONE){
            message += "Stone ";
        }
        else if(resource.first == Course::BasicResource::ORE){
            message += "Ore ";
        }
        message += std::to_string(resource.second) + "\n";
    }
    return message;
}

void MainWindow::updateVillageUI()
{
    auto playersResources = GEHandler_->getResources(currentPlayer_);
    auto villageResources =
            std::static_pointer_cast<Teemu::Village>(currentSelected_)->returnStock();
    //Set stock displays
    ui->villageMoney->display(villageResources.at(Course::BasicResource::MONEY));
    ui->villageFood->display(villageResources.at(Course::BasicResource::FOOD));
    ui->villageWood->display(villageResources.at(Course::BasicResource::WOOD));
    ui->villageStone->display(villageResources.at(Course::BasicResource::STONE));
    ui->villageOre->display(villageResources.at(Course::BasicResource::ORE));

    //Set buy limits based on the minimum between players money and village
    //resources (=> buying can never fail)
    int foodBuyMaximum =std::min(
                villageResources.at(Course::BasicResource::FOOD),
                playersResources.at(Course::BasicResource::MONEY));
    int woodBuyMaximum = std::min(
                villageResources.at(Course::BasicResource::WOOD),
                playersResources.at(Course::BasicResource::MONEY));
    int stoneBuyMaximum = std::min(
                villageResources.at(Course::BasicResource::STONE),
                playersResources.at(Course::BasicResource::MONEY));
    int oreBuyMaximum = std::min(
                villageResources.at(Course::BasicResource::ORE),
                playersResources.at(Course::BasicResource::MONEY));

    ui->sliderFoodBuy->setMaximum(foodBuyMaximum);
    ui->sliderWoodBuy->setMaximum(woodBuyMaximum);
    ui->sliderStoneBuy->setMaximum(stoneBuyMaximum);
    ui->sliderOreBuy->setMaximum(oreBuyMaximum);

    ui->spinBoxFoodBuy->setMaximum(foodBuyMaximum);
    ui->spinBoxWoodBuy->setMaximum(woodBuyMaximum);
    ui->spinBoxStoneBuy->setMaximum(stoneBuyMaximum);
    ui->spinBoxOreBuy->setMaximum(oreBuyMaximum);

    //Set sell limits the same way
    int foodSellMaximum = std::min(
                villageResources.at(Course::BasicResource::MONEY),
                playersResources.at(Course::BasicResource::FOOD));
    int woodSellMaximum = std::min(
                villageResources.at(Course::BasicResource::MONEY),
                playersResources.at(Course::BasicResource::WOOD));
    int stoneSellMaximum = std::min(
                villageResources.at(Course::BasicResource::MONEY),
                playersResources.at(Course::BasicResource::STONE));
    int oreSellMaximum = std::min(
                villageResources.at(Course::BasicResource::MONEY),
                playersResources.at(Course::BasicResource::ORE));

    ui->sliderFoodSell->setMaximum(foodSellMaximum);
    ui->sliderWoodSell->setMaximum(woodSellMaximum);
    ui->sliderStoneSell->setMaximum(stoneSellMaximum);
    ui->sliderOreSell->setMaximum(oreSellMaximum);

    ui->spinBoxFoodSell->setMaximum(foodSellMaximum);
    ui->spinBoxWoodSell->setMaximum(woodSellMaximum);
    ui->spinBoxStoneSell->setMaximum(stoneSellMaximum);
    ui->spinBoxOreSell->setMaximum(oreSellMaximum);
}

void MainWindow::disableUI(bool value)
{
    if(value){
        ui->pushButtonHQ->setEnabled(true);
        ui->pushButtonOutpost->setEnabled(false);
        ui->pushButtonFarm->setEnabled(false);
        ui->pushButtonMine->setEnabled(false);
        ui->pushButtonBarracks->setEnabled(false);

        ui->pushButtonAssign->setEnabled(false);
        ui->pushButtonRemove->setEnabled(false);
        ui->pushButtonFocus->setEnabled(false);

        ui->pushButtonFoodSell->setEnabled(false);
        ui->pushButtonWoodSell->setEnabled(false);
        ui->pushButtonStoneSell->setEnabled(false);
        ui->pushButtonOreSell->setEnabled(false);

        ui->pushButtonFoodBuy->setEnabled(false);
        ui->pushButtonWoodBuy->setEnabled(false);
        ui->pushButtonStoneBuy->setEnabled(false);
        ui->pushButtonOreBuy->setEnabled(false);

        ui->pushButtonEndTurn->setEnabled(false);
    }
    else{
        ui->pushButtonHQ->setEnabled(false);
        ui->pushButtonOutpost->setEnabled(true);
        ui->pushButtonFarm->setEnabled(true);
        ui->pushButtonMine->setEnabled(true);
        ui->pushButtonBarracks->setEnabled(true);

        ui->pushButtonAssign->setEnabled(true);
        ui->pushButtonRemove->setEnabled(true);
        ui->pushButtonFocus->setEnabled(true);

        ui->pushButtonFoodSell->setEnabled(true);
        ui->pushButtonWoodSell->setEnabled(true);
        ui->pushButtonStoneSell->setEnabled(true);
        ui->pushButtonOreSell->setEnabled(true);

        ui->pushButtonFoodBuy->setEnabled(true);
        ui->pushButtonWoodBuy->setEnabled(true);
        ui->pushButtonStoneBuy->setEnabled(true);
        ui->pushButtonOreBuy->setEnabled(true);

        ui->pushButtonEndTurn->setEnabled(true);
    }
}

bool MainWindow::tradeWithVillage(Course::ResourceMap amount)
{
    auto tile = std::static_pointer_cast<Teemu::Village>(currentSelected_);
    auto negativeCost = Teemu::negativeResourceMap(amount);
    if(GEHandler_->modifyResources(currentPlayer_,amount)
            && tile->modifyResources(negativeCost)){
        return true;
    }
    else{
        //Never returns false tho
        return false;
    }
}

void MainWindow::addOnBuildActionTiles()
{
    std::vector<std::shared_ptr<Course::TileBase>> tiles;
    auto objects = currentPlayer_->getObjects();
    for(unsigned int i = 0; i < (10*players_) ;i++){
        for(unsigned int a = 0; a < 10*(players_);a++){
            Course::Coordinate cd(i,a);
            auto tile = objectmanager_->getTile(cd);
            tiles.push_back(tile);

        }
    }
    for(auto tile: tiles){
        if(tile->getOwner() == currentPlayer_){
            if(std::find(objects.begin(),objects.end(),tile)
                    == objects.end()){
                currentPlayer_->addObject(tile);
        }
        }
    }
}

void MainWindow::returnResources(Course::ResourceMap resources)
{
    GEHandler_->modifyResources(currentPlayer_,resources);
}

void MainWindow::setCurrentlySelected(std::shared_ptr<Course::TileBase> selected)
{
    currentSelected_ = selected;
    if(currentSelected_->getType() == "Village"){
        updateVillageUI();
        ui->frameVillage->setVisible(true);
    }
    else{
        ui->frameVillage->setVisible(false);
    }
    updateSelectedTile();
}

void MainWindow::wheelEventSlot()
{
    updateScene();
    ui->graphicsView->centerOn(gamescene_->returnCenter());

}

void MainWindow::updateScene()
{
    ui->graphicsView->update();
}

void MainWindow::setSeed(unsigned int seed)
{
    seed_ = seed;
}

void MainWindow::on_pushButtonEndTurn_clicked()
{
    //END OF TURN
    //Resource generation
    for(auto object :currentPlayer_->getObjects()){
        auto tile = std::static_pointer_cast<Course::TileBase>(object);
        tile->generateResources();
    }
    //BEGINNING OF NEW TURN
    ++round_;

    ui->frameVillage->setVisible(false);
    ui->radioButtonBasic->setChecked(true);
    ui->comboBoxFocus->setCurrentIndex(0);

    currentSelected_ = nullptr;
    currentPlayer_= objectmanager_->getPlayer(round_%roundDivider_);
    //Disable everything except HQ button if player doesnt have HQ
    disableUI(not GEHandler_->getHQstatus(currentPlayer_));
    gamescene_->setSelectedNull();
    updateSelectedTile();
    updateUI();
    ui->errorLabel->clear();
}

void MainWindow::on_pushButtonHQ_clicked()
{
        try{
            if(addBuilding<Course::HeadQuarters>(
                        Course::ConstResourceMaps::HQ_BUILD_COST)){
                GEHandler_->setHQstatus(currentPlayer_,true);
                disableUI(not GEHandler_->getHQstatus(currentPlayer_));
                addOnBuildActionTiles();
                ui->errorLabel->setText("Headquarters placed succesfully");
            }
            else{
                if(currentSelected_ == nullptr){
                    ui->errorLabel->setText("No tile selected");
                }
                else{
                    std::string errorMessage = errorMessageMaker(
                                Course::ConstResourceMaps::HQ_BUILD_COST);

                    ui->errorLabel->setText(QString::fromStdString(
                                                "Not enough resources\n"
                                                "Needed resources:\n"
                                                + errorMessage));
                }
            }

        }
        catch(Course::InvalidPointer& error){
            returnResources(Course::ConstResourceMaps::HQ_BUILD_COST);
            ui->errorLabel->setText(QString::fromStdString(error.msg()));
        }
        catch(Course::NotEnoughSpace& error){
            returnResources(Course::ConstResourceMaps::HQ_BUILD_COST);
            ui->errorLabel->setText("Not enough space on tile");
        }
        catch(Course::IllegalAction& error){
            returnResources(Course::ConstResourceMaps::HQ_BUILD_COST);
            ui->errorLabel->setText("Can't place HQ on selected tile");
        }
    updateSelectedTile();
}

void MainWindow::on_pushButtonOutpost_clicked()
{
    try{
        if(addBuilding<Course::Outpost>(
                    Course::ConstResourceMaps::OUTPOST_BUILD_COST)){
            addOnBuildActionTiles();
            ui->errorLabel->setText("Outpost placed succesfully");
        }
        else{
            if(currentSelected_ == nullptr){
                ui->errorLabel->setText("No tile selected");
            }
            else{
                std::string errorMessage = errorMessageMaker(
                            Course::ConstResourceMaps::OUTPOST_BUILD_COST);

                ui->errorLabel->setText(QString::fromStdString(
                                            "Not enough resources\n"
                                            "Needed resources:\n"
                                            + errorMessage));
            }
        }

    }
    catch(Course::InvalidPointer& error){
        returnResources(Course::ConstResourceMaps::OUTPOST_BUILD_COST);
        ui->errorLabel->setText(QString::fromStdString(error.msg()));
    }
    catch(Course::NotEnoughSpace& error){
        returnResources(Course::ConstResourceMaps::OUTPOST_BUILD_COST);
        ui->errorLabel->setText("Not enough space on tile");
    }
    catch(Course::IllegalAction& error){
        returnResources(Course::ConstResourceMaps::OUTPOST_BUILD_COST);
        ui->errorLabel->setText("Can't place Outpost on selected tile");
    }
    updateSelectedTile();
}

void MainWindow::on_pushButtonFarm_clicked()
{
    try{
        if(addBuilding<Course::Farm>(
                    Course::ConstResourceMaps::FARM_BUILD_COST)){
            ui->errorLabel->setText("Farm placed succesfully");
        }
        else{
            if(currentSelected_ == nullptr){
                ui->errorLabel->setText("No tile selected");
            }
            else{
                std::string errorMessage = errorMessageMaker(
                            Course::ConstResourceMaps::FARM_BUILD_COST);

                ui->errorLabel->setText(QString::fromStdString(
                                            "Not enough resources\n"
                                            "Needed resources:\n"
                                            + errorMessage));
            }
        }

    }
    catch(Course::InvalidPointer& error){
        returnResources(Course::ConstResourceMaps::FARM_BUILD_COST);
        ui->errorLabel->setText(QString::fromStdString(error.msg()));
    }
    catch(Course::NotEnoughSpace& error){
        returnResources(Course::ConstResourceMaps::FARM_BUILD_COST);
        ui->errorLabel->setText("Not enough space on tile");
    }
    catch(Course::IllegalAction& error){
        returnResources(Course::ConstResourceMaps::FARM_BUILD_COST);
        ui->errorLabel->setText("Can't place Farm on selected tile");
    }
    updateSelectedTile();
}

void MainWindow::on_pushButtonMine_clicked()
{
    try{
        if(addBuilding<Essi::Mine>(Teemu::MINE_BUILD_COST)){
            ui->errorLabel->setText("Mine placed succesfully");
        }
        else{
            if(currentSelected_ == nullptr){
                ui->errorLabel->setText("No tile selected");
            }
            else{
                std::string errorMessage = errorMessageMaker(Teemu::MINE_BUILD_COST);

                ui->errorLabel->setText(QString::fromStdString(
                                            "Not enough resources\n"
                                            "Needed resources:\n"
                                            + errorMessage));
            }
        }

    }
    catch(Course::InvalidPointer& error){
        returnResources(Teemu::MINE_BUILD_COST);
        ui->errorLabel->setText(QString::fromStdString(error.msg()));
    }
    catch(Course::NotEnoughSpace& error){
        returnResources(Teemu::MINE_BUILD_COST);
        ui->errorLabel->setText("Not enough space on tile");
    }
    catch(Course::IllegalAction& error){
        returnResources(Teemu::MINE_BUILD_COST);
        ui->errorLabel->setText("Can't place Mine on selected tile");
    }
    updateSelectedTile();
}

void MainWindow::on_pushButtonBarracks_clicked()
{
    try{
        if(addBuilding<Teemu::Barracks>(Teemu::BARRACKS_BUILD_COST)){
            ui->errorLabel->setText("Barracks placed succesfully");
        }
        else{
            if(currentSelected_ == nullptr){
                ui->errorLabel->setText("No tile selected");
            }
            else{
                std::string errorMessage = errorMessageMaker(Teemu::BARRACKS_BUILD_COST);

                ui->errorLabel->setText(QString::fromStdString(
                                            "Not enough resources\n"
                                            "Needed resources:\n"
                                            + errorMessage));
            }
        }

    }
    catch(Course::InvalidPointer& error){
        returnResources(Teemu::BARRACKS_BUILD_COST);
        ui->errorLabel->setText(QString::fromStdString(error.msg()));
    }
    catch(Course::NotEnoughSpace& error){
        returnResources(Teemu::BARRACKS_BUILD_COST);
        ui->errorLabel->setText("Not enough space on tile");
    }
    catch(Course::IllegalAction& error){
        returnResources(Teemu::BARRACKS_BUILD_COST);
        ui->errorLabel->setText("Can't place Barracks on selected tile");
    }
    updateSelectedTile();
}

void MainWindow::on_pushButtonAssign_clicked()
{
    Course::ResourceMap recruitmentCost;
    QString workerName;
    try{
        if(currentSelected_ != nullptr){
            //Assign Basic Worker
            if(ui->radioButtonBasic->isChecked()){
                recruitmentCost = Course::ConstResourceMaps::BW_RECRUITMENT_COST;
                workerName = "Basic worker";
                if(addWorker<Course::BasicWorker>(recruitmentCost)){
                    ui->errorLabel->setText("Basic worker placed succesfully");
                }
                else{
                    std::string errorMessage =
                            errorMessageMaker(recruitmentCost);

                    ui->errorLabel->setText(QString::fromStdString(
                                            "Not enough resources\n"
                                            "Needed resources:\n"
                                            + errorMessage));
                }
            }
            //Assign Farmer
            else if(ui->radioButtonFarmer->isChecked()){
                recruitmentCost = Teemu::FARMER_RECRUITMENT_COST;
                workerName = "Farmer";
                if(addWorker<Essi::Farmer>(recruitmentCost)){
                    ui->errorLabel->setText("Farmer placed succesfully");
                }
                else{
                    std::string errorMessage =
                            errorMessageMaker(recruitmentCost);

                    ui->errorLabel->setText(QString::fromStdString(
                                            "Not enough resources\n"
                                            "Needed resources:\n"
                                            + errorMessage));
                }

            }
            //Assign Miner
            else if(ui->radioButtonMiner->isChecked()){
                recruitmentCost = Teemu::MINER_RECRUITMENT_COST;
                workerName = "Miner";
                if(addWorker<Essi::Miner>(recruitmentCost)){
                    ui->errorLabel->setText("Miner placed succesfully");
                }
                else{
                    std::string errorMessage =
                            errorMessageMaker(recruitmentCost);

                    ui->errorLabel->setText(QString::fromStdString(
                                            "Not enough resources\n"
                                            "Needed resources:\n"
                                            + errorMessage));
                }

            }
            //Assign Saboteur
            else if(ui->radioButtonSaboteur->isChecked()){
                recruitmentCost = Teemu::SABOTEUR_RECRUITMENT_COST;
                workerName = "Saboteur";
                    if(addWorker<Essi::Saboteur>(recruitmentCost)){
                        ui->errorLabel->setText("Saboteur placed succesfully");
                        removeWorker("Saboteur");
                    }
                    else{
                        std::string errorMessage =
                                errorMessageMaker(recruitmentCost);

                        ui->errorLabel->setText(QString::fromStdString(
                                                "Not enough resources\n"
                                                "Needed resources:\n"
                                                + errorMessage));
                    }

            }
            //Assign Soldier
            else if(ui->radioButtonSoldier->isChecked()){
                recruitmentCost = Teemu::SOLDIER_RECRUITMENT_COST;
                workerName = "Soldier";
                    if(addWorker<Teemu::Soldier>(recruitmentCost)){
                        ui->errorLabel->setText("Soldier placed succesfully");

                        //Soldiers onBuildAction, because it modifies
                        //MainWindow elements
                        //onBuildAction begins
                        auto owner = currentSelected_->getOwner();
                        auto tiles = currentSelected_->getOwner()->getObjects();
                        for(auto object: tiles){
                            auto tile =
                                    std::static_pointer_cast<Course::TileBase>
                                    (object);
                            //Delete all workers in that players tiles
                            auto workers = tile->getWorkers();
                            for(auto worker: workers){
                                tile->removeWorker(worker);
                                objectmanager_->removeWorker(worker);
                            }
                            //Delete all buildings in that players tiles
                            auto buildings = tile->getBuildings();
                            for(auto building:buildings){
                                tile->removeBuilding(building);
                                objectmanager_->removeBuilding(building);
                            }
                            //Set all tiles removed player owns to be owned
                            //by nobody
                            tile->setOwner(nullptr);
                        }
                        //Remove player from the game
                        objectmanager_->removePlayer(owner);
                        round_--;
                        roundDivider_--;
                        updateUI();
                        updateSelectedTile();
                        //End game if current player is the only player left
                        if(roundDivider_ == 1){
                            QString name = QString::fromStdString(currentPlayer_->getName());
                            disableUI(true);
                            ui->pushButtonHQ->setEnabled(false);
                            ui->errorLabel->setText(name + " wins the game!");
                          }
                        //OnBuildAction ends
                    }

                    else{
                        std::string errorMessage =
                                errorMessageMaker(recruitmentCost);

                        ui->errorLabel->setText(QString::fromStdString(
                                                "Not enough resources\n"
                                                "Needed resources:\n"
                                                + errorMessage));
                    }
            }

            }

        else{
            ui->errorLabel->setText("No tile selected");
        }

    }
    catch(Course::InvalidPointer& error){
        returnResources(recruitmentCost);
        ui->errorLabel->setText(QString::fromStdString(error.msg()));
    }
    catch(Course::NotEnoughSpace& error){
        returnResources(recruitmentCost);
        ui->errorLabel->setText("Not enough space for worker");
    }
    catch(Course::IllegalAction& error){
        returnResources(recruitmentCost);
        ui->errorLabel->setText("Can't assign " + workerName
                                + " to selected tile");
    }
    updateSelectedTile();
}

void MainWindow::on_pushButtonFocus_clicked()
{
    auto text = ui->comboBoxFocus->currentText();
    for(auto object : currentPlayer_->getObjects()){
        auto tile = std::static_pointer_cast<Course::TileBase>(object);
        for(auto worker : tile->getWorkers()){
            if(text == "None"){
                worker->setResourceFocus(Course::BasicResource::NONE);
            }
            else if(text == "Money"){
                worker->setResourceFocus(Course::BasicResource::MONEY);
            }
            else if(text == "Food"){
                worker->setResourceFocus(Course::BasicResource::FOOD);
            }
            else if(text == "Wood"){
                worker->setResourceFocus(Course::BasicResource::WOOD);
            }
            else if(text == "Stone"){
                worker->setResourceFocus(Course::BasicResource::STONE);
            }
            else if(text == "Ore"){
                worker->setResourceFocus(Course::BasicResource::ORE);
            }
        }
    }
    ui->errorLabel->setText("Workers resource focus set to:\n" + text);
}

void MainWindow::on_pushButtonRemove_clicked()
{
    if(currentSelected_ != nullptr && currentSelected_->getOwner() == currentPlayer_){

        if(ui->radioButtonBasic->isChecked()){
            removeWorker("BasicWorker");
        }

        else if(ui->radioButtonFarmer->isChecked()){
            removeWorker("Farmer");

        }

        else if(ui->radioButtonMiner->isChecked()){
            removeWorker("Miner");

        }

        else if(ui->radioButtonSaboteur->isChecked()){
            removeWorker("Saboteur");

        }

        else if(ui->radioButtonSoldier->isChecked()){
            removeWorker("Soldier");
        }
    }
    else{
        ui->errorLabel->setText("You do not own the selected tile");
    }
    updateSelectedTile();
}

void MainWindow::on_pushButtonFoodBuy_clicked()
{
    int amount = ui->spinBoxFoodBuy->value();
    int cost = amount * -1;
    Course::ResourceMap playerChange ={
        {Course::BasicResource::MONEY, cost},
        {Course::BasicResource::FOOD, amount}
    };
    if(tradeWithVillage(playerChange)){
        updateVillageUI();
        updateLCDNumbers();
        ui->errorLabel->setText("Trade succesful");
    }
}

void MainWindow::on_pushButtonWoodBuy_clicked()
{
    int amount = ui->spinBoxWoodBuy->value();
    int cost = amount * -1;
    Course::ResourceMap playerChange ={
        {Course::BasicResource::MONEY, cost},
        {Course::BasicResource::WOOD, amount}
    };
    if(tradeWithVillage(playerChange)){
        updateVillageUI();
        updateLCDNumbers();
        ui->errorLabel->setText("Trade succesful");
    }
}

void MainWindow::on_pushButtonStoneBuy_clicked()
{
    int amount = ui->spinBoxStoneBuy->value();
    int cost = amount * -1;
    Course::ResourceMap playerChange ={
        {Course::BasicResource::MONEY, cost},
        {Course::BasicResource::STONE, amount}
    };
    if(tradeWithVillage(playerChange)){
        updateVillageUI();
        updateLCDNumbers();
        ui->errorLabel->setText("Trade succesful");
    }
}

void MainWindow::on_pushButtonOreBuy_clicked()
{
    int amount = ui->spinBoxOreBuy->value();
    int cost = amount * -1;
    Course::ResourceMap playerChange ={
        {Course::BasicResource::MONEY, cost},
        {Course::BasicResource::ORE, amount}
    };
    if(tradeWithVillage(playerChange)){
        updateVillageUI();
        updateLCDNumbers();
        ui->errorLabel->setText("Trade succesful");
    }
}

void MainWindow::on_pushButtonFoodSell_clicked()
{
    int cost = ui->spinBoxFoodSell->value();
    int amount = cost * -1;
    Course::ResourceMap playerChange ={
        {Course::BasicResource::MONEY, cost},
        {Course::BasicResource::FOOD, amount}
    };
    if(tradeWithVillage(playerChange)){
        updateVillageUI();
        updateLCDNumbers();
        ui->errorLabel->setText("Trade succesful");
    }
}

void MainWindow::on_pushButtonWoodSell_clicked()
{
    int cost = ui->spinBoxWoodSell->value();
    int amount = cost * -1;
    Course::ResourceMap playerChange ={
        {Course::BasicResource::MONEY, cost},
        {Course::BasicResource::WOOD, amount}
    };
    if(tradeWithVillage(playerChange)){
        updateVillageUI();
        updateLCDNumbers();
        ui->errorLabel->setText("Trade succesful");
    }
}

void MainWindow::on_pushButtonStoneSell_clicked()
{
    int cost = ui->spinBoxStoneSell->value();
    int amount = cost * -1;
    Course::ResourceMap playerChange ={
        {Course::BasicResource::MONEY, cost},
        {Course::BasicResource::STONE, amount}
    };
    if(tradeWithVillage(playerChange)){
        updateVillageUI();
        updateLCDNumbers();
        ui->errorLabel->setText("Trade succesful");
    }
}

void MainWindow::on_pushButtonOreSell_clicked()
{
    int cost = ui->spinBoxOreSell->value();
    int amount = cost * -1;
    Course::ResourceMap playerChange ={
        {Course::BasicResource::MONEY, cost},
        {Course::BasicResource::ORE, amount}
    };
    if(tradeWithVillage(playerChange)){
        updateVillageUI();
        updateLCDNumbers();
        ui->errorLabel->setText("Trade succesful");
    }
}

void MainWindow::on_pushButtonPlus_clicked()
{
    gamescene_->zoomIn();
    wheelEventSlot();
}

void MainWindow::on_pushButtonMinus_clicked()
{
    gamescene_->zoomOut();
    wheelEventSlot();
}
