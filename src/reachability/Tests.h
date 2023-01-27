#ifndef SRC_REACHABILITY_TESTS_H_
#define SRC_REACHABILITY_TESTS_H_

#include <stdexcept>
#include <gtest/gtest.h>
#include "Reachability.h"

using namespace ClassProject;

struct ReachabilityTest : testing::Test {

    std::unique_ptr<ClassProject::ReachabilityInterface> fsm2 = std::make_unique<ClassProject::Reachability>(2);

    std::vector<BDD_ID> stateVars2 = fsm2->getStates();
    std::vector<BDD_ID> transitionFunctions;

};

TEST_F(ReachabilityTest, HowTo_Example) { /* NOLINT */
    BDD_ID s0 = stateVars2.at(0);
    BDD_ID s1 = stateVars2.at(1);

    transitionFunctions.push_back(fsm2->neg(s0)); // s0' = not(s0)
    transitionFunctions.push_back(fsm2->neg(s1)); // s1' = not(s1)
    fsm2->setTransitionFunctions(transitionFunctions);

    fsm2->setInitState({false,false});

    EXPECT_TRUE(fsm2->isReachable({false, false}));
    EXPECT_FALSE(fsm2->isReachable({false, true}));
    EXPECT_FALSE(fsm2->isReachable({true, false}));
    EXPECT_TRUE(fsm2->isReachable({true, true}));

    // Now re-assign the initial states and test again
    fsm2->setInitState({true,false});

    EXPECT_FALSE(fsm2->isReachable({false, false}));
    EXPECT_TRUE(fsm2->isReachable({false, true}));
    EXPECT_TRUE(fsm2->isReachable({true, false}));
    EXPECT_FALSE(fsm2->isReachable({true, true}));
}

TEST_F(ReachabilityTest, GetStatesTest) {
    EXPECT_EQ(stateVars2.size(), 2);
    BDD_ID s0 = stateVars2.at(0);
    BDD_ID s1 = stateVars2.at(1);
    EXPECT_TRUE(fsm2->isVariable(s0));
    EXPECT_TRUE(fsm2->isVariable(s1));
}

TEST_F(ReachabilityTest, MoreComplexExample) {
    BDD_ID s0 = stateVars2.at(0);
    BDD_ID s1 = stateVars2.at(1);

    transitionFunctions.push_back(fsm2->True());
    transitionFunctions.push_back(fsm2->or2(fsm2->and2(s0, fsm2->neg(s1)), s1));

    fsm2->setTransitionFunctions(transitionFunctions);
    fsm2->setInitState({false, false});  // 00

    // Reachable states
    EXPECT_TRUE(fsm2->isReachable({false, false}));
    EXPECT_TRUE(fsm2->isReachable({true, false}));
    EXPECT_TRUE(fsm2->isReachable({true, true}));
    EXPECT_FALSE(fsm2->isReachable({false, true}));

    // Change initial state
    fsm2->setInitState({true, true});  // 11

    EXPECT_FALSE(fsm2->isReachable({false, false}));
    EXPECT_FALSE(fsm2->isReachable({true, false}));
    EXPECT_FALSE(fsm2->isReachable({false, true}));
    EXPECT_TRUE(fsm2->isReachable({true, true}));

    // Change initial state
    fsm2->setInitState({false, true});  // 10

    EXPECT_FALSE(fsm2->isReachable({false, false}));
    EXPECT_FALSE(fsm2->isReachable({true, false}));
    EXPECT_TRUE(fsm2->isReachable({false, true}));
    EXPECT_TRUE(fsm2->isReachable({true, true}));

    // Change initial state
    fsm2->setInitState({true, false});  // 01

    EXPECT_FALSE(fsm2->isReachable({false, false}));
    EXPECT_TRUE(fsm2->isReachable({true, false}));
    EXPECT_FALSE(fsm2->isReachable({false, true}));
    EXPECT_TRUE(fsm2->isReachable({true, true}));
}

TEST_F(ReachabilityTest, IsReachableException) {
    EXPECT_THROW(fsm2->isReachable({true}), std::runtime_error);
    EXPECT_THROW(fsm2->isReachable({true, true, true}), std::runtime_error);
    EXPECT_THROW(fsm2->isReachable({}), std::runtime_error);
}

TEST_F(ReachabilityTest, SetTransitionException) {
    BDD_ID s0 = stateVars2.at(0);
    BDD_ID s1 = stateVars2.at(1);

    // More than
    transitionFunctions.push_back(fsm2->True());
    transitionFunctions.push_back(fsm2->or2(fsm2->and2(s0, fsm2->neg(s1)), s1));
    transitionFunctions.push_back(fsm2->and2(s0, s1));  // extra
    EXPECT_THROW(fsm2->setTransitionFunctions(transitionFunctions), std::runtime_error);

    // Less than
    transitionFunctions.clear();
    transitionFunctions.push_back(fsm2->or2(fsm2->and2(s0, fsm2->neg(s1)), s1));
    EXPECT_THROW(fsm2->setTransitionFunctions(transitionFunctions), std::runtime_error);

    // Invalid ID
    transitionFunctions.clear();
    transitionFunctions.push_back(fsm2->or2(fsm2->and2(s0, fsm2->neg(s1)), s1));
    transitionFunctions.push_back(static_cast<BDD_ID>(27));
    EXPECT_THROW(fsm2->setTransitionFunctions(transitionFunctions), std::runtime_error);
}

TEST_F(ReachabilityTest, TestInitStateSizeException) {
    EXPECT_THROW(fsm2->setInitState({}), std::runtime_error);
    EXPECT_THROW(fsm2->setInitState({false}), std::runtime_error);
    EXPECT_THROW(fsm2->setInitState({false, false, false}), std::runtime_error);
}

TEST(ReachabilityTests, TestConstructorException) {
    EXPECT_THROW(Reachability{0}, std::runtime_error);
}


#endif  // SRC_REACHABILITY_TESTS_H_
