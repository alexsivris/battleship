#include "master.h"


Master::Master(QObject *parent) :
    QObject(parent)
{
    qDebug() << "Master starting..." ;
     m_pcoField = new Logic::Field();
     m_pcoNet = new Netzwerk::Netconnect(this);
     m_pcoGui = new Gui::battleship;

     m_pcoGui->setAttribute(Qt::WA_DeleteOnClose);
     m_pcoGui->show();

     // Gui -> Logic
     connect(m_pcoGui, SIGNAL(si_ships_ready_togui(int,int,Logic::ShipSize,int)),
             m_pcoField,SLOT(sl_set_field_tile_gui(int,int,Logic::ShipSize,int)));
     connect(m_pcoGui, SIGNAL(si_shot_gui(u_int8_t, u_int8_t)), m_pcoField, SLOT(sl_shot_gui(u_int8_t, u_int8_t)));
     connect(m_pcoGui, SIGNAL(si_request_start_gui()), m_pcoField, SLOT(sl_req_start_gui()));
     connect(m_pcoGui, SIGNAL(si_a_req_start_gui(u_int8_t)), m_pcoField, SLOT(sl_a_req_start_gui(u_int8_t)));
     connect(m_pcoGui, SIGNAL(si_a_neg_param_me(u_int8_t)), m_pcoField, SLOT(sl_a_param_me(u_int8_t)));

     //von GuiServer
     connect(m_pcoGui, SIGNAL(si_neg_param_gui(u_int8_t,u_int8_t,u_int8_t,u_int8_t,u_int8_t,u_int8_t)),m_pcoField, SLOT(sl_neg_param_gui(u_int8_t,u_int8_t,u_int8_t,u_int8_t,u_int8_t,u_int8_t)));


     // Logic ->Gui
     connect(m_pcoField,SIGNAL(si_req_start_en()),m_pcoGui,SLOT(sl_req_start_en())); // Wenn die Schiffe schon platziert sind
     connect(m_pcoField,SIGNAL(si_neg_param_en(u_int8_t,u_int8_t,u_int8_t,u_int8_t,u_int8_t,u_int8_t)), m_pcoGui, SLOT(sl_neg_param_en(u_int8_t,u_int8_t,u_int8_t,u_int8_t,u_int8_t,u_int8_t)));
     connect(m_pcoField,SIGNAL(si_place_ships()),m_pcoGui,SLOT(sl_place_ships()));
     connect(m_pcoField, SIGNAL(si_set_field_tile_me(int,int,Logic::FieldTileState)), m_pcoGui, SLOT(sl_enemy_tile_hit(int,int,Logic::FieldTileState)));
     connect(m_pcoField, SIGNAL(si_indicate_shot(u_int8_t,u_int8_t)), m_pcoGui, SLOT(sl_indicate_shot(u_int8_t, u_int8_t)));
     connect(m_pcoField, SIGNAL(si_disable_start_button()), m_pcoGui, SLOT(sl_disable_start_button()));
     connect(m_pcoField, SIGNAL(si_enable_start_button()), m_pcoGui, SLOT(sl_enable_start_button()));
     connect(m_pcoField, SIGNAL(si_win_to_gui()), m_pcoGui, SLOT(sl_win_to_gui()));
     connect(m_pcoField, SIGNAL(si_lost_to_gui()), m_pcoGui, SLOT(sl_lost_to_gui()));


     // m_pcoField -> m_pcoNet
     connect(m_pcoField,SIGNAL(si_a_shot_me(u_int8_t,u_int8_t,u_int8_t*)),m_pcoNet,SLOT(sl_a_shot_me(u_int8_t,u_int8_t,u_int8_t*)));
     connect(m_pcoField,SIGNAL(si_shot_me(u_int8_t,u_int8_t)),m_pcoNet,SLOT(sl_shot_me(u_int8_t,u_int8_t)));
     connect(m_pcoField,SIGNAL(si_req_start_gui()),m_pcoNet,SLOT(sl_req_start_me()));
     connect(m_pcoField,SIGNAL(si_a_req_start_gui(u_int8_t)),m_pcoNet,SLOT(sl_a_req_me(u_int8_t)));
     connect(m_pcoField,SIGNAL(si_neg_param_me(u_int8_t,u_int8_t,u_int8_t,u_int8_t,u_int8_t,u_int8_t)),m_pcoNet,SLOT(sl_neg_param_me(u_int8_t,u_int8_t,u_int8_t,u_int8_t,u_int8_t,u_int8_t)));
     // answer to parameter negotiation, to server
     connect(m_pcoField,SIGNAL(si_a_req_me(u_int8_t)),m_pcoNet,SLOT(sl_a_req_me(u_int8_t)));

     // m_pcoNet -> m_pcoField
     connect(m_pcoNet,SIGNAL(si_shot_en(u_int8_t,u_int8_t)),m_pcoField,SLOT(sl_shot_en(u_int8_t,u_int8_t)));
     connect(m_pcoNet,SIGNAL(si_a_shot_en(u_int8_t,u_int8_t,u_int8_t*)),m_pcoField,SLOT(sl_a_shot_en(u_int8_t,u_int8_t,u_int8_t*)));
     connect(m_pcoNet,SIGNAL(si_a_req_en(u_int8_t)),m_pcoField,SLOT(sl_a_req_en(u_int8_t)));
     connect(m_pcoNet,SIGNAL(si_req_start_en()),m_pcoField,SLOT(sl_req_start_en()));
     connect(m_pcoNet,SIGNAL(si_neg_param_en(u_int8_t,u_int8_t,u_int8_t,u_int8_t,u_int8_t,u_int8_t)),m_pcoField,SLOT(sl_neg_param_en(u_int8_t,u_int8_t,u_int8_t,u_int8_t,u_int8_t,u_int8_t)));

     // TODO connect
     // sl_a_req_me sl_req_start_me             si_a_req_en si_req_start_en si_neg_param_en
     // si_wrong_cmd si_socket_error

     // Net <-> Gui
    // Server
     connect(m_pcoGui, SIGNAL(si_start_server()), m_pcoNet, SLOT(sl_start_server()));
     connect(m_pcoGui, SIGNAL(si_request_ip_port()), m_pcoNet, SLOT(sl_request_ip_port()));
     connect(m_pcoGui,SIGNAL(si_disconnect()), m_pcoNet, SLOT(sl_disconnect()));
     connect(m_pcoNet, SIGNAL(si_togui_ip_port(QString,QString)), m_pcoGui, SLOT(sl_togui_ip_port(QString,QString)));
     connect(m_pcoNet, SIGNAL(si_server_connected()), m_pcoGui, SLOT(sl_client_accepted())); // if client is connected to server

     // Client
     connect(m_pcoGui,SIGNAL(si_toclient_ip_port(QString, QString)), m_pcoNet, SLOT(sl_fromgui_ip_port(QString,QString)));
     connect(m_pcoGui,SIGNAL(si_start_client()), m_pcoNet, SLOT(sl_start_client()));

     // History to Gui
      connect(m_pcoNet,SIGNAL(si_togui_history(QString)), m_pcoGui, SLOT(sl_togui_history(QString)));
      connect(m_pcoField,SIGNAL(si_togui_history(QString)),m_pcoGui,SLOT(sl_togui_history(QString)));



      //close program
      connect(m_pcoGui, SIGNAL(si_close_app()), this, SLOT(sl_master_close()));

}

void Master::sl_master_close()
{
    delete m_pcoGui;
    delete m_pcoNet;
    delete m_pcoField;
}
