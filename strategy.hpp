//
// Created by scott on 11/27/25.
//

#ifndef STRATEGY_HPP
#define STRATEGY_HPP

#include<vector>
#include "status.hpp"
#include "coordinator.hpp"

class StrategyIntfc
{
public:
    virtual bool process(vector<vector<Cell*>> &fullSrc, Coordinator &coordinator) = 0;

};

class OnlyOneValidValStrategy : public StrategyIntfc
{
public:
    virtual bool process(vector<vector<Cell*>> &fullSrc, Coordinator &coordinator);
};

class SerialStrategy : public StrategyIntfc
{
public:
    virtual bool process(vector<vector<Cell*>> &fullSrc, Coordinator &coordinator);
private:
    Status process(vector<vector<Cell*>> &fullSrc, Coordinator &coordinator, int row, int col);
};
#endif //STRATEGY_HPP
