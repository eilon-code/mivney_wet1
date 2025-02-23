// 
// 234218 Data Structures 1.
// Semester: 2024B (spring).
// Wet Exercise #1.
// 
// The following header file contains all methods we expect you to implement.
// You MAY add private methods and fields of your own.
// DO NOT erase or modify the signatures of the public methods.
// DO NOT modify the preprocessors in this file.
// DO NOT use the preprocessors in your other code files.
// 

#ifndef PIRRATES24SPRING_WET1_H_
#define PIRRATES24SPRING_WET1_H_

#include "wet1util.h"
#include "avl_tree.h"
#include "ship.h"
#include "pirate.h"

class Ocean {
private:
    AVLTree<Pirate> m_pirateTree;
    AVLTree<Ship> m_shipTree;
    
    //
    // Here you may add anything you want
    //
    
public:
    // <DO-NOT-MODIFY> {
    StatusType printPirates();

    StatusType printShips();

    Ocean();

    virtual ~Ocean();

    void print_avl_tree() const {
        m_shipTree.print();
    }
    
    StatusType add_ship(int shipId, int cannons);

    StatusType remove_ship(int shipId);
    
    StatusType add_pirate(int pirateId, int shipId, int treasure);
    
    StatusType remove_pirate(int pirateId);
    
    StatusType treason(int sourceShipId, int destShipId);

    StatusType update_pirate_treasure(int pirateId, int change);

    output_t<int> get_treasure(int pirateId);

    output_t<int> get_cannons(int shipId);

    output_t<int> get_richest_pirate(int shipId);

    StatusType ships_battle(int shipId1,int shipId2);

    // } </DO-NOT-MODIFY>
};

#endif // PIRRATES24SPRING_WET1_H_
