#include <stdlib.h>     /* exit() */
#include <unistd.h>     /* fork() and getpid() */
#include <stdio.h>      /* printf() */
#include <string.h>

int main(int argc, char **argv) {

    int pid;
    int fd[2];  // pipe

    // create pipe
    if (pipe(fd) < 0) {
        exit(1); //error
    }

    // input jumlah proses berdasarkan masukkan user
    printf("Masukkan jumlah fork proses :\n");
    int count;
    scanf("%d", &count);

    // looping jumlah proses
    for (int j = 0; j < count; j++)
    {
        pid = fork();
    }


    switch (pid) {
    case 0: /* fork returns 0 ke proses anak */
        
        printf("Proses Produce \n");
        int i;
        int msg1;
        
        /* tutup bagian input dari pipe */
        close(fd[0]);

        srand(getpid()); //seed random berdasarkan pid

        /* tulis ke pipe */
        for(i=0; i<5; i++){
            msg1 = rand() % 10;                 // tulis data angka random ke pipe 
            write(fd[1], &msg1, sizeof(msg1));  // tulis ke pipe
            printf("produsen (pid:%d) menghasilkan: %d\n", getpid(), msg1);
            sleep(1);
        }
        break;

    default: /* fork returns pid ke proses ortu */
        
        printf("Proses Consume \n");
        int total_konsum = 0;
        int y;
        
        /* tutup bagian output dari pipe */
        close(fd[1]);
        
        /* baca yang ditulis child dari pipe */
        for(i=0; i<5; i++){
            read(fd[0], &y, sizeof(y)); 
            printf("konsumen (pid:%d) mengambil: %d \n", getpid(), y);
            total_konsum += y;      // jumlahkan angka
            sleep(1);
        }
        // tampilkan hasil penjumlahan
        printf("total konsumsi komsumen (pid:%d) : %d \n", getpid(),total_konsum);
        break;

    case -1: /* error */
        perror("fork");
        exit(1);
    }
    
    exit(0);
}
