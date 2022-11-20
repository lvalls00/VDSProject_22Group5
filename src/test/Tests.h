//
// Created by tobias on 21.11.16.
//

#ifndef SRC_TEST_TESTS_H_
#define SRC_TEST_TESTS_H_

#include <gtest/gtest.h>
#include "../Manager.h"

using ClassProject::BDD_ID;

class ManagerTests : public ::testing::Test {
 public:
  ClassProject::Manager manager;
};

TEST_F(ManagerTests, True) {
    EXPECT_EQ(manager.True(), 1);
}

TEST_F(ManagerTests, False) {
    EXPECT_EQ(manager.False(), 0);
}

TEST_F(ManagerTests, InitialTableSize) {
    EXPECT_EQ(manager.uniqueTableSize(), 2);  // only has true and false at the beggining
}

TEST_F(ManagerTests, TableSizeGrowsWithVariables) {
    manager.createVar("a");
    manager.createVar("b");
    manager.createVar("c");
    EXPECT_EQ(manager.uniqueTableSize(), 5);
}

TEST_F(ManagerTests, CreateVar) {
    // Since first 2 IDs are always true and false
    // the first variable will start from id = 2
    BDD_ID id_a = manager.createVar("a");
    BDD_ID id_b = manager.createVar("b");
    BDD_ID id_c = manager.createVar("c");

    EXPECT_EQ(id_a, 2);
    EXPECT_EQ(id_b, 3);
    EXPECT_EQ(id_c, 4);
}

TEST_F(ManagerTests, IsConstant_and_IsVariable) {
    // True and false nodes are constants
    EXPECT_TRUE(manager.isConstant(manager.True()));
    EXPECT_TRUE(manager.isConstant(manager.False()));
    EXPECT_FALSE(manager.isVariable(manager.True()));
    EXPECT_FALSE(manager.isVariable(manager.False()));

    BDD_ID id_a = manager.createVar("a");
    BDD_ID id_b = manager.createVar("b");

    EXPECT_TRUE(manager.isVariable(id_a));
    EXPECT_TRUE(manager.isVariable(id_b));
    EXPECT_FALSE(manager.isConstant(id_a));
    EXPECT_FALSE(manager.isConstant(id_b));

    // TODO(Write test for more complex functions such as f = a*b | c)
}

TEST_F(ManagerTests, TopVar) {
    BDD_ID id_a = manager.createVar("a");
    BDD_ID id_b = manager.createVar("b");
    EXPECT_EQ(id_a, manager.topVar(id_a));
    EXPECT_EQ(id_b, manager.topVar(id_b));

    // TODO(Write test for more complex cases)
}

TEST_F(ManagerTests, ite_TerminalCases) {
    BDD_ID g = manager.createVar("g");
    BDD_ID f = manager.createVar("f");

    EXPECT_EQ(f, manager.ite(manager.True(), f, g));
    EXPECT_EQ(f, manager.ite(manager.False(), g, f));
    EXPECT_EQ(f, manager.ite(f, manager.True(), manager.False()));
    EXPECT_EQ(f, manager.ite(g, f, f));
    EXPECT_EQ(manager.neg(f), manager.ite(f, manager.False(), manager.True()));
}

#endif  // SRC_TEST_TESTS_H_
