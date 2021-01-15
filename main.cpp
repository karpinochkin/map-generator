#include <iostream>
#include "mapgenerator.h"

int main()
{
    auto xs = Map::Generator::generator()->generateMap(30, 30);
    for (int i = 0; i < xs.size1(); ++i) {
        for (int j = 0; j < xs.size2(); ++j) {
            if (2 == static_cast<int>(xs(i,j))) {
                std::cout << "\x1b[40m" << static_cast<int>(xs(i, j)) << " ";
            }
            else {
                std::cout << "\x1b[44m" << static_cast<int>(xs(i, j)) << " ";
            }
        }
        std::cout << std::endl;
    }
    return 0;
}
