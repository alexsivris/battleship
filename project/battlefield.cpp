#include "battlefield.h"


using namespace Gui;

// *** BEGIN BATTLEFIELD *** //

Battlefield::Battlefield(QWidget *parent) :
    QWidget(parent),
    m_bfUi(new Ui::Battlefield)
{
    m_bfUi->setupUi(this);
    quint8 ui8MapperCounter=0;
    quint8 ui8EnemyCounter =0;

    // to map multiple buttons to one signal si_fb_clicked() which triggers the slot sl_fb_clicked ()
    QSignalMapper *sMapper = new QSignalMapper(this);
    QSignalMapper *enemyMapper = new QSignalMapper(this);

    // set the battlefield-window as the main window
    this->setWindowFlags(Qt::Window);


    // initialization of the pointers which hold the past state of the field tiles
    m_pLs = new LastState;
    m_pBeginState = new LastState;

    // the player shall start by arbitrarily painting a "Batlleship"-type ship
    m_bfUi->bfRadioBattleship->setChecked(true);
    m_bfUi->bfRadioCruiser->setChecked(false);
    m_bfUi->bfRadioDestroyer->setChecked(false);
    m_bfUi->bfRadioSubmarine->setChecked(false);




    // for-itartion to paint own Field
    for (int y=0; y<11; y++)
    {
        for (int x=0; x<11; x++)
        {
            if (y==0 || x ==0)
            {
                // coordinate system in x...
                if (x)
                {
                    QPushButton *qAxis = new QPushButton(QString::number(x-1));
                    qAxis->setStyleSheet("background-color: rgba(255,0,0,0.5);  border: 0px; height: 50%; width: 50%;");
                    qAxis->setEnabled(false);
                    m_bfUi->bfGridOwnShips->addWidget(qAxis,x,y);
                }

                // ...and y direction
                if (y)
                {
                    QPushButton *qAxis = new QPushButton(QString::number(y-1));
                    qAxis->setStyleSheet("background-color: rgba(255,0,0,0.5);  border: 0px; height: 50%; width: 50%;");
                    qAxis->setEnabled(false);
                    m_bfUi->bfGridOwnShips->addWidget(qAxis,x,y);
                }
            }
            else
            {
                // actual field tiles

                QString qsBtnName("bfOwnTiles");

                qsBtnName.append(QString::number(ui8MapperCounter));

                Fieldbutton *btnFieldTile = new Fieldbutton(ui8MapperCounter,y,x,qsBtnName);

                connect(btnFieldTile, SIGNAL(clicked()), sMapper, SLOT(map()));

                //creates a mapping from total button counter ui8MapperCounter
                sMapper->setMapping(btnFieldTile, btnFieldTile);

                //Style button
                btnFieldTile->setStyleSheet("height: 50%; width: 50%;");

                m_bfUi->bfGridOwnShips->addWidget(btnFieldTile,x, y);
                ui8MapperCounter++;
            }
        }
    }

    m_qi8TotalTiles = ui8MapperCounter;

    // actual connection between the created field tiles (buttons) and the slot via QSignalMapper
    connect(sMapper, SIGNAL(mapped(QWidget *)), this, SIGNAL(si_fb_clicked(QWidget *)));
    connect(this, SIGNAL(si_fb_clicked(QWidget *)), this, SLOT(sl_fb_clicked(QWidget *)));


    // for-iteration to pain enemy ships - same principle as own ships
    for (int y=0; y<11; y++)
    {
        for (int x=0; x<11; x++)
        {
            if (y==0 || x ==0)
            {
                if (x)
                {
                    QPushButton *qAxis = new QPushButton(QString::number(x-1));
                    qAxis->setStyleSheet("background-color: rgba(255,0,0,0.5);  border: 0px; height: 50%; width: 50%;");
                    qAxis->setEnabled(false);
                    m_bfUi->bfGridEnemyShips->addWidget(qAxis,x,y);
                }
                if (y)
                {
                    QPushButton *qAxis = new QPushButton(QString::number(y-1));
                    qAxis->setStyleSheet("background-color: rgba(255,0,0,0.5);  border: 0px; height: 50%; width: 50%;");
                    qAxis->setEnabled(false);
                    m_bfUi->bfGridEnemyShips->addWidget(qAxis,x,y);
                }
            }
            else
            {

                QString qsBtnName("bfEnemyTiles");

                qsBtnName.append(QString::number(ui8EnemyCounter));

                Fieldbutton *btnFieldTile = new Fieldbutton(ui8EnemyCounter,y,x,qsBtnName);

                connect(btnFieldTile, SIGNAL(clicked()), enemyMapper, SLOT(map()));

                enemyMapper->setMapping(btnFieldTile, btnFieldTile); //creates a mapping from total button counter ui8MapperCounter

                //Style button
                btnFieldTile->setStyleSheet("height: 50%; width: 50%;");
                btnFieldTile->setCursor(Qt::CrossCursor);

                m_bfUi->bfGridEnemyShips->addWidget(btnFieldTile,x, y);
                ui8EnemyCounter++;
            }
        }
    }

    // whenever an enemy tile is clicked, the sl_enemy_clicked() will be triggered
    connect(enemyMapper, SIGNAL(mapped(QWidget *)), this, SIGNAL(si_enemy_clicked(QWidget *)));
    connect(this, SIGNAL(si_enemy_clicked(QWidget *)), this, SLOT(sl_enemy_clicked(QWidget *)));

    // other buttons
    connect(m_bfUi->bfBtnNewGame, SIGNAL(clicked()), this, SLOT(sl_new_game()));
    connect(m_bfUi->bfBtnUndo, SIGNAL(clicked()), this, SLOT(sl_undo_step()));
    connect(m_bfUi->bfBtnStart, SIGNAL(clicked()), this, SLOT(sl_request_start_gui()));
    connect(m_bfUi->bfBtnCloseMain, SIGNAL(clicked()), this, SLOT(sl_close_app()));

    // main window alignment
    this->move(QApplication::desktop()->availableGeometry().center() - this->rect().center());  //Battlefield in center

    // signal for the "Logic" to negotiate parameters
    emit si_neg_param_gui(10, 10, 1, 2, 3, 4);

}


void Battlefield::sl_disable_start_button()
{
    m_bfUi->bfBtnStart->setEnabled(false);
}

void Battlefield::sl_enable_start_button()
{
    m_bfUi->bfBtnStart->setEnabled(true);
}

void Battlefield::sl_indicate_shot(u_int8_t ui8x, u_int8_t ui8y)
{
    for (int i=0; i<100; i++)
    {
        Fieldbutton *tempBtn;
        QString name = "bfOwnTiles";
        quint8 tmpx,tmpy;
        name.append(QString::number(i));
        tempBtn = QObject::findChild<Fieldbutton *>(name);
        tempBtn->getCoordinates(tmpx,tmpy);
        tmpx -= 1;
        tmpy -= 1;
        if ((tmpx == ui8x) && (tmpy == ui8y))
        {
            tempBtn->setEnabled(false);
            tempBtn->setStyleSheet(" background-color: #000000; width: 50%; height: 50%; border-radius: 5px ");
        }
    }
}

void Battlefield::sl_enemy_tile_hit(int iX, int iY, Logic::FieldTileState eState)
{
    qDebug() << "sl_enemy_tile_hit(): " << iX << "  " << iY;
    for (int i=0; i<100; i++)
    {
        Fieldbutton *tempBtn;
        QString name = "bfEnemyTiles";
        quint8 tmpx,tmpy;
        name.append(QString::number(i));
        tempBtn = QObject::findChild<Fieldbutton *>(name);
        tempBtn->getCoordinates(tmpx,tmpy);
        tmpx -= 1;
        tmpy -= 1;
        if ((tmpx == iX) && (tmpy == iY))
        {
            // WATER ,WATER_AND_HIT, SHIP_PART, SHIP_PART_HIT, SHIP_PART_DEST
            if (eState == Logic::WATER_AND_HIT)
            {
                tempBtn->setEnabled(false);
                tempBtn->setStyleSheet(" background-color: #0000ff; width: 50%; height: 50%; border-radius: 5px ");
            }
            if (eState == Logic::SHIP_PART_DEST)
            {
                qDebug() << " Schiff zerstoert: " << iX << "    " << iY ;
                tempBtn->setEnabled(false);
                tempBtn->setStyleSheet(" background-color: #ff0000; width: 50%; height: 50%; border-radius: 5px ");
            }
            if (eState == Logic::SHIP_PART_HIT)
            {
                tempBtn->setEnabled(false);
                tempBtn->setStyleSheet(" background-color: #00ff00; width: 50%; height: 50%; border-radius: 5px ");
            }
        }
    }


}

void Battlefield::sl_request_start_gui()
{
    emit si_request_start_gui();
}

void Battlefield::sl_close_app()
{
    emit si_close_app();
}

Battlefield::~Battlefield()
{

    delete m_pLs;
    delete m_bfUi;
    qDebug() << "battlefield deleted";


}
void Battlefield::sl_new_game()
{
    // reset the state and appearance of all buttons
    for (quint8 i=0;i<m_qi8TotalTiles;i++)
    {
        QString qsBtnName = "bfOwnTiles";
        qsBtnName.append(QString::number(i));
        Fieldbutton * pFbtn = QObject::findChild<Fieldbutton*>(qsBtnName);
        m_bfUi->bfRadioBattleship->setChecked(true);

        m_bfUi->bfRadioBattleship->setEnabled(true);
        m_bfUi->bfRadioCruiser->setEnabled(false);
        m_bfUi->bfRadioDestroyer->setEnabled(false);
        m_bfUi->bfRadioSubmarine->setEnabled(false);

        m_qi8TileCounter = 0;  // TODO echte Werte erfragen
        direction = NEUTRAL;
        m_qi8CounterBattleship = 1;
        m_qi8CounterCruiser = 2;
        m_qi8CounterDestroyer = 3;
        m_qi8CounterSubmarine = 4;
        pFbtn->setEnabled(true);
        pFbtn->setText("");
        pFbtn->setStyleSheet("width: 50%; height: 50% ");
    }
}

void Battlefield::sl_undo_step()
{

        while (m_qi8TileCounter)
        {
            Fieldbutton *tmpBtn;
            tmpBtn = QObject::findChild<Fieldbutton *>(m_qsList.takeLast());
            tmpBtn->setStyleSheet(" width: 50%; height: 50% ");
            tmpBtn->setText("");
            tmpBtn->setEnabled(true);
            tmpBtn->setTileType(Logic::NO_SHIP);
            direction = NEUTRAL;
            m_qi8TileCounter--;
            qDebug() << m_qi8TileCounter;
        }


}

void Battlefield::sl_enemy_clicked(QWidget * qwEb)
{
    Fieldbutton * efb = static_cast<Fieldbutton *>(qwEb);
    quint8 iX = 0, iY = 0;
    u_int8_t x, y;
    efb->getCoordinates(iX, iY);
    x = iX-1;
    y = iY-1;
    qDebug() << "Enemy coordinates ( " << x << ", " << y << " )";
    emit si_shot_gui(x,y);
}

void Battlefield::sl_draw_battleship()
{
    m_bfUi->bfRadioBattleship->setChecked(true);

}

void Battlefield::sl_fb_clicked(QWidget * qwFb)
{
    Fieldbutton * fb = static_cast<Fieldbutton *>(qwFb);
    quint8 x,y,distx,disty, begindistx, begindisty;

    fb->getCoordinates(x, y);

    QString qsLastObject = m_pLs->getObjectName();

    m_pLs->getDistance(x,y,&distx, &disty);
    m_pBeginState->getDistance(x,y,&begindistx, &begindisty);

    QString qsDistance = QString::number(distx);
    qsDistance.append(", ");
    qsDistance.append(QString::number(disty));

    //
    if (m_bfUi->bfRadioSubmarine->isChecked())
    {
        m_bfUi->bfRadioCruiser->setEnabled(false);
        m_bfUi->bfRadioDestroyer->setEnabled(false);
        m_bfUi->bfRadioBattleship->setEnabled(false);

        //in private Funktion packen
        if ((m_qi8TileCounter < 2)&& (fb->isEnabled()) && (m_qi8CounterSubmarine))
        {
            if (!m_qi8TileCounter)
            {
                m_pLs->setLastState(fb->objectName(),x, y);
                m_pBeginState->setLastState(fb->objectName(),x, y);
                fb->setStyleSheet("background-color: #41BCF0; border-radius: 5px; height: 50%; width: 50%");
                fb->setText(QString::number(4-m_qi8CounterSubmarine));
                fb->setTileType(Logic::SUBMARINE);
                fb->setShipId(4-m_qi8CounterSubmarine);
                fb->setEnabled(false);
                set_push_tile_list(fb->objectName());
                m_qi8TileCounter++;
            }
            else
            {
                if ((x == m_pBeginState->m_ui8PositionX) && (direction == NEUTRAL))
                    direction = VERTICAL;
                if ((y == m_pBeginState->m_ui8PositionY) && (direction == NEUTRAL))
                    direction = HORIZONTAL;
                if ((x == m_pLs->m_ui8PositionX) && (direction == VERTICAL))
                {
                    if (disty == 1)
                    {

                        fb->setStyleSheet("background-color: #41BCF0; border-radius: 5px; height: 50%; width: 50%");
                        fb->setText(QString::number(4-m_qi8CounterSubmarine));
                        m_pLs->setLastState(fb->objectName(),x,y);
                        fb->setTileType(Logic::SUBMARINE);
                        fb->setShipId(4-m_qi8CounterSubmarine);
                        set_push_tile_list(fb->objectName());
                        m_qi8TileCounter++;
                        fb->setEnabled(false);
                    }
                    if ((begindisty == 1) && (m_qi8TileCounter > 0) && (disty > 1))
                    {

                        fb->setStyleSheet("background-color: #41BCF0; border-radius: 5px; height: 50%; width: 50%");
                        fb->setText(QString::number(4-m_qi8CounterSubmarine));
                        m_pBeginState->setLastState(m_pLs->getObjectName(), m_pLs->m_ui8PositionX, m_pLs->m_ui8PositionY);
                        m_pLs->setLastState(fb->objectName(),x,y);
                        fb->setTileType(Logic::SUBMARINE);
                        fb->setShipId(4-m_qi8CounterSubmarine);
                        set_push_tile_list(fb->objectName());
                        m_qi8TileCounter++;
                        fb->setEnabled(false);
                    }
                }
                if ((y == m_pLs->m_ui8PositionY) && (direction == HORIZONTAL))
                {

                    if (distx == 1)
                    {

                        fb->setStyleSheet("background-color: #41BCF0; border-radius: 5px; height: 50%; width: 50%");
                        fb->setText(QString::number(4-m_qi8CounterSubmarine));
                        m_pLs->setLastState(fb->objectName(),x,y);
                        fb->setTileType(Logic::SUBMARINE);
                        fb->setShipId(4-m_qi8CounterSubmarine);
                        set_push_tile_list(fb->objectName());
                        m_qi8TileCounter++;
                        fb->setEnabled(false);
                    }
                    if ((begindistx == 1) && (m_qi8TileCounter > 0) && (distx > 1))
                    {

                        fb->setStyleSheet("background-color: #41BCF0; border-radius: 5px; height: 50%; width: 50%");
                        fb->setText(QString::number(4-m_qi8CounterSubmarine));
                        m_pBeginState->setLastState(m_pLs->getObjectName(), m_pLs->m_ui8PositionX, m_pLs->m_ui8PositionY);
                        m_pLs->setLastState(fb->objectName(),x,y);
                        fb->setTileType(Logic::SUBMARINE);
                        fb->setShipId(4-m_qi8CounterSubmarine);
                        set_push_tile_list(fb->objectName());
                        m_qi8TileCounter++;
                        fb->setEnabled(false);
                    }
                }

            }

        }
        if (m_qi8TileCounter == 2)
        {

            qDebug() << "counter is 2 !!! ";
            m_qi8CounterSubmarine--;
            if (!m_qi8CounterSubmarine)
            {
                m_bfUi->bfRadioCruiser->setEnabled(false);
                m_bfUi->bfRadioDestroyer->setEnabled(false);
                m_bfUi->bfRadioSubmarine->setEnabled(false);
                m_bfUi->bfRadioBattleship->setEnabled(false);

                m_bfUi->bfRadioDestroyer->setChecked(false);
                m_qi8TileCounter = 0;
                direction = NEUTRAL;

                for (quint8 i = 0; i < 100; i++ )  //TODO erfrage wirkliche Spielfeldgroesse
                {
                    Fieldbutton *tempBtn;
                    QString name = "bfOwnTiles";
                    quint8 tmpx,tmpy;
                    name.append(QString::number(i));
                    tempBtn = QObject::findChild<Fieldbutton *>(name);
                    tempBtn->getCoordinates(tmpx,tmpy);

                    // Nach ~8. Iteration -> crash
                    qDebug() << "Alex Gui -------------->>>>>>>    Koordinaten ( " << tmpx << ", " << tmpy << ") Typ: " << tempBtn->getTileType();
                   emit si_ships_ready_togui(tmpx-1,tmpy-1,tempBtn->getTileType(),tempBtn->getShipId());

                }
                // by Richy und Phil
                m_bfUi->bfBtnStart->setEnabled(true);


            }
            else
            {
                m_qi8TileCounter = 0;

                direction = NEUTRAL;
            }
        }

    }
    if (m_bfUi->bfRadioDestroyer->isChecked())
    {
        m_bfUi->bfRadioCruiser->setEnabled(false);
        m_bfUi->bfRadioBattleship->setEnabled(false);
        m_bfUi->bfRadioSubmarine->setEnabled(false);

        //in private Funktion packen
        if ( (m_qi8TileCounter < 3)&& (fb->isEnabled()))
        {
            if (!m_qi8TileCounter)
            {
                m_pLs->setLastState(fb->objectName(),x, y);
                m_pBeginState->setLastState(fb->objectName(),x, y);
                fb->setStyleSheet("background-color: #41F0A1; border-radius: 5px; height: 50%; width: 50%");
                fb->setText(QString::number(3-m_qi8CounterDestroyer));
                fb->setTileType(Logic::DESTROYER);
                fb->setShipId(3-m_qi8CounterDestroyer);
                set_push_tile_list(fb->objectName());
                m_qi8TileCounter++;
                fb->setEnabled(false);

            }
            else
            {
                if ((x == m_pBeginState->m_ui8PositionX) && (direction == NEUTRAL))
                    direction = VERTICAL;
                if ((y == m_pBeginState->m_ui8PositionY) && (direction == NEUTRAL))
                    direction = HORIZONTAL;
                if ((x == m_pLs->m_ui8PositionX) && (direction == VERTICAL))
                {
                    if (disty == 1)
                    {

                        fb->setStyleSheet("background-color: #41F0A1; border-radius: 5px; height: 50%; width: 50%");
                        fb->setText(QString::number(3-m_qi8CounterDestroyer));
                        m_pLs->setLastState(fb->objectName(),x,y);
                        fb->setTileType(Logic::DESTROYER);
                        fb->setShipId(3-m_qi8CounterDestroyer);
                        set_push_tile_list(fb->objectName());
                        m_qi8TileCounter++;
                        fb->setEnabled(false);
                    }
                    if ((begindisty == 1) && (m_qi8TileCounter > 0) && (disty > 1))
                    {

                        fb->setStyleSheet("background-color: #41F0A1; border-radius: 5px; height: 50%; width: 50%");
                        fb->setText(QString::number(3-m_qi8CounterDestroyer));
                        m_pBeginState->setLastState(m_pLs->getObjectName(), m_pLs->m_ui8PositionX, m_pLs->m_ui8PositionY);
                        m_pLs->setLastState(fb->objectName(),x,y);
                        fb->setTileType(Logic::DESTROYER);
                        fb->setShipId(3-m_qi8CounterDestroyer);
                        set_push_tile_list(fb->objectName());
                        m_qi8TileCounter++;
                        fb->setEnabled(false);
                    }
                }
                if ((y == m_pLs->m_ui8PositionY) && (direction == HORIZONTAL))
                {

                    if (distx == 1)
                    {

                        fb->setStyleSheet("background-color: #41F0A1; border-radius: 5px; height: 50%; width: 50%");
                        fb->setText(QString::number(3-m_qi8CounterDestroyer));
                        m_pLs->setLastState(fb->objectName(),x,y);
                        set_push_tile_list(fb->objectName());
                        fb->setShipId(3-m_qi8CounterDestroyer);
                        m_qi8TileCounter++;
                        fb->setEnabled(false);
                    }
                    if ((begindistx == 1) && (m_qi8TileCounter > 0) && (distx > 1))
                    {

                        fb->setStyleSheet("background-color: #41F0A1; border-radius: 5px; height: 50%; width: 50%");
                        fb->setText(QString::number(3-m_qi8CounterDestroyer));
                        m_pBeginState->setLastState(m_pLs->getObjectName(), m_pLs->m_ui8PositionX, m_pLs->m_ui8PositionY);
                        m_pLs->setLastState(fb->objectName(),x,y);
                        fb->setTileType(Logic::DESTROYER);
                        fb->setShipId(3-m_qi8CounterDestroyer);
                        set_push_tile_list(fb->objectName());
                        m_qi8TileCounter++;
                        fb->setEnabled(false);
                    }
                }

            }

        }
        if (m_qi8TileCounter == 3)
        {
            qDebug() << "counter is 3!!! ";
            m_qi8CounterDestroyer--;
            if (!m_qi8CounterDestroyer)
            {
                m_bfUi->bfRadioCruiser->setEnabled(false);
                m_bfUi->bfRadioDestroyer->setEnabled(false);
                m_bfUi->bfRadioSubmarine->setEnabled(true);
                m_bfUi->bfRadioBattleship->setEnabled(false);

                m_bfUi->bfRadioSubmarine->setChecked(true);
                m_qi8TileCounter = 0;
                direction = NEUTRAL;
            }
            else
            {
                m_qi8TileCounter = 0;
                direction = NEUTRAL;
            }
        }

    }
    if (m_bfUi->bfRadioCruiser->isChecked())
    {
        m_bfUi->bfRadioBattleship->setEnabled(false);
        m_bfUi->bfRadioDestroyer->setEnabled(false);
        m_bfUi->bfRadioSubmarine->setEnabled(false);

        //in private Funktion packen
        if ((m_qi8TileCounter < 4) && (fb->isEnabled()))
        {
            if (!m_qi8TileCounter)
            {
                m_pLs->setLastState(fb->objectName(),x, y);
                m_pBeginState->setLastState(fb->objectName(),x, y);
                fb->setStyleSheet("background-color: #F0418D; border-radius: 5px; height: 50%; width: 50%");
                fb->setText(QString::number(2-m_qi8CounterCruiser));
                fb->setTileType(Logic::CRUISER);
                fb->setShipId(2-m_qi8CounterCruiser);
                set_push_tile_list(fb->objectName());
                m_qi8TileCounter++;
                fb->setEnabled(false);
            }
            else
            {
                if ((x == m_pBeginState->m_ui8PositionX) && (direction == NEUTRAL))
                    direction = VERTICAL;
                if ((y == m_pBeginState->m_ui8PositionY) && (direction == NEUTRAL))
                    direction = HORIZONTAL;

                if ((x == m_pLs->m_ui8PositionX) && (direction == VERTICAL))
                {
                    if (disty == 1)
                    {

                        fb->setStyleSheet("background-color: #F0418D; border-radius: 5px; height: 50%; width: 50%");
                        fb->setText(QString::number(2-m_qi8CounterCruiser));
                        m_pLs->setLastState(fb->objectName(),x,y);
                        fb->setTileType(Logic::CRUISER);

                        fb->setShipId(2-m_qi8CounterCruiser);
                        set_push_tile_list(fb->objectName());
                        m_qi8TileCounter++;
                        fb->setEnabled(false);
                    }
                    if ((begindisty == 1) && (m_qi8TileCounter > 0) && (disty > 1))
                    {

                        fb->setStyleSheet("background-color: #F0418D; border-radius: 5px; height: 50%; width: 50%");
                        fb->setText(QString::number(2-m_qi8CounterCruiser));
                        m_pBeginState->setLastState(m_pLs->getObjectName(), m_pLs->m_ui8PositionX, m_pLs->m_ui8PositionY);
                        m_pLs->setLastState(fb->objectName(),x,y);
                        fb->setTileType(Logic::CRUISER);

                        fb->setShipId(2-m_qi8CounterCruiser);
                        set_push_tile_list(fb->objectName());
                        m_qi8TileCounter++;
                        fb->setEnabled(false);
                    }
                }
                if ((y == m_pLs->m_ui8PositionY) && (direction == HORIZONTAL))
                {

                    if (distx == 1)
                    {

                        fb->setStyleSheet("background-color: #F0418D; border-radius: 5px; height: 50%; width: 50%");
                        fb->setText(QString::number(2-m_qi8CounterCruiser));
                        m_pLs->setLastState(fb->objectName(),x,y);
                        fb->setTileType(Logic::CRUISER);
                        set_push_tile_list(fb->objectName());

                        fb->setShipId(2-m_qi8CounterCruiser);
                        m_qi8TileCounter++;
                        fb->setEnabled(false);
                    }
                    if ((begindistx == 1) && (m_qi8TileCounter > 0) && (distx > 1))
                    {

                        fb->setStyleSheet("background-color: #F0418D; border-radius: 5px; height: 50%; width: 50%");
                        fb->setText(QString::number(2-m_qi8CounterCruiser));
                        m_pBeginState->setLastState(m_pLs->getObjectName(), m_pLs->m_ui8PositionX, m_pLs->m_ui8PositionY);
                        m_pLs->setLastState(fb->objectName(),x,y);
                        fb->setTileType(Logic::CRUISER);

                        fb->setShipId(2-m_qi8CounterCruiser);
                        set_push_tile_list(fb->objectName());
                        m_qi8TileCounter++;
                        fb->setEnabled(false);
                    }
                }

            }

        }
        if (m_qi8TileCounter == 4)
        {
            qDebug() << "counter is 4 !!! ";
            m_qi8CounterCruiser--;
            if (!m_qi8CounterCruiser)
            {
                m_bfUi->bfRadioCruiser->setEnabled(false);
                m_bfUi->bfRadioDestroyer->setEnabled(true);
                m_bfUi->bfRadioSubmarine->setEnabled(false);
                m_bfUi->bfRadioBattleship->setEnabled(false);

                m_bfUi->bfRadioDestroyer->setChecked(true);
                m_qi8TileCounter = 0;
                direction = NEUTRAL;
            }
            else
            {
                m_qi8TileCounter = 0;
                direction = NEUTRAL;
            }
        }

    }

    if (m_bfUi->bfRadioBattleship->isChecked())
    {
        m_bfUi->bfRadioCruiser->setEnabled(false);
        m_bfUi->bfRadioDestroyer->setEnabled(false);
        m_bfUi->bfRadioSubmarine->setEnabled(false);

        if ((m_qi8TileCounter < 5) && (fb->isEnabled()))
        {
            if (!m_qi8TileCounter)
            {
                m_pLs->setLastState(fb->objectName(),x, y);
                m_pBeginState->setLastState(fb->objectName(),x, y);
                fb->setStyleSheet("background-color: #ff00cf; border-radius: 5px; height: 50%; width: 50%");
                fb->setText(QString::number(1-m_qi8CounterBattleship));
                fb->setTileType(Logic::BATTLESHIP);

                fb->setShipId(1-m_qi8CounterBattleship);
                set_push_tile_list(fb->objectName());
                m_qi8TileCounter++;
                fb->setEnabled(false);
            }
            else
            {
                if ((x == m_pBeginState->m_ui8PositionX) && (direction == NEUTRAL))
                    direction = VERTICAL;
                if ((y == m_pBeginState->m_ui8PositionY) && (direction == NEUTRAL))
                    direction = HORIZONTAL;


                if ((x == m_pLs->m_ui8PositionX) && (direction == VERTICAL))
                {
                    if (disty == 1)
                    {

                        fb->setStyleSheet("background-color: #ff00cf; border-radius: 5px; height: 50%; width: 50%");
                        fb->setText(QString::number(1-m_qi8CounterBattleship));
                        m_pLs->setLastState(fb->objectName(),x,y);
                        fb->setTileType(Logic::BATTLESHIP);
                        set_push_tile_list(fb->objectName());
                        fb->setShipId(1-m_qi8CounterBattleship);
                        m_qi8TileCounter++;
                        fb->setEnabled(false);
                    }
                    if ((begindisty == 1) && (m_qi8TileCounter > 0) && (disty > 1))
                    {

                        fb->setStyleSheet("background-color: #ff00cf; border-radius: 5px; height: 50%; width: 50%");
                        fb->setText(QString::number(1-m_qi8CounterBattleship));
                        m_pBeginState->setLastState(m_pLs->getObjectName(), m_pLs->m_ui8PositionX, m_pLs->m_ui8PositionY);
                        m_pLs->setLastState(fb->objectName(),x,y);
                        fb->setShipId(1-m_qi8CounterBattleship);
                        fb->setTileType(Logic::BATTLESHIP);
                        set_push_tile_list(fb->objectName());
                        m_qi8TileCounter++;
                        fb->setEnabled(false);
                    }
                }
                if ((y == m_pLs->m_ui8PositionY) && (direction == HORIZONTAL))
                {

                    if (distx == 1)
                    {

                        fb->setStyleSheet("background-color: #ff00cf; border-radius: 5px; height: 50%; width: 50%");
                        fb->setText(QString::number(1-m_qi8CounterBattleship));
                        m_pLs->setLastState(fb->objectName(),x,y);
                        fb->setShipId(1-m_qi8CounterBattleship);
                        fb->setTileType(Logic::BATTLESHIP);
                        set_push_tile_list(fb->objectName());
                        m_qi8TileCounter++;
                        fb->setEnabled(false);
                    }
                    if ((begindistx == 1) && (m_qi8TileCounter > 0) && (distx > 1))
                    {

                        fb->setStyleSheet("background-color: #ff00cf; border-radius: 5px; height: 50%; width: 50%");
                        fb->setText(QString::number(1-m_qi8CounterBattleship));
                        m_pBeginState->setLastState(m_pLs->getObjectName(), m_pLs->m_ui8PositionX, m_pLs->m_ui8PositionY);
                        m_pLs->setLastState(fb->objectName(),x,y);
                        fb->setShipId(1-m_qi8CounterBattleship);
                        fb->setTileType(Logic::BATTLESHIP);
                        set_push_tile_list(fb->objectName());
                        m_qi8TileCounter++;
                        fb->setEnabled(false);
                    }
                }

            }

        }
        if (m_qi8TileCounter == 5)
        {
            qDebug() << "counter is 5 !!! ";
            m_qi8CounterBattleship--;
            if (!m_qi8CounterBattleship)
            {
                m_bfUi->bfRadioCruiser->setEnabled(true);
                m_bfUi->bfRadioDestroyer->setEnabled(false);
                m_bfUi->bfRadioSubmarine->setEnabled(false);
                m_bfUi->bfRadioBattleship->setEnabled(false);
            }


            m_bfUi->bfRadioCruiser->setChecked(true);
            m_qi8TileCounter = 0;
            direction = NEUTRAL;
        }

    }

    }





void Battlefield::set_push_tile_list(QString qsName)
{
    m_qsList << qsName;
}

QString Battlefield::get_pop_tile_list()
{
    return m_qsList.takeLast();
}

// *** EOF BATTLEFIELD *** //

// *** BEGIN FIELDBUTTON *** //

Fieldbutton::Fieldbutton(quint8 id, quint8 x, quint8 y, const QString qsName) :
    m_ui8Id(id), m_qsObjectName(qsName) , m_ui8PositionX(x), m_ui8PositionY(y)
{
    this->setText(m_qsCaption);
    this->setObjectName(m_qsObjectName);
}


Fieldbutton::~Fieldbutton()
{

}

int Fieldbutton::getShipId()
{
    return static_cast<int>(m_qi8ShipId);
}


void Fieldbutton::setShipId(quint8 sId)
{
    m_qi8ShipId = sId;
}

Logic::ShipSize Fieldbutton::getTileType()
{
    return m_sSize;
}

void Fieldbutton::setTileType(Logic::ShipSize sSize)
{
    m_sSize = sSize;
}

void Fieldbutton::getCoordinates(quint8 &paramX, quint8 &paramY) const
{
    paramX = m_ui8PositionX;
    paramY = m_ui8PositionY;
}

void Fieldbutton::setCoordinates(quint8 &paramX, quint8 &paramY)
{

}

// *** EOF FIELDBUTTON *** //

// *** BEGIN LASTSTATE *** //
LastState::LastState()
{

}

void LastState::setLastState(QString object, quint8 x, quint8 y)
{
    m_qsObject = object;
    m_ui8PositionX = x;
    m_ui8PositionY = y;
}

void LastState::getDistance(quint8 x,quint8 y, quint8 *xdest, quint8 *ydest) const
{
    *xdest = abs(abs(x)-abs(m_ui8PositionX));
    *ydest = abs(abs(y)-abs(m_ui8PositionY));
}


QString LastState::getObjectName() const
{
    return m_qsObject;
}

LastState::~LastState()
{
    //delete this;
}


// *** EOF LASTSTATE *** //
