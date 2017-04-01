#include "ship.h"
using namespace Logic;

Ship::Ship(const Logic::ShipSize eSize,const int iId): m_ShipSize(eSize), m_HitPoints(eSize), m_iId(iId)

{
    m_ShipState = Logic::SHIP_OK;
}


Logic::ShipState Ship::get_shipstate() const
{
    return m_ShipState;
}


void Ship::set_shipstate(Logic::ShipState eState)
{
    m_ShipState = eState;
}

Logic::ShipSize Ship::get_shipsize() const
{
    return m_ShipSize;
}

int Ship::get_hitpoints() const
{
    return m_HitPoints;
}

const int Ship::get_ship_id() const
{
    return m_iId;
}

void Ship::decrement_hit_point()
{
    --m_HitPoints;
}

int Ship::get_x(int iI)
{
    return m_qbaShipCoordinates.at(iI*2)-48;
}

int Ship::get_y(int iI)
{
    return m_qbaShipCoordinates.at(iI*2 +1)-48;
}

void Ship::add_coordinates(int iX,int iY)
{
    m_qbaShipCoordinates.append(QString::number((u_int8_t)iX));
    m_qbaShipCoordinates.append(QString::number((u_int8_t)iY));
}


