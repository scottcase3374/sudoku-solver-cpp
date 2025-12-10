#include <iostream>

#include "game.hpp"
#include "strategy.hpp"

int main(int argc, char *argv[]) {

    /**
     * talk points
     * 1) Note we construct a Game instance and immediately start
     * calling methods on it with other methods chained to the
     * method return value of the prior call. This is a "fluent"
     * style API usage.
     *
     * 2) With this design, it is pretty easy to desin new Strategies which can either be added below or replace
     * the initial strategies.  I felt that was a good goal to allow experimenting with new ideas.  What kind
     * of other strategies might be interesting?
     *
     * 2a) The one that may be promising would determine the views with
     * the fewest missing data items and start the solution there; continue with views with next fewest missing
     * cells and repeat.  This may reduce the amount (depth & overall quantity) of backtracking attempts.
     *
     * 2b) other ideas are using a stack based instead of recursive solution.
     */
    auto solve = [](path p) {	 (new Game())->
                  load(*new FileLoader(p)).
                  process( 2,	*new OnlyOneValidValStrategy(),  // we can pass any # of StrategyIntfc based instances
                              *new SerialStrategy()).			// since process uses a vararg parameter.
                  displayBoard(true);};

    if (argc == 2)
    {
        auto p = path(argv[1]);
        if (!is_directory(p))
        {
            cout << "Error: Directory not provided!" << "\n";
            return -1;
        }

        // Take command line arg as a directory, read the file names as paths and pass each path to a new Game instance.
        for (auto &&fname: directory_iterator(p))
        {
            try
            {
                solve(fname.path());
            }
            catch(std::exception ex)
            {
                cout << "Exception: " << ex.what() << "\n";
            }
        }
    }
    return 0;
}
