#include "mapgenerator.h"

namespace Map {

Generator *Generator::generator()
{
    static Generator gen;
    return &gen;
}

Matrix Generator::generateMap(uint rows_count, uint columns_count)
{
    rowsAndColumnsCountCheck(rows_count, columns_count);
    setData(rows_count, columns_count);
    setExternalWalls();
    setPassages();

    return *(matrix.get());
}

void Generator::rowsAndColumnsCountCheck(uint rows_count, uint columns_count) const
{
    uint min = 10;
    uint max = 1000;
    if (rows_count < min || columns_count > max) {
        std::string err = "The rows count and columns count should have minimal value = "
                + std::to_string(min)
                + " and maximum value = " + std::to_string(max)
                + " . But you have this value = " + std::to_string(rows_count)
                + " and " + std::to_string(columns_count);
        throw GeneratorException(err);
    }
}

void Generator::setData(uint rowsCount, uint columnsCount)
{
    rows = rowsCount;
    columns = columnsCount;
    matrix = createMatrix();
}

std::unique_ptr<Matrix> Generator::createMatrix() const
{
    return std::make_unique<Matrix>(rows, columns, Objects::None);
}

void Generator::setExternalWalls()
{
    for (unsigned long i = 0; i < matrix->size1(); ++i) {
        (*matrix)(i, 0) = Objects::Wall;
        (*matrix)(i, matrix->size2() - 1) = Objects::Wall;

        for (unsigned long j = 1; j < matrix->size2() - 1; ++j) {
            if (i == 0 || i == matrix->size1() - 1) {
                (*matrix)(i, j) = Objects::Wall;
            }
        }
    }
}

void Generator::setPassages()
{
    Item start_point = startPointForPassagesGeneration();
    generatePassages(start_point);

    for (uint i = 1; i < matrix->size1() - 1; ++i) {
        for (uint j = 1; j < matrix->size2()- 1; ++j) {
            if (std::size(getUnvisitedNeighbors(Item{i,j})) != 0) {
                generatePassages(Item{i,j});
            }
        }
    }
}

Item Generator::startPointForPassagesGeneration()
{
    uint row_mid = matrix->size1() / 2;
    uint column_mid = matrix->size2() / 2;
    return Item {row_mid, column_mid};
}

void Generator::generatePassages(Item point)
{
    Item currency_point = point;
    while(true) {
        (*matrix)(currency_point.row, currency_point.column) = Objects::Ground;
        auto unvisited_neighbors = getUnvisitedNeighbors(currency_point);
        if (std::size(unvisited_neighbors) == 0) {
            break;
        }

        currency_point = getRandomItem(unvisited_neighbors);
        for (auto i : unvisited_neighbors) {
            if (currency_point != i) {
                (*matrix)(i.row, i.column) = Objects::Wall;
            }
        }
    }
}

std::vector<Item> Generator::getUnvisitedNeighbors(const Item &item)
{
    std::vector<Item> output;
    addUnvisitedItemToVector(Item{item.row - 1, item.column}, output);
    addUnvisitedItemToVector(Item{item.row + 1, item.column}, output);
    addUnvisitedItemToVector(Item{item.row, item.column - 1}, output);
    addUnvisitedItemToVector(Item{item.row, item.column + 1}, output);

//    if ((*matrix)(item.row - 1, item.column) == Objects::None) {
//        output.push_back(Item{item.row - 1, item.column});
//    }

//    if ((*matrix)(item.row + 1, item.column) == Objects::None) {
//        output.push_back(Item{item.row + 1, item.column});
//    }

//    if ((*matrix)(item.row, item.column - 1) == Objects::None) {
//        output.push_back(Item{item.row, item.column - 1});
//    }

//    if ((*matrix)(item.row, item.column + 1) == Objects::None) {
//        output.push_back(Item{item.row, item.column + 1});
//    }

    return output;
}

void Generator::addUnvisitedItemToVector(const Item &item, std::vector<Item> &vector)
{
    if ((*matrix)(item.row, item.column) == Objects::None) {
        vector.push_back(Item{item.row, item.column});
    }
}

Item Generator::getRandomItem(const std::vector<Item> &items)
{
    std::mt19937 gen(std::random_device().operator()());
    std::uniform_int_distribution<> rand(0, std::size(items) - 1);

    return items.at(rand(gen));
}

}
