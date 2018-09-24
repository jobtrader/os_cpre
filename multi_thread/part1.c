#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

void *runner(void *param); /* threads call this function */
int array_length, file_length;
int *array_master;

FILE *freader;  
int *read_file(char *fname) {
    freader = fopen(fname, "rt"); // open file
    int bufsize = file_length;
    char line[80];
    int integer;
    int index = 0;
    int *input = malloc(bufsize*sizeof(int));


    while (fgets(line, 80, freader) != NULL) {
        sscanf(line, "%d", &integer); // read the integer value
        input[index] = integer;
        ++index;
        ++array_length;
    }
    
    fclose(freader); // close the file    
    
    return input;
}


int read_length(char *fname) {
    freader = fopen(fname,"rt"); // open file
    char line[80];
    int file_length = 0;
    
    while (fgets(line, 80, freader) != NULL) {
        file_length += 1;
    }

    return file_length;
}

void merge(int arr[], int left, int middle, int right) {
    
    int i, j, k;
    int half1 = middle - left + 1;
    int half2 = right - middle;

    int first[half1], second[half2]; // temp arrays

    for (i = 0; i < half1; i++) {
        first[i] = arr[left + i];
    }

    for (j = 0; j < half2; j++) {
        second[j] = arr[middle + 1 + j];
    }
    
    i = 0;
    j = 0;
    k = left;

    while (i < half1 && j < half2) {
        
        if (first[i] <= second[j]) {
            arr[k] = first[i];
            ++i;
        } else {
            arr[k] = second[j];
            j++;
        }

        k++;
    }

    while (i < half1) {
        arr[k] = first[i];
        i++;
        k++;
    }

    while (j < half2) {
        arr[k] = second[j];
        j++;
        k++;
    }
}

void merge_sort(int arr[], int l, int r) {
    if (l < r) {
        int middle = (l + (r-1))/ 2;
        merge_sort(arr, l, middle);
        merge_sort(arr, middle+1, r);
        merge(arr, l, middle, r);
    }
}


int main(int argc, char *argv[])
{   
    char *fname = argv[1];
    file_length = read_length(fname);
    array_master = read_file(fname);
    int NTHREADS = file_length / 100;


    if (NTHREADS < 3) NTHREADS = 3;
    else if (NTHREADS > 10) NTHREADS = 10;
    pthread_t thread_id[NTHREADS];
    unsigned int ints[NTHREADS];
    for(unsigned int i=0; i < NTHREADS; i++){
        ints[i] = i;
        pthread_create( &thread_id[i], NULL, runner, &ints[i]);
    }
    for(int j=0; j < NTHREADS; j++)
        pthread_join( thread_id[j], NULL);

    int j;
    for (j = 0; j < array_length; j++) {
        if (j == array_length-1) {
            printf("%d", array_master[j]);
        } else {
            printf("%d, ", array_master[j]);
        }
    } 
    
    pthread_exit(0);    
    
    return 0;
}


void *runner(void *param)
{   
    unsigned int i = *((unsigned int *) param);
    int start_index = i * array_length;
    int stop_index = ((i + 1) * array_length) - 1;
    merge_sort(array_master, start_index, stop_index);

    pthread_exit(0);
}
