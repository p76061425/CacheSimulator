# Cache Simulator

## Environment
* GCC version: 5.4.0
* Linux version: Ubuntu 14.01

## 28 traces from SPEC CPU 2006:</br>
http://faculty.cse.tamu.edu/djimenez/614/hw2/traces/
* working with 64 bit addresses.



## Command-Line Parameters
* There should be five command line parameters in this order:
  1.	nk: the capacity of the cache in kilobytes (an int)
  2.	assoc: the associativity of the cache (an int)
  3.	blocksize: the size of a single cache block in bytes (an int)
  4.	repl: the replacement policy (a char); 'l' means LRU, 'r' means random.
  5.	Input file: memory access address of read/write operations.


## Command Line Format
```sh
$ gzip -d 429.mcf-184B.trace.txt.gz
$ g++ ./cache.cpp -o cache
$ ./cache 2048 64 64 l 429.mcf-184B.trace.txt
```


## Input
* Read traces from the standard input. Each line on the standard input will be a lowercase 'r' (for read) or 'w' (for write) followed by a space and then a 64-bit hexadecimal number giving the address of the memory access. For example, a snippet of a trace file looks like this:</br>
  r 56ecd8</br>
  r 47f639</br>
  r 7ff0001ff</br>
  w 47f63e</br>
  r 4817ef</br>
  r 7d5ab8</br>


## Output
* The output should be a single line of six numbers separated by spaces. These six numbers are:
  1.	The total number of misses,
  2.	The percentage of misses (i.e. total misses divided by total accesses),
  3.	The total number of read misses,
  4.	The percentage of read misses (i.e. total read misses divided by total read accesses),
  5.	The total number of write misses,
  6.	The percentage of write misses (i.e. total write misses divided by total write accesses).

As an example, the following is output from the program:

```sh
$ ./cache 2048 64 64 l 429.mcf-184B.trace.txt

offset_size:6
index_size: 9
tag_size:   49
cache_size:2097152
block_size:64
block_num:32768
index_num:512
offset_size:6
index_size:9
tag_size:49
assoc:64
miss_cnt:55752
hit_cnt:944248
total_cnt:1000000
r_miss_cnt:55703
w_miss_cnt:49
r_cnt:992896
w_cnt:7104

========================================
1.total number of misses:55752
2.percentage of misses:5.5752%
3.total number of read misses:55703
4.percentage of read misses:5.61015%
5.total number of write misses:49
6.percentage of write misses:0.689752%
========================================

```
