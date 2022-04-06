#include <sys/mman.h>   /* mmap()  */
#include <stdlib.h>     /* exit() */
#include <unistd.h>     /* fork() and getpid() */
#include <stdio.h>      /* printf() */
#include <sys/wait.h>   /*wait*/
#include <errno.h>      /* errno */

#define MSGSIZE 5

int main(int argc, char **argv) {

    int pid;    
    int size = MSGSIZE * sizeof(int);
    
    //buat memory map
    void *addr = mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    //printf("Mapped at %p\n", addr);
    int *shared = addr;
    int *shared_idx = mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    *shared_idx = 0;

    // input jumlah proses berdasarkan masukkan user
    printf("Masukkan jumlah fork proses :\n");
    int count;
    scanf("%d", &count);

    // looping jumlah fork proses
    for (int j = 0; j < count; j++)
    {
        pid = fork();
    }

    switch (pid) {
    case 0:         /*  proses anak */
        printf("Proses Produce \n");
        srand(getpid()); //seed random berdasarkan pid

        for (int i = 0; i < 5; i++)
        {
            shared[*shared_idx] = rand() % 10;                 // tulis data angka random ke pipe 
            printf("produsen (pid:%d) menghasilkan: %d\n", getpid(), shared[*shared_idx]);
            *shared_idx += 1;
            sleep(1);
        }
        
        
        break;
    default:        /*  ortu */
        printf("Proses Consume \n");
        int pidWait, status, total_konsum = 0;

        /* wait sampai child selesai */
        while (pidWait = wait(&status)) {   

            if (pidWait == pid)  /* child sukses selesai*/

                //cetak isi memory map
                for (int i = 0; i<MSGSIZE; i++) {
                    printf("konsumen (pid:%d) mengambil: %d \n", getpid(), shared[*shared_idx - 1]);
                    total_konsum += shared[*shared_idx - 1];      // jumlahkan angka
                    *shared_idx -= 1;
                    sleep(1);
                }
                break; //keluar dari loop wait

            if ((pidWait == -1) && (errno != EINTR)) {
                /* ada error*/
                perror("waitpid");
                exit(1);
            }
        }
        // tampilkan hasil penjumlahan
        printf("total konsumsi komsumen (pid:%d) : %d \n", getpid(),total_konsum);
        break;

    case -1:        /* error */
        perror("fork");
        exit(1);
    }
    exit(0);

    return 0;
}

