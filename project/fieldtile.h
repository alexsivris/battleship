#ifndef FIELDTILE_H
#define FIELDTILE_H
#include "ship.h"
#include <QObject>


namespace Logic
{
class FieldTile;
/**
 * @brief The FieldTileState - A tile is either considered: Water=0, SHIP_PART=1, SHIP_PART_HIT=2, or SHIP_PART_DEST=3
 */
enum FieldTileState {WATER ,WATER_AND_HIT, SHIP_PART, SHIP_PART_HIT, SHIP_PART_DEST};

/**
 * @brief The FieldTile class defines the type of each fieldtile, e.g. ship part, water, ...
 */
class FieldTile : public QObject
{

public:
    /**
     * @brief FieldTile a constructor
     */
    FieldTile();
    /**
     * @brief set_field_tile_state
     * @param eState defines the state of the fieldtile
     */
    void set_field_tile_state(Logic::FieldTileState eState);

    /**
     * @brief set_ship_pointer sets the m_pcoSip to ship pointer
     * @param pShip ship that consists out many field tiles
     */
    void set_ship_pointer(Ship* pShip);

    /**
     * @brief set_position_x
     * @param iX saves the X coordinate iX to m_iPositionX
     */
    void set_position_x(int iX);

    /**
     * @brief set_position_y
     * @param iY saves the Y coordinate iY to m_iPositionY
     */
    void set_position_y(int iY);

    /**
     * @brief get_field_tile_state
     * @return the state of the field tile(WATER, WATER_AND_HIT, SHIP_PART,SHIP_PART_HIT,SHIP_PART_DEST)
     */
    FieldTileState get_field_tile_state();

    /**
     * @brief get_ship_pointer
     * @return the pointer to ship if the tile is part of a ship
     */
    Ship* get_ship_pointer();

    /**
     * @brief was_hit
     * @return TRUE if this tile was already hit, FALSE if it has not been hit yet
     */
    bool was_hit();

private:
    /**
     * @brief m_iPositionX X-position of the tile on the field
     */
    int m_iPositionX;
    /**
     * @brief m_iPositionY Y-position of the tile on the field
     */
    int m_iPositionY;

    /**
     * @brief m_FieldState represents the state of the tile (WATER, SHIP_PART, SHIP_PART_HIT, SHIP_PART_DEST)
     */
    Logic::FieldTileState m_FieldState;

    /**
     * @brief m_pcoShip every field tile points to either a ship or to NULL(water)
     */
    Logic::Ship * m_pcoShip;

};
}

#endif // FIELDTILE_H
