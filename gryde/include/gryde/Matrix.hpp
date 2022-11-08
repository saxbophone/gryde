#ifndef COM_SAXBOPHONE_GRYDE_MATRIX_HPP
#define COM_SAXBOPHONE_GRYDE_MATRIX_HPP

#include <array>
#include <initializer_list>
#include <limits>
#include <span>
#include <stdexcept>
#include <type_traits>
#include <vector>

#include <cstddef>

namespace com::saxbophone::gryde {
// abstract base class defining the interface of a class implementing
// Matrix functionality
template <typename T>
class MatrixBase {
public:
    // vritual destructor required due to C++ language rules
    virtual constexpr ~MatrixBase() = default;
    // getters for dimensions
    virtual constexpr std::size_t row_count() const = 0;
    virtual constexpr std::size_t col_count() const = 0;
    // read-only accessor for matrix contents
    virtual constexpr std::span<const T> contents() const = 0;
    // read-write accessor for matrix contents
    virtual constexpr std::span<T> contents() = 0;
    // read-only accessor for a specific cell of the Matrix
    virtual constexpr const T& operator()(std::size_t, std::size_t) const = 0;
    // read-write accessor for a specific cell of the Matrix
    virtual constexpr T& operator()(std::size_t, std::size_t) = 0;
    // generic helper method for determining matching matrix dimensions
    constexpr static bool dimensions_match(const MatrixBase& lhs, const MatrixBase& rhs) {
        return lhs.row_count() == rhs.row_count() and lhs.col_count() == rhs.col_count();
    }
protected:
    // helper to unpack initializer_lists in ctors
    // XXX: because this method is called from a ctor, we have to avoid
    // using any virtual methods in it, hence the need to pass col_count
    // and contents explicitly
    static constexpr void _unpack_initializer_list(
        std::initializer_list<std::initializer_list<T>> l,
        std::size_t col_count,
        std::span<T> contents
    ) {
        // set contents of each row one by one (we allow shortened rows)
        std::size_t row_n = 0;
        for (auto row : l) {
            // just check row doesn't exceed max size
            if (row.size() > col_count) {
                throw std::runtime_error("Oversized row found in initializer_list");
            }
            std::size_t col_n = 0;
            for (auto col : row) {
                contents[row_n * col_count + col_n] = col;
                col_n++;
            }
            row_n++;
        }
    }
    // helper for making submatrices
    constexpr void _populate_submatrix(
        MatrixBase& submatrix,
        std::size_t row,
        std::size_t col
    ) const {
        // cursor indices for output to new matrix
        std::size_t p = 0;
        std::size_t q = 0;
        for (std::size_t m = 0; m < this->row_count(); m++) {
            for (std::size_t n = 0; n < this->col_count(); n++) {
                // skip cells from removed row/column
                if (m == row or n == col) { continue; }
                submatrix(p, q) = this->operator()(m, n);
                // advance the cursors
                q++;
                if (q == submatrix.col_count()) {
                    q = 0;
                    p++;
                }
            }
        }
    }
    // helper for element-wise addition between matrices
    static constexpr void _element_wise_addition(const MatrixBase& lhs, const MatrixBase& rhs, MatrixBase& result) {
        for (std::size_t m = 0; m < result.row_count(); m++) {
            for (std::size_t n = 0; n < result.col_count(); n++) {
                result(m, n) = lhs(m, n) + rhs(m, n);
            }
        }
        return;
    }
};

template <
    typename T,
    std::size_t M = std::numeric_limits<size_t>::max(),
    std::size_t N = std::numeric_limits<size_t>::max()
>
class Matrix : public MatrixBase<T> {
public:
    // default ctor, default-initialised all elements
    constexpr Matrix() : _contents{} {}
    // this ctor sets elements from initialiser list
    constexpr Matrix(std::initializer_list<std::initializer_list<T>> l)
      : _contents{}
      {
        // validate list dimensions
        if (l.size() != M) {
            throw std::runtime_error("Top-level initializer_list is wrong size");
        }
        // set contents of each row one by one (we allow shortened rows)
        MatrixBase<T>::_unpack_initializer_list(l, N, this->_contents);
    }
    // this ctor sets elements from dynamic-size span
    constexpr Matrix(std::span<const T> s) : _contents{} {
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
        if (not MatrixBase<T>::dimensions_match(*this, other)) {
            throw std::runtime_error("Matrix dimensions don't match");
        }
        // use compile-time-sized subspan to convert dynamic span to fixed span
        // this would be UB if we didn't check for correct size first
        auto cells = other.contents().template subspan<0, M * N>();
        for (std::size_t i = 0; i < M * N; i++) {
            _contents[i] = cells[i];
        }
    }
    // vritual destructor required due to C++ language rules
    virtual constexpr ~Matrix() = default;
    // getters for dimensions
    constexpr std::size_t row_count() const override { return M; }
    constexpr std::size_t col_count() const override { return N; }
    // equality operator
    constexpr bool operator==(const Matrix& other) const {
        return this->_contents == other._contents;
    }
    // compare with dynamic-sized Matrix
    bool operator==(const Matrix<T>& other) const {
        return (Matrix<T>)*this == other;
    }
    // read-only accessor for matrix contents
    constexpr std::span<const T> contents() const override {
        return std::span<const T>(_contents);
    }
    // read-write accessor for matrix contents
    constexpr std::span<T> contents() override {
        return std::span<T>(_contents);
    }
    // read-only accessor for a specific cell of the Matrix
    constexpr const T& operator()(std::size_t m, std::size_t n) const override {
        /*
         * avoid -Wtype-limits warning diagnostic for always-false comparison
         * when either of M or N are 0
         */
        if constexpr (M == 0 or N == 0) {
            throw std::runtime_error("Matrix is empty");
        } else {
            // validate indices
            if (m >= M or n >= N) { // compiler complains for check when zero-size
                throw std::runtime_error("Matrix[] indices out of bounds");
            }
            return _contents[m * N + n];
        }
    }
    // read-write accessor for a specific cell of the Matrix
    constexpr T& operator()(std::size_t m, std::size_t n) override {
        /*
         * avoid -Wtype-limits warning diagnostic for always-false comparison
         * when either of M or N are 0
         */
        if constexpr (M == 0 or N == 0) {
            throw std::runtime_error("Matrix is empty");
        } else {
            // validate indices
            if (m >= M or n >= N) { // compiler complains for check when zero-size
                throw std::runtime_error("Matrix[] indices out of bounds");
            }
            return _contents[m * N + n];
        }
    }
    // calculates determinant for square Matrices
    constexpr T determinant() const {
        // check that the Matrix is square at compile-time
        static_assert(M == N, "Determinant is undefined for non-square Matrix");
        // rule out special cases
        if constexpr (M == 0) {
            return T{1};
        } else if constexpr (M == 1) {
            return this->_contents[0];
        } else {
            // recursively calculate determinant
            // get the first row of values
            std::span<const T, N> top_row = this->contents().template subspan<0, N>();
            // make an array of each corresponding submatrix
            std::array<Matrix<T, M - 1, N - 1>, N> submatrices;
            for (std::size_t col = 0; col < N; col++) {
                submatrices[col] = this->submatrix(0, col);
            }
            // sum by adding each entry and subtracting each other entry
            T sum = {};
            for (std::size_t col = 0; col < N; col++) {
                if (col % 2 == 0) { // add when even
                    sum += top_row[col] * submatrices[col].determinant();
                } else { // subtract when odd
                    sum -= top_row[col] * submatrices[col].determinant();
                }
            }
            return sum;
        }
    }
    // fixed-Matrix + fixed-Matrix
    constexpr Matrix operator+(const Matrix& other) const {
        Matrix result;
        MatrixBase<T>::_element_wise_addition(*this, other, result);
        return result;
    }
    // fixed-Matrix + dynamic-Matrix
    Matrix operator+(const Matrix<T>& other) const {
        // validate dimensions
        if (not MatrixBase<T>::dimensions_match(*this, other)) {
            throw std::runtime_error("Matrix dimensions don't match");
        }
        Matrix result;
        MatrixBase<T>::_element_wise_addition(*this, other, result);
        return result;
    }
    // fixed-Matrix * scalar
    constexpr Matrix operator*(const T& scalar) const {
        // TODO: implement scalar multiplication
        return {};
    }
    // fixed-Matrix * fixed-Matrix
    template <std::size_t P>
    constexpr Matrix<T, M, P> operator*(const Matrix<T, N, P>& other) const {
        // Matrix multiplication
        Matrix<T, M, P> output;
        for (std::size_t m = 0; m < M; m++) {
            for (std::size_t p = 0; p < P; p++) {
                // every new cell (m, p) in output matrix:
                for (std::size_t n = 0; n < N; n++) {
                    output(m, p) += ((*this)(m, n) * other(n, p));
                }
            }
        }
        return output;
    }
    // fixed-Matrix * dynamic-Matrix
    Matrix<T> operator*(const Matrix<T>& other) const {
        // TODO: validate compatible dimensions at run-time? (or just relegate check to casting constructor?)
        // TODO: implement Matrix multiplication
        return {};
    }
    // fixed-Matrix transposition
    constexpr Matrix<T, N, M> transpose() const {
        Matrix<T, N, M> transposed;
        // write the rows of this as the columns of transposed
        for (std::size_t m = 0; m < M; m++) {
            for (std::size_t n = 0; n < N; n++) {
                transposed(n, m) = (*this)(m, n);
            }
        }
        return transposed;
    }
    // returns a new fixed-Matrix with the specified row and column removed
    constexpr Matrix<T, M - 1, N - 1> submatrix(std::size_t row, std::size_t col) const {
        // prevent wrap-around on underflow making huge matrices
        static_assert(M > 0 and N > 0, "No more rows or columns to remove");
        // validate row and column indices
        if (row >= M or col >= N) {
            throw std::runtime_error("Row or column index out of bounds");
        }
        // make a smaller matrix
        Matrix<T, M - 1, N - 1> sub;
        // populate it from all cells except those from the removed row and column
        this->_populate_submatrix(sub, row, col);
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
    // contents
    std::array<T, M * N> _contents;
};

// partial class template specialisation for SIZE_MAX-sized matrices, which are dynamic-sized
template <typename T>
class Matrix<
    T,
    std::numeric_limits<size_t>::max(),
    std::numeric_limits<size_t>::max()
> : public MatrixBase<T> {
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
        // validate list dimensions
        if (l.size() != _m) {
            throw std::runtime_error("Top-level initializer_list is wrong size");
        }
        // set contents of each row one by one (we allow shortened rows)
        MatrixBase<T>::_unpack_initializer_list(l, n, this->_contents);
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
    // vritual destructor required due to C++ language rules
    virtual ~Matrix() = default;
    // read-only accessor for matrix contents
    std::span<const T> contents() const override {
        return std::span<const T>(_contents);
    }
    // read-write accessor for matrix contents
    std::span<T> contents() override {
        return std::span<T>(_contents);
    }
    // getters for dimensions
    std::size_t row_count() const override { return _m; }
    std::size_t col_count() const override { return _n; }
    // equality operator
    bool operator==(const Matrix& other) const {
        // validate dimensions before doing the actual comparison
        if (this->_m != other._m or this->_n != other._n) {
            throw std::runtime_error("Matrix dimensions don't match");
        }
        // otherwise, just compare contents
        return this->_contents == other._contents;
    }
    // read-only accessor for a specific cell of the Matrix
    const T& operator()(std::size_t m, std::size_t n) const override {
        // validate indices
        if (m >= _m or n >= _n) {
            throw std::runtime_error("Matrix[] indices out of bounds");
        }
        return _contents[m * _n + n];
    }
    // read-write accessor for a specific cell of the Matrix
    T& operator()(std::size_t m, std::size_t n) override {
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
        // rule out special cases
        if (_m == 0) {
            return T{1};
        } else if (_m == 1) {
            return this->_contents[0];
        } else {
            // recursively calculate determinant
            // get the first row of values
            std::span<const T> top_row = this->contents().subspan(0, _n);
            // make a vector of each corresponding submatrix
            std::vector<Matrix<T>> submatrices(_n);
            for (std::size_t col = 0; col < _n; col++) {
                submatrices[col] = this->submatrix(0, col);
            }
            // sum by adding each entry and subtracting each other entry
            T sum = {};
            for (std::size_t col = 0; col < _n; col++) {
                if (col % 2 == 0) { // add when even
                    sum += top_row[col] * submatrices[col].determinant();
                } else { // subtract when odd
                    sum -= top_row[col] * submatrices[col].determinant();
                }
            }
            return sum;
        }
    }
    // dynamic-Matrix + dynamic-Matrix
    Matrix operator+(const Matrix& other) const {
        // validate dimensions
        if (not MatrixBase<T>::dimensions_match(*this, other)) {
            throw std::runtime_error("Matrix dimensions don't match");
        }
        Matrix result(_m, _n);
        MatrixBase<T>::_element_wise_addition(*this, other, result);
        return result;
    }
    // dynamic-Matrix + fixed-Matrix
    template <std::size_t P, std::size_t Q>
    Matrix<T, P, Q> operator+(const Matrix<T, P, Q>& other) const {
        // validate dimensions
        if (not MatrixBase<T>::dimensions_match(*this, other)) {
            throw std::runtime_error("Matrix dimensions don't match");
        }
        Matrix<T, P, Q> result;
        MatrixBase<T>::_element_wise_addition(*this, other, result);
        return result;
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
            this->_populate_submatrix(sub, row, col);
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
} // namespace com::saxbophone::gryde
#endif // include guard
