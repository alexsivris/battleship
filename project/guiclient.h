/** \brief The graphical user interface of the client-configuration
  * \author Alexandros Sivris
  * \date March 2015
  */
#ifndef GUICLIENT_H
#define GUICLIENT_H


#include "net.h"
#include "battlefield.h"
#include "ui_guiclient.h"
#include <QFile>


namespace Gui {
class GuiClient;
}
namespace Ui {
class GuiClient;
}
/**
 * @brief The GuiClient class generates the widget of the client configuration.
 */

class Gui::GuiClient : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief GuiClient binds the Ui-
     * @param parent
     */
    explicit GuiClient(QWidget *parent = 0);
    ~GuiClient();

signals:
    /**
     * @brief si_tonet_ip_port - sends the IP and port to the network
     */
    void si_tonet_ip_port(QString , QString);

    /**
     * @brief si_client_disconnected - sent if the client is disconnected from the host
     */
    void si_client_disconnected();

    /**
     * @brief si_start_client - sent to start the Client window
     */
    void si_start_client();
    /**
     * @brief si_close_app - sent to close the game
     */
    void si_close_app();
public slots:

    /**
     * @brief sl_emit_ip_port - executed to call the IP and Port from Client
     */
    void sl_emit_ip_port();

    /**
     * @brief sl_client_disconnected - executed to call a disconnect
     */
    void sl_client_disconnected();
    /**
     * @brief sl_close_app - executed to close the game as a client
     */
    void sl_close_app();
private:
    /**
     * @brief m_gcUi - used to refer to anything concerning the client (connections, Ip, port, ...)
     */
    Ui::GuiClient *m_gcUi;
    /**
     * @brief m_pClient - pointer to the Netzwerk::Client
     */
    Netzwerk::Client *m_pClient;
    /**
     * @brief m_pbattlefield - pointer to the Gui::Battlefield
     */
    Gui::Battlefield *m_pbattlefield;
};

#endif // GUICLIENT_H
