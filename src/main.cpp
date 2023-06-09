#include <iostream>

#include "cacheUtils.h"
#include "analyzerUtils.h"


#include "Test.h"

using namespace std;

int main(int argc, char *argv[])
{

    Cache cache;
    InputUtilities inputUtil;
    Analyzer analyzer;

    inputUtil.setCacheProperties(argc, argv, cache);

    inputUtil.printInputProperties();
    cache.printCacheInfo();

    string traceDir = inputUtil.getTraceDir();
    string outputDir = inputUtil.getOutputDir();

    cache.cacheInit();
    analyzer.traceFileTest(cache, traceDir);

    cout << endl;
    analyzer.printAnalyzerProperties();

    // cache.printCacheBody();

    if(!outputDir.empty())
    {
        analyzer.outputToFile(outputDir);
    }


    return 0;
}