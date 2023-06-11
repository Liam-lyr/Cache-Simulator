rm ./results/others/myAssociativity.txt

echo my Associativity test start!

./build/CacheSimulator -t  $PWD/traces/ls.trace -c  $PWD/confs/others/32B-dm-64KB-none-50-wb-wa.txt >> ./results/others/myAssociativity.txt

echo direct-mapped done!

./build/CacheSimulator -t  $PWD/traces/ls.trace -c  $PWD/confs/others/32B-2way-64KB-lru-50-wb-wa.txt >> ./results/others/myAssociativity.txt

echo 2-way done!

./build/CacheSimulator -t  $PWD/traces/ls.trace -c  $PWD/confs/others/32B-4way-64KB-lru-50-wb-wa.txt >> ./results/others/myAssociativity.txt

echo 4-way done!

./build/CacheSimulator -t  $PWD/traces/ls.trace -c  $PWD/confs/others/32B-full-64KB-lru-50-wb-wa.txt >> ./results/others/myAssociativity.txt

echo fully-associated done!

echo my Associativity test done!

echo formatted results with cache information have been written to ./results/others/myAssociativity.txt