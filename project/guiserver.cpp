#include "guiserver.h"


using namespace Gui;

GuiServer::GuiServer(QWidget *parent) :
    QWidget(parent),
    m_gsUi(new Ui::GuiServer)
{
    m_gsUi->setupUi(this);


    connect(m_gsUi->serverBtnStart, SIGNAL(clicked()), this, SLOT(sl_start_server()));
    connect(m_gsUi->serverBtnDc, SIGNAL(clicked()), this, SLOT(sl_server_disconnect()));
    connect(m_gsUi->serverBtnClose, SIGNAL(clicked()), this, SLOT(sl_close_app()));



}

GuiServer::~GuiServer()
{
    delete m_gsUi;
    qDebug() << "server deleted";
}

void GuiServer::sl_close_app()
{
    emit si_close_app();
}


void GuiServer::sl_server_disconnect()
{
    emit si_server_disconnected();
}

void GuiServer::setIpPort(QString ip,QString port)
{
    m_gsUi->serverLeIp->setText(ip);
    m_gsUi->serverLePort->setText(port);
}

void GuiServer::setBfPointer(Battlefield * ptr)
{
    m_pBf = ptr;
}

void GuiServer::sl_start_server()
{

        m_gsUi->serverLePort->setEnabled(false);
        m_gsUi->serverBtnStart->setEnabled(false);
        m_gsUi->serverBtnDc->setEnabled(true);
        m_pBf->setEnabled(false);

        emit si_request_ip_port();
}

void GuiServer::sl_insert_ip(QString qsIp)
{

    //m_gsUi->serverTbIp->append(qsIp);


}

void GuiServer::sl_insert_port(QString qsPort)
{

    m_gsUi->serverLePort->setText(qsPort);
}



void GuiServer::sl_insert_history(QString qsMessage)
{
    //m_gsUi->serverTbHistory->append(qsMessage);
    //m_gsUi->serverTbHistory->append("n");

}
