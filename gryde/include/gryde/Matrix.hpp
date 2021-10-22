#include <array>
#include <initializer_list>
#include <limits>
#include <span>
#include <stdexcept>
#include <type_traits>
#include <vector>

#include <cstddef>

template <
    typename T,
    std::size_t M = std::numeric_limits<size_t>::max(),
    std::size_t N = std::numeric_limits<size_t>::max()
>
class Matrix {
public:
    // default ctor, default-initialised all elements
    constexpr Matrix() : _m(M), _n(N) , _contents{} {}
    // this ctor sets elements from initialiser list
    constexpr Matrix(std::initializer_list<std::initializer_list<T>> l)
      : _m(M)
      , _n(N)
      , _contents{}
      {
        // XXX: much of this function is identical to its dynamic counterpart
        // TODO: Refactor out common code somehow
        // validate list dimensions
        if (l.size() != _m) {
            throw std::runtime_error("Top-level initializer_list is wrong size");
        }
        // set contents of each row one by one (we allow shortened rows)
        std::size_t row_n = 0;
        for (auto row : l) {
            // just check row doesn't exceed max size
            if (row.size() > _n) {
                throw std::runtime_error("Oversized row found in initializer_list");
            }
            std::size_t col_n = 0;
            for (auto col : row) {
                this->operator()(row_n, col_n) = col;
                col_n++;
            }
            row_n++;
        }
    }
    // this ctor sets elements from dynamic-size span
    constexpr Matrix(std::span<const T> s) : _m(M), _n(N) , _contents{} {
        // validate span size
        if (s.size() != M * N) {
            throw std::runtime_error("Span is wrong size");
        }
        for (std::size_t i = 0; i < M * N; i++) {
            _contents[i] = s[i];
        }
    }
    // this ctor initialises fixed Matrix from a dynamic Matrix
    explicit Matrix(const Matrix<T>& other)
      : Matrix()
      {
        // check dimensions of other match our dimensions
        if (other.row_count() != _m or other.col_count() != _n) {
            throw std::runtime_error("Matrix dimensions don't match");
        }
        // use compile-time-sized subspan to convert dynamic span to fixed span
        // this would be UB if we didn't check for correct size first
        auto cells = other.contents().template subspan<0, M * N>();
        for (std::size_t i = 0; i < M * N; i++) {
            _contents[i] = cells[i];
        }
    }
    // getters for dimensions
    constexpr std::size_t row_count() const { return _m; }
    constexpr std::size_t col_count() const { return _n; }
    // defaulted equality operator
    constexpr bool operator==(const Matrix&) const = default;
    // compare with dynamic-sized Matrix
    bool operator==(const Matrix<T>& other) const {
        return (Matrix<T>)*this == other;
    }
    // read-only accessor for matrix contents
    constexpr std::span<const T> contents() const {
        return std::span<const T>(_contents);
    }
    // read-write accessor for matrix contents
    constexpr std::span<T> contents() {
        return std::span<T>(_contents);
    }
    // read-only accessor for a specific cell of the Matrix
    constexpr const T& operator()(std::size_t m, std::size_t n) const {
        // validate indices
        if (m >= _m or n >= _n) {
            throw std::runtime_error("Matrix[] indices out of bounds");
        }
        return _contents[m * _n + n];
    }
    // read-write accessor for a specific cell of the Matrix
    constexpr T& operator()(std::size_t m, std::size_t n) {
        // validate indices
        if (m >= _m or n >= _n) {
            throw std::runtime_error("Matrix[] indices out of bounds");
        }
        return _contents[m * _n + n];
    }
    // calculates determinant for square Matrices
    constexpr T determinant() const {
        // check that the Matrix is square at compile-time
        static_assert(M == N, "Determinant is undefined for non-square Matrix");
        // TODO: calculate determinant properly
        return {};
    }
    // fixed-Matrix + fixed-Matrix
    constexpr Matrix operator+(const Matrix& other) const {
        // TODO: implement element-wise addition
        return {};
    }
    // fixed-Matrix + dynamic-Matrix
    Matrix operator+(const Matrix<T>& other) const {
        // TODO: validate same dimensions at run-time? (or just relegate check to casting constructor?)
        // TODO: implement element-wise addition
        return {};
    }
    // fixed-Matrix * scalar
    constexpr Matrix operator*(const T& scalar) const {
        // TODO: implement scalar multiplication
        return {};
    }
    // fixed-Matrix * fixed-Matrix
    template <std::size_t P>
    constexpr Matrix<T, M, P> operator*(const Matrix<T, N, P>& other) const {
        // TODO: implement Matrix multiplication
        return {};
    }
    // fixed-Matrix * dynamic-Matrix
    Matrix<T> operator*(const Matrix<T>& other) const {
        // TODO: validate compatible dimensions at run-time? (or just relegate check to casting constructor?)
        // TODO: implement Matrix multiplication
        return {};
    }
    // fixed-Matrix transposition
    constexpr Matrix<T, N, M> transpose() const {
        // TODO: implement transposition
        return {};
    }
    // returns a new fixed-Matrix with the specified row and column removed
    constexpr Matrix<T, M - 1, N - 1> submatrix(std::size_t row, std::size_t col) const {
        // XXX: much of this function is identical to its dynamic counterpart
        // TODO: Refactor out common code somehow
        // prevent wrap-around on underflow making huge matrices
        static_assert(M > 0 and N > 0, "No more rows or columns to remove");
        // validate row and column indices
        if (row >= M or col >= N) {
            throw std::runtime_error("Row or column index out of bounds");
        }
        // make a smaller matrix
        Matrix<T, M - 1, N - 1> sub;
        // populate it from all cells except those from the removed row and column
        // cursor indices for output to new matrix
        std::size_t p = 0;
        std::size_t q = 0;
        for (std::size_t m = 0; m < _m; m++) {
            for (std::size_t n = 0; n < _n; n++) {
                // skip cells from removed row/column
                if (m == row or n == col) { continue; }
                sub(p, q) = this->operator()(m, n);
                // advance the cursors
                q++;
                if (q == sub.col_count()) {
                    q = 0;
                    p++;
                }
            }
        }
        return sub;
    }
    // returns a new fixed-Matrix with the specified row removed
    constexpr Matrix<T, M - 1, N> remove_row(std::size_t row) const {
        // prevent wrap-around on underflow making huge matrices
        static_assert(M > 0, "No more rows to remove");
        // TODO: implement
        return {};
    }
    // returns a new fixed-Matrix with the specified column removed
    constexpr Matrix<T, M, N - 1> remove_col(std::size_t col) const {
        // prevent wrap-around on underflow making huge matrices
        static_assert(N > 0, "No more columns to remove");
        // TODO: implement
        return {};
    }
private:
    // dimensions
    std::size_t _m;
    std::size_t _n;
    // contents
    std::array<T, M * N> _contents;
};

// partial class template specialisation for SIZE_MAX-sized matrices, which are dynamic-sized
template <typename T>
class Matrix<
    T,
    std::numeric_limits<size_t>::max(),
    std::numeric_limits<size_t>::max()
> {
public:
    // default ctor, creates dynamic Matrix of zero size (empty matrix)
    Matrix() : _m(0), _n(0) , _contents() {}
    // this ctor sets Matrix size and default-initialises all elements within
    Matrix(std::size_t m, std::size_t n) : _m(m), _n(n) , _contents(m * n) {}
    // this ctor sets Matrix size and elements from initialiser list
    Matrix(
        std::size_t m,
        std::size_t n,
        std::initializer_list<std::initializer_list<T>> l
    )
      : _m(m)
      , _n(n)
      , _contents(m * n)
      {
        // XXX: much of this function is identical to its fixed counterpart
        // TODO: Refactor out common code somehow
        // validate list dimensions
        if (l.size() != _m) {
            throw std::runtime_error("Top-level initializer_list is wrong size");
        }
        // set contents of each row one by one (we allow shortened rows)
        std::size_t row_n = 0;
        for (auto row : l) {
            // just check row doesn't exceed max size
            if (row.size() > _n) {
                throw std::runtime_error("Oversized row found in initializer_list");
            }
            std::size_t col_n = 0;
            for (auto col : row) {
                this->operator()(row_n, col_n) = col;
                col_n++;
            }
            row_n++;
        }
    }
    // this ctor sets Matrix size and elements from dynamic-size span
    Matrix(std::size_t m, std::size_t n, std::span<const T> s)
      : _m(m)
      , _n(n)
      , _contents()
      {
        // validate span size
        if (s.size() != m * n) {
            throw std::runtime_error("Span is wrong size");
        }
        // set contents
        _contents = std::vector(s.begin(), s.end());
    }
    // this ctor initialises dynamic Matrix from a Fixed Matrix
    template <std::size_t P, std::size_t Q>
    explicit Matrix(const Matrix<T, P, Q>& other) : Matrix(P, Q, other.contents()) {}
    // read-only accessor for matrix contents
    std::span<const T> contents() const {
        return std::span<const T>(_contents);
    }
    // read-write accessor for matrix contents
    std::span<T> contents() {
        return std::span<T>(_contents);
    }
    // getters for dimensions
    std::size_t row_count() const { return _m; }
    std::size_t col_count() const { return _n; }
    // defaulted equality operator
    bool operator==(const Matrix&) const = default;
    // read-only accessor for a specific cell of the Matrix
    const T& operator()(std::size_t m, std::size_t n) const {
        // validate indices
        if (m >= _m or n >= _n) {
            throw std::runtime_error("Matrix[] indices out of bounds");
        }
        return _contents[m * _n + n];
    }
    // read-write accessor for a specific cell of the Matrix
    T& operator()(std::size_t m, std::size_t n) {
        // validate indices
        if (m >= _m or n >= _n) {
            throw std::runtime_error("Matrix[] indices out of bounds");
        }
        return _contents[m * _n + n];
    }
    // calculates determinant for square Matrices
    T determinant() const {
        // do check for square Matrix at run-time
        if (_m != _n) {
            throw std::runtime_error("Determinant is undefined for non-square Matrix");
        }
        // TODO: calculate determinant properly
        return {};
    }
    // dynamic-Matrix + dynamic-Matrix
    Matrix operator+(const Matrix& other) const {
        // TODO: validate same dimensions at run-time
        // TODO: implement element-wise addition
        return {};
    }
    // dynamic-Matrix + fixed-Matrix
    template <std::size_t P, std::size_t Q>
    Matrix<T, P, Q> operator+(const Matrix<T, P, Q>& other) const {
        // TODO: validate same dimensions at run-time? (or just relegate check to casting constructor?)
        // TODO: implement element-wise addition
        return {};
    }
    // dynamic-Matrix * scalar
    Matrix operator*(const T& scalar) const {
        // TODO: implement scalar multiplication
        return {};
    }
    // dynamic-Matrix * dynamic-Matrix
    Matrix operator*(const Matrix& other) const {
        // TODO: validate compatible dimensions at run-time? (or just relegate check to casting constructor?)
        // TODO: implement Matrix multiplication
        return {};
    }
    // dynamic-Matrix * fixed-Matrix
    template <std::size_t P, std::size_t Q>
    Matrix operator*(const Matrix<T, P, Q>& other) const {
        // TODO: validate compatible dimensions at run-time? (or just relegate check to casting constructor?)
        // TODO: implement Matrix multiplication
        return {};
    }
    // dynamic-Matrix transposition
    Matrix transpose() const {
        // TODO: implement transposition
        return {};
    }
    // returns a new dynamic-Matrix with the specified row and column removed
    Matrix submatrix(std::size_t row, std::size_t col) const {
        // XXX: much of this function is identical to its fixed counterpart
        // TODO: Refactor out common code somehow
        // prevent wrap-around on underflow making huge matrices
        if (_m < 1 or _n < 1) {
            throw std::runtime_error("No more rows or columns to remove");
        }
        // validate row and column indices
        if (row >= _m or col >= _n) {
            throw std::runtime_error("Row or column index out of bounds");
        }

        // make a smaller matrix
        Matrix sub(_m - 1, _n - 1);
        // populate it from all cells except those from the removed row and column
        // cursor indices for output to new matrix
        std::size_t p = 0;
        std::size_t q = 0;
        for (std::size_t m = 0; m < _m; m++) {
            for (std::size_t n = 0; n < _n; n++) {
                // skip cells from removed row/column
                if (m == row or n == col) { continue; }
                sub(p, q) = this->operator()(m, n);
                // advance the cursors
                q++;
                if (q == sub.col_count()) {
                    q = 0;
                    p++;
                }
            }
        }
        return sub;
    }
    // returns a new dynamic-Matrix with the specified row removed
    Matrix remove_row(std::size_t row) const {
        // prevent wrap-around on underflow making huge matrices
        if (_m < 1) {
            throw std::runtime_error("No more rows to remove");
        }
        // TODO: implement
        return Matrix(_m - 1, _n); // reduce size
    }
    // returns a new dynamic-Matrix with the specified column removed
    Matrix remove_col(std::size_t col) const {
        // prevent wrap-around on underflow making huge matrices
        if (_n < 1) {
            throw std::runtime_error("No more columns to remove");
        }
        // TODO: implement
        return Matrix(_m, _n - 1); // reduce size
    }
private:
    // dimensions
    std::size_t _m;
    std::size_t _n;
    // contents
    std::vector<T> _contents;
};
