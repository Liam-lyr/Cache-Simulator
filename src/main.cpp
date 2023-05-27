#include <iostream>

#include "base.h"

using namespace std;

int main(int argc, char *argv[])
{

    Cache cache;
    InputUtilities inputUtil;

    cache.printCacheInfo();
    inputUtil.getInput(argc, argv);

    inputUtil.printInputProperties();
    cache.printCacheInfo();

    return 0;
}