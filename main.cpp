#include <iostream>

#include "Map.hpp"

int main() {
    dwt::Map<int, int> map;
    map.set(1, 1);
    map.set(-2, -2);
    map.set(3, 3);
    map.set(-5, -5);
    map.set(-9, -9);

#ifdef DEBUG
    map.print();
#endif

    map.set(1, 1);

    try {
        map.get(4);
    }
    catch (dwt::NotFoundKeyException<int>& e) {
        std::cout << e.getKey() << std::endl;
    }

    int valueIn = -5;
    if (map.isSet(valueIn)) {
        std::cout << valueIn << " is in" << std::endl;
    }

    int valueOut = 565;
    if (!map.isSet(valueOut)) {
        std::cout << valueOut << " is out" << std::endl;
    }
}