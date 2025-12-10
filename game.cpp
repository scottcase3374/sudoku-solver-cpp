//
// Created by scott on 11/27/25.
//

#include "game.hpp"
#include <string>
#include <cstdarg>
#include <iostream>
#include <list>
#include <vector>
#include <boost/algorithm/string/join.hpp>
#include <boost/timer/timer.hpp>
using namespace boost::timer;
using namespace std;

Game::Game() : coordinator(*new Coordinator())
{
    init();
}

void Game::init()
{
    /**
     * Our array exists but has no objects at any location.
     */
	for (int i=0; i<9; i++)
	{
		data.push_back(*new vector<Cell*>());
	}

    for (int row=0; row < 9; row++)
    {
        for (int col=0; col < 9; col++)
        {
            data[row].push_back(new Cell(coordinator));
        }
    }

    coordinator.init(data);
}

Game &Game::load(LoaderIntfc &loader)
{
    loader.load(data, coordinator);
    return *this;
}

Game &Game::process(int stratCnt, ...)
{
	va_list args;
	va_start(args, stratCnt);
    boost::timer::cpu_timer  curGameProcStartInstant;
	curGameProcStartInstant.start();
    /**
     * strategy(ies)
     * list of incomplete views
     * loop over incomplete views
     *      find next proposed cell
     *      pick next proposed value
     * 		check validity of proposed val @ proposed cell
     * 		if valid; set value as valid @ cell
     *
     * how are proposed values acquired?
     */
    try
    {
        for (int i=0; i < stratCnt; i++)
        {
            va_arg(args, StrategyIntfc).process(data, coordinator);
        }
    }
    catch(std::exception e)
    {
        cout << e.what() << "\n";
    }

    curGameProcStartInstant.stop();
	va_end(args);
	curGameProcStartInstantDur = curGameProcStartInstant.elapsed() ;
    return *this;
}
void Game::displayBoard(bool includeRemainingValidVals)
{
	constexpr int MILLI_CONV = 1000000;
	//cout << ("Total Time (game_start -> load/setup -> solved): [milliseconds] " << curGameTotalDur.getNano()/MILLI_CONV);
	cout << "Total Time (solve): [milliseconds] " << (int)((curGameProcStartInstantDur.system + curGameProcStartInstantDur.user) / MILLI_CONV ) << "\n";

	/**
	 * Talk points
	 * 1) Note the style of for-loop here; we didn't need to know
	 * a specific index so this style is the cleanest in that case.
	 * 2) This is a "procedural" or "imperative" style of processing by
	 * using nested explicit loops. We could use a more functional style
	 * but I wanted to use a mix of styles in places to demonstrate
	 * them.
	 *
	 * I didn't put hardly any time into this; there are surely much better ways of doing this. Some ways
	 * come to mind (templating of various forms) but would incur a dependency and a 3rd party
	 * library you would have to understand..  simple was better here.  I should have cleaned
	 * it up more than it is though. This is an example of "less than ideal".
	 */
	string boardText;
	string extraOutput;
	string boardWithExtra;
	string boardWithExtra1;

	for (vector<Cell*> row : data)
	{
		for (Cell *col : row)
		{
			const string tmp = to_string(get<0>(col->peekProposal())) + " ";  // Remember this declaration is IN the for loop so each iteration
																	 //  is effectively entering a new scope so "const" is valid to use.
			boardText.append(tmp);
			string tmpStr;
			formatValidVal(tmpStr, col->getValidVals());

			string fmtString = "%-19s";
			{
				char buffer[500];
				sprintf(buffer, fmtString.c_str(), tmp.c_str());
				boardWithExtra.append(buffer); // board - formated
			}
			{
				char buffer[500];
				sprintf(buffer, fmtString.c_str(), tmpStr.c_str());
				boardWithExtra1.append(buffer); // valid vals - formatted
			}
		}
		boardText.append("\n");

		extraOutput.append(boardWithExtra);
		extraOutput.append("\n");
		extraOutput.append(boardWithExtra1);
		extraOutput.append("\n");

		boardWithExtra1.clear();
		boardWithExtra.clear();
	}

	cout << boardText << "\n";
	cout << "\n\n\n";

	if (includeRemainingValidVals)
		cout << extraOutput << "\n";
}

/**
 * Utility method for forming part of the output for the display of the "valid values".  We take the set of values and simply generate
 * them into  the form "[x,x,x..]". If the set contains all #'s (1-9) then we shorten the display form to [*].
 *
 * @param outStr This is a non-null instance; data is appended to this.
 * @param validVals A set of potentially valid vals; this is formatted into a nicely formatted text format
 *
 * @return length of text appended to outStr
 */

int Game::formatValidVal(string &outStr, const set<int> validVals) const
{
	vector<string> strs;
	for(auto it = validVals.cbegin(); it != validVals.cend(); it++) {
		strs.push_back(std::to_string(*it));
	}

	string valWithComma = boost::algorithm::join(strs, ",");
	string valStr = validVals.size() == 9 ? "[*]" : format("[{}]", valWithComma);
	outStr.append(valStr);
	return valStr.length();
}