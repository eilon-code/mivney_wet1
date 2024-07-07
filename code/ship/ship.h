#ifndef SHIP_H
#define SHIP_H

#include "Pirate.h"
#include "comparable.h"

class Ship : Comparable {
public:
    Ship(int shipId, int cannons) : m_id(shipId), m_cannons(cannons) {};
    ~Ship() = default;

    void movePirateInto(const Pirate& pirate){
        pirate.replaceSheep(*this);
        insertPirate(pirate);
    }

    void createPirateIn(int pirateId, int pirateTreasure){
        const Pirate& pirate = Pirate(pirateId, pirateTreasure, *this);
        insertPirate(pirate);
    }

    bool operator<(const Comparable& other) const override {
        return m_id < ((Ship&) other).getId();
    }

    bool operator>(const Comparable& other) const override {
        return m_id > ((Ship&) other).getId();
    }

    bool operator==(const Comparable& other) const override {
        return m_id == ((Ship&) other).getId();
    }

    int getId() const {
        return m_id;
    }

    int getCannons() const {
        return m_cannons;
    }

private:
    int m_id;
    int m_cannons;
    // AVL<Pirate> m_piratesOnShip
    // Pirate& m_veteranPirate
    // int m_coinOffset

    void insertPirate(const Pirate& pirate); // TODO: insert pirate to avl-tree or trees
};

#endif // SHIP_H
