#include "guiclient.h"

using namespace Gui;

GuiClient::GuiClient(QWidget *parent) :
    QWidget(parent),
    m_gcUi(new Ui::GuiClient)
{
    m_gcUi->setupUi(this);


    //klicke Button und rufe Slot auf der Signal emitiert
    connect(m_gcUi->clientBtnConnect, SIGNAL(clicked()), this, SLOT(sl_emit_ip_port()));
    //emitiertes Signal ruft slot im Client-Objekt auf

    connect(m_gcUi->clienBtnDisconnect, SIGNAL(clicked()), this, SLOT(sl_client_disconnected()));
    connect(m_gcUi->clientBtnClose, SIGNAL(clicked()), this, SLOT(sl_close_app()));

}

GuiClient::~GuiClient()
{
    delete m_gcUi;
    qDebug() << "client deleted";
}

void GuiClient::sl_close_app()
{
    emit si_close_app();
}

void GuiClient::sl_emit_ip_port()
{
    emit si_tonet_ip_port(m_gcUi->clientLeIp->text(), m_gcUi->clientLePort->text());

}

void GuiClient::sl_client_disconnected()
{
    emit si_client_disconnected();
}

