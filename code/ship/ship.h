#ifndef SHIP_H
#define SHIP_H

#include "Pirate.h"
#include "comparable.h"
#include <avl_tree.h>

class Ship : Comparable {
public:
    Ship(int shipId, int cannons) : m_id(shipId), m_cannons(cannons), m_coinOffset(0), m_richestPirate(nullptr) {};
    // Ship(const Ship& other) {
    //     m_cannons = other.m_cannons;
    //     m_coinOffset = other.m_coinOffset;
    // }
    ~Ship() = default;

    StatusType removePirate(int pirateId);

    StatusType movePirateIn(Pirate& pirate){
        pirate.replaceShip(this);
        pirate.updateTreasure(0 - m_coinOffset);
        return insertPirate(pirate);
    }

    StatusType createPirateIn(int pirateId, int pirateTreasure){
        Pirate pirate = Pirate(pirateId, pirateTreasure, this);
        pirate.updateTreasure(0 - m_coinOffset);
        return insertPirate(pirate);
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

    int getPiratesOnShip() const {
        return m_piratesOnShip.size();
    }

    output_t<int> getRichestPirateId() const {
        if (getPiratesOnShip() == 0) {
            return StatusType::FAILURE;
        }
        return (*m_richestPirate).getId();
    }

    int evaluate() const {
        return std::min(m_cannons, getPiratesOnShip());
    }

    void shiftBalance(int change) {
        m_coinOffset += change;
    }

    int getBalance() const {
        return m_coinOffset;
    }

private:
    int m_id;
    int m_cannons;
    int m_coinOffset;
    AVLTree<Pirate> m_piratesOnShip; // ordered by time on ship
    AVLTree<Pirate> m_piratesOnShipOrderedByRichness;
    Pirate* m_richestPirate;

    StatusType insertPirate(const Pirate& pirate); // TODO: insert pirate to avl-tree or trees
};

#endif // SHIP_H
