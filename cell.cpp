//
// Created by scott on 11/18/25.
//

#include "coordinator.hpp"
#include "cell.hpp"
#include "status.hpp"
#include <iostream>

Cell::Cell(Coordinator &coordinator)
    : coordinator(coordinator)
{}

void Cell::setProposedVal(int val)
{
    if (Status::ACCEPT != std::get<1>(content))
    {
        content = make_tuple<int, Status>(std::move(val), Status::UNKNOWN);
    }
}

void Cell::setAcceptedVal(int val)
{
    content = make_tuple<int,Status>(std::move(val), Status::ACCEPT);
}

tuple<int, Status> Cell::peekProposal()
{
    return content;
}

Status Cell::getStatus()
{
    return std::get<1>(content);
}

Status Cell::reject()
{
    if (Status::ACCEPT != getStatus())
        content = make_tuple(DEFAULT_VAL, Status::UNKNOWN);
    else
    {
        cout << "attempt reject of an ACCEPTED cell. content: " << get<0>(content) << "\n";
    }
    return Status::REJECT;
}

bool Cell::isUnset()
{
    return DEFAULT_VAL == std::get<0>(content);
}

void Cell::removeUsedVal(int val)
{
    validVals.erase(val);
}

set<int> &Cell::getValidVals()
{
    return validVals;
}