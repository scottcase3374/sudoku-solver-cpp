//
// Created by scott on 11/18/25.
//

#ifndef VIEW_HPP
#define VIEW_HPP

#include <vector>
#include "cell.hpp"
#include "status.hpp"


class BaseView
{
public:
    virtual Status validate() const = 0;
    virtual void updateUsedValues(int val) = 0;
};

class RowView : public BaseView
{
public:
    RowView(vector<vector<Cell*> > &fullSrc, int row);
    virtual Status validate() const;
    virtual void updateUsedValues(int val);

private:
    vector<Cell*> items;
};

class ColView: public BaseView
{
public:
    ColView(vector<vector<Cell*> > &fullSrc, int col);
    virtual Status validate() const;
    virtual void updateUsedValues(int val);

private:
    vector<Cell*> items;
};

class BlockView : public BaseView
{
public:
    BlockView(vector<vector<Cell*>>  &fullSrc , int blockRowStart, int blockColStart);
    virtual Status validate() const;
    virtual void updateUsedValues(int val);

private:
    vector<vector<Cell*>> &extractCellBlock(vector<vector<Cell*>> &fullSrc, int blockRowStart, int blockColStart);
    vector<Cell*> &extractRowCells(vector<vector<Cell*>> &fullSrc,  int rowStart,  int colStart);

    vector<vector<Cell*>> cellBlock;
};
#endif //VIEW_HPP
