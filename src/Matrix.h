#ifndef __MATRIX_H
#define __MATRIX_H
#include "common.h"
#define DIMENSION 10

struct pair_key_hash {
    template <class T1, class T2>
    std::size_t operator()(const std::pair<T1, T2>& key) const {
        auto h1 = std::hash<T1>{}(key.first);
        auto h2 = std::hash<T2>{}(key.second);
        return h1 ^ h2;
    }
};

struct pair_key_cond {
    template <class T1, class T2>
    bool operator()(const std::pair<T1, T2>& lhs, const std::pair<T1, T2>& rhs) const {
        return lhs.first == rhs.first && lhs.second == rhs.second;
    }
};

template <typename T, T Default>
class Matrix {
private:
    std::unordered_map<std::pair<size_t, size_t>, T, pair_key_hash, pair_key_cond> _arr;
    T _default = Default;
    std::vector<std::tuple<size_t, size_t, T>> filled;
    size_t readPos = 0;
    class Row;
public:

    Row operator [] (int r){
        return {*this, r};
    }

    T readData(size_t rowIndex, size_t colIndex) {
        auto result = _arr.find({rowIndex, colIndex});
        if (result != _arr.end()) {
            return result->second;
        }
        return _default;
    }

    void writeData(size_t rowIndex, size_t colIndex, T& item) {
        if (item == _default) {
            T v = readData(rowIndex, colIndex);
            if (v != _default) {
                _arr.erase({rowIndex, colIndex});
                for(auto iter = filled.begin(); iter != filled.end();) {
                    auto& [r,c,v] = *iter;
                    if (r == rowIndex && c == colIndex) {
                        filled.erase(iter);
                        continue;
                    }
                    ++iter;
                }
            }
        }
        auto result = _arr.insert(std::pair<std::pair<size_t, size_t>, T> {{rowIndex, colIndex}, item});
        if (result.second) {
            filled.push_back({rowIndex, colIndex, item});
        }
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
