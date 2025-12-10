//
// Created by scott on 11/27/25.
//

#include "strategy.hpp"
#include <iostream>
#include <string>
#include <cstdio>

bool OnlyOneValidValStrategy::process(vector<vector<Cell*>> &fullSrc, Coordinator &coordinator)
{
    bool retVal = false;
    for (vector<Cell *>row : fullSrc)
    {
        for (Cell *col : row)
        {
            if (col->getValidVals().size() == 1)  // Only 1 possible value remaining
            {
                col->setAcceptedVal(*col->getValidVals().cbegin());
                col->getValidVals().clear();  // clear the valid values data once we used that value.
                retVal = true;
            }
        }
    }
    return retVal;
}

bool SerialStrategy::process(vector<vector<Cell*>> &fullSrc, Coordinator &coordinator)
{
    return process(fullSrc, coordinator, 0, -1) != Status::REJECT;
}

Status SerialStrategy::process(vector<vector<Cell*>> &fullSrc, Coordinator &coordinator, int row, int col)
{
	//  *******************
	//  Row/col # handling
	// ********************
	// prep cur row/col info
	int tmpCol = col;
	col = tmpCol + 1;
	col %= 9;

	if (col < tmpCol)
	{
		row++;
	}

	//*******************************
	// Terminate deeper traversal and start unwinding the recursive calls
	//*******************************
	if (row == 9) {
		return Status::ACCEPT;
	}
	// *************************************
	// Traversal prep - this data is manipulated on non-Accept cells/returns
	// *************************************
	// Get current row/col data - prepare before depth first traversal
	Cell *cell = fullSrc[row][col];
	Status curCellStatus = cell->getStatus();

	if (Status::ACCEPT == curCellStatus) {
		return process(fullSrc, coordinator, row, col);
	}
	else if (Status::UNKNOWN == curCellStatus) {
		auto it = cell->getValidVals().begin();
		auto endIt = cell->getValidVals().end();
		while (it != endIt) {
		// If we don't have a current value in the cell then we should set one from the currently known potential values.

			int tmpVal = *it;
			cell->setProposedVal(tmpVal);

			Status curGroupStatus = coordinator.test(row, col);
			if (Status::REJECT !=  curGroupStatus)
			{
				// process next cell since this cell is currently ACCEPT or UNKNOWN.
				Status curProcessStatus = process(fullSrc, coordinator, row, col);

				// if we have ACCEPT; start unwinding the recursive calls.
				if (Status::ACCEPT == curProcessStatus) {
					return curProcessStatus;
				}
			}

			++it;
		}
	}

	return cell->reject();;
}