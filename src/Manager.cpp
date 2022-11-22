#include "Manager.h"

#include <algorithm>
#include <set>

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

    BDD_ID topvar_i = topVar(i);
    BDD_ID topvar_t = topVar(t);
    BDD_ID topvar_e = topVar(e);

    // The smallest index larger than kTrueId is the top variable
    BDD_ID topvar = GetMinimumTopVariable(topvar_i, topvar_t, topvar_e);

    BDD_ID cofactor_true_i = coFactorTrue(i, topvar);
    BDD_ID cofactor_true_t = coFactorTrue(t, topvar);
    BDD_ID cofactor_true_e = coFactorTrue(e, topvar);

    BDD_ID cofactor_false_i = coFactorFalse(i, topvar);
    BDD_ID cofactor_false_t = coFactorFalse(t, topvar);
    BDD_ID cofactor_false_e = coFactorFalse(e, topvar);

    BDD_ID result_high = ite(cofactor_true_i, cofactor_true_t, cofactor_true_e);
    BDD_ID result_low = ite(cofactor_false_i, cofactor_false_t, cofactor_false_e);

    if (result_high == result_low) {
        return result_high;
    }

    BDD_ID existing_node_id = kFalseId;
    bool existing_node = FindMatchingNode(result_high, result_low, topvar, &existing_node_id);
    if (existing_node) {
        return existing_node_id;
    }

    Node new_node;
    new_node.high_id = result_high;
    new_node.low_id = result_low;
    new_node.id = nodes_.size();
    new_node.top_var_id = topvar;
    new_node.label = ":p";
    nodes_.push_back(new_node);

    // TODO(Implement computed table and labels)

    return new_node.id;
}

BDD_ID Manager::coFactorTrue(BDD_ID f, BDD_ID x) {
    // f is constant
    // x is constant
    // f.top > x
    if (isConstant(f) || isConstant(x) || nodes_[f].top_var_id > x) {
        return f;
    } else if (nodes_[f].top_var_id == x) {
        // f.top == x
        return nodes_[f].high_id;
    }

    BDD_ID T = coFactorTrue(nodes_[f].high_id, x);
    BDD_ID F = coFactorTrue(nodes_[f].low_id, x);
    return ite(nodes_[f].top_var_id, T, F);
}

BDD_ID Manager::coFactorFalse(BDD_ID f, BDD_ID x) {
    // f is constant
    // x is constant
    // f.top > x
    if (isConstant(f) || isConstant(x) || nodes_[f].top_var_id > x) {
        return f;
    } else if (nodes_[f].top_var_id == x) {
        // f.top == x
        return nodes_[f].low_id;
    }

    BDD_ID T = coFactorFalse(nodes_[f].high_id, x);
    BDD_ID F = coFactorFalse(nodes_[f].low_id, x);
    return ite(nodes_[f].top_var_id, T, F);
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
    return nodes_[topVar(root)].label;
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

BDD_ID Manager::high(BDD_ID x) {
    return nodes_[x].high_id;
}

BDD_ID Manager::low(BDD_ID x) {
    return nodes_[x].low_id;
}

BDD_ID Manager::GetMinimumTopVariable(BDD_ID x, BDD_ID y, BDD_ID z) {
    std::set<BDD_ID> ordered_set{x, y, z};
    for (const BDD_ID id : ordered_set) {
        if (id > kTrueId) return id;
    }

    return kFalseId;
}

}  // namespace ClassProject
