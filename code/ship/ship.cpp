#include "ship.h"

StatusType Ship::removePirate(int pirateId)
{
    return StatusType();
}

StatusType Ship::changePirateTreasure(int pirateId, int change)
 {
    output_t<PirateRank&> searchResult = m_piratesOnShipOrderedByRichness.get(pirateId);
    if (searchResult.status() != StatusType::SUCCESS) {
        return searchResult.status();
    }
    PirateRank pirateRank = searchResult.ans();
    Pirate* piratePointer = pirateRank.getPiratePointer();

    StatusType result = m_piratesOnShipOrderedByRichness.remove(pirateId);
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
    output_t<Pirate*&> result = findVeteranPirate();
    if (result.status() != StatusType::SUCCESS) {
        return result.status();
    }
    Pirate& veteranPirate = *result.ans();
    int veteranPirateId = veteranPirate.getId();
    return removePirate(veteranPirateId);
}

StatusType Ship::insertPirate(const Pirate &pirate)
{
}

StatusType Ship::updateRichestPirate()
{
    output_t<PirateRank&> result = m_piratesOnShipOrderedByRichness.getMax();
    if (result.status() != StatusType::SUCCESS) {
        return result.status();
    }
    PirateRank& richestPirate = result.ans();
    m_richestPirate = richestPirate.getPiratePointer();
}
