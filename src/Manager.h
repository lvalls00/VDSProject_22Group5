// A minimalistic BDD library, following Wolfgang Kunz lecture slides
//
// Created by Markus Wedler 2014

#ifndef VDSPROJECT_MANAGER_H
#define VDSPROJECT_MANAGER_H

#include "ManagerInterface.h"

#include <vector>
#include <string>

namespace ClassProject {

struct Node {
    int id;
    int high_id;
    int low_id;
    int top_var_id;
    std::string label;
};

class Manager : public ManagerInterface {
private:
    std::vector<Node> nodes_;
};




}

#endif
