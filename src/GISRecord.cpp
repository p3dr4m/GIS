#include "GISRecord.h"

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
    coordinateIndex->updateBoundsOfTree(boundingBox);
}

void GISRecord::insertRecord(vector<string> row, int lineNum, int offset) {
    float latDec, lngDec;

    // Trim the string
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
        return;  // do nothing
    }

    // Insert lat/long into the coordinate index
    coordinateIndex->insert(latDec, lngDec, offset, lineNum);
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

int GISRecord::getImportedNames() {
    return 0;
}

int GISRecord::getLongestProbe() {
    return 0;
}

int GISRecord::getAvgNameLength() {
    return 0;
}


/**
 * The constructor takes a DMS (Degrees, Minutes, Seconds) string as an argument.
 * The string size must be either 7 or 8 characters long, excluding the direction character.
 * For a 7-character string (latitude), the format is DDMMSSX, where DD is degrees, MM is minutes,
 * SS is seconds, and X is direction (N, S, E, W).
 * For an 8-character string (longitude), the format is DDDMMSSX, where DDD is degrees, MM is minutes,
 * SS is seconds, and X is direction (N, S, E, W).
 * If the input string does not meet these requirements, the constructor throws an invalid_argument exception.
 */
DMS::DMS(const string &dms) {
    if (dms.size() != 7 && dms.size() != 8) {
        throw invalid_argument("Invalid DMS string");
    }

    int degreeDigits = (dms.size() == 8) ? 3 : 2;
    degrees = stoi(dms.substr(0, degreeDigits));
    minutes = stoi(dms.substr(degreeDigits, 2));
    seconds = stoi(dms.substr(degreeDigits + 2, 2));

    if (dms.back() == 'N' || dms.back() == 'S' || dms.back() == 'E' || dms.back() == 'W') {
        direction = dms.back();
    } else {
        throw invalid_argument("Invalid direction in DMS string");
    }
}

string DMS::toString() const {
    string degree_str = to_string(degrees);
    string minute_str = to_string(minutes);
    string second_str = to_string(seconds);

    if (degrees < 100) degree_str.insert(degree_str.begin(), 2 - degree_str.length(), '0');
    minute_str.insert(minute_str.begin(), 2 - minute_str.length(), '0');
    second_str.insert(second_str.begin(), 2 - second_str.length(), '0');

    return degree_str + minute_str + second_str + direction;
}

float DMS::toFloat() const {
    auto decimalVersion = static_cast<float>(degrees);
    decimalVersion += static_cast<float>(minutes) / 60.0f;
    decimalVersion += static_cast<float>(seconds) / 3600.0f;

    if (direction == 'S' || direction == 'W') {
        decimalVersion *= -1;
    }

    return decimalVersion;
}

string DMS::toTotalSeconds() const {
    int totalSeconds = degrees * 3600 + minutes * 60 + seconds;
    if (direction == 'S' || direction == 'W') {
        totalSeconds *= -1;
    }
    return to_string(totalSeconds);
}
