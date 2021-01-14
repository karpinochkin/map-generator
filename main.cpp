#include <iostream>
#include "mapgenerator.h"

int main()
{
    auto xs = Map::Generator::generator()->generateMap(10, 10);
    for (int i = 0; i < xs.size1(); ++i) {
        for (int j = 0; j < xs.size2(); ++j) {
            std::cout << static_cast<int>(xs(i, j)) << " ";
        }
        std::cout << std::endl;
    }
    return 0;
}
