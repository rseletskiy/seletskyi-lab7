#pragma once

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <utility>

class Prylad {
private:
    std::string startPoint;
    std::string endPoint;
    int durationDays = 0;

public:
    Prylad() = default;

    Prylad(std::string startPoint, std::string endPoint, int durationDays)
        : startPoint(std::move(startPoint)), endPoint(std::move(endPoint)),
          durationDays(durationDays) {}

    bool input() {
        std::string start, end, line;
        int days = 0;

        std::cout << "Start point: ";
        if (!std::getline(std::cin, start)) return false;

        std::cout << "End point: ";
        if (!std::getline(std::cin, end)) return false;

        while (true) {
            std::cout << "Duration (days): ";
            if (!std::getline(std::cin, line)) return false;

            std::istringstream value(line);
            if (value >> days && days > 0 && (value >> std::ws).eof()) break;

            std::cout << "Enter a positive integer.\n";
        }

        startPoint = std::move(start);
        endPoint = std::move(end);
        durationDays = days;
        return true;
    }

    bool operator>=(const Prylad& other) const {
        return durationDays >= other.durationDays;
    }

    friend std::istream& operator>>(std::istream& input, Prylad& route) {
        std::string start, end;
        int days = 0;
        if (input >> std::quoted(start) >> std::quoted(end) >> days) {
            if (days <= 0) input.setstate(std::ios::failbit);
            else route = Prylad(std::move(start), std::move(end), days);
        }
        return input;
    }

    friend std::ostream& operator<<(std::ostream& output, const Prylad& route) {
        return output << std::quoted(route.startPoint) << ' '
                      << std::quoted(route.endPoint) << ' ' << route.durationDays;
    }

    void output() const {
        std::cout << "Start point: " << startPoint << '\n'
                  << "End point: " << endPoint << '\n'
                  << "Duration (days): " << durationDays << '\n';
    }
};

