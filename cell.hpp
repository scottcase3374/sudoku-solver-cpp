//
// Created by scott on 11/18/25.
//

#ifndef CELL_HPP
#define CELL_HPP

#include <tuple>
#include <set>
using namespace std;

#include "status.hpp"
class Coordinator;

class Cell
{
public:
    Cell(Coordinator &coordinator);

    void setAcceptedVal(int val);
    tuple<int, Status> peekProposal();
    void setProposedVal(int val);
    Status getStatus();
    Status reject();
    bool isUnset();
    void removeUsedVal(int val);
    set<int> &getValidVals();

private:
    static constexpr int DEFAULT_VAL = 0;

    /*
     * Cell content; The initial cell state - 0 as the value and "unknown" as status.
     */
    tuple<int, Status> content = make_tuple(DEFAULT_VAL, Status::UNKNOWN);

    set<int> validVals = {1,2,3,4,5,6,7,8,9};

    // The coordinator reference - manage some cross view interactions and such.
    Coordinator &coordinator;
};
#endif //CELL_HPP
