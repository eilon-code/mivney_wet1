#include "pirates24b1.h"
#include "pirate.h"
#include "ship.h"

Ocean::Ocean(){}

Ocean::~Ocean()=default;

StatusType Ocean::add_ship(int shipId, int cannons)
{
    if (shipId <= 0 || cannons < 0) {
        return StatusType::INVALID_INPUT;
    }
    Ship newShip(shipId, cannons);
    StatusType result = m_shipTree.insert(newShip).status();
    return result;
}

StatusType Ocean::remove_ship(int shipId)
{
    if (shipId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    return m_shipTree.remove(shipId); // dummi constructor
}

StatusType Ocean::add_pirate(int pirateId, int shipId, int treasure)
{
    if (shipId <= 0 || pirateId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    output_t<Ship*> searchResult = m_shipTree.get(shipId); // dummi constructor
    if (searchResult.status() != StatusType::SUCCESS) {
        return searchResult.status(); // such a ship do not exist or other issues
    }

    Pirate newPirate(pirateId, treasure);
    output_t<Pirate*> pirateSearch = m_pirateTree.insert(newPirate);
    if (pirateSearch.status() != StatusType::SUCCESS) {
        return pirateSearch.status();
    }
    Pirate* pirate = pirateSearch.ans();
    Ship* ship = searchResult.ans();
    return ship->movePirateIn(pirate);
}

StatusType Ocean::remove_pirate(int pirateId)
{
    if (pirateId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    output_t<Pirate*> searchResult = m_pirateTree.get(pirateId); // dummi constructor
    if (searchResult.status() != StatusType::SUCCESS) {
        return searchResult.status();
    }

    Pirate* pirate = searchResult.ans();
    Ship* ship = pirate->getShip();
    StatusType result = ship->removePirate(pirate);
    if (result != StatusType::SUCCESS) {
        return result;
    }
    return m_pirateTree.remove(pirateId); // dummi constructor
}

StatusType Ocean::treason(int sourceShipId, int destShipId)
{
    if (sourceShipId <= 0 || destShipId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    output_t<Ship*> searchResult1 = m_shipTree.get(sourceShipId); // dummi constructor
    if (searchResult1.status() != StatusType::SUCCESS) {
        return searchResult1.status();
    }
    Ship* sourceShip = searchResult1.ans();
    if (sourceShip->isRemovable()) {
        return StatusType::FAILURE;
    }

    output_t<Ship*> searchResult2 = m_shipTree.get(destShipId); // dummi constructor
    if (searchResult2.status() != StatusType::SUCCESS) {
        return searchResult2.status();
    }
    Ship* destShip = searchResult2.ans();
    
    output_t<Pirate*> deletedPirate = sourceShip->removeVeteranPirate();
    if (deletedPirate.status() != StatusType::SUCCESS) {
        return deletedPirate.status();
    }
    
    Pirate* pirate = deletedPirate.ans();
    return destShip->movePirateIn(pirate);
}

StatusType Ocean::update_pirate_treasure(int pirateId, int change)
{
    if (pirateId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    output_t<Pirate*> searchResult = m_pirateTree.get(pirateId);
    if (searchResult.status() != StatusType::SUCCESS) {
        return searchResult.status();
    }
    Pirate* pirate = searchResult.ans();
    Ship* ship = pirate->getShip();
    
    return ship->changePirateTreasure(pirate, change);
}

output_t<int> Ocean::get_treasure(int pirateId)
{
    if (pirateId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    output_t<Pirate*> searchResult = m_pirateTree.get(pirateId); // dummi constructor
    if (searchResult.status() != StatusType::SUCCESS) {
        return searchResult.status();
    }
    Pirate* pirate = searchResult.ans();
    Ship* ship = pirate->getShip();
    return ship->getBalance() + pirate->getTreasure(); // the ship "owns" some of the pirate's treasure
}

output_t<int> Ocean::get_cannons(int shipId)
{
    if (shipId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    output_t<Ship*> searchResult = m_shipTree.get(shipId); // dummi constructor
    if (searchResult.status() != StatusType::SUCCESS) {
        return searchResult.status();
    }
    Ship* ship = searchResult.ans();
    return ship->getCannons();
}

output_t<int> Ocean::get_richest_pirate(int shipId)
{
    if (shipId <= 0) {
        return StatusType::INVALID_INPUT;
    }
    output_t<Ship*> searchResult = m_shipTree.get(shipId); // dummi constructor
    if (searchResult.status() != StatusType::SUCCESS) {
        return searchResult.status();
    }
    Ship* ship = searchResult.ans();
    return ship->getRichestPirateId();
}

StatusType Ocean::ships_battle(int shipId1,int shipId2)
{
    if (shipId1 == shipId2 || shipId1 <= 0 || shipId2 <= 0) {
        return StatusType::INVALID_INPUT;
    }
    
    output_t<Ship*> searchResult1 = m_shipTree.get(shipId1); // dummi constructor
    if (searchResult1.status() != StatusType::SUCCESS) {
        return searchResult1.status();
    }
    Ship* ship1 = searchResult1.ans();

    output_t<Ship*> searchResult2 = m_shipTree.get(shipId2); // dummi constructor
    if (searchResult2.status() != StatusType::SUCCESS) {
        return searchResult2.status();
    }
    Ship* ship2 = searchResult2.ans();
    
    if (ship1->evaluate() < ship2->evaluate()) {
        ship1->shiftBalance(0 - ship2->getPiratesOnShip());
        ship2->shiftBalance(ship1->getPiratesOnShip());
    } else if (ship1->evaluate() > ship2->evaluate()) {
        ship2->shiftBalance(0 - ship1->getPiratesOnShip());
        ship1->shiftBalance(ship2->getPiratesOnShip());
    }
    return StatusType::SUCCESS;
}



