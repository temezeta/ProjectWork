#include "startdialog.h"
#include "ui_startdialog.h"

#include <QTextBrowser>

StartDialog::StartDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StartDialog)
{
    QWidget::setFixedSize(246,198);
    ui->setupUi(this);
    ui->buttonBox->setEnabled(false);
    ui->playerNameLabel->setVisible(false);
    ui->playerNameLineEdit->setVisible(false);
}

StartDialog::~StartDialog()
{
    delete ui;
}

void StartDialog::addPlayerName()
{
    playerNames_.push_back(ui->playerNameLineEdit->text().toStdString());
    if ( playerNames_.size() < players_ ) {
        std::string newLabel = "Player " + std::to_string\
                (playerNames_.size() + 1) + " name";
        ui->playerNameLabel->setText(QString::fromStdString(newLabel));
    } else {
        ui->playerNameLineEdit->setVisible(false);
        ui->playerNameLabel->setVisible(false);

        ui->gridLayout->removeWidget(ui->playerNameLabel);
        ui->gridLayout->removeWidget(ui->playerNameLineEdit);

        std::string newLabel = "Players are: ";
        for ( unsigned long int i = 0 ; i < playerNames_.size() ; i++ ) {
            newLabel = newLabel + playerNames_.at(i);
            if ( i < playerNames_.size() - 1 ) {
                newLabel = newLabel + ", ";
            }
        }
        QTextBrowser* newTextLabel = new QTextBrowser;
        ui->gridLayout->addWidget(newTextLabel, 2, 0, 1, 2);
        newTextLabel->setPlainText(QString::fromStdString(newLabel));
        ui->setButton->setEnabled(false);
        ui->buttonBox->setEnabled(true);

        emit playerNamesGiven(playerNames_);
    }
}

void StartDialog::on_setButton_clicked()
{
    std::string newPlayersName = ui->playerNameLineEdit->text().toStdString();
    if ( ui->playerNameLabel->isVisible() == false ) {
        ui->playersSpinBox->setEnabled(false);
        ui->seedSpinBox->setEnabled(false);
        ui->playerNameLabel->setVisible(true);
        ui->playerNameLineEdit->setVisible(true);
        ui->setButton->setText(QString::fromStdString("Set name"));

        value_ = ui->seedSpinBox->value();
        players_ =ui ->playersSpinBox->value();
        emit seedChanged(value_);
        emit playersChanged(players_);
    } //Check if playername field is empty or contains a name already registered
    else if ( ( ui->playerNameLineEdit->text() != QString::fromStdString("") ) &&
                ( ( std::find(playerNames_.begin(),playerNames_.end(),
                            newPlayersName)==playerNames_.end()) ) ) {
        addPlayerName();
        ui->playerNameLineEdit->clear();
    }
}
