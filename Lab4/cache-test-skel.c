/*
 * CSE 351 Lab 4 (Caches and Cache-Friendly Code)
 * Part 1 - Inferring Mystery Cache Geometries
 *
 * Name(s):  Kevin Kuk, Jaehyeon Ahn
 * NetID(s): 2377312, 2377324
 *
 * NOTES:
 * 1. When using access_cache() you do not need to provide a "real" memory
 * addresses. You can use any convenient integer value as a memory address,
 * you should not be able to cause a segmentation fault by providing a memory
 * address out of your programs address space as the argument to access_cache.
 *
 * 2. Do NOT change the provided main function, especially the print statement.
 * If you do so, the autograder may fail to grade your code even if it produces
 * the correct result.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "support/mystery-cache.h"


/* Returns the size (in B) of each block in the cache. */
int get_block_size(void) {
  int initial_address = 0;
  int size = 0;

  access_cache(initial_address);
  while(access_cache(size) == TRUE) {
    size++;
  }

  return size;
}


/* Returns the size (in B) of the cache. */
int get_cache_size(int block_size) {
  int cache_size = 0;
  int initial_address = 0;
  int check_address = block_size;

  flush_cache();
  access_cache(initial_address);
  
  while(access_cache(initial_address) == TRUE) {
    cache_size = block_size;

    while (cache_size <= check_address) {
      cache_size += block_size;
      access_cache(cache_size);
    }

    check_address += block_size;
  }

  return cache_size;
}


/* Returns the associativity of the cache. */
int get_cache_assoc(int cache_size) {
  int assoc = 0;
  int initial_address = 0;

  flush_cache();
  access_cache(initial_address);
  
  while(access_cache(initial_address) == TRUE) {
    assoc++;

    int index = 0;
    while (index <= assoc) {
      access_cache(index * cache_size);
      index++;
    }
  }

  return assoc;
}


/* Run the functions above on a given cache and print the results. */
int main(int argc, char* argv[]) {
  int size;
  int assoc;
  int block_size;
  char do_block_size, do_size, do_assoc;
  do_block_size = do_size = do_assoc = 0;
  if (argc == 1) {
    do_block_size = do_size = do_assoc = 1;
  } else {
    for (int i = 1; i < argc; i++) {
      if (strcmp(argv[i], "block_size") == 0) {
        do_block_size = 1;
        continue;
      }
      if (strcmp(argv[i], "size") == 0) {
        do_size = 1;
        continue;
      }
      if (strcmp(argv[i], "assoc") == 0) {
        do_assoc = 1;
      }
    }
  }

  if (!do_block_size && !do_size && !do_assoc) {
    printf("No function requested!\n");
    printf("Usage: ./cache-test\n");
    printf("Usage: ./cache-test {block_size/size/assoc}\n");
    printf("\tyou may specify multiple functions\n");
    return EXIT_FAILURE;
  }

  cache_init(0, 0);

  block_size = size = assoc = -1;
  if (do_block_size) {
    block_size = get_block_size();
    printf("Cache block size: %d bytes\n", block_size);
  }
  if (do_size) {
    if (block_size == -1) block_size = get_block_size();
    size = get_cache_size(block_size);
    printf("Cache size: %d bytes\n", size);
  }
  if (do_assoc) {
    if (block_size == -1) block_size = get_block_size();
    if (size == -1) size = get_cache_size(block_size);
    assoc = get_cache_assoc(size);
    printf("Cache associativity: %d\n", assoc);
  }
}
