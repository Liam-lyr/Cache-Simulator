rm ./results/others/myReplacePolicy.txt

echo my Replace Policy test start!

./build/CacheSimulator -t  $PWD/traces/ls.trace -c  $PWD/confs/others/32B-4way-64KB-lru-50-wb-wa.txt >> ./results/others/myReplacePolicy.txt

echo lru done!

./build/CacheSimulator -t  $PWD/traces/ls.trace -c  $PWD/confs/others/32B-4way-64KB-random-50-wb-wa.txt >> ./results/others/myReplacePolicy.txt

echo random done!

echo my Replace Policy test done!

echo formatted results with cache information have been written to ./results/others/myReplacePolicy.txt