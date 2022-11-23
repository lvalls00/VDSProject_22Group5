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

TEST_F(ManagerTests, ite) {
    BDD_ID f = manager.createVar("f");
    BDD_ID g = manager.createVar("g");

    BDD_ID f_and_g = manager.ite(f, g, manager.False());

    // Top variable is f
    EXPECT_EQ(manager.topVar(f_and_g), f);

    // Low node is 0
    EXPECT_EQ(manager.low(f_and_g), manager.False());

    // High node is g
    EXPECT_EQ(manager.high(f_and_g), g);
}

TEST_F(ManagerTests, Neg) {
    EXPECT_EQ(manager.neg(manager.True()), manager.False());
    EXPECT_EQ(manager.neg(manager.False()), manager.True());

    BDD_ID f = manager.createVar("f");
    BDD_ID nf = manager.neg(f);

    // We have no getter methods for the low and high nodes
    // so we negate nf and check if it goes back to f
    EXPECT_EQ(f, manager.neg(nf));

    // TODO(Try to improve this)
    // EXPECT_EQ(manager.coFactorFalse(nf), manager.True());
    // EXPECT_EQ(manager.coFactorTrue(nf), manager.False());
}

TEST_F(ManagerTests, Cofactor_TerminalCases) {
    // f is constant = True and False
    EXPECT_EQ(manager.coFactorFalse(manager.True()), manager.True());
    EXPECT_EQ(manager.coFactorFalse(manager.False()), manager.False());
    EXPECT_EQ(manager.coFactorTrue(manager.True()), manager.True());
    EXPECT_EQ(manager.coFactorTrue(manager.False()), manager.False());

    // f constant, x not constant
    BDD_ID g = manager.createVar("g");
    EXPECT_EQ(manager.coFactorFalse(manager.True(), g), manager.True());
    EXPECT_EQ(manager.coFactorFalse(manager.False(), g), manager.False());
    EXPECT_EQ(manager.coFactorTrue(manager.True(), g), manager.True());
    EXPECT_EQ(manager.coFactorTrue(manager.False(), g), manager.False());

    // x is constant
    EXPECT_EQ(manager.coFactorTrue(g, manager.True()), g);
    EXPECT_EQ(manager.coFactorTrue(g, manager.False()), g);
    EXPECT_EQ(manager.coFactorFalse(g, manager.True()), g);
    EXPECT_EQ(manager.coFactorFalse(g, manager.False()), g);

    // f.top > x
    BDD_ID h = manager.createVar("h");
    EXPECT_EQ(manager.coFactorTrue(h, g), h);
    EXPECT_EQ(manager.coFactorFalse(h, g), h);

    // f.top == x
    EXPECT_EQ(manager.coFactorTrue(g), manager.True());
    EXPECT_EQ(manager.coFactorTrue(g, g), manager.True());
    EXPECT_EQ(manager.coFactorFalse(g), manager.False());
    EXPECT_EQ(manager.coFactorFalse(g, g), manager.False());

    // TODO(Improve with more complex cases)
}

TEST_F(ManagerTests, TopVariableName) {
    BDD_ID f = manager.createVar("f");
    BDD_ID g = manager.createVar("g");

    EXPECT_EQ(manager.getTopVarName(f), "f");
    EXPECT_EQ(manager.getTopVarName(g), "g");

    // TODO(Improve with more complex cases)
}

TEST_F(ManagerTests, LogicalOperations) {
    BDD_ID f = manager.createVar("f");
    BDD_ID g = manager.createVar("g");

    BDD_ID and2 = manager.and2(f, g);
    EXPECT_EQ(manager.low(and2), manager.False());
    EXPECT_EQ(manager.high(and2), g);

    BDD_ID or2 = manager.or2(f, g);
    EXPECT_EQ(manager.low(or2), g);
    EXPECT_EQ(manager.high(or2), manager.True());

    BDD_ID xor2 = manager.xor2(f, g);
    EXPECT_EQ(manager.low(xor2), g);
    EXPECT_EQ(manager.high(xor2), manager.neg(g));

    BDD_ID nand2 = manager.nand2(f, g);
    EXPECT_EQ(manager.low(nand2), manager.True());
    EXPECT_EQ(manager.high(nand2), manager.neg(g));

    BDD_ID nor2 = manager.nor2(f, g);
    EXPECT_EQ(manager.low(nor2), manager.neg(g));
    EXPECT_EQ(manager.high(nor2), manager.False());

    BDD_ID xnor2 = manager.xnor2(f, g);
    EXPECT_EQ(manager.low(xnor2), manager.neg(g));
    EXPECT_EQ(manager.high(xnor2), g);

    // TODO(Improve with more complex cases)
}

TEST_F(ManagerTests, FindNodes) {
    BDD_ID f = manager.createVar("f");
    BDD_ID g = manager.createVar("g");
    manager.createVar("z");  // this will be in the unique table but not in the set

    BDD_ID and2 = manager.and2(f, g);

    std::set<BDD_ID> expected_set{g, and2, manager.True(), manager.False()};
    std::set<BDD_ID> actual_set;
    manager.findNodes(and2, actual_set);
    EXPECT_EQ(actual_set, expected_set);
}

TEST_F(ManagerTests, FindVars) {
    BDD_ID f = manager.createVar("f");
    BDD_ID g = manager.createVar("g");
    manager.createVar("z");  // this will be in the unique table but not in the set

    BDD_ID and2 = manager.and2(f, g);

    std::set<BDD_ID> expected_set{f, g};
    std::set<BDD_ID> actual_set;
    manager.findVars(and2, actual_set);
    EXPECT_EQ(actual_set, expected_set);
}

TEST_F(ManagerTests, FullExample_Part1) {
    BDD_ID a = manager.createVar("a");
    BDD_ID b = manager.createVar("b");
    BDD_ID c = manager.createVar("c");
    BDD_ID d = manager.createVar("d");

    BDD_ID a_or_b = manager.or2(a, b);
    BDD_ID c_and_d = manager.and2(c, d);
    BDD_ID result = manager.and2(a_or_b, c_and_d);

    EXPECT_EQ(result, 9);
    EXPECT_EQ(manager.high(result), 7);
    EXPECT_EQ(manager.low(result), 8);
    EXPECT_EQ(manager.topVar(result), 2);

    std::cout << manager.GetLabel(result) << std::endl;
}

#endif  // SRC_TEST_TESTS_H_
