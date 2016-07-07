
#include "layout.hpp"

namespace nepd {

bool layout::operator ==(const layout& rhs) const {
    if (N != rhs.N || filled_idx.size() != rhs.filled_idx.size()) return false;

    const idx_t N2 = N*N;
    std::vector<char> mask(N2, 0);
    std::vector<char> rhs_mask(N2, 0);

    for (idx_t k: filled_idx) mask[k] = 1;
    for (idx_t k: rhs.filled_idx) rhs_mask[k] = 1;

    const idx_t N1 = N - 1;
    const idx_t transforms[][6] = {
        {1, 0, 0, 0, 1, 0}, {0, -1, N1, 1, 0, 0}, {-1, 0, N1, 0, -1, N1}, {0, 1, 0, -1, 0, N1},
        {-1, 0, N1, 0, 1, 0}, {0, -1, N1, -1, 0, N1}, {1, 0, 0, 0, -1, N1}, {0, 1, 0, 1, 0, 0}};

    bool has_eq = false;
    for (const auto& tr: transforms) {
        bool trans_eq = true;
        for (idx_t i = 0; i < N2; i++) {
            idx_t x = i%N, y = i/N;
            idx_t tx = tr[0]*x + tr[1]*y + tr[2];
            idx_t ty = tr[3]*x + tr[4]*y + tr[5];
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
void layout::print(std::ostream& os, idx_t row) const {
    idx_t k = 0;
    idx_t start_row = row < 0 ? 0 : row;
    idx_t end_row = row < 0 ? N - 1 : row;
    while (k < filled_idx.size() && filled_idx[k] < start_row*N) k++;
    for (idx_t i = start_row; i <= end_row; i++) {
        for (idx_t j = 0; j < N; j++) {
            bool filled = (k < filled_idx.size() && filled_idx[k] == j + i*N);
            os << (filled ? "o" : ".");
            k += filled;
        }
        if (row < 0) os << "\n";
    }
}


}  // namespace nepd
