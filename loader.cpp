//
// Created by scott on 11/19/25.
//
#include "loader.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <boost/algorithm/string.hpp>

using namespace std;

FileLoader::FileLoader(path filePath)
	: filePath(filePath)
{}

bool FileLoader::load(vector<vector<Cell *>> &data, Coordinator &coordinator)
{
    cout << "Loading " << filePath << "\n";
	char buffer[50];
	int row =  -1;
	int col = 0;

	ifstream stream(filePath.generic_string(), ios_base::in);
	while (!stream.getline(buffer, sizeof(buffer)).eof())
	{
		string str(buffer);
		boost::trim(str);
		if (str.empty()) { continue;}

		vector<string> strs;
		boost::split(strs, str, boost::is_any_of(" "));
		auto it = strs.cbegin();
		while (it != strs.cend())
		{
			col %= 9;
			int cTmp = col++;
			if (cTmp == 0) // any time we are at the 0 column, we should increment our row number.
			{
				row++;  // note initial -1 above
			}

			try
			{
				auto c = *it;
				if (c == "x") {
					++it;
					continue;
				}

				int val = stoi(c);
				data[row][cTmp]->setAcceptedVal(val);
				data[row][cTmp]->getValidVals().clear();  // remove remaining vals
				coordinator.removedUsedVal(val, row, cTmp);  // remove val from views
			}
			catch (std::exception e)
			{
				cout << "exception: " << e.what() << "\n";
			}
			++it;
		}
	}

    return true;
}