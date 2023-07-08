//
// Created by p3dr4 on 2023-07-08.
//

#ifndef INC_8042_PROJECT_DMS_H
#define INC_8042_PROJECT_DMS_H

#include <string>
#include <stdexcept>

/**
 * The constructor takes a DMS (Degrees, Minutes, Seconds) string as an argument.
 * The string size must be either 7 or 8 characters long, excluding the direction character.
 * For a 7-character string (latitude), the format is DDMMSSX, where DD is degrees, MM is minutes,
 * SS is seconds, and X is direction (N, S, E, W).
 * For an 8-character string (longitude), the format is DDDMMSSX, where DDD is degrees, MM is minutes,
 * SS is seconds, and X is direction (N, S, E, W).
 * If the input string does not meet these requirements, the constructor throws an invalid_argument exception.
 */
struct DMS {
    int degrees;
    int minutes;
    int seconds;
    char direction;

    DMS() = default;

    explicit DMS(const std::string &dms) {
        if (dms.size() != 7 && dms.size() != 8) {
            throw std::invalid_argument("Invalid DMS string");
        }

        int degreeDigits = (dms.size() == 8) ? 3 : 2;
        degrees = stoi(dms.substr(0, degreeDigits));
        minutes = stoi(dms.substr(degreeDigits, 2));
        seconds = stoi(dms.substr(degreeDigits + 2, 2));

        if (dms.back() == 'N' || dms.back() == 'S' || dms.back() == 'E' || dms.back() == 'W') {
            direction = dms.back();
        } else {
            throw std::invalid_argument("Invalid direction in DMS string");
        }
    }

    std::string toString() const {
        std::string degree_str = std::to_string(degrees);
        std::string minute_str = std::to_string(minutes);
        std::string second_str = std::to_string(seconds);

        if (degrees < 100) degree_str.insert(degree_str.begin(), 2 - degree_str.length(), '0');
        minute_str.insert(minute_str.begin(), 2 - minute_str.length(), '0');
        second_str.insert(second_str.begin(), 2 - second_str.length(), '0');

        return degree_str + minute_str + second_str + direction;
    }

    float toFloat() const {
        auto decimalVersion = static_cast<float>(degrees);
        decimalVersion += static_cast<float>(minutes) / 60.0f;
        decimalVersion += static_cast<float>(seconds) / 3600.0f;

        if (direction == 'S' || direction == 'W') {
            decimalVersion *= -1;
        }

        return decimalVersion;
    }

    std::string toTotalSeconds() const {
        int totalSeconds = degrees * 3600 + minutes * 60 + seconds;
        if (direction == 'S' || direction == 'W') {
            totalSeconds *= -1;
        }
        return std::to_string(totalSeconds);
    }


    std::string toLogString() const {
        std::string logDirection;
        if (direction == 'N') {
            logDirection = "North";
        } else if (direction == 'S') {
            logDirection = "South";
        } else if (direction == 'E') {
            logDirection = "East";
        } else if (direction == 'W') {
            logDirection = "West";
        }
        return std::to_string(degrees) + "d " + std::to_string(minutes) + "m " + std::to_string(seconds) + "s " +
               logDirection;
    }
};

#endif //INC_8042_PROJECT_DMS_H
