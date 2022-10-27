#include <cstddef>
#include <array>
#include <initializer_list>

#include <catch2/catch.hpp>


template <typename T, std::size_t N>
class ContainerBase {
    virtual constexpr const T& operator()(std::size_t i) const = 0;
};

template <typename T, std::size_t N>
class Container : public ContainerBase<T, N> {
public:
    constexpr Container(std::initializer_list<T> data) {
        std::copy(data.begin(), data.end(), _items.begin());
    }
    constexpr const T& operator()(std::size_t i) const override {
        return _items[i];
    }
private:
    std::array<T, N> _items;
};

TEST_CASE("MSVC buggy constexpr test case") {
    constexpr Container<int, 3> C = {2, -91, 7};
    constexpr int F = C(1);

    STATIC_REQUIRE(F == -91);
}
