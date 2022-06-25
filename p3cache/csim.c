#define _GNU_SOURCE
#include<getopt.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "cachelab.h"

typedef struct {
    char operation;
    uint64_t address;
    int bytes;
} instruction_t;

typedef struct {
    int valid;
    int timestamp;
    uint64_t tag;
} cacheline_t;

typedef struct {
    int s; //set bits
    int E; //# of lines in a set
    int b; //block-offset bits
    int S; //total # of sets
} args_t;

typedef struct {
    cacheline_t *cachelines;
} set_t;

typedef struct {
    set_t *sets;
} cache_t;

FILE *input;
char *trace_name;

args_t *parse_command_line(int argc, char *argv[])
{
    args_t *args = (args_t*)malloc(sizeof(args_t));
    getopt(argc, argv, "s:");
    args->s = atoi(optarg);
    args->S = pow(2, args->s);
    getopt(argc, argv, "E:");
    args->E = atoi(optarg);
    getopt(argc, argv, "b:");
    args->b = atoi(optarg);
    getopt(argc, argv, "t:");
    trace_name = optarg;
    printf("%s \n", trace_name);
    return args;
}

/*parse_line parses one line of the trace file to separate the operation, 
address and number of bytes and store them in their respective fields in the
cacheline_t struct defined above*/
instruction_t *parse_line(char *line)
{
    char operation;
    uint64_t address;
    int bytes;
    instruction_t *life_info = (instruction_t*)malloc(sizeof(instruction_t));
    sscanf(line, " %c %lx,%d", &operation, &address, &bytes);
    life_info->operation = operation;
    life_info->address = address;
    life_info->bytes = bytes;
    return life_info;
}

int hit_count = 0;
int miss_count = 0;
int eviction_count = 0;

void process_helper(instruction_t *line, args_t* args, cache_t* cache, int timestamp)
{
     int is_hit = 0;
    int evict_index = 0;
    int empty_index = -1;
    
    int tag = (64 - (args->s + args->b));
    uint64_t address_tag = line->address >> (args->s + args->b);
        uint64_t temp = line->address << tag;
        uint64_t set_id = temp >> (tag + args->b);
        set_t set = cache->sets[set_id];
        int low = set.cachelines[0].timestamp;
        for (int i = 0; i < args->E; i++) {
            if (set.cachelines[i].valid == 1) {
                if (set.cachelines[i].tag == address_tag) {
                    hit_count++;
                    is_hit = 1;
                    set.cachelines[i].timestamp = timestamp;
                } else if (set.cachelines[i].timestamp < low) {
                    low = set.cachelines[i].timestamp;
                    evict_index = i;
                }
                } else if (empty_index == -1) {
                    empty_index = i;
                }
            }
            if (is_hit != 1) {
                miss_count++;
                if (empty_index > -1) {
                    set.cachelines[empty_index].valid = 1;
                    set.cachelines[empty_index].tag = address_tag;
                    set.cachelines[empty_index].timestamp = timestamp;
                } else if (empty_index < 0) {
                    eviction_count++;
                    //is_evict = 1;
                    set.cachelines[evict_index].tag = address_tag;
                    set.cachelines[evict_index].timestamp = timestamp;
                }
}
}
int *process(int argc, char *argv[])
{
    int *arr = (int*)malloc(sizeof(int) * 3);
    args_t *args = parse_command_line(argc, argv);
    input = fopen(trace_name, "r");
    cache_t *cache = (cache_t*)malloc(sizeof(cache_t));
    cache->sets = malloc(sizeof(set_t) * args->S);
    for (int i = 0; i < args->S; i++) {
        cache->sets[i].cachelines = malloc(sizeof(args_t) * args->E);
        for (int j = 0; j < args->E; j++) {
            cache->sets[i].cachelines[j].valid = 0;
            cache->sets[i].cachelines[j].tag = 0;
            cache->sets[i].cachelines[j].timestamp = 0;
        }
    }
    char *buffer = NULL;
    size_t buffer_size = 0;
    int count = 0;
    ssize_t line_size;
    if (input == NULL)
    printf("NULL");
    line_size = getline(&buffer, &buffer_size, input);
    while (line_size >= 0) {
        count++;
        instruction_t *line = parse_line(buffer);
            if (line->operation == 'L'||line->operation == 'S') 
            {
                process_helper(line, args, cache, count);
            }
            if (line->operation == 'M') 
            {
                process_helper(line, args, cache, count);
                process_helper(line, args, cache, count);
            }
            line_size = getline(&buffer, &buffer_size, input);
        } 
        arr[0] = hit_count;
        arr[1] = miss_count;
        arr[2] = eviction_count;
        return arr;  
    }

int main(int argc, char *argv[])
{
    int *final_arr = process(argc, argv);
    int hitCount = final_arr[0];
    int missCount = final_arr[1];
    int evictionCount = final_arr[2];
    printSummary(hitCount, missCount, evictionCount);
    return 0;
}
