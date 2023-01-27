#ifndef SRC_REACHABILITY_REACHABILITY_H_
#define SRC_REACHABILITY_REACHABILITY_H_

#include "ReachabilityInterface.h"

#include <vector>

namespace ClassProject {

class Reachability : public ReachabilityInterface {
 public:
  explicit Reachability(unsigned int stateSize);

  const std::vector<BDD_ID> &getStates() const override;
  bool isReachable(const std::vector<bool> &stateVector) override;
  void setTransitionFunctions(
      const std::vector<BDD_ID> &transitionFunctions) override;
  void setInitState(const std::vector<bool> &stateVector) override;

private:
  void CalculateCR();

  std::vector<BDD_ID> state_variables_;
  std::vector<BDD_ID> next_state_variables_;
  std::vector<BDD_ID> transition_functions_;
  std::vector<bool> initial_state_;
  bool needs_update_;
  BDD_ID current_CR_;
};

}  // namespace ClassProject
#endif  // SRC_REACHABILITY_REACHABILITY_H_
