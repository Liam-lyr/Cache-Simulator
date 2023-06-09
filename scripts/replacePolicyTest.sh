# test effect of associativity on cache performance

rm ./results/replacePolicyTest.txt

echo Replace Policy test start!

./build/CacheSimulator -t  $PWD/traces/ls.trace -c  $PWD/confs/32B-4way-64KB-lru-50-wb-wa.txt >> ./results/replacePolicyTest.txt

echo lru done!

./build/CacheSimulator -t  $PWD/traces/ls.trace -c  $PWD/confs/32B-4way-64KB-random-50-wb-wa.txt >> ./results/replacePolicyTest.txt

echo random done!

echo Replace Policy test done!

echo formatted results with cache information have be written to ./results/replacePolicyTest.txt