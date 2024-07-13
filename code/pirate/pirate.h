#ifndef PIRATE_H
#define PIRATE_H
#include "comparable.h"
#include "linked_list.h"

class Ship;

class Pirate : public Comparable {
public:
    class PiratePointer {
        public:
            PiratePointer() : m_pointer(nullptr) {}
            PiratePointer(Pirate* pointer) : m_pointer(pointer) {}

            Pirate& operator->() const {
                return *m_pointer;
            }

            Pirate* get() const {
                return m_pointer;
            }
        private:
            Pirate* m_pointer;
    };
    Pirate(int id, int treasure) : m_id(id), m_treasure(treasure), m_ship(nullptr), m_pointerInShipList(nullptr) {}
    Pirate(int id) : m_id(id), m_treasure(0), m_ship(nullptr) {} // dummi constructor
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

    List<Pirate::PiratePointer>::Node* getShipNode() const {
        return m_pointerInShipList;
    }

    void setShipNode(List<Pirate::PiratePointer>::Node* node) {
        m_pointerInShipList = node;
    }
private:
    int m_id;
    int m_treasure;
    Ship* m_ship;
    List<Pirate::PiratePointer>::Node* m_pointerInShipList;
};

#endif // PIRATE_H
