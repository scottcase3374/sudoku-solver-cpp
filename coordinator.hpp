//
// Created by scott on 11/18/25.
//

#ifndef COORDINATOR_HPP
#define COORDINATOR_HPP

#include "cell.hpp"
#include "view.hpp"
#include "status.hpp"

class Coordinator
{
public:
    Coordinator();
    void init(vector<vector<Cell*>> &data);
    void setupBlockViews(vector<vector<Cell*>> &data);
    void setupRowViews(vector<vector<Cell*>> &data);
    void setupColViews(vector<vector<Cell*>> &data);
    void removedUsedVal(int val, int row, int col);
    Status test(int row, int col) const;
    BlockView &rowColToBlock(int row, int col) const;
    Status filtStatus(const vector<Status> stats) const;

private:
    vector<vector<BlockView*>> blockViews;
    vector<ColView*> colViews;
    vector<RowView*> rowViews;
};
#endif //COORDINATOR_HPP
