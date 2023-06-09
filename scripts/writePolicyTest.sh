# test effect of associativity on cache performance

rm ./results/writePolicyTest.txt

echo Write Policy test start!

./build/CacheSimulator -t  $PWD/traces/ls.trace -c  $PWD/confs/8B-2way-8KB-lru-50-wb-wa.txt >> ./results/writePolicyTest.txt

echo Write Back + Write Allocate done!

./build/CacheSimulator -t  $PWD/traces/ls.trace -c  $PWD/confs/8B-2way-8KB-lru-50-wt-nwa.txt >> ./results/writePolicyTest.txt

echo Write Through + Not Write Allocate done!

echo Write Policy test done!

echo formatted results with cache information have be written to ./results/writePolicyTest.txt