#ifndef __NEPD_ARRANGER__
#define __NEPD_ARRANGER__

#include <cassert>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>


//  "Non Equal Pairwise Distance" grid arranger
//  Finds all the possible ways to place N points into NxN grid cells, so that
//  every pairwise distance between two points is unique
class nepd_arranger {
public:
    struct layout {
        int N;
        std::vector<int> filled_idx;

        //  compares two layouts, considering rotated/mirrored ones equal
        inline bool operator ==(const layout& rhs) const {
            if (N != rhs.N || filled_idx.size() != rhs.filled_idx.size()) return false;

            const int N2 = N*N;
            std::vector<char> mask(N2, 0);
            std::vector<char> rhs_mask(N2, 0);

            for (int k: filled_idx) mask[k] = 1;
            for (int k: rhs.filled_idx) rhs_mask[k] = 1;

            const int N1 = N - 1;
            const int transforms[][6] = {
                {1, 0, 0, 0, 1, 0}, {0, -1, N1, 1, 0, 0}, {-1, 0, N1, 0, -1, N1}, {0, 1, 0, -1, 0, N1},
                {-1, 0, N1, 0, 1, 0}, {0, -1, N1, -1, 0, N1}, {1, 0, 0, 0, -1, N1}, {0, 1, 0, 1, 0, 0}};

            bool has_eq = false;
            for (const auto& tr: transforms) {
                bool trans_eq = true;
                for (int i = 0; i < N2; i++) {
                    int x = i%N, y = i/N;
                    int tx = tr[0]*x + tr[1]*y + tr[2];
                    int ty = tr[3]*x + tr[4]*y + tr[5];
                    if (mask[i] != rhs_mask[tx + ty*N]) {
                        trans_eq = false;
                        break;
                    }
                }
                has_eq |= trans_eq;
            }
            return has_eq;
        }

        //  pretty prints the layout (either whole or a row) to a stream 
        void print(std::ostream& os, int row = -1) const {
            int k = 0;
            int start_row = row < 0 ? 0 : row;
            int end_row = row < 0 ? N - 1 : row;
            while (k < filled_idx.size() && filled_idx[k] < start_row*N) k++;
            for (int i = start_row; i <= end_row; i++) {
                for (int j = 0; j < N; j++) {
                    bool filled = (k < filled_idx.size() && filled_idx[k] == j + i*N);
                    os << (filled ? "o" : ".");
                    k += filled;
                }
                if (row < 0) os << "\n";
            }
        }
    };

    nepd_arranger(int N) : N_(N), N2_(N*N) {
        build_dist_table();
    }

    //  returns square of the distance between two cell indices
    inline int dist2(int cellA, int cellB) const {
        assert(cellA >= 0 && cellB >= 0 && cellA < N2_ && cellB < N2_);
        int dx = (cellA % N_) - (cellB % N_);
        int dy = (cellA / N_) - (cellB / N_);
        return dx*dx + dy*dy;
    }

    //  masks out all the cells with given distance from the given one 
    //  (only cells with higher index than the from_cell are masked out)
    inline void mask_with_distance(int from_cell, int dist2, std::vector<char>& mask) const {
        assert(mask.size() == N2_);
        const auto& de = dist_table_[from_cell];
        auto range = de.dist_to_cell.equal_range(dist2);
        for (auto it = range.first; it != range.second; ++it) {
            mask[it->second] = 1;
        }
    }


    //  masks out all the cells with the same distance from cell1 and cell2
    //  (only cells with higher index than max of the two that are masked out)
    inline void mask_equidist(int cell1, int cell2, std::vector<char>& mask) const {
        assert(mask.size() == N2_);
        const auto& de1 = dist_table_[cell1];
        const auto& de2 = dist_table_[cell2];
        for (const auto& el : de1.dist_to_cell) {
            int d2 = el.first;
            auto range = de2.dist_to_cell.equal_range(d2);
            for (auto it = range.first; it != range.second; ++it) {
                mask[it->second] |= (el.second == it->second);
            }
        }
    }

    //  returns true if layout conforms to the "no equal pairwise distance" invariant
    inline bool is_valid_layout(const layout& lt) const {
        if (lt.N != N_) return false;
        std::unordered_set<int> distances;
        for (int i = 0; i < N_; i++) {
            for (int j = i + 1; j < N_; j++) {
                int d2 = dist2(lt.filled_idx[i], lt.filled_idx[j]);
                if (distances.find(d2) != distances.end()) return false;
                distances.insert(d2);
            }
        }
        return true;
    }

    //  finds all the layouts of size N that conform 
    //  to the "Non Equal Pairwise Distance" invariant (not accounting rotated/mirrored ones)
    void find_conforming_layouts(std::vector<layout>& res) {
        layout cur_layout;
        cur_layout.N = N_;
        
        auto& cf = cur_layout.filled_idx;
        cf.resize(N_);

        typedef std::vector<char> cvec;
        std::vector<cvec> mask_stack(N_, cvec(N2_, 0));

        int depth = 0;
        int idx = 0;
        while (idx < N2_ - N_ || depth > 1) {
            if (idx == N2_) {
                //  dead end, invalid placement, go level up
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
                        for (int i = 0; i <= depth; i++) {
                            for (int j = i + 1; j <= depth; j++) {
                                mask_with_distance(idx, dist2(cf[i], cf[j]), mask);
                            }

                            const int d2 = dist2(cf[i], idx);
                            for (int j = 0; j <= depth; j++) {
                                mask_with_distance(cf[j], d2, mask);
                            }
                        }
                        for (int i = 0; i < depth; i++) {
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
        std::unordered_multimap<int, int> dist_to_cell;
    };

    const int N_, N2_;
    std::vector<dist_table_entry> dist_table_;

    void build_dist_table() {
        dist_table_.resize(N2_);
        for (int i = 0; i < N2_; i++) {
            dist_table_entry& de = dist_table_[i];
            for (int j = i + 1; j < N2_; j++) {
                int d2 = dist2(i, j);
                de.dist_to_cell.insert({d2, j});
            }
        }
    }
};


#endif