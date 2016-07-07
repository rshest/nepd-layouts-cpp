#ifndef __ARRANGER__
#define __ARRANGER__

#include <cassert>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <vector>

#include "layout.hpp"

namespace nepd {

//  "Non Equal Pairwise Distance" grid arranger
//  Finds all the possible ways to place N points into NxN grid cells, so that
//  every pairwise distance between two points is unique
class arranger {
public:

    arranger(idx_t N) : N_(N), N2_(N*N) {
        build_dist_table();
    }

    //  returns square of the distance between two cell indices
    inline idx_t dist2(idx_t cellA, idx_t cellB) const {
        assert(cellA >= 0 && cellB >= 0 && cellA < N2_ && cellB < N2_);
        idx_t dx = (cellA % N_) - (cellB % N_);
        idx_t dy = (cellA / N_) - (cellB / N_);
        return dx*dx + dy*dy;
    }

    //  masks out all the cells with given distance from the given one 
    //  (only cells with higher index than the from_cell are masked out)
    inline void mask_with_distance(idx_t from_cell, idx_t dist2, std::vector<char>& mask) const {
        assert(mask.size() == N2_);
        const auto& de = dist_table_[from_cell];
        auto range = de.dist_to_cell.equal_range(dist2);
        for (auto it = range.first; it != range.second; ++it) {
            mask[it->second] = 1;
        }
    }


    //  masks out all the cells with the same distance from cell1 and cell2
    //  (only cells with higher index than max of the two that are masked out)
    inline void mask_equidist(idx_t cell1, idx_t cell2, std::vector<char>& mask) const {
        assert(mask.size() == N2_);
        const auto& de1 = dist_table_[cell1];
        for (idx_t k: de1.equidist_table[cell2]) mask[k] = 1;
    }

    //  returns true if layout conforms to the "no equal pairwise distance" invariant
    inline bool is_valid_layout(const layout& lt) const {
        if (lt.N != N_) return false;
        std::unordered_set<idx_t> distances;
        for (idx_t i = 0; i < N_; i++) {
            for (idx_t j = i + 1; j < N_; j++) {
                idx_t d2 = dist2(lt.filled_idx[i], lt.filled_idx[j]);
                if (distances.find(d2) != distances.end()) return false;
                distances.insert(d2);
            }
        }
        return true;
    }

    //  finds all the layouts of size N that conform 
    //  to the "Non Equal Pairwise Distance" invariant (not accounting rotated/mirrored ones)
    void find_conforming_layouts(std::vector<layout>& res) const {
        layout cur_layout;
        cur_layout.N = N_;
        
        auto& cf = cur_layout.filled_idx;
        cf.resize(N_);

        typedef std::vector<char> cvec;
        std::vector<cvec> mask_stack(N_, cvec(N2_, 0));
        std::vector<idx_t> distances;
        distances.reserve(N_*(N_ - 1)/2);

        idx_t depth = 0;
        idx_t idx = 0;
        const idx_t NEnd = N2_/2 + 1;
        while (idx < NEnd || depth > 1) {
            if (idx == N2_) {
                //  dead end, invalid placement, go level up
                if (depth < N_) {
                    distances.erase(distances.end() - (depth - 1), distances.end());
                }
                depth--;
                idx = cf[depth - 1] + 1;
            } else {
                //  go level down
                cf[depth] = idx;
                cvec& mask = mask_stack[depth];
                if (depth > 0) {
                    //  update the mask
                    cvec& prev_mask = mask_stack[depth - 1];
                    prev_mask[idx] = 1;
                
                    if (depth < N_ - 1) {
                        std::copy(prev_mask.begin(), prev_mask.end(), mask.begin());

                        //  mask out all the now-impossible cells
                        for (idx_t d2 : distances) {
                            mask_with_distance(idx, d2, mask);
                        }
                        for (idx_t i = 0; i < depth; i++) {
                            const idx_t d2 = dist2(cf[i], idx);
                            distances.push_back(d2);
                            for (idx_t j = 0; j <= depth; j++) {
                                mask_with_distance(cf[j], d2, mask);
                            }
                        }
                        for (idx_t i = 0; i < depth; i++) {
                            mask_equidist(cf[i], idx, mask);
                        }
                    }
                }
                depth++;
            }

            if (depth == N_) {
                //  all of the cells have been placed
                if (idx < N2_ &&
                    std::find(res.begin(), res.end(), cur_layout) == res.end()) 
                {
                    //  found a new layout
                    assert(is_valid_layout(cur_layout));
                    res.push_back(cur_layout);
                }
                idx = N2_;
            } else {
                //  find the next cell that does not violate the invariant
                idx++;
                const cvec& mask = mask_stack[depth - 1];
                while (idx < N2_ && mask[idx]) idx++;
            }
        }
    }

private:
    struct dist_table_entry {
        std::unordered_multimap<idx_t, idx_t> dist_to_cell;
        std::vector<std::vector<idx_t>> equidist_table;
    };

    const idx_t N_, N2_;
    std::vector<dist_table_entry> dist_table_;

    void build_dist_table() {
        dist_table_.resize(N2_);
        for (idx_t i = 0; i < N2_; i++) {
            dist_table_entry& de = dist_table_[i];
            de.equidist_table.resize(N2_);
            
            for (idx_t j = i + 1; j < N2_; j++) {
                idx_t d2 = dist2(i, j);
                de.dist_to_cell.insert({d2, j});

                for (idx_t k = j + 1; k < N2_; k++) {
                    idx_t d2i = dist2(k, i);
                    idx_t d2j = dist2(k, j);
                    if (d2i == d2j) de.equidist_table[j].push_back(k);
                }
            }
        }
    }
};

}  // namespace nepd


#endif