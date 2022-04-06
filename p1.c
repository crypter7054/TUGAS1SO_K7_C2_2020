#include <stdio.h>      /* printf() */
#include <stdlib.h>     /* exit() */
#include <unistd.h>     /* fork() and getpid() */
#include <string.h>
#include <sys/mman.h>   /* mmap()  */
#include <sys/wait.h>   /*wait*/
#include <errno.h>	/* errno */
#include <time.h>
// #include <conio.h>

#define MSGSIZE 10

int main(int argc, char const *argv[])
{
        int inbuf[MSGSIZE]; //buffer
        int fd[2];//untuk pipe
        
        //buat pipe
        if (pipe(fd) < 0) {
                exit(1); // pipe error
        }

        int pid = fork();

        switch (pid) {
        case 0:
                close(fd[0]);
                int x;
                
                // seed berdasarkan waktu
                srand(time(NULL));
                x = rand() % 10 + 1; //random angka 1 sampai 10
                printf("produsen membuat: %i \n", x);
                if (write(fd[1], &x, sizeof(int)) == -1)
                {
                        printf("error writing to the pipe\n");
                }
                close(fd[1]);
                break;
        
        default:
                close(fd[1]);
                int y;
                if(read(fd[0], &y, sizeof(int)) == -1){
                        printf("error reading from the pipe\n");
                }
                close(fd[0]);
                printf("Konsumen menerima %d\n", y);
                break;
        
        case -1: /* error fork */
                perror("fork");
                exit(1);

        }
        
        return 0;
}


// int main(int argc, char const *argv[])
// {
//         int inbuf[MSGSIZE]; //buffer
//         int pid;
//         int fd[2]; //pipe

//         //create pipe
//         if (pipe(fd) < 0) {
//             exit(1); //error
//         }

//         printf("mulai \n");
        
//         int test1 = rand();
//         // int test2 = rand();
//         printf("ini test : %d\n", test1);               
//         // printf("ini test : %d\n", test2);

//         switch (pid = fork()) {
//         case 0:         /* fork returns 0 ke proses anak */
//                 printf("Proses anak \n");
//                 // tulis data ke pipe
//                 int msg1  = rand(); // random angka 1 sampai 10
//                 /* tutup bagian input dari pipe */
//                 close(fd[0]);
//                 // tulis ke pipe
//                 write(fd[1], msg1, MSGSIZE);
//                 break;
//         default:        /* fork returns pid ke proses ortu */
//                 printf("Proses ortu\n");
//                 /* tutup bagian output dari pipe */
//                 close(fd[1]);
//                 // baca yang ditulis child dari pipe
//                 read(fd[0], inbuf, MSGSIZE); //buffer terisi
//                 printf("proses child menulis pesan ke-1: %i \n", inbuf[0]);
//                 break;
//         case -1:        /* error */
//                 perror("fork");
//                 exit(1);
//         }
//         exit(0);



//         return 0;
// }

