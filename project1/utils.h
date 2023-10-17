#pragma once
#include <unistd.h>

#define MAX_CHILDREN 24

typedef struct segment{

    int seg_id;
    int seg_size;
    char** entries; // this will possibly be entry type

}Segment;

typedef struct memory{

    Segment curr_segment;
    sem_t curr_semaphore;
    sem_t binary_sem; // father locks with this
    int requested_segments[MAX_CHILDREN];
    int shm_id;
    
}Memory;

void child_birth(int n_segments, int n_children, int requests, int lines_per_segment, int line_count, int s_grade);

Segment **initialize_segments(int line_count, int s_grade, char **str_arr);
Memory *initialize_memory(int n_segments);
