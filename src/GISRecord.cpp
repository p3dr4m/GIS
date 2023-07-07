#include <fstream>
#include <map>
#include "GISRecord.h"
#include "interface/SystemManager.h"
#include "interface/Logger.h"

using namespace std;


/**
 * Sets the bounds of the GISRecord
 * @param minLat
 * @param maxLat
 * @param minLong
 * @param maxLong
 */
void GISRecord::setBounds(float minLat, float maxLat, float minLong, float maxLong) {
    BoundingBox boundingBox = BoundingBox(minLat, maxLat, minLong, maxLong);
    clearIndices();
    coordinateIndex->updateBoundsOfTree(boundingBox);
}

void GISRecord::insertRecord(vector<string> row, int lineNum, int offset) {
    float latDec, lngDec;

    string latDecStr = row[PRIM_LAT_DEC];
    string lngDecStr = row[PRIM_LONG_DEC];

    string latDmsStr = row[PRIMARY_LAT_DMS];
    string lngDmsStr = row[PRIM_LONG_DMS];

    // Check if either DEC or DMS are missing
    if (!latDecStr.empty() && !lngDecStr.empty()) {
        // Both DEC values are present
        latDec = stof(latDecStr);
        lngDec = stof(lngDecStr);
    } else if (!latDmsStr.empty() && !lngDmsStr.empty()) {
        // DEC values are missing, but DMS are present
        latDec = DMS(latDmsStr).toFloat();
        lngDec = DMS(lngDmsStr).toFloat();
    } else {
        // Both DEC and DMS are missing
        return;
    }

    // Insert lat/long into the coordinate index
    coordinateIndex->insert(latDec, lngDec, offset, lineNum);
    nameIndex->insert(row[FEATURE_NAME], row[STATE_ALPHA], offset);
}


vector<int> GISRecord::findRecords(const string &name, const string &state) {
    vector<int> result = nameIndex->find(name, state);
    return result;
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

vector<Record> GISRecord::getRecords(const vector<int> &offsets) {
    vector<Record> records;
    Logger &logger = Logger::getInstance();

    for (auto offset: offsets) {
        if (buffer.exists(offset)) {
            Record record = buffer.get(offset);
            records.push_back(record);
            buffer.put(record);
        } else {
            Record record = SystemManager::goToOffset(logger.getDatabaseFilePath(), offset);
            buffer.put(record);
            records.push_back(record);
        }
    }
    return records;
}

vector<Record> GISRecord::getRecords(const vector<int> &offsets, const string &filterOption) {
    vector<Record> records;
    Logger &logger = Logger::getInstance();

    for (auto offset: offsets) {
        if (buffer.exists(offset)) {
            records.push_back(buffer.get(offset));
        } else {
            Record record = SystemManager::goToOffset(logger.getDatabaseFilePath(), offset);
            records.push_back(record);
        }
    }
    vector<Record> filteredRecords = Logger::filterRecords(records, filterOption);

    for (const auto &record: filteredRecords) {
        buffer.put(record);
    }

    return filteredRecords;
}

