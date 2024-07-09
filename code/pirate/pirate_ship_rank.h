#ifndef PIRATE_SHIP_RANK_H
#define PIRATE_SHIP_RANK_H

#include "comparable.h"
#include "pirate.h"

class PirateShipRank : public Comparable {
private:
    Pirate* m_pirate;

    Pirate& getPirate() const {
        return *getPiratePointer();
    }

public:
    PirateShipRank(Pirate* pirate) : m_pirate(pirate) {}

    Pirate* getPiratePointer() const {
        return m_pirate;
    }

    bool operator<(const Comparable& other) const override {
        if (*this == other) {
            return false;
        }

        PirateShipRank o = (PirateShipRank&) other;
        return getIdOnShip() < o.getIdOnShip();
    }
    
    bool operator==(const Comparable& o) const override {
        PirateShipRank other = (PirateShipRank&) o;
        return getIdOnShip() == other.getIdOnShip();
    }

    int getIdOnShip() const {
        return getPirate().getIdOnShip();
    }
};

#endif // PIRATE_SHIP_RANK_H
