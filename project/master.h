#ifndef MASTER_H
#define MASTER_H

#include "battleship.h"
#include <QObject>
#include "netconnect.h"
#include "field.h"
#include <QDebug>

/**
 * @brief The Master class brings together all main classes and creates all objects
 */
class Master : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Master
     * @param parent
     */
    explicit Master(QObject *parent = 0);
private:
    /**
     * @brief m_pcoField - pointer to the Logic::Field
     */
    Logic::Field * m_pcoField;
    /**
     * @brief m_pcoNet - pointer to the Netzwerk::Netconnect
     */
    Netzwerk::Netconnect* m_pcoNet;
    /**
     * @brief m_pcoGui - pointer to the Gui::battleship
     */
    Gui::battleship *m_pcoGui;


signals:

public slots:
    /**
     * @brief sl_master_close - used to close the game
     */
    void sl_master_close();

};

#endif // MASTER_H
