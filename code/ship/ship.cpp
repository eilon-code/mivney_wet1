#include "ship.h"

StatusType Ship::removePirate(int pirateId)
{
    return StatusType();
}

StatusType Ship::removeVeteranPirate()
{
    output_t<Pirate&> result = findVeteranPirate();
    if (result.status() != StatusType::SUCCESS) {
        return result.status();
    }
    Pirate& veteranPirate = result.ans();
    int veteranPirateId = veteranPirate.getId();
    return removePirate(veteranPirateId);
}

StatusType Ship::insertPirate(const Pirate &pirate)
{
}

StatusType Ship::updateRichestPirate()
{
    output_t<Pirate&> result = m_piratesOnShipOrderedByRichness.getMax();
    if (result.status() != StatusType::SUCCESS) {
        return result.status();
    }
    Pirate& richestPirate = result.ans();
    m_richestPirate = &richestPirate;
}
