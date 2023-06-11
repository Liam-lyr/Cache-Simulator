rm ./results/blockSize-cacheSize.txt

echo Block Size - Cache Size test start!

# 16 KB cache size

./build/CacheSimulator -t $PWD/traces/ls.trace -c $PWD/confs/hitRate-blockSize-cacheSize/32B-dm-16KB-lru-50-wb-wa.txt >> ./results/blockSize-cacheSize.txt

echo 32B - 16KB done!

./build/CacheSimulator -t  $PWD/traces/ls.trace -c  $PWD/confs/hitRate-blockSize-cacheSize/64B-dm-16KB-lru-50-wb-wa.txt >> ./results/blockSize-cacheSize.txt

echo 64B - 16KB done!

./build/CacheSimulator -t  $PWD/traces/ls.trace -c  $PWD/confs/hitRate-blockSize-cacheSize/128B-dm-16KB-lru-50-wb-wa.txt >> ./results/blockSize-cacheSize.txt

echo 128B - 16KB done!

./build/CacheSimulator -t  $PWD/traces/ls.trace -c  $PWD/confs/hitRate-blockSize-cacheSize/256B-dm-16KB-lru-50-wb-wa.txt >> ./results/blockSize-cacheSize.txt

echo 256B - 16KB done!

# 32 KB cache size

./build/CacheSimulator -t $PWD/traces/ls.trace -c $PWD/confs/hitRate-blockSize-cacheSize/32B-dm-32KB-lru-50-wb-wa.txt >> ./results/blockSize-cacheSize.txt

echo 32B - 32KB done!

./build/CacheSimulator -t  $PWD/traces/ls.trace -c  $PWD/confs/hitRate-blockSize-cacheSize/64B-dm-32KB-lru-50-wb-wa.txt >> ./results/blockSize-cacheSize.txt

echo 64B - 32KB done!

./build/CacheSimulator -t  $PWD/traces/ls.trace -c  $PWD/confs/hitRate-blockSize-cacheSize/128B-dm-32KB-lru-50-wb-wa.txt >> ./results/blockSize-cacheSize.txt

echo 128B - 32KB done!

./build/CacheSimulator -t  $PWD/traces/ls.trace -c  $PWD/confs/hitRate-blockSize-cacheSize/256B-dm-32KB-lru-50-wb-wa.txt >> ./results/blockSize-cacheSize.txt

echo 256B - 32KB done!

# 64 KB cache size

./build/CacheSimulator -t $PWD/traces/ls.trace -c $PWD/confs/hitRate-blockSize-cacheSize/32B-dm-64KB-lru-50-wb-wa.txt >> ./results/blockSize-cacheSize.txt

echo 32B - 64KB done!

./build/CacheSimulator -t  $PWD/traces/ls.trace -c  $PWD/confs/hitRate-blockSize-cacheSize/64B-dm-64KB-lru-50-wb-wa.txt >> ./results/blockSize-cacheSize.txt

echo 64B - 64KB done!

./build/CacheSimulator -t  $PWD/traces/ls.trace -c  $PWD/confs/hitRate-blockSize-cacheSize/128B-dm-64KB-lru-50-wb-wa.txt >> ./results/blockSize-cacheSize.txt

echo 128B - 64KB done!

./build/CacheSimulator -t  $PWD/traces/ls.trace -c  $PWD/confs/hitRate-blockSize-cacheSize/256B-dm-64KB-lru-50-wb-wa.txt >> ./results/blockSize-cacheSize.txt

echo 256B - 64KB done!

# 128 KB cache size

./build/CacheSimulator -t $PWD/traces/ls.trace -c $PWD/confs/hitRate-blockSize-cacheSize/32B-dm-128KB-lru-50-wb-wa.txt >> ./results/blockSize-cacheSize.txt

echo 32B - 128KB done!

./build/CacheSimulator -t  $PWD/traces/ls.trace -c  $PWD/confs/hitRate-blockSize-cacheSize/64B-dm-128KB-lru-50-wb-wa.txt >> ./results/blockSize-cacheSize.txt

echo 64B - 128KB done!

./build/CacheSimulator -t  $PWD/traces/ls.trace -c  $PWD/confs/hitRate-blockSize-cacheSize/128B-dm-128KB-lru-50-wb-wa.txt >> ./results/blockSize-cacheSize.txt

echo 128B - 128KB done!

./build/CacheSimulator -t  $PWD/traces/ls.trace -c  $PWD/confs/hitRate-blockSize-cacheSize/256B-dm-128KB-lru-50-wb-wa.txt >> ./results/blockSize-cacheSize.txt

echo 256B - 128KB done!

# 256 KB cache size

./build/CacheSimulator -t $PWD/traces/ls.trace -c $PWD/confs/hitRate-blockSize-cacheSize/32B-dm-256KB-lru-50-wb-wa.txt >> ./results/blockSize-cacheSize.txt

echo 32B - 256KB done!

./build/CacheSimulator -t  $PWD/traces/ls.trace -c  $PWD/confs/hitRate-blockSize-cacheSize/64B-dm-256KB-lru-50-wb-wa.txt >> ./results/blockSize-cacheSize.txt

echo 64B - 256KB done!

./build/CacheSimulator -t  $PWD/traces/ls.trace -c  $PWD/confs/hitRate-blockSize-cacheSize/128B-dm-256KB-lru-50-wb-wa.txt >> ./results/blockSize-cacheSize.txt

echo 128B - 256KB done!

./build/CacheSimulator -t  $PWD/traces/ls.trace -c  $PWD/confs/hitRate-blockSize-cacheSize/256B-dm-256KB-lru-50-wb-wa.txt >> ./results/blockSize-cacheSize.txt

echo 256B - 256KB done!

# done

echo Block Size - Cache Size test done!

echo formatted results with cache information have been written to ./results/blockSize-cacheSize.txt