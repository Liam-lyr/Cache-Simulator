#include <iostream>

#include "base.h"

using namespace std;

int main(int argc, char *argv[])
{

    Cache cache;
    InputUtilities inputUtil;

    inputUtil.setCacheProperties(argc, argv, cache);

    inputUtil.printInputProperties();
    cache.printCacheInfo();

    

    return 0;
}