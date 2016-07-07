#ifndef __NEPD_ARRANGER__
#define __NEPD_ARRANGER__

#include <unordered_map>
#include <vector>
#include <cassert>

#include "layout.hpp"

namespace nepd {

//  "Non Equal Pairwise Distance" grid arranger
//  Finds all the possible ways to place N points into NxN grid cells, so that
//  every pairwise distance between two points is unique
class arranger {
public:

    arranger(idx_t N);

    //  returns square of the distance between two cell indices
    inline idx_t dist2(idx_t cellA, idx_t cellB) const;

    //  masks out all the cells with given distance from the given one 
    //  (only cells with higher index than the from_cell are masked out)
    inline void mask_with_distance(idx_t from_cell, idx_t dist2, std::vector<char>& mask) const;

    //  masks out all the cells with the same distance from cell1 and cell2
    //  (only cells with higher index than max of the two that are masked out)
    inline void mask_equidist(idx_t cell1, idx_t cell2, std::vector<char>& mask) const;

    //  returns true if layout conforms to the "no equal pairwise distance" invariant
    bool is_valid_layout(const layout& lt) const;

    //  finds all the layouts of size N that conform 
    //  to the "Non Equal Pairwise Distance" invariant (not accounting rotated/mirrored ones)
    void find_conforming_layouts(std::vector<layout>& res) const;

private:
    struct dist_table_entry {
        std::unordered_multimap<idx_t, idx_t> dist_to_cell;
        std::vector<std::vector<idx_t>> equidist_table;
    };

    const idx_t N_, N2_;
    std::vector<dist_table_entry> dist_table_;

    void build_dist_table();
};

inline idx_t arranger::dist2(idx_t cellA, idx_t cellB) const {
    assert(cellA >= 0 && cellB >= 0 && cellA < N2_ && cellB < N2_);
    idx_t dx = (cellA % N_) - (cellB % N_);
    idx_t dy = (cellA / N_) - (cellB / N_);
    return dx*dx + dy*dy;
}

inline void arranger::mask_with_distance(idx_t from_cell, idx_t dist2, std::vector<char>& mask) const {
    assert(mask.size() == N2_);
    const auto& de = dist_table_[from_cell];
    auto range = de.dist_to_cell.equal_range(dist2);
    for (auto it = range.first; it != range.second; ++it) {
        mask[it->second] = 1;
    }
}


inline void arranger::mask_equidist(idx_t cell1, idx_t cell2, std::vector<char>& mask) const {
    assert(mask.size() == N2_);
    const auto& de1 = dist_table_[cell1];
    for (idx_t k: de1.equidist_table[cell2]) mask[k] = 1;
}

}  // namespace nepd


#endif