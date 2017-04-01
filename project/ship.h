#ifndef SHIP_H
#define SHIP_H
#include <inttypes.h>
#include <string>
#include <map>
#include <QObject>

namespace Logic
{



/**
 * @brief The Shipstate states if: SHIP_OK = 0, SHIP_HIT =1, SHIP_DESTROYED=2
 */
enum  ShipState { SHIP_OK,SHIP_HIT,SHIP_DESTROYED};
/**
 * @brief The ShipSize shows the (constant) amount of field tiles per ship
 */
enum  ShipSize {NO_SHIP=0, SUBMARINE = 2, DESTROYER = 3,CRUISER= 4, BATTLESHIP = 5};

/**
 * @brief The Ship class contains the coordinates of a ship and its status(destroyed,partially hit,not hit at all)
 *
 */


class Ship : public QObject
{
public:
    /**
     * @brief Ship - a constructor
     * @param eSize - number of field tiles
     * @param Id - every ShipSize has its own Id starting by 0
     */
    Ship(Logic::ShipSize eSize,const int Id);
    /**
     * @brief get_shipsize
     * @return the number of field tiles which represent a ship
     */
    Logic::ShipSize get_shipsize(void) const;
    /**
     * @brief get_shipstate
     * @return the status of the whole ship: SHIP_OK, SHIP_HIT, SHIP_DESTROYED
     */
    Logic::ShipState get_shipstate(void) const;

    /**
     * @brief set_shipstate
     * @param eState stes the current state of the ship
     */
    void set_shipstate(Logic::ShipState eState);
    
    /**
     * @brief get_hitpoints
     * @return the current hit points of a ship. zero hit points == ship is destroyed
     */
    int get_hitpoints() const;

    /**
     * @brief add_coordinates adds coordinates of a ship to the map m_mShipCoordinates
     * @param iX - X coordinate of a ship tile
     * @param iY - Y coordinate of a ship tile
     */
    void add_coordinates(int iX,int iY);

    /**
     * @brief get_ship_id
     * @return the id of the ship
     */
    const int get_ship_id() const;

    /**
     * @brief decrement_hit_point decreases the hit points by one
     */
    void decrement_hit_point();

    /**
     * @brief getXAt returns the first Element at iI position in the map m_mShipCoordinates
     * @param iI position of the first element in the map you want to have
     * @return the first element at the iI position in the map m_mShipCoordinates
     */
    int get_x(int iI);

    /**
     * @brief getSecondAt returns the second Element at iI position in the map m_mShipCoordinates
     * @param iI position of the second element in the map you want to have
     * @return the second element at the iI position in the map m_mShipCoordinates
     */
    int get_y(int iI);

private:
    /**
     * @brief m_ShipState - during the game the ship is either SHIP_OK or SHIP_HIT or SHIP_DESTROYED
     */
    Logic::ShipState m_ShipState;
    /**
     * @brief m_ShipSize - this is the signature of the ship(BATTLESHIP, CRUISER, DESTROYER, SUBMARINE)
     */
    const Logic::ShipSize m_ShipSize;
    /**
     * @brief m_HitPoints - zero hit points == ship is SHIP_DESTROYED
     */
    int m_HitPoints;

    /**
     * @brief m_Id - every ship is identifyable over its size and its id
     */
    const int m_iId;



    /**
     * @brief m_qbaShipCoordinates - contains all coordinates of a ship
     */
    QByteArray m_qbaShipCoordinates;




};

}

#endif // SHIP_H
