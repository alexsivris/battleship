#ifndef FIELD_H
#define FIELD_H
#include "fieldtile.h"
#include <vector>
#include <QObject>
#include <QString>


/**
 * @brief The namespace Logic has the purpose of grouping the classes Logic::Field, Logic::FieldTile and Logic::Ship together as the game's Logic
 */
namespace Logic
{
class Field;
/**
 * @brief HitResult - these enums will be returned after a player shoots
 */
enum HitResult  {NOT_HIT,   HIT,    HIT_AND_DESTROYED,  HIT_AND_DESTROYED_GAME_END, HIT_OUT_OF_RNG=0x11,    HIT_ERROR=0x20};
/**
 * @brief The TurnState enum indicates whether it is the players turn to play or not
 */
enum TurnState {NOT_MY_TURN, MY_TURN};
/**
 * @brief The ReadytoPlaySate enums indicate whether the players are ready to play or not
 */
enum ReadytoPlaySate {NOT_READY_TO_PLAY,READY_TO_PLAY};
/**
 * @brief The GameState enum indicates whether the players are playing, not playing, placing their ships or if they have won/lost
 */
enum GameState {PLAYING,NOT_PLAYING,PLACING_SHIPS,WON,LOST};
/**
 * @brief The RequestAnwser enum enumes for answer to field negotiations
 */
enum RequestAnwser {OK = 0x00, DENIED_OR_NOT_READY = 0x01, NOT_SUPPORTED = 0x02, MESSAGE_INCOMPLETE = 0x03, TIMEOUT = 0x04, ANNY_ERROR =20};
/**
 * @brief The Field class contains the games' main logic. It creates the playing field and fills the field tiles with information (coordinates, state)
 */
class Field : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Field the construcor
     */
    Field();

    /**
     * @brief ~Field() the destructor
     */
    ~Field();

    /**
     * @brief get_battleship_count
     * @return number of battleships
     */
    int get_battleship_count(void) const;
    /**
     * @brief get_cruiser_count
     * @return number of cruisers
     */
    int get_cruiser_count(void) const;
    /**
     * @brief get_destroyer_count
     * @return number of destroyers
     */
    int get_destroyer_count(void) const;
    /**
     * @brief get_submarine_count
     * @return number of submarines
     */
    int get_submarine_count(void) const;

    /**
     * @brief fill_field_tile_information - each field tile should point to the ship which it represents.
     * @param iX - X coordinate of the field tile
     * @param iY - Y coordinate of the field tile
     * @param pShip - pointer to ship object
     */
    void fill_field_tile_information(int iX, int iY, Logic::Ship* pShip);

    /**
     * @brief get_field_dim_x
     * @return the field dimensions in X direction
     */
    int get_field_dim_x() const;

    /**
     * @brief get_field_dim_y
     * @return the field dimensions in Y direction
     */
    int get_field_dim_y() const;

    /**
     * @brief set_field_param - this function is called when the field parameters are negotiated and accepted
     * @param x - length of the playing field in X direction
     * @param y - length of the playing field in Y direction
     * @param iCountBattleShip - number of battle ships
     * @param iCountCruiser - number of cruisers
     * @param iCountDestroyer - number of destroyers
     * @param iCountSubmarine - number of submarines
     */
    void set_field_param();

    /**
     * @brief is_my_turn check whether it is the players turn or not
     * @return return TRUE if it is the players turn, FALSE if it is not
     */
    bool is_my_turn();

    /**
     * @brief is_game_over
     * @return TRUE if all ships have the state SHIP_DESTROYED, otherwise it returns FALSE
     */
    bool is_game_over();

    /**
     * @brief set_turn sets the turn
     * @param eTurn either MY_TURN or NOT_MY_TURN
     */
    void set_turn(TurnState eTurn);

    /**
     * @brief get_turn gets the turn
     * @return either MY_TURN or NOT_MY_TURN
     */
    TurnState get_turn();

    /**
     * @brief set_ready_to_play_state sets variable m_eReadyToPlayState to YES or NO
     * @param eState is either NO or YES
     */
    void set_ready_to_play_state(ReadytoPlaySate eState);

    /**
     * @brief get_ready_to_play_state a function that is called when the player wants to know if both players are ready to play or not
     * @return whether both players are ready to play or not
     */
    ReadytoPlaySate get_ready_to_play_state();

    /**
     * @brief set_last_shot_position_x saves the x coordinates where the last shot was placed
     * @param iX X position of the field tile
     */
    void set_last_shot_position_x(int iX);

    /**
     * @brief set_last_shot_position_y saves the y coordinates where the last shot was placed
     * @param iY X position of the field tile
     */
    void set_last_shot_position_y(int iY);

    /**
     * @brief get_last_shot_position_x
     * @return the X coordinate where the last shot was placed
     */
    int get_last_shot_position_x();

    /**
     * @brief get_last_shot_position_y
     * @return Y coordinate where the last shot was placed
     */
    int get_last_shot_position_y();

    /**
     * @brief set_game_state
     * @param eState PLAYING,NOT_PLAYING,WON,LOST
     */
    void set_game_state(GameState eState);

    /**
     * @brief get_game_state
     * @return the state of the game PLAYING, NOT_PLAYING, WON, LOST
     */
    GameState get_game_state();

    /**
     * @brief set_neg_param saves requested game parameters to a struct
     * @param uiX X dimension of the field
     * @param uiY Y dimension of the field
     * @param uiCountBattleship number of BATTLESHIPS
     * @param iCountCruiser number of CRUISERS
     * @param iCountDestroyer number of DESTROYERS
     * @param iCountSubmarine number of SUBMARINES
     */
    void save_neg_param(u_int8_t uiX, u_int8_t uiY, u_int8_t uiCountBattleship, u_int8_t uiCountCruiser, u_int8_t uiCountDestroyer, u_int8_t uiCountSubmarine);


signals:

    /**
     * @brief si_a_shot_me
     * @param iStatus is either: hit,not hit, hit and destroyed, hit & destroyed & gameover...
     * @param iLength - length of the ship that has been destroyed
     * @param iShipPositions contains the coordinates of the tiles which built the ship
     */
    void si_a_shot_me(u_int8_t iStatus,u_int8_t iLength,u_int8_t* iShipPositions);

    /**
     * @brief si_shot_me the field class emits this signal for the network
     * @param iX - the X coordinate of the field tile
     * @param iY - the Y coordinate of the field tile
     */
    void si_shot_me(u_int8_t iX, u_int8_t iY);
    /**
     * @brief si_neg_param_me - emitted for the network
     * @param iX - length of the playing field in X direction
     * @param iY - length of the playing field in Y direction
     * @param iCountBattleShip - number of battle ships
     * @param iCountCruiser - number of cruisers
     * @param iCountDestroyer - number of destroyers
     * @param iCountSubmarine - number of submarines
     */
    void si_neg_param_me(u_int8_t uiX, u_int8_t uiY, u_int8_t uiCountBattleship, u_int8_t uiCountCruiser, u_int8_t uiCountDestroyer, u_int8_t uiCountSubmarine);

    /**
     * @brief si_a_req_en //ACHTUNG KEINE AHNUNG OB ICH DAS BRAUCHE -- HAHA OK
     * @param uiStatus
     */
    void si_a_req_en(u_int8_t uiStatus); //TODO siehe oben

    /**
     * @brief si_a_neg_param_me
     * @param uiStatus enum RequestAnwser
     */
    void si_a_neg_param_me(u_int8_t uiStatus);

    /**
     * @brief si_req_start_me is emitted when the GUI signals a clicked request_start_button
     */
    void si_req_start_gui();
    /**
     * @brief si_shot_gui sends the coordinates of the clicked tile (a shot) from the GUI
     * @param iX - X coordinate of the shot
     * @param iY - Y coordinate of the shot
     */
    void si_shot_gui(u_int8_t iX, u_int8_t iY);
    /**
     * @brief si_set_field_tile_me signals the GUI that and which field tile has changed
     * @param iX X position of the field tile
     * @param iY Y position of the field tile
     * @param eState enum FieldTileState
     */
    void si_set_field_tile_me(int iX, int iY, Logic::FieldTileState eState); //TODO connect that with Alex !


    /**
     * @brief si_not_supported_me is emitted if the enemy does not support the requested game
     */
    void si_not_supported_me();

    /**
     * @brief si_msg_incomplete is emitted if the enemy complains that a message was incomplete
     */
    void si_msg_incomplete_me();

    /**
     * @brief si_timeout emitted when the enemy complains about a timeout
     *
     */
    void si_timeout_me();

    /**
     * @brief si_error is emitted if any error occours or the status code of an answer is invalid
     */
    void si_error_me();
    /**
     * @brief si_a_req_me KEINE AHNUNG OB ER DAS BRAUCHT
     * @param uiStatus
     */
    void si_a_req_me(u_int8_t uiStatus);

    /**
     * @brief si_togui_history si to make the gui display a message during the game
     * @param qsString String containg message what is going on
     */
    void si_togui_history(QString qsString);

    /**
     * @brief si_req_start_en forwarded signal when network told the logicn that enemy requested game start
     */
    void si_req_start_en();

    /**
     * @brief si_a_req_start_gui signal for the network containg the decission of the gui
     */
    void si_a_req_start_gui(u_int8_t uiStatus);

    /**
     * @brief si_activate_me this signals activates the enemy field in the gui
     */
    void si_activate_me();

    /**
     * @brief si_neg_param_en sends the game parameters (negotiated by the opponent) to the GUI
     * @param uiX - length of the field in X direction
     * @param uiY - length of the field in Y direction
     * @param uiCountBattleship - number of placeable Battleships
     * @param uiCountCruiser - number of placeable Cruisers
     * @param uiCountDestroyer - number of placeable Destroyers
     * @param uiCountSubmarine - number of placeable Submarines
     */
    void si_neg_param_en(u_int8_t uiX, u_int8_t uiY, u_int8_t uiCountBattleship, u_int8_t uiCountCruiser, u_int8_t uiCountDestroyer, u_int8_t uiCountSubmarine);


    /**
     * @brief si_place_ships - this signal tells the GUI to allow the ship placement
     */
    void si_place_ships();

    void si_indicate_shot(u_int8_t uiX,u_int8_t uiY);
    /**
     * @brief si_disable_start_button disables the start button
     */
    void si_disable_start_button();

    /**
     * @brief si_enable_start_button enables the start button
     */
    void si_enable_start_button();

    /**
     * @brief si_win_to_gui gui displays a window to the winner
     */
    void si_win_to_gui();

    /**
     * @brief si_lost_to_gui gui displays a window to the looser
     */
    void si_lost_to_gui();


    // void si_req_start_en() //TODO declreate and implementate

    //si_wrong_cmd //TODO declerate and implementate




public slots:

    /**
     * @brief sl_set_field_tile_gui - creates ship objects from the ship class and pushes them to the m_vShip vector
     * @param iX - the X coordinate of the field tile
     * @param iY - the Y coordinate of the field tile
     * @param esize - the ShipSize, showing what kind of ship you are dealing with
     * @param iId - individual ship ID for every type of ShipSize
     */
    void sl_set_field_tile_gui(int iX, int iY, Logic::ShipSize eSize, int iId );

    /**
     * @brief sl_neg_param_en is executed when receiving a signal from network or GUI containing the negotiated field parameters
     * @param iX - length of the gamefield in x direction
     * @param iY - length of the gamefield in y direction
     * @param iCountBattleShip - number of battle ships
     * @param iCountCruiser - number of cruisers
     * @param iCountDestroyer - number of destroyers
     * @param iCountSubmarine - number of submarines
     */
    void sl_neg_param_en( u_int8_t uiX, u_int8_t uiY, u_int8_t uiCountBattleship, u_int8_t uiCountCruiser, u_int8_t uiCountDestroyer, u_int8_t uiCountSubmarine );


    /**
     * @brief sl_shot_en is executed after si_shot_en(..,..);
     * @param iX X coordinate where the enemy hit on the field
     * @param iY Y coordinate where the enemy hit on the field
     */
    void sl_shot_en(u_int8_t iX, u_int8_t iY);



    /**
     * @brief sl_a_shot_en executed after by si_a_shot_en(..,..,..,);
     * @param iStatus is either: hit,not hit, hit and destroyed, hit & destroyed & gameover...
     * @param iLength - length of the ship that has been destroyed
     * @param iShipPositions contains the coordinates of the tiles which built the ship
     */
    void sl_a_shot_en(u_int8_t iStatus,u_int8_t iLength,u_int8_t *iShipPositions);


    /**
     * @brief sl_req_start_gui is executed when gui wants to start the game
     */
    void sl_req_start_gui();

    /**
     * @brief sl_shot_gui is executed when GUI sends coordinates of a shot
     * @param iX X Position of the clicked tile
     * @param iY Y Position of the clicked tile
     */
    void sl_shot_gui(u_int8_t iX, u_int8_t iY);

    /**
     * @brief sl_neg_param_gui slot is executed when gui whants to playn with these game parameters
     * @param uiX X dimension of the field
     * @param uiY Y dimesnion of the field
     * @param uiCountBattleship numbers of BATTLESHIPS
     * @param iCountCruiser number of CRUISERS
     * @param iCountDestroyer number of DESTROYERS
     * @param iCountSubmarine number SUBMARINES
     */
    void sl_neg_param_gui(u_int8_t uiX, u_int8_t uiY, u_int8_t uiCountBattleship, u_int8_t iCountCruiser, u_int8_t iCountDestroyer, u_int8_t iCountSubmarine);

    /**
     * @brief sl_a_req_en slot is executed after signal si_a_req_en() is emmited
     * @param uiStatus contains the Status id which the enemy sent
     */
    void sl_a_req_en(u_int8_t uiStatus);

    /**
     * @brief sl_req_start_en the network triggers this slot with si_req_start_en()
     */
    void sl_req_start_en();

    /**
     * @brief sl_a_req_start_gui is executed to tell the GUI if the player wants to begin the game or does not
     * @param uiStatus - yes  0x00; no  0x01
     */
    void sl_a_req_start_gui(u_int8_t uiStatus);

    /**
     * @brief sl_a_param_me is executed to get the state "placing ships" and to send OK to the Client
     * @param ui8Status
     */
    void sl_a_param_me(u_int8_t ui8Status);


private:



    /**
     * @brief fill_m_v_field_tile_vector - creates m_iDimX*m_iDimY field tile elements on the heap and puts them in the 2D Vector;
     */
    void fill_my_field_vector();

    /**
     * @brief fill_enemy_field_vector- creates m_iDimX*m_iDimY field tile elements on the heap and puts them in the 2D Vector;
     */
    void fill_enemy_field_vector();

    /**
     * @brief m_iDimX - negotiated X-dimension of the field
     */
    int m_iDimX;

    /**
     * @brief m_iDimY - negotiated Y-dimension of the field
     */
    int m_iDimY;

    /**
     * @brief m_iLastShotX saves the X position of the current shot we did on the gui
     */
    int m_iLastShotX;

    /**
     * @brief m_iLastShotY saves the Y position of the current shot we did on the gui
     */
    int m_iLastShotY;

    /**
     * @brief m_iCountBattleShip - negotiated numbers of battle ships
     */
    int m_iCountBattleShip;

    /**
     * @brief m_iCountCruiser - negotiated numbers of cruisers
     */
    int m_iCountCruiser;

    /**
     * @brief m_iCountDestroyer - negotiated numbers of destroyers
     */
    int m_iCountDestroyer;

    /**
     * @brief m_iCountSubmarine - negotiated numbers of submarines
     */
    int m_iCountSubmarine;

    /**
     * @brief m_vfieldtile - two dimensional vector. Its elements contain a pointer to a single field tile.
     */
    std::vector< std::vector<Logic::FieldTile* > > m_pvfieldtileme;

    /**
     * @brief m_pvfieldtileenemy - two dimensional vector. Its elements contain a pointer to a single field tile.
     */
    std::vector< std::vector<Logic::FieldTile* > > m_pvfieldtileenemy;

    /**
     * @brief m_vShip - This vector contains all existing ship objects, destroyed or not
     */
    std::vector<Logic::Ship*> m_vShip;

    /**
     * @brief m_eTurn saves whether it is the players turn or the enemies turn
     */
    TurnState m_eTurn;
    /**
     * @brief m_eReadyToPlayState saves whether the players are READY_TO_PLAY or NOT_READY_TO_PLAY
     */
    ReadytoPlaySate m_eReadyToPlayState;

    GameState m_eGameState;

    /**
     * @brief The NegParam struct is used to negotiate game parameters such as field size and amount of ships
     */
    struct NegParam
    {
        u_int8_t uiX;
        u_int8_t uiY;

        u_int8_t uiCountBattleship;
        u_int8_t uiCountCruiser;
        u_int8_t uiCountDestroyer;
        u_int8_t uiCountSubmarine;

    } m_sNegParam;

    QString qsNOT_PLAYING = "Es wird nicht gespielt";
    QString qsPLACING_SHIPS = "Schiffe muessen jetzt platziert werden";
    QString qsPLAYING = "Es wird gespielt";
    QString qsNEG_PARAM_OK = "Spieleparameter wurden angenommen";
    QString qsDENIED = "Spieleparameter wurden abgelehnt oder Gegenspieler ist nicht bereit";
    QString qsNOT_SUPPORTED = "Spieleparameter werden nicht unterstützt";
    QString qsMSG_INCOMPLETE= "Nachricht ist unvollstaendig";
    QString qsTIMEOUT = "Timeout beim Warten auf Bestaetigung";
    QString qsNOT_HIT = "Wasser";
    QString qsHIT = "Getroffen!";
    QString qsHIT_AND_DESTROYED = "Getroffen und versenkt";
    QString qsNOT_YOUR_TURN = "Nicht an der Reihe";
    QString qsOUT_OF_RANGE = "Ausserhalb des Feldes";
    QString qsANY_ERROR = "Ups... hihi. ähh FeHleR :)";
    QString qsMY_TURN = "Sie sind dran";
    QString qsENEMY_TURN = "Der Gegner ist dran";
    QString qsWAIT_FOR_ANSW = "Warte auf eine Antwort des Gegners";
    QString qsREQUEST = "Sie wurden aufgefordert das Spiel anzunehmen";
    QString qsACCEPTED = "Das Spiel wurde angenommen";
    QString qsWIN = "Sie haben gewonnen!";
    QString qsLOST = "Sie haben das Spiel verloren ... ";




};
}



#endif // FIELD_H
