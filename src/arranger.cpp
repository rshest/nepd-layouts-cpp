#include <cassert>
#include <unordered_set>
#include <algorithm>

#include "arranger.hpp"

namespace nepd {

arranger::arranger(idx_t N) : N_(N), N2_(N*N) {
    build_dist_table();
}

bool arranger::is_valid_layout(const layout& lt) const {
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


void arranger::find_conforming_layouts(std::vector<layout>& res) const {
    layout cur_layout;
    cur_layout.N = N_;
        
    auto& cf = cur_layout.filled_idx;
    cf.resize(N_);

    typedef std::vector<char> cvec;
    std::vector<cvec> mask_stack(N_ + 1, cvec(N2_, 0));
    std::vector<idx_t> distances;
    distances.reserve(N_*(N_ - 1)/2);

    idx_t depth = 0;
    idx_t idx = 0;
    const idx_t NEnd = N_/2 + 1;
    while (true) {
        if (idx == N2_) {   
            //  dead end, invalid placement, go level up
            if (depth < N_) {
                distances.erase(distances.end() - (depth - 1), distances.end());
            }
            depth--;
            idx = cf[depth];
            if (depth == 0 && idx >= NEnd) {
                //  exhausted all the possibilities
                break;
            }
        } else {
            //  go level down
            cf[depth] = idx;
            cvec& prev_mask = mask_stack[depth];
            prev_mask[idx] = 1;
                
            if (depth < N_ - 1) {
                cvec& mask = mask_stack[depth + 1];
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
            const cvec& mask = mask_stack[depth];
            while (idx < N2_ && mask[idx]) idx++;
        }
    }
}

void arranger::build_dist_table() {
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

}  // namespace nepd