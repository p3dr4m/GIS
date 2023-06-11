#include "GISRecord.h"

using namespace std;


DMS GISRecord::fillDMS(string value) {
    DMS dms{};

    int degreeSize;
    int minSecSize = 2;
    int charSize = 1;

    if (value.length() == 8) {
        degreeSize = 3;

        dms.degrees = stoi(value.substr(0, degreeSize));
        dms.minutes = stoi(value.substr(3, minSecSize));
        dms.seconds = stoi(value.substr(5, minSecSize));
        dms.direction = (char) ((value.substr(7, charSize))[0]);

    } else if (value.length() == 7) {
        degreeSize = 2;

        dms.degrees = stoi(value.substr(0, degreeSize));
        dms.minutes = stoi(value.substr(2, minSecSize));
        dms.seconds = stoi(value.substr(4, minSecSize));
        dms.direction = (char) ((value.substr(6, charSize))[0]);
    } else {
        throw invalid_argument("Invalid DMS format");
    }

    return dms;
}


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

void GISRecord::insertRecord(vector<string> row, int lineNum, int offset){
    // file offset
    // db line
    // get lat, long

//    float latDec = stof(row[PRIM_LAT_DEC]);
//    float longDec = stof(row[PRIM_LONG_DEC]);

    string latDMS = row[PRIMARY_LAT_DMS];
    string longDMS = row[PRIM_LONG_DMS];

    DMS Lat = fillDMS(latDMS);
    DMS Lng = fillDMS(longDMS);

    float latDec = convertDMS(Lat);
    float longDec = convertDMS(Lng);



    // float of dms lat, long
    coordinateIndex->insert(latDec, longDec, lineNum, offset);



    // get feature name
    // get state alpha
//    nameIndex->insert();
}

vector<int> GISRecord::findRecords(float lat, float lng) {
    vector<int> result = coordinateIndex->searchRecords(lat, lng);
    return  result;
}

vector<int> GISRecord::findRecordsInBounds(float longitude, float latitude, float halfWidth, float halfHeight) {
    Coordinate centralLocation = Coordinate(longitude, latitude);

    vector<int> result = coordinateIndex->searchRecordsInBounds(centralLocation, halfWidth, halfHeight);
    return result;
}