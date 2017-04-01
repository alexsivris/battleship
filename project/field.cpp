#include "field.h"
#include "ship.h"
#include <QDebug>
#include "fieldtile.h"
using namespace Logic;

Field::Field()
{

set_game_state(NOT_PLAYING);
set_ready_to_play_state(NOT_READY_TO_PLAY);
connect(this,SIGNAL(si_a_req_me(u_int8_t)),this,SLOT(sl_a_req_en(u_int8_t)));

}

Field::~Field()
{
    //Delete all the ships and submarines from the heap
    for(int i = 0; i <m_vShip.size(); i++)
    {
        qDebug() << "Delete Ship";
        delete m_vShip[i];
    }

    //Delete all field tiles from my field from the heap
    for(int i = 0; i < get_field_dim_x(); i++)
    {
        for(int j = 0; j < get_field_dim_y(); j++)
        {
            delete m_pvfieldtileme[i][j];
        }
    }

    //Delete all field tiles from enemies field from the heap
    for(int i = 0; i < get_field_dim_x(); i++)
    {
        for(int j = 0; j < get_field_dim_y(); j++)
        {
            delete m_pvfieldtileenemy[i][j];
        }
    }
}

bool Field::is_game_over()
{
    for(int i = 0; i<m_vShip.size();i++)
    {
        if(m_vShip[i]->get_shipstate() != SHIP_DESTROYED)
        {
            return false;
        }
    }
    set_game_state(LOST);
    return true;
}

void Field::save_neg_param(u_int8_t uiX, u_int8_t uiY, u_int8_t uiCountBattleship, u_int8_t uiCountCruiser, u_int8_t uiCountDestroyer, u_int8_t uiCountSubmarine)
{
    m_sNegParam.uiX = uiX;
    m_sNegParam.uiY = uiY;
    m_sNegParam.uiCountBattleship = uiCountBattleship;
    m_sNegParam.uiCountCruiser = uiCountCruiser;
    m_sNegParam.uiCountDestroyer = uiCountDestroyer;
    m_sNegParam.uiCountSubmarine = uiCountSubmarine;
}

void Field::fill_my_field_vector()
{
    m_pvfieldtileme.resize(get_field_dim_x(),std::vector<Logic::FieldTile*>(get_field_dim_y(),NULL));
    for(int i = 0; i< get_field_dim_x();i++)
    {
        for(int j = 0; j < get_field_dim_y();j++)
        {
            m_pvfieldtileme[i][j] = new Logic::FieldTile; // TODO create the FieldTile
        }
    }
}

void Field::fill_enemy_field_vector()
{
    m_pvfieldtileenemy.resize(get_field_dim_x(),std::vector<Logic::FieldTile*>(get_field_dim_y(),NULL));
    for(int i = 0; i< get_field_dim_x();i++)
    {
        for(int j = 0; j < get_field_dim_y();j++)
        {
            m_pvfieldtileenemy[i][j] = new Logic::FieldTile(); // TODO create the FieldTile
        }
    }
}

bool Field::is_my_turn()
{
    if(get_turn() == MY_TURN)
    {
        return true;
    }
    else
        return false;
}

void Field::fill_field_tile_information(int iX, int iY, Ship *pShip)
{
    m_pvfieldtileme[iX][iY]->set_ship_pointer(pShip);
    m_pvfieldtileme[iX][iY]->set_position_x(iX);
    m_pvfieldtileme[iX][iY]->set_position_y(iY);
    m_pvfieldtileme[iX][iY]->set_field_tile_state(SHIP_PART);
}

//SLOTS
void Field::sl_neg_param_gui(u_int8_t uiX, u_int8_t uiY, u_int8_t uiCountBattleship, u_int8_t uiCountCruiser, u_int8_t uiCountDestroyer, u_int8_t uiCountSubmarine)
{
    qDebug() << "X:" << uiX << " Y: " << uiY;
    set_game_state(NOT_PLAYING);
    emit si_togui_history(qsNOT_PLAYING);
    save_neg_param(uiX,uiY,uiCountBattleship,uiCountCruiser,uiCountDestroyer,uiCountSubmarine);
    emit si_neg_param_me(uiX,uiY,uiCountBattleship,uiCountCruiser,uiCountDestroyer,uiCountSubmarine);
    emit si_togui_history(qsWAIT_FOR_ANSW);

}

void Field::sl_a_req_en(u_int8_t uiStatus)
{
    if(get_game_state() == NOT_PLAYING)
    {

        switch(uiStatus)
        {
        case OK:
            set_field_param(); // writes game params out of the struct in the seperate member variables
            fill_my_field_vector();
            fill_enemy_field_vector();
            set_game_state(PLACING_SHIPS);
            emit si_togui_history(qsNEG_PARAM_OK);
            emit si_togui_history(qsPLACING_SHIPS);
            emit si_place_ships();
            emit si_disable_start_button();
            break;
        case DENIED_OR_NOT_READY:
            set_game_state(PLACING_SHIPS);
            emit si_togui_history(qsDENIED);
            emit si_togui_history(qsPLACING_SHIPS);
            break;
        case NOT_SUPPORTED:
            set_game_state(NOT_PLAYING);
            emit si_not_supported_me();
            emit si_togui_history(qsNOT_SUPPORTED);
            emit si_togui_history(qsNOT_PLAYING);
            break;
        case MESSAGE_INCOMPLETE:
            set_game_state(NOT_PLAYING);
            emit si_msg_incomplete_me();
            emit si_togui_history(qsMSG_INCOMPLETE);
            emit si_togui_history(qsNOT_PLAYING);
            break;
        case TIMEOUT:
            set_game_state(NOT_PLAYING);
            emit si_timeout_me();
            emit si_togui_history(qsTIMEOUT);
            emit si_togui_history(qsNOT_PLAYING);
            break;
        case ANNY_ERROR:
            set_game_state(NOT_PLAYING);
            emit si_error_me();
            emit si_togui_history(qsANY_ERROR);
            emit si_togui_history(qsNOT_PLAYING);
            break;
        default:
            set_game_state(NOT_PLAYING);
            emit si_error_me();
            emit si_togui_history(qsANY_ERROR);
            emit si_togui_history(qsNOT_PLAYING);
            break;
        }
        return;

    }
    else if(get_game_state() == PLACING_SHIPS)
    {

        switch(uiStatus)
        {
        case OK:
            emit si_activate_me(); //TODO connect to gui, now gui can start placing ships
            emit si_togui_history(qsNEG_PARAM_OK);
            set_game_state(PLAYING);
            emit si_togui_history(qsPLAYING);
            set_turn(MY_TURN);
            emit si_togui_history(qsMY_TURN);
            set_ready_to_play_state(READY_TO_PLAY);
            break;
        case DENIED_OR_NOT_READY:
            emit si_togui_history(qsDENIED);
            set_game_state(PLACING_SHIPS);
            emit si_togui_history(qsPLACING_SHIPS);
            emit si_enable_start_button();
            break;
        case TIMEOUT:
            emit si_togui_history(qsTIMEOUT);
            set_game_state(NOT_PLAYING);
            emit si_togui_history(qsNOT_PLAYING);
            break;
        case NOT_SUPPORTED:
            emit si_togui_history(qsNOT_SUPPORTED);
            set_game_state(NOT_PLAYING);
            emit si_togui_history(qsNOT_PLAYING);
            break;
        case MESSAGE_INCOMPLETE:
            emit si_togui_history(qsMSG_INCOMPLETE);
            set_game_state(NOT_PLAYING);
            emit si_togui_history(qsNOT_PLAYING);
            break;
        case ANNY_ERROR:
            emit si_togui_history(qsANY_ERROR);
            set_game_state(NOT_PLAYING);
            emit si_togui_history(qsNOT_PLAYING);
            break;
        default:
            emit si_togui_history(qsANY_ERROR);
            set_game_state(NOT_PLAYING);
            emit si_togui_history(qsNOT_PLAYING);
            break;

        }
        return;

    }
    else
    {
        emit si_togui_history("Fehler in LOGIC:: sl_a_req_en() ");
    }
}

void Field::sl_req_start_gui()
{
    emit si_req_start_gui();
    emit si_togui_history(qsWAIT_FOR_ANSW);
    emit si_disable_start_button();
    set_ready_to_play_state(READY_TO_PLAY);
}

void Field::sl_req_start_en() ////ACHTUNG ????
{
    emit si_togui_history(qsREQUEST);
    emit si_req_start_en();


}

void Field::sl_a_req_start_gui(u_int8_t uiStatus)
{
    emit si_a_req_start_gui(uiStatus);
    if(uiStatus == OK)
    {
        emit si_togui_history(qsACCEPTED);
        set_game_state(PLAYING);
        set_turn(NOT_MY_TURN);
        set_ready_to_play_state(READY_TO_PLAY);
        emit si_togui_history(qsPLAYING);
        emit si_togui_history(qsENEMY_TURN);
    }
    else if(uiStatus ==DENIED_OR_NOT_READY)
    {
        emit si_togui_history(qsDENIED);
        emit si_togui_history(qsDENIED);
        set_ready_to_play_state(NOT_READY_TO_PLAY);
    }
    else
    {
        qDebug()<<"Fehler in in sl_a_req_start_gui() field.cpp erwarte entweder eine Zusgage oder eine ABsage 0x00 oder 0x01";
    }
    return;
}

void Field::sl_a_param_me(u_int8_t ui8Status)
{
    //set_game_state(PLACING_SHIPS);
    emit si_a_req_me(ui8Status);
}

void Field::sl_shot_gui(u_int8_t iX,u_int8_t iY)
{
    qDebug() << "sl_shot_gui im Field " << iX << " " << iY;
    set_last_shot_position_x(iX);
    set_last_shot_position_y(iY);
    emit si_shot_me(iX,iY);
    return;
}

void Field::sl_a_shot_en(u_int8_t iStatus, u_int8_t iLength, u_int8_t *iShipPositions)
{
    qDebug()<< iShipPositions[0] << " "<< iShipPositions[1] << " "<< iShipPositions[2] << " "<< iShipPositions[3] ;
    switch(iStatus)
    {
    //OK
    case 0x00:
        m_pvfieldtileenemy[get_last_shot_position_x()][get_last_shot_position_y()]->set_field_tile_state(WATER_AND_HIT);
        emit si_set_field_tile_me(get_last_shot_position_x(),get_last_shot_position_y(),WATER_AND_HIT);
        set_turn(NOT_MY_TURN);
        emit si_togui_history(qsNOT_HIT);
        emit si_togui_history(qsENEMY_TURN);
        break;
        //Ship hit
    case 0x01:
        m_pvfieldtileenemy[get_last_shot_position_x()][get_last_shot_position_y()]->set_field_tile_state(SHIP_PART_HIT);
        emit si_set_field_tile_me(get_last_shot_position_x(),get_last_shot_position_y(),SHIP_PART_HIT);
        set_turn(NOT_MY_TURN);
        emit si_togui_history(qsHIT);
        emit si_togui_history(qsENEMY_TURN);
        break;
        //Ship hit & destroyed
    case 0x02:
        for(int i = 0; i< iLength/2 ; i++)
        {
            qDebug()<< iShipPositions[0] << " "<< iShipPositions[1] << " "<< iShipPositions[2] << " "<< iShipPositions[3] ;
            m_pvfieldtileenemy[iShipPositions[i*2]][iShipPositions[i*2+1]]->set_field_tile_state(SHIP_PART_DEST);
            emit si_set_field_tile_me(iShipPositions[i*2],iShipPositions[i*2+1],SHIP_PART_DEST);
            qDebug()<< iShipPositions[i*2] << " " << iShipPositions[i*2+1];
        }
        set_turn(NOT_MY_TURN);
        emit si_togui_history(qsHIT_AND_DESTROYED);
        emit si_togui_history(qsENEMY_TURN);
        break;
        //ship hit& destroyed & WON
    case 0x03:
        for(int i = 0; i < iLength/2 ; i++)
        {
            m_pvfieldtileenemy[iShipPositions[i*2]][iShipPositions[i*2+1]]->set_field_tile_state(SHIP_PART_DEST);
            emit si_set_field_tile_me(iShipPositions[i*2],iShipPositions[i*2+1],SHIP_PART_DEST);
        }
        set_turn(NOT_MY_TURN);
        set_game_state(WON);
        emit si_togui_history(qsWIN);
        emit si_win_to_gui();
        break;
        //was not my turn
    case 0x10:
        //TODO emit Fehlermeldung, wir sind nicht dran
        emit si_togui_history("Sie sind nicht an der Reihe");
        //TODO turn anpassen, gegebenfalls
        break;
        //out of range
    case 0x11:
        //TODO emit Spiel abbrechen, irgendwie konnten wir auserhalb des Spieleldes schießen
        emit si_togui_history(qsOUT_OF_RANGE);
        break;
        //any error
    case 0x20:
        //TODO emit sonstiger Fehler
        emit si_togui_history(qsANY_ERROR);
        break;
    }
}

void Field::sl_shot_en(u_int8_t iX, u_int8_t iY)
{



    //local variables which are going to be passed by a signal
    u_int8_t iStatus;
    u_int8_t iLength;
    u_int8_t iShipPositions[10];

    qDebug() << "sl_shot_en" << iX << " " << iY;
    //are we actually ready to play?
    if(get_ready_to_play_state() == NOT_READY_TO_PLAY )
    {

        iStatus = 0x20;
        iLength = 0;
        emit si_a_shot_me(iStatus,iLength,iShipPositions);
        emit si_togui_history(qsDENIED);
        return;
    }

    //is it really the enemies turn?
    if(is_my_turn() == true)
    {
        //it is my turn, not enemies turn
        iStatus = 0x10;
        iLength = 0;
        emit si_a_shot_me(iStatus,iLength,iShipPositions);
        emit si_togui_history("Gegner ist ein Depp, wollte schiessen, aber ich bin dran");
        return;
    }

    //is the shot in range of our field ?
    if(iX > get_field_dim_x() || iY > get_field_dim_y())
    {
        //the enemy hit outside of our field
        iStatus = 0x11;
        iLength = 0;
        emit si_a_shot_me(iStatus,iLength,iShipPositions);
        emit si_togui_history(qsOUT_OF_RANGE);
        return;
    }
    //now we look what this hit is doing to our fiel
    //is it a hit or hit destroyed or hit destroyed&win or no hit?

    //first we check wether the enemy already hit on this field
    if(m_pvfieldtileme[iX][iY]->was_hit() == true)
    {
        iStatus = 0x20;
        iLength = 0;
        emit si_togui_history("Gegner ist ein Depp, hat bereits auf das Feld geschossen");
        emit si_a_shot_me(iStatus,iLength,iShipPositions);
        return;
    }
    //second we check wether this field tile is part of a ship or not
    if(m_pvfieldtileme[iX][iY]->get_ship_pointer() != NULL)
    {
        //tell the gui that enemy shot on this field
        emit si_indicate_shot(iX,iY);

        //decrease hitpoints of the ship
        m_pvfieldtileme[iX][iY]->get_ship_pointer()->decrement_hit_point();
        m_pvfieldtileme[iX][iY]->get_ship_pointer()->set_shipstate(SHIP_HIT);

        //maybe this ship is destroyed now
        if(m_pvfieldtileme[iX][iY]->get_ship_pointer()->get_hitpoints() == 0)
        {
            m_pvfieldtileme[iX][iY]->get_ship_pointer()->set_shipstate(SHIP_DESTROYED);
            //check wether the game is over
            if(is_game_over() == true)
            {
                //hit & destroyed & gameover
                iStatus = 0x03;
                iLength = m_pvfieldtileme[iX][iY]->get_ship_pointer()->get_shipsize();
                for(int i = 0; i < iLength; i++)
                {
                    iShipPositions[i*2]=m_pvfieldtileme[iX][iY]->get_ship_pointer()->get_x(i*2);

                    iShipPositions[i*2+1]=m_pvfieldtileme[iX][iY]->get_ship_pointer()->get_y(i*2+1);
                }
                emit si_a_shot_me(iStatus,iLength,iShipPositions);
                set_game_state(LOST);
                emit si_togui_history(qsLOST);
                emit si_lost_to_gui();
                return;
            }

            //hit & destroyed
            iStatus = 0x02;
            iLength = m_pvfieldtileme[iX][iY]->get_ship_pointer()->get_shipsize();
            for(int i = 0; i< iLength; i++)
            {
                iShipPositions[i*2] = m_pvfieldtileme[iX][iY]->get_ship_pointer()->get_x(i);
                iShipPositions[i*2+1] = m_pvfieldtileme[iX][iY]->get_ship_pointer()->get_y(i);
            }
            qDebug()<< iShipPositions[0] << " " << iShipPositions[1] << " "<< iShipPositions[1] << " " << iShipPositions[2];
            emit si_a_shot_me(iStatus,iLength,iShipPositions);
            set_turn(MY_TURN);
            emit si_togui_history("Ihr Schiff wurde versenkt");
            emit si_togui_history(qsMY_TURN);
            return;

        }

        //ship was hit, hit but not destroyed
        iStatus=0x01;
        iLength=0;
        emit si_a_shot_me(iStatus,iLength,iShipPositions);
        set_turn(MY_TURN);
        emit si_togui_history("Eines Ihrer Schiffe wurde getroffen");
        emit si_togui_history(qsMY_TURN);

        return;

    }
    //enemy hit the water
    else if(m_pvfieldtileme[iX][iY]->get_ship_pointer() == NULL)
    {
        //tell the gui that enemy shot on this field
        emit si_indicate_shot(iX,iY);

        m_pvfieldtileme[iX][iY]->set_field_tile_state(WATER_AND_HIT);
        qDebug() << "ShipPointer " << m_pvfieldtileme[iX][iY]->get_ship_pointer();
        qDebug() << "TileState " << m_pvfieldtileme[iX][iY]->get_field_tile_state();
        //enemy hit just water
        iStatus = 0x00;
        iLength = 0;
        emit si_a_shot_me(iStatus,iLength,iShipPositions);
        set_turn(MY_TURN);
        emit si_togui_history("Wasser..");
        emit si_togui_history(qsMY_TURN);
        return;
    }
}

void Field::sl_neg_param_en(u_int8_t uiX, u_int8_t uiY, u_int8_t uiCountBattleship, u_int8_t uiCountCruiser, u_int8_t uiCountDestroyer, u_int8_t uiCountSubmarine)
{
        qDebug() <<" "<<uiX <<" "<< uiY <<" "<<uiCountBattleship <<" "<< uiCountCruiser <<" " <<uiCountDestroyer <<" " <<uiCountSubmarine;
        save_neg_param(uiX,uiY,uiCountBattleship,uiCountCruiser,uiCountDestroyer,uiCountSubmarine);
        set_field_param();
        emit si_neg_param_en(uiX,uiY,uiCountBattleship,uiCountCruiser,uiCountDestroyer,uiCountSubmarine);
        set_game_state(PLACING_SHIPS);
}

void Field::sl_set_field_tile_gui(int iX, int iY, Logic::ShipSize eSize, int iId)
{
    //check wether gui gave us a ship part or just water
    qDebug() << "m_vShipSize: " << m_vShip.size();
    if(eSize == Logic::NO_SHIP)
    {
        return;
    }
    //If the vector is empty, no ship is in the vector, always creat the first ship
    if(m_vShip.size() == 0)
    {
        qDebug()<<"X :" << get_field_dim_x() << " Y: " << get_field_dim_y();
        m_vShip.push_back(new Logic::Ship(eSize,iId));
        m_vShip.front()->add_coordinates(iX,iY);
        //save the new information to the actual field
        fill_field_tile_information(iX,iY,m_vShip.front());
        return;
    }
    //if at least one ship exists in the vector, we have to find the right ship
    //according to the ShipSize and its id
    else if(m_vShip.size() > 0)
    {
        for(int i = 0; i<m_vShip.size();i++)
        {
            if(m_vShip[i]->get_shipsize() == eSize)
            {
                if(m_vShip[i]->get_ship_id() == iId)
                {
                    //we have found an already existing ship
                    //now we add the addition coordinates
                    m_vShip[i]->add_coordinates(iX,iY);
                    //save the new information to the actual field
                    fill_field_tile_information(iX,iY,m_vShip.at(i)); //Mögliches Problem wegen Zeiegr- Refernz
                    return;
                }

             }
           }
        }

    //if the vector is not empty and we did not find the corresponding ship
    //we have to add a new ship

        m_vShip.push_back(new Logic::Ship(eSize,iId));
        m_vShip.back()->add_coordinates(iX,iY); //Achtung die Benutzung Pointer zu Refernz koennte
        //Probleme machen !!!
        //save the new information to the actual field
        fill_field_tile_information(iX,iY,m_vShip.back());
        return;

    //Now we have new information about the ship
    //Now we have to update the field tiles

}


//SETTER
void Field::set_field_param()
{
    m_iDimX = m_sNegParam.uiX;
    m_iDimY = m_sNegParam.uiY;
    m_iCountBattleShip = m_sNegParam.uiCountBattleship;
    m_iCountCruiser = m_sNegParam.uiCountCruiser;
    m_iCountDestroyer = m_sNegParam.uiCountDestroyer;
    m_iCountSubmarine = m_sNegParam.uiCountSubmarine;
}

void Field::set_game_state(GameState eState)
{
    m_eGameState = eState;
}

void Field::set_turn(TurnState eTurn)
{
    m_eTurn = eTurn;
}

void Field::set_last_shot_position_y(int iY)
{
    m_iLastShotY = iY;
}

void Field::set_last_shot_position_x(int iX)
{
    m_iLastShotX = iX;
}

void Field::set_ready_to_play_state(ReadytoPlaySate eState)
{
    m_eReadyToPlayState = eState;
}




//GETTER


int Field::get_field_dim_x() const
{
    return m_iDimX;
}

int Field::get_field_dim_y() const
{
    return m_iDimY;
}

int Field::get_submarine_count() const
{
    return m_iCountSubmarine;
}

int Field::get_battleship_count() const
{
    return m_iCountBattleShip;
}

int Field::get_cruiser_count() const
{
    return m_iCountCruiser;
}

int Field::get_destroyer_count() const
{
    return m_iCountDestroyer;
}

int Field::get_last_shot_position_y()
{
    return m_iLastShotY;
}

GameState Field::get_game_state()
{
    return m_eGameState;
}

int Field::get_last_shot_position_x()
{
    return m_iLastShotX;
}

TurnState Field::get_turn()
{
    return m_eTurn;
}

ReadytoPlaySate Field::get_ready_to_play_state()
{
    return m_eReadyToPlayState;
}








