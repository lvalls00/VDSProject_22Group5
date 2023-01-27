#include "Reachability.h"

namespace ClassProject {
Reachability::Reachability(unsigned int stateSize)
    : ReachabilityInterface(stateSize) {
        if (stateSize == 0) {
            throw std::runtime_error("Number of state variables cannot be zero");
        }

        // Create the state variables
        for (unsigned int i = 0; i < stateSize; i++) {
            BDD_ID id = createVar("s" + i);
            state_variables_.push_back(id);
            transition_functions_.push_back(id);
            initial_state_.push_back(false);
        }
    }

bool Reachability::isReachable(const std::vector<bool> &stateVector) {
    return false;
}

const std::vector<BDD_ID> &Reachability::getStates() const {
    return state_variables_;
}

void Reachability::setTransitionFunctions(
    const std::vector<BDD_ID> &transitionFunctions) {
        if (transitionFunctions.size() != state_variables_.size()) {
            throw std::runtime_error("Size of transition function vector doesn't match number of state variables");
        }

        size_t max_id = uniqueTableSize() - 1;
        for (auto id : transitionFunctions) {
            if (id > max_id) {
                throw std::runtime_error("Invalid BDD_ID");
            }
        }

  transition_functions_ = transitionFunctions;
}

void Reachability::setInitState(const std::vector<bool> &stateVector) {
    if (stateVector.size() != state_variables_.size()) {
        throw std::runtime_error("Size of initial state vector is different from number of state variables");
    }

    initial_state_ = stateVector;
}

}  // namespace ClassProject
