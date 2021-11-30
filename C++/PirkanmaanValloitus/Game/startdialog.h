#ifndef STARTDIALOG_H
#define STARTDIALOG_H

#include <QDialog>

namespace Ui {
class StartDialog;
}
/**
 * @brief The StartDialog class is used to input information for the game
 * 1. Input player amount
 * 2. Input seed
 * 3. Give players names
 */
class StartDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StartDialog(QWidget *parent = 0);
    ~StartDialog();
    /**
     * @brief addPlayerName adds player to playerNames_
     */
    void addPlayerName();
signals:
    /**
     * @brief seedChanged send seeed value to mainwindow
     * @param value
     */
    void seedChanged(unsigned int value);
    /**
     * @brief playersChanged send player amount to mainwindow
     * @param players
     */
    void playersChanged(unsigned int players);
    /**
     * @brief playerNamesGiven send given playernames to mainwindow
     * @param playerNames
     */
    void playerNamesGiven(std::vector<std::string> playerNames);
private slots:
    /**
     * @brief on_setButton_clicked emits a signal appropriate to type
     */
    void on_setButton_clicked();

private:
    Ui::StartDialog *ui;
    unsigned int value_;
    unsigned int players_;
    std::vector<std::string> playerNames_;
};

#endif // STARTDIALOG_H
