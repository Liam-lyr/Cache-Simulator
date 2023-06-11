rm ./results/others/myWritePolicy.txt

echo my Write Policy test start!

./build/CacheSimulator -t  $PWD/traces/ls.trace -c  $PWD/confs/others/8B-2way-8KB-lru-50-wb-wa.txt >> ./results/others/myWritePolicy.txt

echo Write Back + Write Allocate done!

./build/CacheSimulator -t  $PWD/traces/ls.trace -c  $PWD/confs/others/8B-2way-8KB-lru-50-wt-nwa.txt >> ./results/others/myWritePolicy.txt

echo Write Through + Not Write Allocate done!

echo my Write Policy test done!

echo formatted results with cache information have been written to ./results/others/myWritePolicy.txt