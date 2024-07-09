#ifndef PIRATE_H
#define PIRATE_H
#include "comparable.h"

class Ship;

class Pirate : public Comparable {
private:
    int m_id;
    long long int m_idOnShip;
    int m_treasure;
    Ship* m_ship;

public:
    Pirate(int id, int treasure) : m_id(id), m_treasure(treasure), m_ship(nullptr){}
    Pirate(int id) : m_id(id), m_idOnShip(-1), m_treasure(0), m_ship(nullptr) {} // dummi constructor
    // Pirate(const Pirate&) = delete;

    void updateTreasure(int change);
    int getId() const;
    int getTreasure() const;
    Ship* getShip() const;
    void replaceShip(Ship* newShip);

    bool operator<(const Comparable& other) const override {
        return m_id < ((Pirate&) other).getId();
    }

    bool operator==(const Comparable& other) const override {
        return m_id == ((Pirate&) other).getId();
    }

    long long int getIdOnShip() {
        return m_idOnShip;
    }
};

#endif // PIRATE_H
