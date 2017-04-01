/** \brief The game starts here
  * \author Alexandros Sivris
  * \date March 2015
  */
#ifndef BATTLESHIP_H
#define BATTLESHIP_H

#include <QWidget>
#include "guiserver.h"
#include "guiclient.h"
#include "net.h"
#include "battlefield.h"
#include <QDesktopWidget>
#include <QTextBrowser>
#include "field.h"
#include "ui_battleship.h"
#include <QFile>
#include <QDebug>
#include "net.h"
#include <QMessageBox>

namespace Gui {
class battleship;
}
namespace Ui {
class battleship;
}

class Gui::battleship : public QWidget
{
    Q_OBJECT

public:
    explicit battleship(QWidget *parent = 0);
    ~battleship();

void setBfPointer(Battlefield * ptr);
    Gui::battleship *m_pubtn;
public slots:
    /**
     * @brief sl_server_settings - contains the Server settings, executed right before a new Server is created
     */
    void sl_server_settings();
    /**
     * @brief sl_client_settings - contains the Client settings, executed right before a new Client is created
     */
    void sl_client_settings();
    /**
     * @brief pipelines si_togui_ip_port(QString, QString) to the master-class
     */
    void sl_togui_ip_port(QString, QString);
    /**
     * @brief sl_ships_ready_togui - implicates that all of the ships are set in place, sends the coordinates of all ships to the GUI
     * @param x
     * @param y
     * @param sSize
     * @param id
     */
    void sl_ships_ready_togui(int x,int y,Logic::ShipSize sSize,int id);
    /**
     * @brief pipelines si_request_ip_port() to the master-class
     */
    void sl_request_ip_port();
    /**
     * @brief pipelines si_toclient_ip_port(QString, QString) to the master-class
     */
    void sl_toclient_ip_port(QString, QString);
    /**
     * @brief si_disconnect() to master-class
     */
    void sl_disconnect();
    /**
     * @brief receives data from master class
     */
    void sl_togui_history(QString);
    /**
     * @brief launches the client configuration and emits the corresponding signal for the master-class
     */
    void sl_start_client();
    /**
     * @brief emits si_neg_param_gui(u_int8_t iX, u_int8_t iY, u_int8_t iCountBattleship, u_int8_t iCountCruiser, u_int8_t iCountDestroyer, u_int8_t iCountSubmarine) to the master class
     * @param iX
     * @param iY
     * @param iCountBattleship
     * @param iCountCruiser
     * @param iCountDestroyer
     * @param iCountSubmarine
     */
    void sl_neg_param_gui(u_int8_t iX, u_int8_t iY, u_int8_t iCountBattleship, u_int8_t iCountCruiser, u_int8_t iCountDestroyer, u_int8_t iCountSubmarine);
    /**
     * @brief si_shot_gui(u_int8_t uiX, u_int8_t uiY) to master class when a shot is fired to one of the enemy tiles
     * @param uiX
     * @param uiY
     */
    void sl_shot_gui(u_int8_t uiX, u_int8_t uiY);
    /**
     * @brief si_close_app() to the master class, which handles the closing sequence of the program
     */
    void sl_close_app();
    /**
     * @brief si_request_start_gui() to the master class which triggers a slot in the Field class
     */
    void sl_request_start_gui();
    /**
     * @brief emits si_request_start_gui(quint8) which contains either 0x00 (OK) or 0x01 (DENIED) as a parameter
     */
    void sl_req_start_en();
    /**
     * @brief when client is connected and must verify the game parameters
     */
    void sl_client_accepted();
    /**
     * @brief emits signal to negotiate enemy parameters
     */
    void sl_neg_param_en(u_int8_t,u_int8_t,u_int8_t,u_int8_t,u_int8_t,u_int8_t);
    /**
     * @brief enables the battlefield
     */
    void sl_place_ships(); // by Phil und Richard

    void sl_enemy_tile_hit(int iX, int iY, Logic::FieldTileState eState);
    void sl_indicate_shot(u_int8_t, u_int8_t);
    void sl_disable_start_button();
    void sl_enable_start_button();
    void sl_win_to_gui();
    void sl_lost_to_gui();
signals:
    /**
     * @brief triggered when a shot is fired at the enemy field
     * @param uiX x-coordinate of target
     * @param uiY y-coordinate of target
     */
    void si_shot_gui(u_int8_t uiX, u_int8_t uiY);
    /**
     * @brief negotiate own parameters
     * @param iX dimension(x)
     * @param iY dimension(y)
     * @param iCountBattleship amount of battleships
     * @param iCountCruiser amount of cruisers
     * @param iCountDestroyer amount of destroyers
     * @param iCountSubmarine amount of submarines
     */
    void si_neg_param_gui(u_int8_t iX, u_int8_t iY, u_int8_t iCountBattleship, u_int8_t iCountCruiser, u_int8_t iCountDestroyer, u_int8_t iCountSubmarine);
    /**
     * @brief emits when the client-mode is clicked
     */
    void si_client_btn_clicked();
    /**
     * @brief request ip and port when starting the server
     */
    void si_request_ip_port();
    /**
     * @brief ships are ready in the logic and sent to the gui
     */
    void si_ships_ready_togui(int,int,Logic::ShipSize,int);
    /**
     * @brief emits when the server is started
     */
    void si_start_server();
    /**
     * @brief emits when client is started
     */
    void si_start_client();
    /**
     * @brief signal to client: sending ip and port
     */
    void si_toclient_ip_port(QString, QString);
    /**
     * @brief disconnect
     */
    void si_disconnect();
    /**
     * @brief finally close app
     */
    void si_close_app();
    /**
     * @brief request to the gui to start
     */
    void si_request_start_gui();
    /**
     * @brief answer to the request from the gui
     */
    void si_a_req_start_gui(u_int8_t);
    /**
     * @brief client accepted the game
     */
    void si_client_accepted();
    /**
     * @brief enemy negotiates my parameters
     * @param ui8Status status of tiles
     */
    void si_a_neg_param_me(u_int8_t ui8Status);

    /**
     * @brief si_enemy_tile_hit enemy hits tile
     * @param iX x-coordinate
     * @param iY y-coordinate
     * @param eState ship type
     */
    void si_enemy_tile_hit(int iX, int iY, Logic::FieldTileState eState);
    void si_indicate_shot(u_int8_t, u_int8_t);
    void si_disable_start_button();
    void si_enable_start_button();

private slots:
    //void on_battleshipBtnClientMode_clicked();

private:
    /**
     * @brief m_bsUi pointer to the ui namespace of the battleship class (to access the graphical elements)
     */
    Ui::battleship *m_bsUi;
    /**
     * @brief m_pgs pointer to the GuiServer class
     */
    Gui::GuiServer *m_pgs;
    /**
     * @brief m_pgc pointer to the GuiClient class
     */
    Gui::GuiClient *m_pgc;
    /**
     * @brief m_pbattlefield pointer to the battlefield class
     */
    Gui::Battlefield *m_pbattlefield;
    /**
     * @brief m_qtbHistory pointer to the history window, which will be dynamically updated during the course of the program
     */
    QTextBrowser *m_qtbHistory;
};

#endif // BATTLESHIP_H
