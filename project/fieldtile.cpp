#include "fieldtile.h"

using namespace Logic;

FieldTile::FieldTile()


{
    set_field_tile_state(WATER);
    set_ship_pointer(NULL);
}

void FieldTile::set_ship_pointer(Ship *pShip)
{
    m_pcoShip = pShip;
}

void FieldTile::set_field_tile_state(FieldTileState eState)
{
    m_FieldState = eState;
}

void FieldTile::set_position_x(int iX)
{
    m_iPositionX = iX;
}

void FieldTile::set_position_y(int iY)
{
    m_iPositionY = iY;
}

FieldTileState FieldTile::get_field_tile_state()
{
    return m_FieldState;
}

Ship *FieldTile::get_ship_pointer()
{
    return m_pcoShip;
}

bool FieldTile::was_hit()
{
    if(get_field_tile_state() == WATER || get_field_tile_state() == SHIP_PART)
    {
        return false;
    }
    else
    {
        return true;
    }
}
