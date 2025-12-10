//
// Created by scott on 11/19/25.
//
#include <list>
#include <iostream>
#include "coordinator.hpp"

Coordinator::Coordinator()
{}

void Coordinator::init(vector<vector<Cell*>> &data)
{
    setupBlockViews(data);
    setupRowViews(data);
    setupColViews(data);
}

void Coordinator::setupBlockViews(vector<vector<Cell*>> &data)
{
    for (int r=0; r<9; r+=3)
    {
        auto &block = *new vector<BlockView *>;

        for (int c=0; c<9; c+=3)
        {
            block.push_back(new BlockView(data,r, c));
        }
        blockViews.push_back(block);
    }
}

void Coordinator::setupRowViews(vector<vector<Cell*>> &data)
{
    for (int r=0; r < 9; r++) {
        rowViews.push_back(new RowView(data, r));
    }
}

void Coordinator::setupColViews(vector<vector<Cell*>> &data)
{
    for (int c=0; c < 9; c++)
    {
        colViews.push_back(new ColView(data, c));
    }
}

void Coordinator::removedUsedVal(int val, int row, int col)
{
    colViews[col]->updateUsedValues(val);
    rowViews[row]->updateUsedValues(val);
    rowColToBlock(row, col).updateUsedValues(val);
}

Status Coordinator::test(int row, int col) const
{
    vector<Status> stats {
     {colViews[col]->validate(),
            rowViews[row]->validate(),
            rowColToBlock(row, col).validate()}};
    return filtStatus(stats);
}

BlockView &Coordinator::rowColToBlock(int row, int col) const
{
    int tmpRow = row / 3;
    int tmpCol = col / 3;
    return *blockViews[tmpRow][tmpCol];
}

Status Coordinator::filtStatus(const vector<Status> stat) const
{
    Status retStatus = Status::ACCEPT;

    auto it = stat.cbegin();
    auto end = stat.cend();
    for (; it != end; ++it)
    {
        auto status = *it;
        if (Status::REJECT == status)
        {
            //  no need to check further.
            retStatus = Status::REJECT;
            break;
        }
        else if (Status::UNKNOWN == status)
        {
            // If unknown then we should continue
            // to make sure it isn't a reject
            retStatus = Status::UNKNOWN;
        }
        // Fall through (default if no changes) is ACCEPT .
    }
    return retStatus;
}