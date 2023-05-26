#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *greet(void *data);

// procedure main:
int main(void)
{
    // create_thread(greet)
    pthread_t thread;
    size_t dato = 2;
    int error = pthread_create(&thread, /*thread atributes*/ NULL, greet, /*function arguments*/ (void*)dato);
    if (error == EXIT_SUCCESS)
    {
        // print("Hello from main thread")
        printf("Hello from main thread\n");
        pthread_join(thread, NULL);
    }
    else
    {
        fprintf(stderr, "Error: could not create secondary thread");
    }
    return error;
} // end procedure

// procedure greet:
void *greet(void *data)
{
    size_t data_1 = (size_t)data;
    // print("Hello from secondary thread")
    if (data_1 > 0)
    {
        printf("Saludo %zu de el hilo 2\n",data_1);
        data_1--;
        greet((void*)data_1);
    }else if(data_1 == 0){
        printf("Adios de #%zu\n",data_1);
    }
    return NULL;
} // end procedure
