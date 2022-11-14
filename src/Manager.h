// A minimalistic BDD library, following Wolfgang Kunz lecture slides
//
// Created by Markus Wedler 2014

#ifndef SRC_MANAGER_H_
#define SRC_MANAGER_H_

#include "ManagerInterface.h"

#include <vector>
#include <string>

namespace ClassProject {

struct Node {
    BDD_ID id;
    BDD_ID high_id;
    BDD_ID low_id;
    BDD_ID top_var_id;
    std::string label;
};

class Manager : public ManagerInterface {
 public:
  Manager();
  virtual ~Manager();

  BDD_ID createVar(const std::string &label) override;
  const BDD_ID &True() override;
  const BDD_ID &False() override;
  bool isConstant(BDD_ID f) override;
  bool isVariable(BDD_ID x) override;
  BDD_ID topVar(BDD_ID f) override;
  BDD_ID ite(BDD_ID i, BDD_ID t, BDD_ID e) override;
  BDD_ID coFactorTrue(BDD_ID f, BDD_ID x) override;
  BDD_ID coFactorFalse(BDD_ID f, BDD_ID x) override;
  BDD_ID coFactorTrue(BDD_ID f) override;
  BDD_ID coFactorFalse(BDD_ID f) override;
  BDD_ID neg(BDD_ID a) override;
  BDD_ID and2(BDD_ID a, BDD_ID b) override;
  BDD_ID or2(BDD_ID a, BDD_ID b) override;
  BDD_ID xor2(BDD_ID a, BDD_ID b) override;
  BDD_ID nand2(BDD_ID a, BDD_ID b) override;
  BDD_ID nor2(BDD_ID a, BDD_ID b) override;
  BDD_ID xnor2(BDD_ID a, BDD_ID b) override;
  std::string getTopVarName(const BDD_ID &root) override;
  void findNodes(const BDD_ID &root, std::set<BDD_ID> &nodes_of_root) override;
  void findVars(const BDD_ID &root, std::set<BDD_ID> &vars_of_root) override;
  size_t uniqueTableSize() override;

 private:
  static const BDD_ID kTrueId;
  static const BDD_ID kFalseId;

  std::vector<Node> nodes_;
};

}  // namespace ClassProject

#endif  // SRC_MANAGER_H_
