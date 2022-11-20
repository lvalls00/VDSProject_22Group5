#include "Manager.h"

namespace ClassProject {

const BDD_ID Manager::kTrueId = 1;
const BDD_ID Manager::kFalseId = 0;

Manager::Manager() {
    nodes_.push_back({kFalseId, kFalseId, kFalseId, kFalseId, "False"});
    nodes_.push_back({kTrueId, kTrueId, kTrueId, kTrueId, "True"});
}

Manager::~Manager() {}

BDD_ID Manager::createVar(const std::string &label) {
    BDD_ID id = nodes_.size();
    nodes_.push_back({id, True(), False(), id, label});
    return id;
}

const BDD_ID &Manager::True() {
    return kTrueId;
}

const BDD_ID &Manager::False() {
    return kFalseId;
}

bool Manager::isConstant(BDD_ID f) {
    return f == kTrueId || f == kFalseId;
}

bool Manager::isVariable(BDD_ID x) {
    Node &node = nodes_[x];
    return node.id == x &&
           node.high_id == kTrueId &&
           node.low_id == kFalseId &&
           node.top_var_id == x;
}

BDD_ID Manager::topVar(BDD_ID f) {
    return nodes_[f].top_var_id;
}

BDD_ID Manager::ite(BDD_ID i, BDD_ID t, BDD_ID e) {
    // Terminal cases
    if (i == True()) {  // ite(1, f, g)
        return t;
    } else if (i == False()) {  // ite(0, g, f)
        return e;
    } else if (t == True() && e == False()) {  // ite(f, 1, 0)
        return i;
    } else if (t == e) {  // ite(g, f, f)
        return t;
    } else if (t == False() && e == True()) {  // ite(f, 0, 1)
        return neg(i);
    }

    // TODO(Implement the rest)

    return kFalseId;
}

BDD_ID Manager::coFactorTrue(BDD_ID f, BDD_ID x) {
    return 0;
}

BDD_ID Manager::coFactorFalse(BDD_ID f, BDD_ID x) {
    return 0;
}

BDD_ID Manager::coFactorTrue(BDD_ID f) {
    return coFactorTrue(f, nodes_[f].top_var_id);
}

BDD_ID Manager::coFactorFalse(BDD_ID f) {
    return coFactorFalse(f, nodes_[f].top_var_id);
}

BDD_ID Manager::neg(BDD_ID a) {
    // Special cases for true and false
    // Because keeping the top_var_id would not work
    if (a == kTrueId) return kFalseId;
    if (a == kFalseId) return kTrueId;

    const Node &node = nodes_[a];

    Node new_node;
    new_node.high_id = node.low_id;
    new_node.low_id = node.high_id;
    new_node.top_var_id = node.top_var_id;

    if (FindMatchingNode(new_node.high_id, new_node.low_id, new_node.top_var_id,
                         &new_node.id)) {
      return new_node.id;  // return the already existing id
    }

    // Otherwise create a new one
    new_node.id = nodes_.size();
    new_node.label = "not " + node.label;
    nodes_.push_back(new_node);

    return new_node.id;
}

BDD_ID Manager::and2(BDD_ID a, BDD_ID b) {
    return 0;
}

BDD_ID Manager::or2(BDD_ID a, BDD_ID b) {
    return 0;
}

BDD_ID Manager::xor2(BDD_ID a, BDD_ID b) {
    return 0;
}

BDD_ID Manager::nand2(BDD_ID a, BDD_ID b) {
    return 0;
}

BDD_ID Manager::nor2(BDD_ID a, BDD_ID b) {
    return 0;
}

BDD_ID Manager::xnor2(BDD_ID a, BDD_ID b) {
    return 0;
}

std::string Manager::getTopVarName(const BDD_ID &root) {
    return "";
}

void Manager::findNodes(const BDD_ID &root, std::set<BDD_ID> &nodes_of_root) {}

void Manager::findVars(const BDD_ID &root, std::set<BDD_ID> &vars_of_root) {}

size_t Manager::uniqueTableSize() {
    return nodes_.size();
}

bool Manager::FindMatchingNode(BDD_ID high, BDD_ID low, BDD_ID top_var, BDD_ID *result) {
    for (const Node &node : nodes_) {
        if (node.high_id == high && node.low_id == low && node.top_var_id == top_var) {
            *result = node.id;
            return true;
        }
    }

    return false;
}

}  // namespace ClassProject
