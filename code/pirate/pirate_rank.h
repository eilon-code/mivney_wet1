#ifndef PIRATE_RANK_H
#define PIRATE_RANK_H

#include "pirate.h"

class PirateRank : public Comparable {
private:
    Pirate* m_pirate;

public:
    PirateRank(Pirate* pirate) : m_pirate(pirate) {}

    Pirate* getPiratePointer() const {
        return m_pirate;
    }

    bool operator<(const Comparable& other) const override {
        if (*this == other) {
            return false;
        }

        PirateRank o = (PirateRank&) other;
        if (getTreasure() < o.getTreasure()) {
            return true;
        }
        if (getTreasure() > o.getTreasure()) {
            return false;
        }
        return getId() < o.getId();
    }

    bool operator==(const Comparable& o) const override {
        PirateRank other = (PirateRank&) o;
        return getTreasure() == other.getTreasure() && getId() == other.getId();
    }

    int getId() const {
        return m_pirate->getId();
    }

    int getTreasure() const {
        return m_pirate->getTreasure();
    }
};

#endif // PIRATE_RANK_H
