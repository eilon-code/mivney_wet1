#include "pirates24b1.h"
#include "wet1util.h"
#include "avl_tree.h"

Ocean::Ocean(){}

Ocean::~Ocean()
{
    m_shipTree.~AVLTree();
    m_pirateTree.~AVLTree();
}

StatusType Ocean::add_ship(int shipId, int cannons)
{
    if (shipId <= 0 || cannons < 0) {
        return StatusType::INVALID_INPUT;
    }
    StatusType result = m_shipTree.search(shipId);
    if (result == StatusType::SUCCESS) {
        return StatusType::FAILURE; // found existing ship with the same id
    } else if (result != StatusType::FAILURE) {
        return result; // memory issue
    }

    Ship* ship;
    try
    {
        ship = new Ship(shipId, cannons);
    }
    catch(const std::exception& e)
    {
        return StatusType::ALLOCATION_ERROR;
    }
    result = m_shipTree.insert(*ship);

    if (result != StatusType::SUCCESS) {
        delete ship;
    }
    return StatusType::SUCCESS;
}

StatusType Ocean::remove_ship(int shipId)
{
    if (shipId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    output_t<Ship&> searchResult = m_shipTree.get(shipId);
    if (searchResult.status() != StatusType::SUCCESS) {
        return searchResult.status();
    }
    const Ship& ship = searchResult.ans();
    if (ship.getPiratesOnShip() > 0) {
        return StatusType::FAILURE;
    }
    return m_shipTree.remove(ship.getId());
}

StatusType Ocean::add_pirate(int pirateId, int shipId, int treasure)
{
    if (shipId <= 0 || pirateId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    StatusType pirateSearch = m_pirateTree.search(pirateId);
    if (pirateSearch == StatusType::SUCCESS) {
        return StatusType::FAILURE; // already an existing pirate with the same id
    } else if (pirateSearch != StatusType::FAILURE) {
        return pirateSearch; // memory issue
    }

    output_t<Ship&> searchResult = m_shipTree.get(shipId);
    if (searchResult.status() != StatusType::SUCCESS) {
        return searchResult.status(); // such a ship do not exist or other issues
    }
    Ship& ship = searchResult.ans();
    return ship.createPirateIn(pirateId, treasure);
}

StatusType Ocean::remove_pirate(int pirateId)
{
    if (pirateId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    output_t<Pirate&> searchResult = m_pirateTree.get(pirateId);
    if (searchResult.status() != StatusType::SUCCESS) {
        return searchResult.status();
    }

    const Pirate& pirate = searchResult.ans();
    Ship& ship = *pirate.getShip();
    StatusType result = ship.removePirate(pirate.getId());
    if (result != StatusType::SUCCESS) {
        return result;
    }
    return m_pirateTree.remove(pirateId);
}

StatusType Ocean::treason(int sourceShipId, int destShipId)
{
    if (sourceShipId <= 0 || destShipId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    output_t<Ship&> searchResult1 = m_shipTree.get(sourceShipId);
    if (searchResult1.status() != StatusType::SUCCESS) {
        return searchResult1.status();
    }
    Ship& sourceShip = searchResult1.ans();
    if (sourceShip.getPiratesOnShip() == 0) {
        return StatusType::FAILURE;
    }

    output_t<Ship&> searchResult2 = m_shipTree.get(destShipId);
    if (searchResult2.status() != StatusType::SUCCESS) {
        return searchResult2.status();
    }
    Ship& destShip = searchResult2.ans();

    // TODO: add some checks for valid action

    output_t<Pirate&> deletedPirate = sourceShip.removeVeteranPirate();
    if (deletedPirate.status() != StatusType::SUCCESS) {
        return deletedPirate.status();
    }
    
    return destShip.movePirateInto(deletedPirate.ans());
}

StatusType Ocean::update_pirate_treasure(int pirateId, int change)
{
    if (pirateId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    output_t<Pirate&> searchResult = m_pirateTree.get(pirateId);
    if (searchResult.status() != StatusType::SUCCESS) {
        return searchResult.status();
    }
    const Pirate& pirate = searchResult.ans();
    Ship& ship = *pirate.getShip();
    
    return ship.changePirateTreasure(pirateId, change);
}

output_t<int> Ocean::get_treasure(int pirateId)
{
    if (pirateId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    output_t<Pirate&> searchResult = m_pirateTree.get(pirateId);
    if (searchResult.status() != StatusType::SUCCESS) {
        return searchResult.status();
    }
    const Pirate& pirate = searchResult.ans();
    Ship& ship = *pirate.getShip();
    return ship.getBalance() + pirate.getTreasure(); // the ship "owns" some of the pirate's treasure
}

output_t<int> Ocean::get_cannons(int shipId)
{
    if (shipId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    output_t<Ship&> searchResult = m_shipTree.get(shipId);
    if (searchResult.status() != StatusType::SUCCESS) {
        return searchResult.status();
    }
    const Ship& ship = searchResult.ans();
    return ship.getCannons();
}

output_t<int> Ocean::get_richest_pirate(int shipId)
{
    if (shipId <= 0) {
        return StatusType::INVALID_INPUT;
    }
    output_t<Ship&> searchResult = m_shipTree.get(shipId);
    if (searchResult.status() != StatusType::SUCCESS) {
        return searchResult.status();
    }
    const Ship& ship = searchResult.ans();
    return ship.getRichestPirateId();
}

StatusType Ocean::ships_battle(int shipId1,int shipId2)
{
    if (shipId1 == shipId2 || shipId1 <= 0 || shipId2 <= 0) {
        return StatusType::INVALID_INPUT;
    }
    
    output_t<Ship&> searchResult1 = m_shipTree.get(shipId1);
    if (searchResult1.status() != StatusType::SUCCESS) {
        return searchResult1.status();
    }
    Ship& ship1 = searchResult1.ans();

    output_t<Ship&> searchResult2 = m_shipTree.get(shipId2);
    if (searchResult2.status() != StatusType::SUCCESS) {
        return searchResult2.status();
    }
    Ship& ship2 = searchResult2.ans();
    
    if (ship1.evaluate() < ship2.evaluate()) {
        ship1.shiftBalance(0 - ship2.getPiratesOnShip());
        ship2.shiftBalance(ship1.getPiratesOnShip());
    } else if (ship1.evaluate() > ship2.evaluate()) {
        ship2.shiftBalance(0 - ship1.getPiratesOnShip());
        ship1.shiftBalance(ship2.getPiratesOnShip());
    }
    return StatusType::SUCCESS;
}



