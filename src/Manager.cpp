#include "Manager.h"

#include <algorithm>
#include <utility>

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
    }

    // Check the computed table
    IntTriplet node_hash{i, t, e};
    auto result = computed_table_.find(node_hash);
    if (result != computed_table_.end()) {  // it already exists
        return (*result).second;
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
        computed_table_[node_hash] = result_high;
        return result_high;
    }

    BDD_ID existing_node_id = kFalseId;
    bool existing_node = FindMatchingNode(result_high, result_low, topvar, &existing_node_id);
    if (existing_node) {
        computed_table_[node_hash] = existing_node;
        return existing_node_id;
    }

    Node new_node;
    new_node.high_id = result_high;
    new_node.low_id = result_low;
    new_node.id = nodes_.size();
    new_node.top_var_id = topvar;
    new_node.label = ":p";
    nodes_.push_back(new_node);

    computed_table_[node_hash] = new_node.id;

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
    BDD_ID result = ite(a, False(), True());
    nodes_[a].label = "not(" + nodes_[a].label + ")";
    return result;
}

void Manager::UpdateLabel(BDD_ID node, const std::string &prefix, BDD_ID a, BDD_ID b) {
    nodes_[node].label = prefix + "(" + nodes_[a].label + ", " + nodes_[b].label + ")";
}

BDD_ID Manager::and2(BDD_ID a, BDD_ID b) {
    BDD_ID result = ite(a, b, False());
    UpdateLabel(result, "and2", a, b);
    return result;
}

BDD_ID Manager::or2(BDD_ID a, BDD_ID b) {
    BDD_ID result = ite(a, True(), b);
    UpdateLabel(result, "or2", a, b);
    return result;
}

BDD_ID Manager::xor2(BDD_ID a, BDD_ID b) {
    BDD_ID result = ite(a, neg(b), b);
    UpdateLabel(result, "xor2", a, b);
    return result;
}

BDD_ID Manager::nand2(BDD_ID a, BDD_ID b) {
    BDD_ID result = ite(neg(a), True(), neg(b));
    UpdateLabel(result, "nand2", a, b);
    return result;
}

BDD_ID Manager::nor2(BDD_ID a, BDD_ID b) {
    BDD_ID result = ite(neg(a), neg(b), False());
    UpdateLabel(result, "nor2", a, b);
    return result;
}

BDD_ID Manager::xnor2(BDD_ID a, BDD_ID b) {
    BDD_ID result = ite(a, b, neg(b));
    UpdateLabel(result, "xnor2", a, b);
    return result;
}

std::string Manager::getTopVarName(const BDD_ID &root) {
    return nodes_[topVar(root)].label;
}

void Manager::findNodes(const BDD_ID &root, std::set<BDD_ID> &nodes_of_root) {
    nodes_of_root.insert(root);

    if (isConstant(root)) {
        return;
    } else {
        findNodes(nodes_[root].low_id, nodes_of_root);
        findNodes(nodes_[root].high_id, nodes_of_root);
    }
}

void Manager::findVars(const BDD_ID &root, std::set<BDD_ID> &vars_of_root) {
    if (isConstant(root)) {
        return;
    } else {
        vars_of_root.insert(nodes_[root].top_var_id);
        findVars(nodes_[root].low_id, vars_of_root);
        findVars(nodes_[root].high_id, vars_of_root);
    }
}

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

std::string Manager::getLabel(BDD_ID x) {
    return nodes_[x].label;
}

BDD_ID Manager::GetMinimumTopVariable(BDD_ID x, BDD_ID y, BDD_ID z) {
    std::set<BDD_ID> ordered_set{x, y, z};
    for (const BDD_ID id : ordered_set) {
        if (id > kTrueId) return id;
    }

    return kFalseId;
}

}  // namespace ClassProject
