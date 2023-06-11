rm ./results/associativity.txt

echo Associativity test start!

./build/CacheSimulator -t  $PWD/traces/ls.trace -c  $PWD/confs/hitRate-associativity/32B-dm-128KB-lru-50-wb-wa.txt >> ./results/associativity.txt

echo direct-mapped done!

./build/CacheSimulator -t  $PWD/traces/ls.trace -c  $PWD/confs/hitRate-associativity/32B-2way-128KB-lru-50-wb-wa.txt >> ./results/associativity.txt

echo 2-way done!

./build/CacheSimulator -t  $PWD/traces/ls.trace -c  $PWD/confs/hitRate-associativity/32B-4way-128KB-lru-50-wb-wa.txt >> ./results/associativity.txt

echo 4-way done!

./build/CacheSimulator -t  $PWD/traces/ls.trace -c  $PWD/confs/hitRate-associativity/32B-8way-128KB-lru-50-wb-wa.txt >> ./results/associativity.txt

echo 8-way done!

./build/CacheSimulator -t  $PWD/traces/ls.trace -c  $PWD/confs/hitRate-associativity/32B-full-128KB-lru-50-wb-wa.txt >> ./results/associativity.txt

echo fully-associated done!

echo Associativity test done!

echo formatted results with cache information have been written to ./results/associativity.txt