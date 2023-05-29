#include "GISRecord.h"

using namespace std;


/**
 * Converts a DMS coordinate to a decimal coordinate
 * @param dms
 * @return
 */
float GISRecord::convertDMS(DMS dms) {
    auto decimalVersion = static_cast<float>(dms.degrees);
    decimalVersion += static_cast<float>(dms.minutes) / 60.f;
    decimalVersion += static_cast<float>(dms.seconds) / 3600.f;

    if (dms.direction == 'N' || dms.direction == 'E')
        return decimalVersion;
    else
        return decimalVersion * -1;
}

/**
 * Sets the bounds of the GISRecord
 * @param minLat
 * @param maxLat
 * @param minLong
 * @param maxLong
 */
void GISRecord::setBounds(DMS minLat, DMS maxLat, DMS minLong, DMS maxLong) {
    bounds.minLat = convertDMS(minLat);
    bounds.maxLat = convertDMS(maxLat);
    bounds.minLong = convertDMS(minLong);
    bounds.maxLong = convertDMS(maxLong);
    std::string nineTabs(9, '\t');
    std::string sixTabs(6, '\t');
    std::string threeTabs(3, '\t');

    std::string logLine = nineTabs + std::to_string(bounds.maxLat) +
                          "\n" + sixTabs + std::to_string(bounds.minLong) +
                          threeTabs + std::to_string(bounds.maxLong) +
                          "\n" + nineTabs + std::to_string(bounds.minLat);
    // log bounds from logger class
    logger.log(logLine, Logger::WORLD, true, true);

//    coordinateIndex->updateBoundsOfTree(bounds);
    // update bounds of coordinate index
    coordinateIndex->updateBoundsOfTree(bounds);
}

