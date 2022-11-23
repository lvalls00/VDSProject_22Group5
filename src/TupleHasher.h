#ifndef SRC_TUPLEHASHER_H_
#define SRC_TUPLEHASHER_H_

#include <tuple>
#include <utility>

#include "ManagerInterface.h"

namespace {
inline void hash_combine(std::size_t &seed, const ClassProject::BDD_ID &v) {
  std::hash<ClassProject::BDD_ID> hasher;
  seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}
}  // namespace

namespace ClassProject {
using IntTriplet = std::tuple<ClassProject::BDD_ID, ClassProject::BDD_ID,
                              ClassProject::BDD_ID>;
}  // namespace ClassProject

namespace std {
template<>
struct hash<ClassProject::IntTriplet> {
  inline size_t operator()(const ClassProject::IntTriplet &v) const {
    size_t seed = 0;
    hash_combine(seed, std::get<0>(v));
    hash_combine(seed, std::get<1>(v));
    hash_combine(seed, std::get<2>(v));
    return seed;
  }
};
}  // namespace std

#endif  // SRC_TUPLEHASHER_H_
