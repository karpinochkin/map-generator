#ifndef MAPGENERATOR_H
#define MAPGENERATOR_H

#include <boost/numeric/ublas/matrix.hpp>
#include <random>
#include <set>

namespace Map {

enum class Objects {
    None = 0,
    Ground = 1,
    Wall = 2,
    Point = 3,
    Trap = 4
};

using Matrix = boost::numeric::ublas::matrix<Objects>;

class GeneratorException : public std::exception {
public:
    explicit GeneratorException(const char* message) noexcept : err(message) {

    }

    explicit GeneratorException(const std::string& message) noexcept : err(message) {

    }

    virtual const char* what() const throw () {
        return err.c_str();
    }
    ~GeneratorException() throw() = default;
private:
    std::string err;
};

struct Item {
    uint row;
    uint column;

    bool operator!=(const Item& item) const {
        return (row != item.row || column != item.column);
    }
};

class Generator
{
public:
    Generator(Generator&) = delete;
    Generator& operator=(Generator&) = delete;
public:
    static Generator* generator();
    Matrix generateMap(uint rows_count, uint columns_count);

private:
    Generator() = default;
    ~Generator() = default;

private:
    void rowsAndColumnsCountCheck(uint rows_count, uint columns_count) const;
    std::unique_ptr<Matrix> createMatrix() const;
    void setData(uint rows_count, uint columns_count);
    void setExternalWalls();
    void setPassages();
    Item startPointForPassagesGeneration();
    void generatePassages(Item point);
    std::vector<Item> getUnvisitedNeighbors(const Item&);
    void addUnvisitedItemToVector(const Item&, std::vector<Item>&);
    Item getRandomItem(const std::vector<Item>&);
    void setIternalWalls(const Item &currency_point, const std::vector<Item> &);
    int getCountNeighborsIsWall(const Item& point);
    int getCountColumnNeighborsIsWall(const Item& point);
    int getCountRowNeighborsIsWall(const Item& point);
    bool isWall(const Item& point);

private:
    uint rows{};
    uint columns{};
    std::unique_ptr<Matrix> matrix;
};

}

#endif // MAPGENERATOR_H
