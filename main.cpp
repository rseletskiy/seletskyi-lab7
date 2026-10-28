#include "array.hpp"
#include "prylad.hpp"

int main() {
    Array<int> numbers(6);
    Array<Prylad> routes(5);

    std::cout << "Enter 6 integers:\n";
    if (!(std::cin >> numbers)) return 1;

    std::cout << "Enter 5 routes: start end days\n";
    if (!(std::cin >> routes)) return 1;

    std::cout << "\nNumbers:\n" << numbers
              << "After last maximum: " << numbers.countAfterLastMax()
              << "\n\nRoutes:\n" << routes
              << "After last maximum: " << routes.countAfterLastMax() << '\n';
}
