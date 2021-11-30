#include "main_window.hh"
#include <QGraphicsRectItem>
#include <QKeyEvent>
#include <chrono>

//Public
MainWindow::MainWindow(QWidget* parent):
    QMainWindow(parent),
    seed_(""),
    seedNumber_(1),
    rng_(std::default_random_engine(seedNumber_)),
    direction_(DEFAULT_DIRECTION),
    areaSize_(DEFAULT_AREA_SIZE),
    dead_(false),
    difficulty_(DEFAULT_DIFFICULTY),
    gameStarted(false),
    solidWalls(DEFAULT_WALL_TYPE),
    snake_{},
    food_(Point{-1,-1}),
    score_(0),
    highscore_(0),
    time_(0),
    paused_(false),
    gameOverText_("")
{

    ui_.setupUi(this);
    ui_.graphicsView->setScene(&scene_);

    //Initialize values if settings weren't touch

    //Set limits for seed value to avoid stoi-crash
    ui_.lineEditSeed->setValidator(new QIntValidator(0,INT_MAX,this));

    ui_.lcdNumberHighscore->display(highscore_);
    ui_.pushButtonPause->hide();

    connect(&timer_, &QTimer::timeout, this, &MainWindow::moveSnakeHead);
    connect(&timerCountTime_, &QTimer::timeout, this, &MainWindow::countTime);

}

void MainWindow::keyPressEvent(QKeyEvent* event) {
    int directionHorizontal = direction_.first;
    int directionVertical = direction_.second;
    //Dont keep moving snake if key is held
    if(event->isAutoRepeat()){
        return;
    }

    //Only allow key presses if game is ongoing and unpaused
    else if((not gameOver()) and gameStarted and not paused_){
        //Don't move if pressed key is to opposite or to the direction of travel
        if( (event->key() == Qt::Key_W) and
            (directionVertical != 1 ) and  (directionVertical != -1 )){

            //Stop timer to make snake move more fluently when button is pressed
            direction_ = {0, -1};
            //Move snake once when button is pressed
            moveSnakeHead();
            return;
        }

        else if( (event->key() == Qt::Key_A) and
               (directionHorizontal != 1) and (directionHorizontal != -1)){
            direction_ = {-1, 0};
            moveSnakeHead();
            return;
        }

        else if( (event->key() == Qt::Key_S) and
               (directionVertical != -1) and (directionVertical != 1 )){

            timer_.stop();
            direction_ = {0, 1};
            moveSnakeHead();
            timer_.start();
            return;
        }

        else if( (event->key() == Qt::Key_D) and
               (directionHorizontal != -1) and (directionHorizontal != 1)){

            timer_.stop();
            direction_ = {1, 0};
            moveSnakeHead();
            timer_.start();
            return;
        }
        return;
    }
}


//Private slots
void MainWindow::on_playButton_clicked() {
    //Initialize for new game with selected settings
    init();
    adjustSceneArea();

    //Start game
    timer_.setInterval(difficulty_);
    timer_.start();
    timerCountTime_.start(1000);
}

void MainWindow::on_spinBoxX_valueChanged(int arg1)
{
    //Set area size
    areaSize_ = arg1;
}

void MainWindow::on_horizontalSliderDifficulty_valueChanged(int value)
{
    //Set game difficulty (snakes initial movement speed)
    if(value == 1){
        ui_.labelDifficultyState->setText("Easy");
        difficulty_ = 600;
    }

    else if(value == 2){
        ui_.labelDifficultyState->setText("Medium");
        difficulty_ = 400;
    }

    else if(value == 3){
        ui_.labelDifficultyState->setText("Hard");
        difficulty_ = 250;
    }
}

void MainWindow::on_lineEditSeed_editingFinished()
{
    seed_ = ui_.lineEditSeed->text().toStdString();
}

void MainWindow::on_pushButtonPause_clicked()
{
    if(not paused_){
        paused_ = true;
        timer_.stop();
        timerCountTime_.stop();
        ui_.labelGameStatus->setText("Game paused");
    }
    else if(paused_){
        paused_ = false;
        timer_.start();
        timerCountTime_.start();
        ui_.labelGameStatus->setText("");
    }
}

void MainWindow::on_checkBoxWalls_toggled(bool checked)
{
    if(checked){
        solidWalls = true;
    }
    else{
        solidWalls = false;
    }
}


//Private
//Initialize gamescene for new game

void MainWindow::init()
{
    //Clear GUI elements and snake
    scene_.clear();
    snake_ = {};

    //Set games status to beginning state
    dead_ = false;
    paused_ = false;
    gameStarted = true;
    score_ = 0;
    time_ = 0;
    direction_ = DEFAULT_DIRECTION;

    //Set displays to their beginning states
    ui_.lcdNumberScore->display(score_);
    ui_.lcdNumberTimeSeconds->display(0);
    ui_.lcdNumberTimeMinutes->display(0);
    ui_.labelGameStatus->setText("");
    ui_.labelGameOver->setText("");
    ui_.playButton->setText("Restart");
    ui_.pushButtonPause->show();

    seed_ = ui_.lineEditSeed->text().toStdString();
    seedNumber_ = getRandomSeed(seed_);
    rng_ = std::default_random_engine(seedNumber_);

    //Create new snake with just head in the middle of the screen
    const Point head{(areaSize_/2),(areaSize_/2)};
    snake_.push_back(head);

    //Disable settings
    ui_.widgetSize->setDisabled(true);

    //Add first food to position
    moveFood();
}


void MainWindow::adjustSceneArea() {
    //Make square grid the size given by areaSize_
    const QRectF area(0, 0, areaSize_, areaSize_);
    scene_.setSceneRect(area);
    ui_.graphicsView->fitInView(area);
}

void MainWindow::drawSnake()
{
    scene_.clear();
    //Draw background
    QRectF backgroundRect(0,0,1,1);
    QPen penBackground(Qt::white, 0);
    QBrush brushBackground(QColor(255, 226, 224)); //Red
    for(int i = 0; i  < areaSize_; i++){
        for(int j = 0; j < areaSize_; j++){
           auto backgroundItem = scene_.addRect(backgroundRect,penBackground, brushBackground);
           backgroundItem->setPos(i,j);
        }
    }
    //Snake style setup
    QRectF snakeRect(0, 0, 1, 1);
    QPen pen(Qt::white, 0);
    QBrush brushHead(QColor(115,110,60)); //Brown

    int colorOffset = 0;

    //Place snakes head and body on the GUI
    for(Point point: snake_){
        //Snakes body gets lighter the further it is from the head
        QBrush brushBody(QColor(0, 255-colorOffset , 70)); //Shades of green

        QGraphicsRectItem* snakePart;

        //Make snakes head different color than the body
        if (point == getHead()){
            snakePart = scene_.addRect(snakeRect, pen, brushHead);
        }

        else{
            snakePart = scene_.addRect(snakeRect, pen, brushBody);
        }

        snakePart->setPos(point.x, point.y);

        //Increase darkness of snakes body for each part
        if(colorOffset < 250){
            colorOffset = colorOffset + 10;
        }
    }

    //Place food on the GUI
    QRectF foodRect(0, 0, 1, 1);
    QBrush brushFood(QColor(255,111,96)); //Red

    QGraphicsRectItem* snakePart = nullptr;
    snakePart = scene_.addRect(foodRect, pen, brushFood);
    snakePart->setPos(food_.x, food_.y);


}

bool MainWindow::isInSquare(const Point& square) const{
    // Check if any of the Points stored in snake_ match the given Point.
    for (const Point& part : snake_) {
        if (part == square) {
            return true;
        }
    }

    return false;
}

void MainWindow::moveSnakeHead() {
    if (gameOver()) {
        //If game is over update the GUI and stop the game
        if(score_ > highscore_){
            highscore_ = score_;
        }
        ui_.labelGameStatus->setText(gameOverText_);
        ui_.labelGameOver->setText("Game over!");
        ui_.playButton->setText("Play");
        ui_.lcdNumberHighscore->display(highscore_);
        ui_.pushButtonPause->hide();

        timer_.stop();
        timerCountTime_.stop();
        gameStarted = false;

        //Re-enable settings
        ui_.widgetSize->setEnabled(true);
        return;
    }

    // Figure out the destination coordinates.
    Point newHead = getHead();

    //Move head to new position
    newHead.x =  newHead.x + direction_.first;
    newHead.y =  newHead.y + direction_.second;

    //If walls are solid, snake dies when it hits them
    if(solidWalls){
        if ((newHead.x < 0 || newHead.x >= areaSize_ )||
            (newHead.y < 0 || newHead.y >= areaSize_)) {
            dead_ = true;
            return;
        }
    }
    //Snakes head comes out from the otherside of the area when it hits the wall
    //if they are not solid
    else{
        if(newHead.x < 0){
            newHead.x = areaSize_-1;
        }
        else if(newHead.x >= areaSize_){
            newHead.x = 0;
        }
        else if(newHead.y < 0){
            newHead.y = areaSize_-1;
        }
        else if(newHead.y >= areaSize_){
            newHead.y = 0;
        }
    }

    // Move rest of the snake
    moveSnake(newHead);
}

bool MainWindow::gameOver()  {
    return gameLost() | gameWon();
}

//Game is won if snake fills the entire screen or enough time has elapsed
bool MainWindow::gameWon()  {
    return (snake_.size() >=
            static_cast<long unsigned>(areaSize_) * static_cast<long unsigned>(areaSize_))
            or (time_ == TIME_TO_WIN);
}

//Game is lost if snake has died
bool MainWindow::gameLost() {
    if (dead_){
        gameOverText_ = "You lost.";
        return true;
    }

    gameOverText_ = "You won!";
    return false;
}


const MainWindow::Point MainWindow::getHead() const {
    // The snake's head is the last one in the vector.
    const Point head = snake_.back();
    return head;
}

const MainWindow::Point MainWindow::getTail() const {
    // The snake's tail is the first one in the vector.
    const Point tail = snake_.front();
    return tail;
}

void MainWindow::moveFood() {
    // Move food out of sight when it's no longer needed.
    if (gameWon()) {
        food_.x = -1;
        food_.y = -1;
        return;
    }

    // Keep trying random Points until an empty square is found.
    std::uniform_int_distribution<int> areaSize_dist(0, areaSize_-1);
    while (true) {
        food_.x = areaSize_dist(rng_);
        food_.y = areaSize_dist(rng_);
        if (!isInSquare(food_)) {
            // Snakeless square found, stop moving the food around.
            return;
        }
    }
}

void MainWindow::moveSnake(const Point& newHead) {
    // There shouldn't be any problems if the snake is only a head or
    // if it doesn't yet occupy the square it's moving into.
    if ((snake_.size() > 1) & (isInSquare(newHead))) {

        const Point neck = snake_.at(snake_.size()-2);

        if (newHead == neck) {
            // If the destination square is the square before the head,
            // don't move but don't die either.
            return;

        } else if (newHead != getTail()) {
            // If the destination square contains anything but the "neck" or
            // the tail, the snake dies.
            dead_ = true;
            return;
        }
    }

    // Move the snake.
    snake_.push_back(newHead);

    // New food must be placed somewhere once one gets eaten.
    // Also, the snake should stretch a little.
    if (newHead == food_) {

        ++score_;

        //Snake gets faster everytime it eats a food up to a threshold
        if(score_ < 20){
            timer_.setInterval(difficulty_-10*score_);
        }

        ui_.lcdNumberScore->display(score_);

        moveFood();
    } else {
        //Remove snakes tail if it didnt eat food
        snake_.erase(snake_.begin());
    }
    drawSnake();
}

void MainWindow::countTime()
{
    ++time_;
    //Update timer display
    ui_.lcdNumberTimeSeconds->display(time_%60);
    ui_.lcdNumberTimeMinutes->display(time_/60);
}

unsigned int MainWindow::getRandomSeed(std::string seed) {
    // Convert (as much of) the string to a number if possible.
    if (!seed.empty() && std::isdigit(seed.front())) {
        return static_cast<unsigned int>(std::stoi(seed));
    }

    // Use current time as a seed if the given seed isn't a number.
    const auto time_point_now = std::chrono::system_clock::now();
    const auto elapsed = time_point_now.time_since_epoch();
    const auto secs = std::chrono::duration_cast<std::chrono::seconds>(elapsed);

    return static_cast<unsigned int>(secs.count());
}
