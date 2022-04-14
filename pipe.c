#include <stdlib.h>     /* exit() */
#include <unistd.h>     /* fork() and getpid() */
#include <stdio.h>      /* printf() */
#include <sys/wait.h> /*wait*/

int main(int argc, char **argv) {

    int fd[2];  // pipe

    // create pipe
    if (pipe(fd) < 0) {
        exit(1); //error
    }

    // input jumlah proses berdasarkan masukkan user
    printf("Masukkan jumlah fork proses produsen : ");
    int countp;
    scanf("%d", &countp);
    printf("Masukkan jumlah fork proses konsumen : ");
    int countk;
    scanf("%d", &countk);
    

    int pid_main = fork();
    switch (pid_main){
    case 0: /* fork returns 0 ke proses anak */

        // lakukan fork sebanyak inputan user
        for (int p = 1; p < countp; p++){
            fork();
        }
        
        printf("Proses Produce \n");
        int msg1;
        
        /* tutup bagian input dari pipe */
        close(fd[0]);

        srand(getpid()); //seed random berdasarkan pid

        /* tulis ke pipe */
        for(int i=0; i<2; i++){
            msg1 = rand() % 10;                 // tulis data angka random ke pipe 
            write(fd[1], &msg1, sizeof(msg1));  // tulis ke pipe
            printf("produsen (pid:%d) menghasilkan: %d\n", getpid(), msg1);
            sleep(1);
        }
        break;

    default: /* fork returns pid ke proses ortu */
        printf("\n");
        int pid_c = fork();

        switch(pid_c){
        case 0:

            // lakukan fork sebanyak inputan user
            for (int k = 1; k < countk; k++){
                fork();
            }


            printf("Proses Consume \n");
            int total_konsum = 0, y;
            

            /* tutup bagian output dari pipe */
            close(fd[1]);
            
            /* baca yang ditulis child dari pipe */
            for(int i=0; i<2; i++){
                read(fd[0], &y, sizeof(y)); 
                printf("konsumen (pid:%d) mengambil: %d \n", getpid(), y);
                total_konsum += y;      // jumlahkan angka
                sleep(1);
            }
            // tampilkan hasil penjumlahan
            printf("total konsumsi konsumen (pid:%d) : %d \n", getpid(),total_konsum);
            break;
        
        default:
            // wait sampai proses child selesai
            wait(NULL);
            break;
        }

    }
    
    exit(0);
}
