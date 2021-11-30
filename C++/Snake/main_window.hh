#ifndef PRG2_SNAKE2_MAINWINDOW_HH
#define PRG2_SNAKE2_MAINWINDOW_HH

#include "ui_main_window.h"

#include <QGraphicsScene>
#include <QMainWindow>
#include <QTimer>
#include <random>
#include <string>



/* \class MainWindow
 * \brief Implements the main window through which the game is played.
 */
class MainWindow: public QMainWindow {
    Q_OBJECT

    //Default settings for a game
    const int TIME_TO_WIN = 120;
    const std::pair<int,int> DEFAULT_DIRECTION = {-1, 0};
    const int DEFAULT_AREA_SIZE = 5;
    const int DEFAULT_DIFFICULTY = 600;
    const bool DEFAULT_WALL_TYPE = true;

    //Struct for storing points for snake parts
    struct Point {
        // These operators are needed when comparing two Points to each other.
        bool operator==(const Point& rhs) const {
            return x == rhs.x && y == rhs.y;
        }

        bool operator!=(const Point& rhs) const {
            return x != rhs.x || y != rhs.y;
        }

        int x;
        int y;
    };  // struct Point

public:

    /* \brief Construct a MainWindow.
     *
     * \param[in] parent The parent widget of this MainWindow.
     */
    explicit MainWindow(QWidget* parent = nullptr);

    /* \brief Destruct a MainWindow.
     */
    ~MainWindow() override = default;

    /* \brief Change the Snake's bearing when certain keys get pressed.
     *
     * \param[in] event Contains data on pressed and released keys.
     */
    void keyPressEvent(QKeyEvent* event) override;


private slots:

    /* \brief Start the game with selected settings.
     */
    void on_playButton_clicked();

    /* \brief Set playable areas size
     */
    void on_spinBoxX_valueChanged(int arg1);

    /* \brief Change game difficulty with slider.
     * Affects snakes initial movement speed
     */
    void on_horizontalSliderDifficulty_valueChanged(int value);

    /* \brief Edit seed value or leave empty for one based on time.
     */
    void on_lineEditSeed_editingFinished();

    /* \brief Pause or unpause the game
     */
    void on_pushButtonPause_clicked();

    /* \brief Changes wall type
     */
    void on_checkBoxWalls_toggled(bool checked);

private:

    /* \brief Initialize games variables and state for new game
     *
     * Called when Play-button pressed
     */
    void init();

    /* \brief Make the playfield visible and fit it into the view.
     *
     * Should be called every time the field's size changes.
     */
    void adjustSceneArea();

    /* \brief Draw snake to the GUI from the background storage vector snake_
     *
     * Called everytime the snake moves
     */
    void drawSnake();

    /* Check if the snake currently occupies a certain square.
     *
     * square: Coordinates of the square to check.
     * return: `true` if a part of the snake is in the square.
     */
    bool isInSquare(const Point& square) const;

    /* \brief Move the Snakes head by a square and check for collisions.
     *
     * If wall gets in the way, snake comes back from the other side
     */
    void moveSnakeHead();

    /* \brief Check if the game is over
     *
     * return: `true` if it is
     */
    bool gameOver();

    /* \brief Check if the game has been won.
     *
     * return: `true` if the snake has filled the field or 2 minutes has passed
     */
    bool gameWon();

    /* \brief Check if the game has been lost.
     *
     * return: `true` if the snake is dead.
     */
    bool gameLost();

    /* \brief Get the snake's head's coordinates.
     *
     * return: Coordinates of the snake's head.
     */
    const Point getHead() const;

    /* \brief Get the snake's tail's coordinates.
     *
     * return: Coordinates of the snake's tail.
     */
    const Point getTail() const;

    /* \brief Relocate food to a random, snakeless location.
     *
     * Hides the food if the snake has completely filled the field.
     */
    void moveFood();

    /* \brief Move the snake in selected direction, eating anything in its path.
     * Check for collisions with snake itself
     * If the food got eaten a new one got placed somewhere.
     * The snake also grew a bit.
     */
    void moveSnake(const Point& newHead);

    /* \brief Count time spent in a single game
     */
    void countTime();

    /* \brief Convert given seed to unsigned int or if invalid input,
     *        get one based on time
     * return: unsigned int for seedNumber_
     */
    unsigned int getRandomSeed(std::string seed);

    Ui::MainWindow ui_;                 /**< Accesses the UI widgets. */

    QGraphicsScene scene_;              /**< Manages drawable objects. */

    QTimer timer_;                      /**< Triggers the Snake to move. */
    QTimer timerCountTime_;             /**< Counts time spend on a game */

    std::string seed_;                  /**< Player input for seed */
    unsigned int seedNumber_;           /**< Seed converted to unsigned int */
    std::default_random_engine rng_;    /**< Randomizes food locations. */

    std::pair<int,int> direction_;      /**< The direction the snake is moving*/
    int areaSize_;                      /**< Playable area size X-coordinate */
    bool dead_;                         /**< Snakes life status */
    int difficulty_;                    /**< Determines game difficulty */
    bool gameStarted;                   /**< Determines if game is started */
    bool solidWalls;                    /**< Wall collision type*/

    std::vector<Point> snake_;          /**< Snake parts X- and Y-coordinates */
    Point food_;                        /**< Foods X- and Y-coordinates */

    int score_;                         /**< Score of the current game */
    int highscore_;                     /**< Highest score achieved*/
    int time_;                          /**< Time in seconds */

    bool paused_;                        /**< Game pause status */
    QString gameOverText_;              /**< Text displayed when game is over */

};  // class MainWindow


#endif  // PRG2_SNAKE2_MAINWINDOW_HH
