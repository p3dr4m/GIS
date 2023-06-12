#include "GISRecord.h"

using namespace std;


/**
 * Converts a DMS coordinate to a decimal coordinate
 * @param dms
 * @return
 */
float GISRecord::convertDmsToFloat(DMS dms) {
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
void GISRecord::setBounds(float minLat, float maxLat, float minLong, float maxLong) {
    bounds.minLat = minLat;
    bounds.maxLat = maxLat;
    bounds.minLong = minLong;
    bounds.maxLong = maxLong;
    string nineTabs(9, '\t');
    string sixTabs(6, '\t');
    string threeTabs(3, '\t');

    string logLine = nineTabs + to_string(bounds.maxLat) +
                     "\n" + sixTabs + to_string(bounds.minLong) +
                     threeTabs + to_string(bounds.maxLong) +
                     "\n" + nineTabs + to_string(bounds.minLat);
    // log bounds from logger class
    logger.log(logLine, Logger::WORLD, true, true);

    // update bounds of coordinate index
    coordinateIndex->updateBoundsOfTree(bounds);
}

void GISRecord::insertRecord(vector<string> row, int lineNum, int offset) {
    // file offset
    // db line
    // get lat, long

//    float latDec = stof(row[PRIM_LAT_DEC]);
//    float longDec = stof(row[PRIM_LONG_DEC]);

    string lat = row[PRIMARY_LAT_DMS];
    string lng = row[PRIM_LONG_DMS];

    float latDec = DMS(lat).toFloat();
    float lngDec = DMS(lng).toFloat();

    // float of dms lat, long
    coordinateIndex->insert(latDec, lngDec, offset, lineNum);

    // get feature name
    // get state alpha
//    nameIndex->insert();
}

vector<int> GISRecord::findRecords(float lat, float lng) {
    vector<int> result = coordinateIndex->searchRecords(lat, lng);
    return result;
}

vector<int> GISRecord::findRecords(float longitude, float latitude, float halfWidth, float halfHeight) {
    Coordinate centralLocation = Coordinate(longitude, latitude);

    vector<int> result = coordinateIndex->searchRecordsInBounds(centralLocation, halfWidth, halfHeight);
    return result;
}
