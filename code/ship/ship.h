#ifndef SHIP_H
#define SHIP_H

#include "pirate.h"
#include "comparable.h"
#include "avl_tree.h"
#include "pirate_rank.h"
#include "linked_list.h"

class Ship : public Comparable {
public:
    Ship(int shipId, int cannons) : m_id(shipId), m_cannons(cannons), m_coinOffset(0), m_richestPirate(nullptr) {}
    Ship(int shipId) : m_id(shipId), m_cannons(-1), m_coinOffset(0), m_richestPirate(nullptr) {} // dumi ship
    ~Ship()=default;

    StatusType removePirate(Pirate* pirate);

    StatusType movePirateIn(Pirate* pirate){
        pirate->replaceShip(this);
        pirate->updateTreasure(0 - m_coinOffset);
        return insertPirate(pirate);
    }

    bool operator<(const Comparable& other) const override {
        return m_id < ((Ship&) other).getId();
    }

    bool operator==(const Comparable& other) const override {
        return m_id == ((Ship&) other).getId();
    }

    bool isRemovable() const override {
        return getPiratesOnShip() == 0;
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

    StatusType changePirateTreasure(Pirate* pirate, int change);
    output_t<Pirate*> removeVeteranPirate();

private:
    int m_id;
    int m_cannons;
    int m_coinOffset;
    List<Pirate::PiratePointer> m_piratesOnShip; // ordered by time on ship
    AVLTree<PirateRank> m_piratesOnShipOrderedByRichness;
    Pirate* m_richestPirate;

    StatusType insertPirate(Pirate* pirate);
    StatusType updateRichestPirate();
    output_t<Pirate::PiratePointer*> findVeteranPirate() {
        return m_piratesOnShip.first();
    }
};

#endif // SHIP_H
