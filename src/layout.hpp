#ifndef __NEPD_LAYOUT__
#define __NEPD_LAYOUT__

#include <iostream>
#include <vector>

namespace nepd {

typedef int16_t idx_t;

struct layout {
    idx_t N;
    std::vector<idx_t> filled_idx;

    //  compares two layouts, considering rotated/mirrored ones equal
    bool operator ==(const layout& rhs) const;

    //  pretty prints the layout (either whole or a row) to a stream 
    void print(std::ostream& os, idx_t row = -1) const;
};

}  // namespace nepd


#endif