rm ./results/replacePolicy.txt

echo Replace Policy test start!

./build/CacheSimulator -t  $PWD/traces/ls.trace -c  $PWD/confs/hitRate-replacePolicy/32B-4way-128KB-lru-50-wb-wa.txt >> ./results/replacePolicy.txt

echo lru done!

./build/CacheSimulator -t  $PWD/traces/ls.trace -c  $PWD/confs/hitRate-replacePolicy/32B-4way-128KB-rand-50-wb-wa.txt >> ./results/replacePolicy.txt

echo random done!

echo Replace Policy test done!

echo formatted results with cache information have been written to ./results/replacePolicy.txt