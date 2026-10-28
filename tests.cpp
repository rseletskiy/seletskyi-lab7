#include "array.hpp"
#include "prylad.hpp"

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <utility>

void expect(bool condition) {
    if (!condition) throw std::runtime_error("Test failed");
}

struct ThrowingValue {
    inline static int alive = 0;
    inline static bool fail = false;
    ThrowingValue() { ++alive; }
    ~ThrowingValue() { --alive; }
    ThrowingValue& operator=(const ThrowingValue&) {
        if (fail) throw std::runtime_error("Copy failed");
        return *this;
    }
};

template <typename T>
std::string describe(const Array<T>& array) {
    std::ostringstream output;
    output << array;
    return output.str();
}

int main() {
    try {
        Array<int> original{3, 8, 1, 8, 5, 2};
        expect(original.countAfterLastMax() == 2);
        expect(Array<int>{9, 3, 2}.countAfterLastMax() == 2);
        expect(Array<int>{1, 9}.countAfterLastMax() == 0);
        expect(Array<int>{4, 4, 4}.countAfterLastMax() == 0);
        expect(Array<int>{-3, -1, -4, -1, -5}.countAfterLastMax() == 1);
        expect(Array<int>{7}.countAfterLastMax() == 0);
        expect(Array<int>().countAfterLastMax() == 0);

        Array<int> copy(original);
        std::istringstream changed("1 2 3 4 5 6");
        expect(static_cast<bool>(changed >> copy));
        expect(describe(copy) == "1\n2\n3\n4\n5\n6\n");
        expect(describe(original) == "3\n8\n1\n8\n5\n2\n");
        Array<int> assigned(1);
        assigned = original;
        expect(describe(assigned) == describe(original));
        Array<int>& alias = assigned;
        assigned = alias;
        expect(assigned.countAfterLastMax() == 2);
        Array<int> moved(std::move(copy));
        expect(describe(moved) == "1\n2\n3\n4\n5\n6\n");
        expect(describe(copy).empty());
        expect(copy.countAfterLastMax() == 0);
        assigned = std::move(moved);
        expect(describe(assigned) == "1\n2\n3\n4\n5\n6\n");
        expect(describe(moved).empty());

        Array<Prylad> routes{{"Z", "Y", 2}, {"A", "B", 5}, {"C", "D", 5}, {"E", "F", 3}};
        expect(routes.countAfterLastMax() == 1);
        Array<Prylad> routeCopy(routes);
        std::istringstream routeInput("\"New York\" \"Los Angeles\" 1 A B 2 C D 3 E F 4");
        expect(static_cast<bool>(routeInput >> routeCopy));
        expect(routeCopy.countAfterLastMax() == 0);
        expect(routes.countAfterLastMax() == 1);
        expect(describe(routeCopy).find("\"New York\" \"Los Angeles\" 1\n") == 0);
        Array<Prylad> roundTrip(4);
        std::istringstream serialized(describe(routeCopy));
        expect(static_cast<bool>(serialized >> roundTrip));
        expect(describe(roundTrip) == describe(routeCopy));
        std::istringstream invalid("A B 0");
        expect(!(invalid >> roundTrip));
        std::istringstream incomplete("1");
        expect(!(incomplete >> original));

        {
            Array<ThrowingValue> source(3), target(2);
            ThrowingValue::fail = true;
            bool caught = false;
            try {
                Array<ThrowingValue> failedCopy(source);
            } catch (const std::runtime_error&) {
                caught = true;
            }
            expect(caught && ThrowingValue::alive == 5);
            caught = false;
            try {
                target = source;
            } catch (const std::runtime_error&) {
                caught = true;
            }
            expect(caught && ThrowingValue::alive == 5);
            ThrowingValue::fail = false;
        }
        expect(ThrowingValue::alive == 0);
        std::cout << "All tests passed\n";
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
