#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct node {
    unsigned int tag;
    unsigned int block;
    int valid;
};

int hit = 0;
int miss = 0;
int evictions = 0;

void printBinary(unsigned int num, unsigned int n) {
    int i, mask;
    for(i = n - 1; i >= 0; i--) {
        mask = 1 << i;
        printf("%d", (num & mask) ? 1 : 0);
    }
    printf("\t\t");
}

void display(struct node *cache, unsigned int lines, unsigned int tag_bits) {
    int i;
    printf("Index \t Valid \t Tag \t\t\t Data(decimal)\n");
    for(i = 0; i < lines; i++) {
        if (cache[i].tag != -1) {
            printf("%u \t %d \t", i, cache[i].valid);
            printBinary(cache[i].tag, tag_bits);
            printf(" Block %u \n", cache[i].block);
        } else {
            printf("%u \t %d \t - \t\t\t %u \n", i, cache[i].valid, cache[i].block);
        }
    }
}

void directMapping(struct node *cache, unsigned int ref, unsigned int offset_bits, unsigned int index_bits, unsigned int tag_bits) {
    int offset = ref & ((1 << offset_bits) - 1);
    int temp = ref >> offset_bits;
    int index = temp & ((1 << index_bits) - 1);
    int tag = temp >> index_bits;

    printf("\n");
    printf("----------------------INSTRUCTION BREAKDOWN----------------------\n");
    printf("Tag \t\t\t Index \t\t Offset \n");
    printf("%u \t\t\t %u \t\t %u \n", tag, index, offset);
    printBinary(tag, tag_bits);
    printBinary(index, index_bits);
    printBinary(offset, offset_bits);
    printf("\n");
    printf("%d bits \t\t %d bits \t %d bits\n", tag_bits, index_bits, offset_bits);
    printf("------------------------------------------------------------------\n");
    printf("\n\n");

    if(cache[index].valid == 0) {
        printf("Miss\n");
        miss++;
        cache[index].tag = tag;
        cache[index].valid = 1;
        cache[index].block = temp;
    } else {
        if(cache[index].tag != tag) {
            printf("Miss\n");
            miss++;
            evictions++;
            cache[index].tag = tag;
            cache[index].block = temp;
        } else {
            printf("Hit\n");
            hit++;
        }
    }
}

int is_pwr_of_two(unsigned int n) {
    return (n & (n - 1)) == 0 && n != 0;
}

int main() {
    int i;
    unsigned int mem_size, cache_size, block_size, offset_bits, lines, index_bits, tag_bits;
    int ref;

    printf("Enter the size of main memory: ");
    scanf("%u", &mem_size);
    while(mem_size <= 0 || !is_pwr_of_two(mem_size)) {
        printf("Main memory size should be greater than 0 and a power of 2. Please enter again: \n\n");
        printf("Enter the size of main memory: ");
        scanf("%u", &mem_size);
    }

    printf("Enter the size of the cache: ");
    scanf("%u", &cache_size);
    while (cache_size <= 0 || !is_pwr_of_two(cache_size)) {
        printf("Cache size should be greater than 0 and a power of 2. Please enter again: \n\n");
        printf("Enter the size of the cache: ");
        scanf("%u", &cache_size);
    }

    printf("Enter the block size: ");
    scanf("%u", &block_size);
    while (block_size <= 0 || !is_pwr_of_two(block_size)) {
        printf("Block size should be greater than 0 and a power of 2. Please enter again: \n\n");
        printf("Enter the block size: ");
        scanf("%u", &block_size);
    }

    lines = cache_size / block_size;
    struct node cache[lines];
    offset_bits = log2(block_size);
    index_bits = log2(lines);
    tag_bits = log2(mem_size) - offset_bits - index_bits;

    for(i = 0; i < lines; i++) {
        cache[i].valid = 0;
        cache[i].block = 0;
        cache[i].tag = -1;
    }

    printf("---------------------------CACHE TABLE----------------------------\n");
    display(cache, lines, tag_bits);
    printf("------------------------------------------------------------------\n");

    while(1) {
        printf("\nEnter main memory address (in decimal) (-1 to exit) : ");
        scanf("%d", &ref);
        if(ref == -1) {
            break;
        }
        directMapping(cache, ref, offset_bits, index_bits, tag_bits);
        printf("\n\n");
        printf("---------------------------CACHE TABLE----------------------------\n");
        display(cache, lines, tag_bits);
        printf("------------------------------------------------------------------\n");
        printf("hits: %d misses: %d evictions: %d\n", hit, miss, evictions);
        printf("------------------------------------------------------------------\n");
    }

    printf("------------------------FINAL CACHE TABLE-------------------------\n");
    display(cache, lines, tag_bits);
    printf("------------------------------------------------------------------\n");
    printf("\ntotal hits: %d total misses: %d total evictions: %d\n", hit, miss, evictions);

    return 0;
}
