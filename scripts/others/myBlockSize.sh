rm ./results/others/myBlockSize.txt

echo my Block Size test start!

./build/CacheSimulator -t $PWD/traces/ls.trace -c $PWD/confs/others/8B-2way-64KB-lru-50-wb-wa.txt >> ./results/others/myBlockSize.txt

echo 8B done!

./build/CacheSimulator -t  $PWD/traces/ls.trace -c  $PWD/confs/others/16B-2way-64KB-lru-50-wb-wa.txt >> ./results/others/myBlockSize.txt

echo 16B done!

./build/CacheSimulator -t  $PWD/traces/ls.trace -c  $PWD/confs/others/32B-2way-64KB-lru-50-wb-wa.txt >> ./results/others/myBlockSize.txt

echo 32B done!

./build/CacheSimulator -t  $PWD/traces/ls.trace -c  $PWD/confs/others/8192B-2way-64KB-lru-50-wb-wa.txt >> ./results/others/myBlockSize.txt

echo 8192B done!

echo my Block Size test done!

echo formatted results with cache information have be written to ./results/others/myBlockSize.txt