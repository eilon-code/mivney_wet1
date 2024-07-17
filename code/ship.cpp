#include "ship.h"

StatusType Ship::removePirate(Pirate* pirate)
{
    // assums the pirate indeed is in the ship.
    StatusType output = m_piratesOnShip.remove(pirate->getShipNode());
    if (output != StatusType::SUCCESS) {
        return output;
    }
    PirateRank pirateRank(pirate);
    output = m_piratesOnShipOrderedByRichness.remove(pirateRank);// PirateRank(pirate)
    if (output != StatusType::SUCCESS) {
        return output;
    }

    return updateRichestPirate();
}

StatusType Ship::changePirateTreasure(Pirate* pirate, int change)
{
    PirateRank rank = PirateRank(pirate);
    output_t<PirateRank*> searchResult = m_piratesOnShipOrderedByRichness.get(rank);
    if (searchResult.status() != StatusType::SUCCESS) {
        return searchResult.status();
    }
    PirateRank pirateRank = *searchResult.ans();
    Pirate* piratePointer = pirateRank.getPiratePointer();

    StatusType result = m_piratesOnShipOrderedByRichness.remove(pirateRank);
    if (result != StatusType::SUCCESS) {
        return result;
    }
    piratePointer->updateTreasure(change);
    PirateRank pirateRankNew(piratePointer);
    m_piratesOnShipOrderedByRichness.insert(pirateRankNew); // should work, because deletion of the same object worked
    return updateRichestPirate();
}

output_t<Pirate*> Ship::removeVeteranPirate()
{
    output_t<Pirate*> result = findVeteranPirate();
    if (result.status() != StatusType::SUCCESS) {
        return result.status();
    }
    Pirate* veteranPirate = result.ans();
    StatusType output = removePirate(veteranPirate);
    if (output != StatusType::SUCCESS) {
        return output;
    }
    return veteranPirate;
}

StatusType Ship::insertPirate(Pirate *pirate)
{
    // assums the action is legal
    output_t<List<Pirate*>::Node*> insertionResult = m_piratesOnShip.add(pirate);
    if (insertionResult.status() != StatusType::SUCCESS) {
        return insertionResult.status();
    }
    pirate->setShipNode(insertionResult.ans());
    PirateRank pirateRank(pirate);
    StatusType output = m_piratesOnShipOrderedByRichness.insert(pirateRank).status();
    if (output != StatusType::SUCCESS) {
        return output;
    }
    return updateRichestPirate();
}

StatusType Ship::updateRichestPirate()
{
    if (isRemovable()) {
        m_richestPirate = nullptr;
        return StatusType::SUCCESS;
    }
    output_t<PirateRank*> result = m_piratesOnShipOrderedByRichness.getMax();
    if (result.status() != StatusType::SUCCESS) {
        return result.status();
    }
    PirateRank* richestPirate = result.ans();
    m_richestPirate = richestPirate->getPiratePointer();
    return StatusType::SUCCESS;
}
