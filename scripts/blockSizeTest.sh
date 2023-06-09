# test effect of associativity on cache performance

rm ./results/blockSizeTest.txt

echo Block Size test start!

./build/CacheSimulator -t $PWD/traces/ls.trace -c $PWD/confs/8B-2way-64KB-lru-50-wb-wa.txt >> ./results/blockSizeTest.txt

echo 8B done!

./build/CacheSimulator -t  $PWD/traces/ls.trace -c  $PWD/confs/16B-2way-64KB-lru-50-wb-wa.txt >> ./results/blockSizeTest.txt

echo 16B done!

./build/CacheSimulator -t  $PWD/traces/ls.trace -c  $PWD/confs/32B-2way-64KB-lru-50-wb-wa.txt >> ./results/blockSizeTest.txt

echo 32B done!

./build/CacheSimulator -t  $PWD/traces/ls.trace -c  $PWD/confs/8192B-2way-64KB-lru-50-wb-wa.txt >> ./results/blockSizeTest.txt

echo 8192B done!

echo Block Size test done!

echo formatted results with cache information have be written to ./results/blockSizeTest.txt