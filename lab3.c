#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>


int XDIM = 10000;
int YDIM = 10000;

// We return the pointer
u_int16_t **alloc_matrix(void) /* Allocate the array */
{
    /* Check if allocation succeeded. (check for NULL pointer) */
    int i, j, k; 
    u_int16_t **array;
    array = malloc(XDIM*sizeof(u_int16_t *));
    for(i = 0 ; i < XDIM ; i++)
        array[i] = malloc(YDIM*sizeof(u_int16_t));
  
    for(j=0; j<XDIM; j++)
        for(k=0; k<YDIM; k++)
            memset(&array[k][j], j, sizeof(u_int16_t));
    return array;
}


void fill(u_int16_t** arr) {
    int i, j;
    time_t t1; 
    srand(time(NULL));
    for(i = 0 ; i < XDIM ; i++)
        for(j = 0 ; j < YDIM ; j++)
            arr[i][j] = (u_int16_t)(rand() % 100);
}

void compute(u_int16_t** arr, int kern[3][3]){
    u_int16_t tmp_sum[9];
    u_int16_t dato, accum;
    int i = 0, j = 0, k = 0, l = 0;
    FILE *archivo;
    archivo = fopen("log.txt", "a");
    while(i < XDIM){
        while(j < YDIM){
            //fprintf(archivo, "processing: %d - %d \n", i, j);
            if(i >= 1 && j >=1 && i < XDIM-1 && j <YDIM-1){
                for(k = 0; k < 3; k++)
                    for(l = 0; l < 3; l++){
                        int x = i + (l-1);
                        int y = j + (k-1);
                        dato = arr[x][y];
                        tmp_sum[l*3+k] = 2*(2*kern[l][k]*dato)*0.001 + 1;
                    }

                accum = 0;
                for(k = 0; k < 3; k++)
                    for(l = 0; l < 3; l++)
                        accum = accum + tmp_sum[k*3+l];
  
            }
            arr[i][j] = accum;
            j++;
        }
    i++;   
}


void print(u_int16_t** arr) {
    int i, j;
    FILE *archivo;
    archivo = fopen("log.txt", "a");
    for(i = 0 ; i < XDIM ; i++)
        for(j = 0 ; j < YDIM ; j++)
            fprintf(archivo, "array[%d][%d] = %f\n", i, j, arr[i][j]);
}

int main(void)
{
    int i = 0;
    u_int16_t **arr;
    int kern[3][3] = {0, -1, 0, -1, 5, -1, 0, -1, 0};

    arr = alloc_matrix();
    fill(arr);
    compute(arr, kern);
    print(arr);

    return 0;
}
