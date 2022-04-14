#include <sys/mman.h>   /* mmap()  */
#include <stdlib.h>     /* exit() */
#include <unistd.h>     /* fork() and getpid() */
#include <stdio.h>      /* printf() */
#include <sys/wait.h>   /*wait*/
#include <errno.h>      /* errno */

#define MSGSIZE 5

int main(int argc, char **argv) {

    int size = MSGSIZE * sizeof(int);
    
    //buat memory map
    void *addr = mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    int *shared = addr;
    int *shared_idx = mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    *shared_idx = 0;
    int *terminate = mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    *terminate = 0;

    // input jumlah proses berdasarkan masukkan user
    printf("Masukkan jumlah fork proses produsen : ");
    int countp;
    scanf("%d", &countp);
    *terminate = countp;
    printf("Masukkan jumlah fork proses konsumen : ");
    int countk;
    scanf("%d", &countk);

    int pid_main = fork();
    switch (pid_main) {
    case 0:         /*  proses produce */

        // lakukan fork sebanyak inputan user
        for (int p = 1; p < countp; p++){
            fork();
        }

        //sleep(5);
        printf("Proses Produce \n");
        srand(getpid()); //seed random berdasarkan pid

        for (int i = 0; i < 1; i++){
            shared[*shared_idx] = rand() % 10;                 // tulis data angka random ke pipe 
            printf("produsen (pid:%d) menghasilkan: %d\n", getpid(), shared[*shared_idx]);
            *shared_idx += 1;
            sleep(1);
        }
        
        *terminate = *terminate - 1;
        break;
    default:        /*  consume */
        printf("\n");
        int pid_c = fork();
        switch(pid_c){
        case 0:
            // lakukan fork sebanyak inputan user
            for (int k = 1; k < countk; k++){
                fork();
            }
            
            printf("Proses Consume \n");
            int total_konsum = 0;

            //cetak isi memory map
            for (int i = 0; i<1; i++) {
            
                if(*shared_idx != 0){
                    printf("konsumen (pid:%d) mengambil: %d \n", getpid(), shared[*shared_idx - 1]);
                    total_konsum += shared[*shared_idx - 1];      // jumlahkan angka
                    *shared_idx -= 1;
                    
                }else{
                    if(*terminate != 0){
                        while((*shared_idx == 0) && (*terminate != 0)){
                            //printf("waiting...\n");
                            sleep(1);
                        }
                        i--;
                    }
                }
            
                sleep(1);
            }
            // tampilkan hasil penjumlahan
            printf("total konsumsi konsumen (pid:%d) : %d \n", getpid(),total_konsum);
            break;
        
        default:
            wait(NULL);
            break;
        }
        
        break;
    }

    exit(0);

    return 0;
}
