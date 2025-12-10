//
// Created by scott on 11/20/25.
//
#include <ranges>
#include <algorithm>
#include "view.hpp"

#include <iostream>

#include "cell.hpp"

BlockView::BlockView(vector<vector<Cell*>> &fullSrc , int blockRowStart, int blockColStart)
{
    cellBlock = extractCellBlock(fullSrc, blockRowStart, blockColStart);
}

Status BlockView::validate() const
{
    /*
     * represent used values - each index location represents count of
     * the associated "value" in the view. e.g if all 9 cells had no
     * current value; there would be a 9 at index 0 since 0 is used as
     * the "no-value" value.  Of the 9 cells, if only the numbers 1,2,3 existed
     * uniquely in the cells then values at indexes 1,2,3 would be 1 and the
     * remaining indexes would have a value of 0.
     * If only the number 5 existed 3 times in the 9 cells then the value at
     * index 5 would be 3 withe the remaining index containing 0.
     */
    vector USED_VALS = {0,0,0,0,0,0,0,0,0,0};

    // Func to count cell values 0-9.
    auto c = [&USED_VALS](Cell &x) { USED_VALS[std::get<0>(x.peekProposal())]++; };

    /* Walk the list of cells; track usage of each val from the cells
     * Updates USED_VALS as indicated above.
     */

    //Arrays.asList(cellBlock).stream().flatMap(cba -> Arrays.asList(cba).stream()).forEach(c);
    auto x = std::ranges::join_view(cellBlock);
    for (auto cell : x)
    {
        c(*cell);
    }
    /*
     * Talk point:
     * regarding "retStatus"; since uninitialized, all code paths must set a val. It
     * would be better to initialize it to UNKNOWN in this case - then the
     * final "else" can just go away since it would occur by default with the initial
     * value if not changed.
     */
    Status retStatus = Status::UNKNOWN; // count @ idx 0 would be > 1
    if (ranges::count(++USED_VALS.begin(), USED_VALS.end(), 1) == 9) // count @ idx 1-9 = 1
    {
        retStatus = Status::ACCEPT;
    }
    else if (*ranges::max_element(++USED_VALS.begin(), USED_VALS.end()) > 1) // count for any idx 1-9 > 1
    {
        retStatus = Status::REJECT;
    }

    return retStatus;
}

void BlockView::updateUsedValues(int val)
{
    for (int r = 0; r < 3; r++)
    {
        for (int c = 0; c < 3; c++)
        {
            cellBlock[r][c]->removeUsedVal(val);
        }
    }
}

vector<vector<Cell*>> &BlockView::extractCellBlock(vector<vector<Cell*>> &fullSrc, int blockRowStart, int blockColStart)
{
    vector<vector<Cell*>> &vect = * new vector<vector<Cell*>>;
    vect.push_back(extractRowCells(fullSrc, blockRowStart++, blockColStart));
    vect.push_back(extractRowCells(fullSrc, blockRowStart++, blockColStart));
    vect.push_back(extractRowCells(fullSrc, blockRowStart, blockColStart));
    return vect;
}

vector<Cell*> &BlockView::extractRowCells(vector<vector<Cell*>> &fullSrc,  int rowStart,  int colStart)
{
    // The number of cells we extract at a time from a row; this could be moved to a class dedicated to
    // constants or made a private static final int class member. Moving to a constants class though creates
    // a small dependency though. Since this value isn't really useful outside of this class, it is
    // probably ok either as is or as a static final member.
     constexpr int BLOCK_WIDTH = 3;

    /**
     * Talk points
     * Note that "Arrays" is from our import of java.util.Arrays
     * The "fullSrc[rowStart]" provides a reference to the contained
     * array.
     *
     *         top-level
     *          array
     *            |
     *           \./       contained array col #'s within row
     *                   0 1 2 3 4 5 6 7 8
     *  rowStart  0 ->   C C C C C C C C C
     * (i.e.row#) 1 ->   C C C C C C C C C
     *            2 ->   C C C C C C C C C
     *            3 ->   C C C C C C C C C
     *            4 ->   C C C C C C C C C
     *            5 ->   C C C C C C C C C
     *            6 ->   C C C C C C C C C
     *            7 ->   C C C C C C C C C
     *            8 ->   C C C C C C C C C
     *
     *  Also take note that the "to", meaning colStart+blockWidth, is not inclusive.
     *  So a colStart of 0 with a resulting to of "0+3" means that
     *  cols 0, 1, 2 will be copied and not 0,1,2,3.. Make sure you
     *  read the documentation which makes this clear.
     */
    vector<Cell*> &vect = *new vector<Cell*>();
    vector<Cell*> &cols = fullSrc[rowStart];
    auto colStartIt = cols.cbegin();
    colStartIt+=colStart;
    auto colEndIt = colStartIt+BLOCK_WIDTH;
    vect.assign(colStartIt, colEndIt);

    return vect;

}

RowView::RowView(vector<vector<Cell*>> &fullSrc, int row)
    : items(fullSrc[row])
{}

Status RowView::validate() const
{
    vector<int> USED_VALS = {0,0,0,0,0,0,0,0,0,0};

    // Func to count cell values 0-9.
    auto c = [&USED_VALS](Cell &x){ return USED_VALS[std::get<0>(x.peekProposal())]++; };

    // Walk the list of cells; track usage of each val from the cells
    for (auto x : items)
        c(*x);

    Status retStatus = Status::UNKNOWN; // count @ idx 0 must be > 1 which means there are "unknown" values
    if (ranges::all_of(++USED_VALS.begin(), USED_VALS.end(), [](int i){return i == 1;} )) // find whether ALL idx 1-9 have distinct val (no dupe, all vals used)
    {
        retStatus = Status::ACCEPT;
    }
    else if (*ranges::max_element(++USED_VALS.begin(), USED_VALS.end()) > 1) // count for any idx 1-9 > 1
    {
        retStatus = Status::REJECT;
    }

    return retStatus;
}

void RowView::updateUsedValues(int val)
{
    for(auto c: items)
    {
        c->removeUsedVal(val);
    }
}

ColView::ColView(vector<vector<Cell*>> &fullSrc, int col)
{
    for (int row=0; row < fullSrc.size(); row++)
    {
        items.push_back(fullSrc[row][col]);
    }
}

Status ColView::validate() const
{
    vector<int> USED_VALS = {0,0,0,0,0,0,0,0,0,0};

    // Func to count cell values 0-9.
    auto c = [&USED_VALS](Cell &x){ return USED_VALS[std::get<0>(x.peekProposal())]++; };

    // Walk the list of cells; track usage of each val from the cells
    for (auto x : items)
        c(*x);

    Status retStatus = Status::UNKNOWN; // count @ idx 0 must be > 1 which means there are "unknown" values and there must be no dupes @ idx 1-9
    if (ranges::all_of(++USED_VALS.begin(), USED_VALS.end(), [](int i){return i == 1;} )) // find whether ALL idx 1-9 have distinct val (no dupe, all vals used)
    {
        retStatus = Status::ACCEPT;
    }
    else if (*ranges::max_element(++USED_VALS.begin(), USED_VALS.end()) > 1) // count for any idx 1-9 > 1
    {
        retStatus = Status::REJECT;
    }

    return retStatus;
}

void ColView::updateUsedValues(int val)
{
    for(int i=0; i<items.size(); i++)
    {
        items[i]->removeUsedVal(val);
    }
}
