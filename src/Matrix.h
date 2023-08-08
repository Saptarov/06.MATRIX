#ifndef __MATRIX_H
#define __MATRIX_H
#include "common.h"
#define DIMENSION 10

template <typename T, T Default>
class Matrix {
private:
    T _arr[DIMENSION][DIMENSION];
    T _default = Default;
    std::vector<std::tuple<size_t, size_t, T>> filled;
    size_t readPos = 0;
    class Row;
public:
    Matrix() {
        for(int i = 0; i < DIMENSION; ++i) {
            for(int j = 0; j < DIMENSION; ++j) {
                _arr[i][j] = Default;
            }
        }
    }

    Row operator [] (int r){
        return {*this, r};
    }

    T& readData(size_t rowIndex, size_t colIndex) {
        for(auto& item : filled) {
            auto& [x, y, v] = item;
            if (x == rowIndex && y == colIndex) {
                return v;
            }
        }
        return (_arr[rowIndex][colIndex] = _default);
    }

    void writeData(size_t rowIndex, size_t colIndex, const T& item) {
        _arr[rowIndex][colIndex] = item;
        filled.push_back({rowIndex, colIndex, item});
    }

    size_t size() {
        return filled.size();
    }

    decltype(filled.begin()) begin() {
        return filled.begin();
    }

    decltype(filled.end()) end() {
        return filled.end();
    }

private:
    class Row
    {
    public:
        Row( Matrix& matrix, int rowIndex )
            : _matrix(matrix)
            , _rowIndex(rowIndex)
        {}
        std::size_t& rowIndex() {
            return _rowIndex;
        }
        Matrix& parent() {
            return _matrix;
        }
    private:
        class Col
        {
            Row&        _row;
            std::size_t _colIndex;
            std::size_t& _rowIndex;
        public:
            Col( Row& row, int colIndex )
                : _row(row)
                , _colIndex(colIndex)
                , _rowIndex(row.rowIndex())
            {}
            operator T() const {
                return _row.parent().readData(_rowIndex, _colIndex);
            }
            Col& operator = (T newValue)
            {
                if (newValue != _row.parent().readData(_rowIndex, _colIndex)) {
                    _row.parent().writeData(_rowIndex, _colIndex, newValue);
                }
                return *this ;
            }
        };
    private:
        Matrix&     _matrix ;
        std::size_t _rowIndex ;
    public:
        Col operator [] (T r){
            return {*this, r};
        }
    };
};

#endif
