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
    return 0;
}

const BDD_ID &Manager::True() {
    return kTrueId;
}

const BDD_ID &Manager::False() {
    return kFalseId;
}

bool Manager::isConstant(BDD_ID f) {
    return false;
}

bool Manager::isVariable(BDD_ID x) {
    return false;
}

BDD_ID Manager::topVar(BDD_ID f) {
    return 0;
}

BDD_ID Manager::ite(BDD_ID i, BDD_ID t, BDD_ID e) {
    return 0;
}

BDD_ID Manager::coFactorTrue(BDD_ID f, BDD_ID x) {
    return 0;
}

BDD_ID Manager::coFactorFalse(BDD_ID f, BDD_ID x) {
    return 0;
}

BDD_ID Manager::coFactorTrue(BDD_ID f) {
    return 0;
}

BDD_ID Manager::coFactorFalse(BDD_ID f) {
    return 0;
}

BDD_ID Manager::neg(BDD_ID a) {
    return 0;
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
    return 0;
}

}  // namespace ClassProject
