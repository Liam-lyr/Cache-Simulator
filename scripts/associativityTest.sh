# test effect of associativity on cache performance

rm ./results/associativityTest.txt

echo Associativity test start!

./build/CacheSimulator -t  $PWD/traces/ls.trace -c  $PWD/confs/32B-dm-64KB-none-50-wb-wa.txt >> ./results/associativityTest.txt

echo direct-mapped done!

./build/CacheSimulator -t  $PWD/traces/ls.trace -c  $PWD/confs/32B-2way-64KB-lru-50-wb-wa.txt >> ./results/associativityTest.txt

echo 2-way done!

./build/CacheSimulator -t  $PWD/traces/ls.trace -c  $PWD/confs/32B-4way-64KB-lru-50-wb-wa.txt >> ./results/associativityTest.txt

echo 4-way done!

./build/CacheSimulator -t  $PWD/traces/ls.trace -c  $PWD/confs/32B-full-64KB-lru-50-wb-wa.txt >> ./results/associativityTest.txt

echo fully-associated done!

echo Associativity test done!

echo formatted results with cache information have be written to ./results/associativityTest.txt