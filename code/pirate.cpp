#include "pirate.h"
#include "ship.h"

void Pirate::updateTreasure(int change) {
    m_treasure += change;
}

int Pirate::getId() const {
    return m_id;
}

int Pirate::getTreasure() const {
    return m_treasure;
}

Ship* Pirate::getShip() const {
    return m_ship;
}

void Pirate::replaceShip(Ship* newShip) {
    m_treasure += (m_ship != nullptr ? m_ship->getBalance() : 0);
    m_ship = newShip;
}

int Pirate::getShipId() const
{
    return m_ship->getId();
}
