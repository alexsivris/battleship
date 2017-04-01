#include "battleship.h"



using namespace Gui;

battleship::battleship(QWidget *parent) :
    QWidget(parent),
    m_bsUi(new Ui::battleship)
{
    m_bsUi->setupUi(this);

    m_pgs = new GuiServer;
    m_pgc = new GuiClient;
    m_pbattlefield = new Battlefield;

    m_pgc->setWindowFlags(Qt::FramelessWindowHint);
    m_pgs->setWindowFlags(Qt::FramelessWindowHint);
    m_pbattlefield->setWindowFlags(Qt::FramelessWindowHint);



    this->move(QApplication::desktop()->availableGeometry().center() - this->rect().center());

    connect(m_bsUi->battleshipBtnServerMode, SIGNAL(clicked()), this, SLOT(sl_server_settings()));
    connect(m_bsUi->battleshipBtnClientMode, SIGNAL(clicked()), this, SLOT(sl_client_settings()));
    //connect(m_pgs, SIGNAL(si_request_ip_port()), , SLOT());
    connect(m_pbattlefield, SIGNAL(si_ships_ready_togui(int,int,Logic::ShipSize,int)), this, SLOT(sl_ships_ready_togui(int,int,Logic::ShipSize,int)));
    connect(m_pbattlefield, SIGNAL(si_shot_gui(u_int8_t, u_int8_t)), this, SLOT(sl_shot_gui(u_int8_t, u_int8_t)));
    connect(m_pbattlefield, SIGNAL(si_request_start_gui()), this, SLOT(sl_request_start_gui()));

    //server mode slot
    connect(m_pgs, SIGNAL(si_request_ip_port()), this, SLOT(sl_request_ip_port()));
    connect(m_pgs, SIGNAL(si_server_disconnected()), this, SLOT(sl_disconnect()));

    //client mode
    connect(m_pgc, SIGNAL(si_tonet_ip_port(QString, QString)), SLOT(sl_toclient_ip_port(QString, QString)));
    connect(m_pgc, SIGNAL(si_client_disconnected()), this, SLOT(sl_disconnect()));
    connect(m_pgs, SIGNAL(si_neg_param_gui(u_int8_t,u_int8_t,u_int8_t,u_int8_t,u_int8_t,u_int8_t)), this, SLOT(sl_neg_param_gui(u_int8_t,u_int8_t,u_int8_t,u_int8_t,u_int8_t,u_int8_t)));


    //close app

    connect(m_pgc, SIGNAL(si_close_app()), this, SLOT(sl_close_app()));
    connect(m_pgs, SIGNAL(si_close_app()), this, SLOT(sl_close_app()));
    connect(m_pbattlefield, SIGNAL(si_close_app()), this,SLOT(sl_close_app()));

    // slot of battlefield
    connect(this, SIGNAL(si_enemy_tile_hit(int,int,Logic::FieldTileState)), m_pbattlefield, SLOT(sl_enemy_tile_hit(int, int, Logic::FieldTileState)));
    connect(this, SIGNAL(si_indicate_shot(u_int8_t,u_int8_t)), m_pbattlefield, SLOT(sl_indicate_shot(u_int8_t, u_int8_t)));
    connect(this, SIGNAL(si_disable_start_button()), m_pbattlefield, SLOT(sl_disable_start_button()));
    connect(this, SIGNAL(si_enable_start_button()), m_pbattlefield, SLOT(sl_enable_start_button()));

}


battleship::~battleship()
{
    delete m_bsUi;
    delete m_pgs;
    delete m_pgc;
    delete m_pbattlefield;
    delete m_qtbHistory;
    qDebug() << "battleship deleted";
}

void battleship::sl_disable_start_button()
{
    emit si_disable_start_button();

}
void battleship::sl_enable_start_button()
{
    emit si_enable_start_button();
}

void battleship::sl_indicate_shot(u_int8_t ui8x, u_int8_t ui8y)
{
    emit si_indicate_shot(ui8x,ui8y);
}

void battleship::sl_win_to_gui()
{
    QMessageBox::StandardButton msg;
    msg = QMessageBox::question(m_pbattlefield, "The game is over", "Congratulations!!! You are the winner", QMessageBox::Ok|QMessageBox::Cancel);
    emit si_close_app();
}
void battleship::sl_lost_to_gui()
{
    QMessageBox::StandardButton msg;
    msg = QMessageBox::question(m_pbattlefield, "The game is over", "You lost...", QMessageBox::Ok|QMessageBox::Cancel);
    emit si_close_app();
}

void battleship::sl_neg_param_en(u_int8_t x,u_int8_t y,u_int8_t bs,u_int8_t cru,u_int8_t dest,u_int8_t sub)
{
    qDebug() << "sl_a_neg_param_en !!!!";
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(m_pbattlefield,"You have been connected", " The field 10x10: 1 Battleship, 2 Cruiser, 3 Destroyer and 4 Submarines ", QMessageBox::Ok|QMessageBox::Cancel );
    if (reply == QMessageBox::Ok)
    {
        qDebug() << "Ok to parameters";
        emit si_a_neg_param_me(0x00);
    }
    else
    {
        emit si_a_neg_param_me(0x02);
    }
}

void battleship::sl_enemy_tile_hit(int iX, int iY, Logic::FieldTileState eState)
{
   emit si_enemy_tile_hit(iX, iY, eState);
}

void battleship::sl_client_accepted()
{
    qDebug() << "sl_client_accepted !!!!";
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(m_pbattlefield,"You have been connected", " The field 10x10: 1 Battleship, 2 Cruiser, 3 Destroyer and 4 Submarines ", QMessageBox::Ok|QMessageBox::Cancel );
    if (reply == QMessageBox::Ok)
    {
        emit si_neg_param_gui(10,10,1,2,3,4);
        qDebug() << "OK in box";
    }
}

// by Phil und Richard
void battleship::sl_place_ships()
{
    m_pbattlefield->setEnabled(true);
}

void battleship::sl_req_start_en()
{
    qDebug() << "msg box";
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(m_pbattlefield,"Your enemy is ready", "Would you like to start the game?", QMessageBox::Yes|QMessageBox::No );
    if (reply == QMessageBox::Yes)
    {
        emit si_a_req_start_gui(0x00);
        emit si_disable_start_button();
    }

    else
        emit si_a_req_start_gui(0x01);
}

void battleship::sl_request_start_gui()
{
    emit si_request_start_gui();
}

void battleship::sl_close_app()
{
    emit si_close_app();
}

void battleship::sl_shot_gui(u_int8_t uiX, u_int8_t uiY)
{
    qDebug() << "sl_shot_gui im battleship " << uiX << " " << uiY;
    emit si_shot_gui(static_cast<int>(uiX), static_cast<int>(uiY));
}

void battleship::sl_neg_param_gui(u_int8_t iX, u_int8_t iY, u_int8_t iCountBattleship, u_int8_t iCountCruiser, u_int8_t iCountDestroyer, u_int8_t iCountSubmarine)
{
    emit si_neg_param_gui(iX, iY, iCountBattleship, iCountCruiser, iCountDestroyer, iCountSubmarine);
}


void battleship::sl_start_client()
{

}

void battleship::sl_togui_history(QString qsHistory)
{
    m_qtbHistory->append(qsHistory);
}

void battleship::sl_disconnect()
{
    emit si_disconnect();
}

void battleship::sl_toclient_ip_port(QString ip, QString port)
{

    emit si_toclient_ip_port(ip,port);
}

void battleship::sl_request_ip_port()
{


    // fuer Netzwerkklasse
    emit si_request_ip_port();

}

void battleship::sl_ships_ready_togui(int x,int y,Logic::ShipSize sSize,int id)
{
    emit si_ships_ready_togui(x,y,sSize,id);
}

void battleship::sl_togui_ip_port(QString ip, QString port)
{
    qDebug() << ip << ":"<< port;
    m_pgs->setIpPort(ip,port);
}

void battleship::sl_server_settings()
{

    int height = 0;
    int width = 0;
    m_pgs->setAttribute(Qt::WA_DeleteOnClose);
    m_pbattlefield->setAttribute(Qt::WA_DeleteOnClose);


    //Placement of TextBrowser
    m_qtbHistory = new QTextBrowser;
    m_qtbHistory->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    m_qtbHistory->show();
    m_qtbHistory->move(m_pbattlefield->x() + m_pbattlefield->width() - m_qtbHistory->width(),m_pbattlefield->y());



    QRect rec = QApplication::desktop()->screenGeometry();
    height = rec.height();
    width = rec.width();
   // m_pgs->move(static_cast<int>((width/2)-(m_pbattlefield->frameGeometry().width() - m_pbattlefield->width()/2)-(m_pgs->width())),
             //   static_cast<int>((height/2)-(m_pbattlefield->height()/2)-(m_pbattlefield->frameGeometry().height()-m_pbattlefield->height())));
    m_pgs->move(m_pbattlefield->x() - m_pgs->frameGeometry().width() - (m_pbattlefield->frameGeometry().width() - m_pbattlefield->geometry().width()),
                m_pbattlefield->y());
    qDebug() << (m_pbattlefield->frameGeometry().width() - m_pbattlefield->width());

    m_pgs->setBfPointer(m_pbattlefield);
     m_pbattlefield->setEnabled(false);
    m_pbattlefield->show();
    m_pgs->show();
    emit si_start_server();
    this->hide();
}

void battleship::sl_client_settings()
{

    int height = 0, width = 0;
    m_pgc->setAttribute(Qt::WA_DeleteOnClose);
    m_pbattlefield->setAttribute(Qt::WA_DeleteOnClose);

    //Placement of TextBrowser
    m_qtbHistory = new QTextBrowser;
    m_qtbHistory->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    m_qtbHistory->show();
    m_qtbHistory->move(m_pbattlefield->x() + m_pbattlefield->width() - m_qtbHistory->width(),m_pbattlefield->y());

    QRect rec = QApplication::desktop()->screenGeometry();
    height = rec.height();
    width = rec.width();

    m_pgc->move(m_pbattlefield->x() - m_pgc->frameGeometry().width() - (m_pbattlefield->frameGeometry().width() - m_pbattlefield->geometry().width()),
                m_pbattlefield->y());
    m_pbattlefield->setEnabled(false);
    m_pbattlefield->show();
    m_pgc->show();
    emit si_start_client();


    this->hide();


}


