#ifndef PIRATE_H
#define PIRATE_H
#include "comparable.h"

class Ship;

class Pirate : Comparable {
private:
    int m_id;
    int m_treasure;
    Ship* m_ship;

public:
    Pirate() : m_id(-1), m_treasure(0), m_ship(nullptr) {}
    Pirate(int id, int treasure, Ship* ship) : m_id(id), m_treasure(treasure), m_ship(ship){}
    Pirate(int id) : m_id(id), m_treasure(0), m_ship(nullptr){} // dummi constructor

    void updateTreasure(int change);
    int getId() const;
    int getTreasure() const;
    Ship* getShip() const;
    void replaceShip(Ship* newShip);

    bool operator<(const Comparable& other) const override {
        return m_id < ((Pirate&) other).getId();
    }

    bool operator>(const Comparable& other) const override {
        return m_id > ((Pirate&) other).getId();
    }

    bool operator==(const Comparable& other) const override {
        return m_id == ((Pirate&) other).getId();
    }
};

#endif // PIRATE_H
