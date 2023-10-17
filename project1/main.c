#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "utils.h"

int main(int argc, char const *argv[]) {

    int s_grade = atoi(argv[1]);
    int n_children = atoi(argv[2]);
    int requests = atoi(argv[3]);

    if (n_children > MAX_CHILDREN) {
        printf("Too many children...exiting...\n");
        exit(EXIT_FAILURE);
    }
    
    FILE* fp = fopen(argv[4],"r"); // input file directly from command line
    if (fp == NULL) {
        perror("File opening error\n");
        exit(EXIT_FAILURE);
    }
    int line_count = 0;
    char c;

    do{
        c = fgetc(fp);
        if (c == '\n') {
            line_count++;
        }
        // counting file lines to calculate the needed number of segments
    }while(c != EOF);

    fclose(fp);
    fp = fopen(argv[4], "r"); // close and open the file to re-initialize fp for the following process

    char **str_arr = malloc(line_count * sizeof(char *)); // this holds every line before segmentation

    char *line_buffer = NULL;
    size_t len = 0;
    ssize_t nread;
    
    int i = 0; 
    while (getline(&line_buffer,&len, fp) > 0) { 
        
        str_arr[i] = malloc((len+1) * sizeof(char));
        memcpy(str_arr[i], line_buffer, len); 
        
        // restart loop values
        line_buffer = NULL; 
        len = 0;
        ++i;
    }

    int n_segments = line_count / s_grade;
    int lines_per_segment = line_count / n_segments;

    if (line_count % s_grade != 0) n_segments++;

    Segment **segments = initialize_segments(line_count,s_grade,str_arr); // these are the segments we will use

    // free str_arr
    for (int i = 0; i < line_count; i++) {
        free(str_arr[i]);
    }
    free(str_arr);

    // for (int i = 0; i < size; i++){
        
    //     printf("Segment id : %d\n\n",segments[i]->seg_id);
    //     printf("Segment seg_size : %d\n\n", segments[i]->seg_size);
    //     printf("Segment 0 entry : %s", segments[i]->entries[0]);
    //     printf("Segment 9 entry : %s", segments[i]->entries[(segments[i]->seg_size)-1]);

    // }
    
    child_birth(n_segments, n_children, requests, lines_per_segment, line_count, s_grade);
    
    // free segments
    for (int i = 0; i < n_segments; i++){

        for (int j = 0; j < segments[i]->seg_size; j++) {
            free(segments[i]->entries[j]);
        }
        free(segments[i]->entries);
        free(segments[i]);
        
    }
    free(segments);

    fclose(fp);
    exit(EXIT_SUCCESS);
}