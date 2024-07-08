#ifndef PIRATE_H
#define PIRATE_H

#include "Ship.h"

class Pirate : Comparable {
private:
    int m_id;
    int m_treasure;
    Ship* m_ship;

public:
    Pirate(int id, int treasure, Ship* ship) : m_id(id), m_treasure(treasure), m_ship(ship){}

    void updateTreasure(int change);
    int getId() const;
    int getTreasure() const;
    Ship* getShip() const;
    void replaceSheep(Ship* newShip);

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
