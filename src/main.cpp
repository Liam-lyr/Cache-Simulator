#include <iostream>

#include "cacheUtils.h"
#include "analyzerUtils.h"

using namespace std;

int main(int argc, char *argv[])
{

    Cache cache;
    InputUtilities inputUtil;
    Analyzer analyzer;

    inputUtil.setCacheProperties(argc, argv, cache);

    inputUtil.printInputProperties();
    cache.printCacheInfo();
    analyzer.printAnalyzerProperties();

    string traceDir = inputUtil.getTraceDir();
    string outputDir = inputUtil.getOutputDir();

    cache.cacheInit();
    analyzer.traceFileTest(cache, traceDir);

    cache.printCacheBody();

    return 0;
}
