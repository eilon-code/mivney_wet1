#include "pirates24b1.h"


Ocean::Ocean()
{
    
}

Ocean::~Ocean()
{
    
}

StatusType Ocean::add_ship(int shipId, int cannons)
{
    Ship ship = Ship(shipId, cannons); // maybe add a try/catch ?
    return m_shipTree.insert(ship);
}

StatusType Ocean::remove_ship(int shipId)
{
    Ship ship = Ship(shipId, 0); // maybe add a try/catch ?
    return m_shipTree.remove(ship);
}

StatusType Ocean::add_pirate(int pirateId, int shipId, int treasure)
{
    Ship ship = Ship(shipId, 0); // maybe add a try/catch ?
    output_t<const Ship&> shipOutput = m_shipTree.get(ship);
    if (shipOutput.status == StatusType.Success) {
        return ((Ship&)shipOutput.ans).createPirate(pirateId, treasure);
    }
    return StatusType::FAILURE;
}

StatusType Ocean::remove_pirate(int pirateId)
{
    return StatusType::FAILURE;
}

StatusType Ocean::treason(int sourceShipId, int destShipId)
{
    return StatusType::FAILURE;
}

StatusType Ocean::update_pirate_treasure(int pirateId, int change)
{
    return StatusType::FAILURE;
}

output_t<int> Ocean::get_treasure(int pirateId)
{
    return 0;
}

output_t<int> Ocean::get_cannons(int shipId)
{
    return 0;
}

output_t<int> Ocean::get_richest_pirate(int shipId)
{
    return 0;
}

StatusType Ocean::ships_battle(int shipId1,int shipId2)
{
    return StatusType::FAILURE;
}



