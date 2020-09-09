#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>


typedef struct{
    int $promedio;
    char * hist;
    pid_t $pid;
}Hijo;

void printHist(Hijo *, int);
void generateHist(Hijo *, int);
void escribir(int *fd, int);
int leer(int *fd);

    int main(int argc, const char *argv[])
{
    Hijo * pid, * aux, * final;
    int numero, estado, w_time, nproc = 0;
    char buffer[256];
    int fd[2];

    printf("Ingrese la cantidad de procesos: ");
    fgets(buffer, 256, stdin);
    numero = atoi(buffer);

    pid = (Hijo *) malloc(sizeof(Hijo) * numero);
    aux = pid;
    final = aux + numero; 

    for(; aux < final; aux++){

        pipe(fd);
        aux->$pid = fork();

        if (aux->$pid == -1)
        {
            printf("Hubo un error al crear el proceso hijo \n");
            printf("Se crearón: %d hijos. \n", nproc);
            exit(-1);
        }
        else if (aux->$pid == 0)
        {
            ++nproc;
            aux->$pid = getpid();
            aux->$promedio = (aux->$pid + getppid())/2;
            printf("Soy el hijo con PID: %d y mi promedio es: %d\n", aux->$pid, aux->$promedio);
            escribir(fd, aux->$promedio);
            exit(0);
        }
        else
        {
            if (waitpid(aux->$pid, &estado, 0) != -1)
            {
                if (WIFEXITED(estado)){
                    aux->$promedio = leer(fd);
                    generateHist(pid, numero);               
                }
            }
            
        }
    }

    printHist(pid, numero);

    for(; aux < final; ++aux){
        free(aux->hist);
    }
    free(pid);

    return 0;
}

int  leer(int *fd)
{
    int promedio;

    close(fd[1]);
    read(fd[0], &promedio, sizeof(int));

    return promedio;
}
void escribir(int *fd, int promedio)
{
    close(fd[0]);
    write(fd[1], &promedio, sizeof(int));
}

void generateHist(Hijo *pid, int numero)
{
    int histSize;
    Hijo * aux = pid;
    Hijo * final = pid + numero;

    for (; aux < final; ++aux)
    {
        histSize = aux->$promedio * 0.01;
        aux->hist = (char *) malloc(sizeof(char) * histSize);
        while (histSize != 0)
        {
            strcat(aux->hist, "*");
            --histSize;
        }
    }
}

void printHist(Hijo * pid, int numero){

    Hijo * aux = pid;
    Hijo * final = aux + numero;
    
    printf("PID Hijo \t Promedio \t Histograma\n");
    for (; aux < final; ++aux)
    {
        printf("%d \t\t %d \t\t %s\n", aux->$pid, aux->$promedio, aux->hist);
    }
    printf("\n");
}