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

void child_birth(int n_segments, int n_children, int requests, int lines_per_segment, int line_count, int s_grade) {

    // create semaphores equal to segments
    sem_t semaphore[n_segments];

    for (int i = 0; i < n_segments; ++i) {
        sem_init(&semaphore[i], 1, 0);
    }

    Memory *shared_mem = initialize_memory(n_segments); // initialize shared memory
    if (shared_mem == NULL) {
        exit(EXIT_FAILURE);
    }
    
    // create number of children given
    pid_t pid; // usually an int
    for (int i = 0; i < n_children; i++) {

        pid = fork();

        if (pid == -1) {
            perror("Fork error\n");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0){ // this is the child process
            
            int my_id = i;
            // create log
            srand(time(NULL) - getpid()); // seed rand
            int prev_request = -1; // used for odds
            int line_request = -1;

            for (int j = 0; j < requests; j++) {
            
                // request line
                if (prev_request > -1) { 

                    int odds = (rand() % 100);
                    if(odds <= 70){
                        line_request = prev_request; // chance to be the same segment
                    }else{
                        line_request = (rand() % (line_count)); // chance to be new segment
                    }
                }
                else
                {
                    line_request = (rand() % (line_count)); // chance to be new segment
                }
                
                prev_request = line_request; //
                int seg_request = (line_request/s_grade) - 1;
                int line = (line_request%s_grade);
                shared_mem->requested_segments[my_id] = line_request; // give request

                printf("<%d,%d>\n", seg_request+1,line);
            }
            
            // printf("HELLO FROM CHILD\n");
            exit(EXIT_SUCCESS);
            // wait for segment
            // execute request

            

            // after that's done parent calls segment back with FIFO logic or leaves it untouched if it is still needed
        }
    }

    // parent is waiting for children to terminate
    for (int i = 0; i < n_children; i++)
        wait(NULL);

    // destroy semaphores
    for (int i = 0; i < n_segments; i++){
        sem_destroy(&semaphore[i]);
    }
}

Segment **initialize_segments(int line_count, int s_grade, char **str_arr) {

    int n_segments = line_count / s_grade; // number of segments = lines/segmentation_grade
    if (line_count % s_grade != 0)
        n_segments++; // one extra segment for the remaining lines if there are any

    int lines_per_segment = line_count / n_segments; // calculate lines per segment
    if (line_count % n_segments != 0)
        lines_per_segment++;
    
    Segment **segments = malloc(n_segments * sizeof(Segment*));

    for (int i = 0; i < n_segments; i++) {
        segments[i] = malloc(sizeof(Segment));
        segments[i]->seg_id = i;

        if (i == n_segments-1 && line_count%s_grade != 0){
            
            segments[i]->seg_size = line_count%s_grade;

        }else{segments[i]->seg_size = lines_per_segment;}

        segments[i]->entries = malloc(segments[i] -> seg_size * sizeof(char*));

        for (int j = 0; j < segments[i]->seg_size; j++) {

            segments[i] -> entries[j] = malloc(strlen( (str_arr[i * s_grade + j]) + 1) * sizeof(char));
            memcpy(segments[i]->entries[j], str_arr[i * s_grade + j], strlen((str_arr[i * s_grade + j])));
        }

    }

    return segments;
    
}

Memory *initialize_memory(int n_segments){

    int id;
    key_t key;

    Memory *shm_ptr; // pointer to shared memory

    key = ftok(".", 'a');

    id = shmget(key, sizeof(Memory), IPC_CREAT | 0666);
    if (id == -1) {
        perror("shmget error\n");
        return NULL;
    }
    
    shm_ptr = (Memory *)shmat(id, NULL, 0);
    if (shm_ptr == (void *)-1) {
        perror("shmat error\n");
        return NULL;
    }

    shmctl(id, IPC_RMID, NULL); // when fully detached, destroy.
    shm_ptr -> shm_id = id;

    // initialize curr_segment also
    memset(&(shm_ptr->curr_segment), 0, sizeof(Segment));
    // initialize curr_semaphore
    sem_init(&(shm_ptr->curr_semaphore),1,0);

    return shm_ptr;

}
