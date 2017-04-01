/** \brief Contains all the elements of the own and enemy field
  * \author Alexandros Sivris
  * \date March 2015
  */
#ifndef BATTLEFIELD_H
#define BATTLEFIELD_H

#include <QWidget>
#include <QPushButton>
#include <QString>
#include "ship.h"
#include <QDebug>
#include <QSignalMapper>
#include <stdlib.h>
#include <QMoveEvent>
#include <QDesktopWidget>
#include <QList>
#include "field.h"
#include "ui_battlefield.h"


/**
 * @brief The namespace Gui has the purpose of grouping the classes Gui::Battlefield, Gui::Fieldbutton and Gui::LastState together as the game's graphic interface
 */
namespace Gui {
class LastState;
class Battlefield;
class Fieldbutton;

}
namespace Ui {
class Battlefield;
}

/**
/** * @brief The LastState class stores the state of the last clicked button and makes sure that the fieldtiles of
 * one ship are connected (e.g. battleship-> five tiles right next to each other).
 * Closely connected to the Fieldbutton class
 */

class Gui::LastState
{

public:
    /**
     * @brief LastState - The constructor
     */
    explicit LastState();
    /**
     * @brief ~LastState - The destructor
     */
    ~LastState();

    /**
     * @brief save the state of the last button clicked
     * @param object
     * @param x is the X coordinate of the last clicked field tile
     * @param y is the Y coordinate of the last clicked field tile
     */
    void setLastState(QString object, quint8 x, quint8 y);
    /**
     * @brief getDistance - checks if the distance from the LastState to the new Tile is only =1 (ship tiles have to be connected!)
     */
    void getDistance(quint8,quint8, quint8*,quint8*) const;
    /**
     * @brief gets the object name of the tile
     * @return
     */
    QString getObjectName() const;
    /**
     * @brief m_ui8PositionX - The X position of the last clicked tile
     */
    quint8 m_ui8PositionX = 0;
    /**
     * @brief m_ui8PositionY - The Y position of the last clicked tile
     */
    quint8 m_ui8PositionY = 0;

private:
    /**
     * @brief attribute to store the object name of the tile
     */
    QString m_qsObject;


    /**
     * @brief m_ShipSize - the number of fieldtiles for a ship
     */
    Logic::ShipSize m_ShipSize;
};

/**
 * @brief The Fieldbutton class stores the clicked coordinates
 *  of the ships at the start of the game
 */

class Gui::Fieldbutton : public QPushButton
{
    Q_OBJECT
public:
    /**
     * @brief constructor - initialization of the member variables m_ui8Id (const quint8), m_qsObjectName (const QString) , m_ui8PositionX (const quint8), m_ui8PositionY (const quint8) and caption and object name of the tile
     */
    Fieldbutton(quint8, quint8, quint8, const QString);
    /**
     * @brief destructor - delete this object
     */
    ~Fieldbutton();

    /**
     * @brief getCoordinates - checks the Coordinates of the clicked buttons
     * @param paramX x-coordinate
     * @param paramY y-coordinate
     */
    void getCoordinates(quint8 &paramX, quint8 &paramY) const;
    /**
     * @brief get the object name
     * @return
     */
    QString getObjectName() const;

    /**
     * @brief setCoordinates
     * @param paramX x-coordinate
     * @param paramY y-coordinate
     */
    void setCoordinates(quint8 &paramX, quint8 &paramY);
    /**
     * @brief set the type of the tile
     * @param stype NO_SHIP=0, SUBMARINE = 2, DESTROYER = 3,CRUISER= 4, BATTLESHIP = 5
     */
    void setTileType(Logic::ShipSize stype);
    /**
     * @brief get the type of the tile
     * @return Logic::ShipSize
     */
    Logic::ShipSize getTileType();

    /**
     * @brief setting the ship id during positioning
     * @param sId parameter id
     */
    void setShipId(quint8 sId);

    /**
     * @brief get the ship id
     * @return int
     */
    int getShipId();

private:

    /**
     * @brief contains the caption of the tile
     */
    const QString  m_qsCaption;

    /**
     * @brief contains the object name
     */
    const QString  m_qsObjectName;

    /**
     * @brief contains the x-coordinate
     */
    const quint8 m_ui8PositionX = 0;

    /**
     * @brief contains the y-coordinate
     */
    const quint8 m_ui8PositionY = 0;

    /**
     * @brief contains the tile id
     */
    const quint8 m_ui8Id = 0;

    /**
     * @brief contains the ship id
     */
    quint8 m_qi8ShipId = 0;

    Logic::ShipSize m_sSize = Logic::NO_SHIP;


};

/**
 * @brief The Battlefield class shows the player where he set his fleet
 * after he placed his ships (by clicking the field tiles)
 */

class Gui::Battlefield : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief constructor - set up own/enemy field and position the corresponding tiles accordingly. These tiles are bound via QSignalMapper to the slots sl_fb_clicked() and/or sl_enemy_clicked().
     * @param parent pointer to a QWidget
     */
    explicit Battlefield(QWidget *parent = 0);


    ~Battlefield();

    /**
     * @brief set up a QList which contains the history of the tile placement. This function becomes handy in the "Undo" step.
     * @param qsName object name of tile
     */
    void set_push_tile_list(QString qsName);
    /**
     * @brief get last object from list and remove it. This happens in a loop to ensure that all tiles that belong to a ship are delete from the history.
     * @return QString
     */
    QString get_pop_tile_list();


signals:
    /**
     * @brief when own tile is clicked
     */
    void si_fb_clicked(QWidget *);
    /**
     * @brief when enemy tile is clicked
     */
    void si_enemy_clicked(QWidget *);
    /**
     * @brief when every ship is placed and ready to be handed to the Logic-class
     */
    void si_ships_ready_togui(int,int,Logic::ShipSize,int);
    /**
     * @brief logic negotiates parameters with gui
     * @param iX dimension (x) of the fields
     * @param iY dimension (y) of the fields
     * @param iCountBattleship amount of battleships
     * @param iCountCruiser amount of cruisers
     * @param iCountDestroyer amount of destroyers
     * @param iCountSubmarine amount of submarines
     */
    void si_neg_param_gui(int iX, int iY, int iCountBattleship, int iCountCruiser, int iCountDestroyer, int iCountSubmarine);
    /**
     * @brief triggered when a shot is fired in the gui
     */
    void si_shot_gui(u_int8_t, u_int8_t);
    /**
     * @brief triggers when the user clicks the close button in the connection dialog
     */
    void si_close_app();
    /**
     * @brief request to the logic to start the game
     */
    void si_request_start_gui();

public slots:
    /**
     * @brief triggered by si_fb_clicked() when player clicks on one of his own tiles
     */
    void sl_fb_clicked(QWidget *);
    /**
     * @brief enables just the radio button "Battleship"
     */
    void sl_draw_battleship();
    /**
     * @brief triggered by si_enemy_clicked() when player fires a shot at an enemy tile
     */
    void sl_enemy_clicked(QWidget *);
    /**
     * @brief clears the game field
     */
    void sl_new_game();
    /**
     * @brief undoes the last step of the history
     */
    void sl_undo_step();
    /**
     * @brief deletes this class and all the others
     */
    void sl_close_app();
    /**
     * @brief pipelines the signal si_request_start_gui() to the master-class
     */
    void sl_request_start_gui();
    void sl_enemy_tile_hit(int iX, int iY, Logic::FieldTileState eState);
    void sl_indicate_shot(u_int8_t, u_int8_t);
    void sl_disable_start_button();
    void sl_enable_start_button();

private:

    /**
     * @brief QList-object that contains the history of the button placement on the own field
     */
    QList<QString> m_qsList;

    /**
     * @brief pointer to the user interface elements of the battlefield class
     */
    Ui::Battlefield *m_bfUi;

    /**
     * @brief pointer to the last state
     */
    Gui::LastState *m_pLs;
    /**
     * @brief pointer to the first state
     */
    Gui::LastState *m_pBeginState;

    /**
     * @brief m_qi8TotalTiles - Total tiles that have to be clicked to place all ships
     */
    quint8 m_qi8TotalTiles = 0;

    /**
     * @brief m_qi8TileCounter - Counter for clicked tiles
     */
    quint8 m_qi8TileCounter = 0;

    /**
     * @brief m_qi8CounterBattleship - number of placeable Battleships
     */
    quint8 m_qi8CounterBattleship = 1;
    /**
     * @brief m_qi8CounterCruiser - number of placeable Cruisers
     */
    quint8 m_qi8CounterCruiser = 2;
    /**
     * @brief m_qi8CounterDestroyer - number of placeable Destroyers
     */
    quint8 m_qi8CounterDestroyer = 3;
    /**
     * @brief m_qi8CounterSubmarine - number of placeable Submarines
     */
    quint8 m_qi8CounterSubmarine = 4;
    /**
     * @brief enum type that summarizes the direction of the ship placement
     */
    enum eDirection {
        NEUTRAL,
        HORIZONTAL,
        VERTICAL
    } direction = NEUTRAL;
};



#endif // BATTLEFIELD_H
