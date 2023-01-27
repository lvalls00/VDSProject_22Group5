#include "Reachability.h"

namespace ClassProject {
Reachability::Reachability(unsigned int stateSize)
    : ReachabilityInterface(stateSize), needs_update_(true), current_CR_(False()) {
        if (stateSize == 0) {
            throw std::runtime_error("Number of state variables cannot be zero");
        }

        // Create the state variables
        for (unsigned int i = 0; i < stateSize; i++) {
            BDD_ID id = createVar("s" + i);
            state_variables_.push_back(id);
            transition_functions_.push_back(id);
            initial_state_.push_back(false);

            BDD_ID next_state = createVar("s'" + i);
            next_state_variables_.push_back(next_state);
        }
    }

bool Reachability::isReachable(const std::vector<bool> &stateVector) {
    if (stateVector.size() != state_variables_.size()) {
        throw std::runtime_error("Size of state vector is different from number of state variables");
    }

    if (needs_update_) {
        CalculateCR();
    }

    size_t size = state_variables_.size();
    BDD_ID temp = current_CR_;
    for (size_t i = 0; i < size; i++) {
        temp = stateVector.at(i) ? coFactorTrue(temp) : coFactorFalse(temp);
    }

    return temp == True();
}

void Reachability::CalculateCR() {
    // Calculate the transition relation and char. func of the initial state
    BDD_ID transition = True();
    BDD_ID C_initial_state = True();

    size_t size = state_variables_.size();
    for (size_t i = 0; i < size; i++) {
        // Transition relation
        BDD_ID next_s = next_state_variables_.at(i);
        BDD_ID not_next_s = neg(next_s);

        BDD_ID delta = transition_functions_.at(i);
        BDD_ID not_delta = neg(delta);

        BDD_ID temp = and2(next_s, delta);
        BDD_ID temp2 = and2(not_next_s, not_delta);
        transition = and2(transition, or2(temp, temp2));

        // Initial state Characteristic function
        BDD_ID s = state_variables_.at(i);
        BDD_ID not_s = neg(s);

        if (initial_state_.at(i) == true) {
            C_initial_state = and2(C_initial_state, s);
        } else {
            C_initial_state = and2(C_initial_state, not_s);
        }
    }

    // Begin iteration
    BDD_ID CR_it = C_initial_state;
    BDD_ID CR = CR_it;
    do {
        CR = CR_it;
        BDD_ID temp_img = and2(CR, transition);
        BDD_ID renaming_function = True();  // this will be used to put img in terms of current states

        for (size_t i = 0; i < size; i++) {
            BDD_ID id = state_variables_.at(i);
            // Quantify out the states
            temp_img = or2(coFactorTrue(temp_img, id), coFactorFalse(temp_img, id));

            // Already calculate the xors for the renaming of variables
            renaming_function = and2(renaming_function, xnor2(id, next_state_variables_.at(i)));
        }

        // Put the image in terms of current states
        temp_img = and2(renaming_function, temp_img);

        // Quantify out the next state variables
        for (auto id : next_state_variables_) {
            temp_img = or2(coFactorTrue(temp_img, id), coFactorFalse(temp_img, id));
        }

        CR_it = or2(CR, temp_img);
    } while(CR != CR_it);

    current_CR_ = CR;
    needs_update_ = false;
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
  needs_update_ = true;
}

void Reachability::setInitState(const std::vector<bool> &stateVector) {
    if (stateVector.size() != state_variables_.size()) {
        throw std::runtime_error("Size of initial state vector is different from number of state variables");
    }

    initial_state_ = stateVector;
    needs_update_ = true;
}

}  // namespace ClassProject
