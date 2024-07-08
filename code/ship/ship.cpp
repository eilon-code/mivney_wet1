#include "ship.h"

StatusType Ship::removePirate(Pirate pirate)
{
    // assums the pirate indeed is in the ship.
    StatusType output = m_piratesOnShip.remove(&pirate);
    if (output != StatusType::SUCCESS) {
        return output;
    }
    output = m_piratesOnShipOrderedByRichness.remove(&pirate);// PirateRank(&pirate)
    if (output != StatusType::SUCCESS) {
        return output;
    }

    return updateRichestPirate();
}

StatusType Ship::changePirateTreasure(Pirate& pirate, int change)
 {
    PirateRank rank = PirateRank(&pirate);
    output_t<PirateRank&> searchResult = m_piratesOnShipOrderedByRichness.get(rank);
    if (searchResult.status() != StatusType::SUCCESS) {
        return searchResult.status();
    }
    PirateRank pirateRank = searchResult.ans();
    Pirate* piratePointer = pirateRank.getPiratePointer();

    StatusType result = m_piratesOnShipOrderedByRichness.remove(pirateRank);
    if (result != StatusType::SUCCESS) {
        return result;
    }
    piratePointer->updateTreasure(change);
    PirateRank pirateRankNew = PirateRank(piratePointer);
    m_piratesOnShipOrderedByRichness.insert(pirateRankNew); // should work, because deletion of the same object worked
    return StatusType::SUCCESS;
}

StatusType Ship::removeVeteranPirate()
{
    output_t<Pirate*const&> result = findVeteranPirate();
    if (result.status() != StatusType::SUCCESS) {
        return result.status();
    }
    Pirate& veteranPirate = *result.ans();
    return removePirate(veteranPirate);
}

StatusType Ship::insertPirate(const Pirate &pirate)
{
}

StatusType Ship::updateRichestPirate()
{
    output_t<const PirateRank&> result = m_piratesOnShipOrderedByRichness.getMax();
    if (result.status() != StatusType::SUCCESS) {
        return result.status();
    }
    const PirateRank& richestPirate = result.ans();
    m_richestPirate = richestPirate.getPiratePointer();
}
