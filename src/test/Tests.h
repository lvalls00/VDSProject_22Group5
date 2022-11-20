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
}

#endif  // SRC_TEST_TESTS_H_
