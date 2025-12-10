//
// Created by scott on 11/18/25.
//

#ifndef GAME_HPP
#define GAME_HPP

#include <boost/timer/timer.hpp>

#include "loader.hpp"
#include "strategy.hpp"

class Game
{
public:
    Game();
    Game &load(LoaderIntfc &loader);
    Game &process(int stratCnt, ...);
    void displayBoard(bool includeRemainingValidVals);

private:
    void init();
    // This is a coordinator which manages the views.
    Coordinator &coordinator;
    int formatValidVal(string &outStr, const set<int> validVals) const;

    /**
     * The primary game data
     */
    vector<vector<Cell*>> data;

    boost::timer::cpu_times curGameProcStartInstantDur;
};

#endif //GAME_HPP
