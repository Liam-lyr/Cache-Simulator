rm ./results/writePolicy.txt
rm ./results/simplifiedOutputFiles/writePolicy.txt

echo Write Policy test start!

./build/CacheSimulator -t  $PWD/traces/ls.trace -c  $PWD/confs/hitRate-writePolicy/32B-dm-128KB-lru-50-wb-wa.txt -o $PWD/results/simplifiedOutputFiles/writePolicy.txt >> ./results/writePolicy.txt

echo Write Back + Write Allocate done!

./build/CacheSimulator -t  $PWD/traces/ls.trace -c  $PWD/confs/hitRate-writePolicy/32B-dm-128KB-lru-50-wt-nwa.txt -o $PWD/results/simplifiedOutputFiles/writePolicy.txt >> ./results/writePolicy.txt

echo Write Through + Not Write Allocate done!

echo Write Policy test done!

echo formatted results with cache information have been written to ./results/writePolicy.txt

echo simplified results have been written to ./results/simplifiedOutputFiles/writePolicy.txt