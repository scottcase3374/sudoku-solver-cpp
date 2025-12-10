//
// Created by scott on 11/18/25.
//

#ifndef LOADER_HPP
#define LOADER_HPP

#include <boost/filesystem.hpp>
#include <vector>
#include "cell.hpp"
#include "coordinator.hpp"
using namespace boost::filesystem;

class LoaderIntfc
{
public:
    virtual bool load(vector<vector<Cell *>> &data, Coordinator &coordinator) = 0;
};

class FileLoader : public LoaderIntfc
{
public:
    FileLoader(const path path);
    bool load(vector<vector<Cell *>> &data, Coordinator &coordinator);

private:
    const path filePath;
};

#endif //LOADER_HPP
