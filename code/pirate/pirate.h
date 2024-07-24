#ifndef PIRATE_H
#define PIRATE_H
#include "../search_tree/comparable.h"
#include "../linked_list/linked_list.h"

class Ship;

class Pirate : public Comparable {
public:
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

    // Overload the << operator for Key
    friend std::ostream& operator<<(std::ostream& os, const Pirate& key) {
        os << std::endl;
        os << "    Pirate ID: " << key.getId() << ", Ship ID:" << key.getShipId() << std::endl;
        return os;
    }

    List<Pirate*>::Node* getShipNode() const {
        return m_pointerInShipList;
    }

    void setShipNode(List<Pirate*>::Node* node) {
        m_pointerInShipList = node;
    }

    int getShipId() const;

private:
    int m_id;
    int m_treasure;
    Ship* m_ship;
    List<Pirate*>::Node* m_pointerInShipList;
};

#endif // PIRATE_H
