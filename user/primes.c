#include "kernel/types.h"
#include "user/user.h"
#include "kernel/stat.h"
#define PRIME_NUM 35
#define READEND 0
#define WRITEEND 1

void child(int* pl);

int main(int argc, char* argv[])
{
    int p[2];
    pipe(p);
    if(fork()==0){
        child(p);
        close(p[READEND]);
    }
    else{
        close(p[READEND]);
        for(int i=2; i<PRIME_NUM+1; i++){
            write(p[WRITEEND],&i,sizeof(int));
        }
        close(p[WRITEEND]);
        wait((int*)0);
    }
    exit(0);
}

void child(int* pl)
{
    int pr[2];
    int n;
    close(pl[WRITEEND]);
    int read_result=read(pl[READEND],&n,sizeof(int));
    if(read_result==0) exit(0);
    printf("prime %d\n",n);
    int created=0;
    int num;
    while(read(pl[READEND],&num,4)!=0){
        if(created==0){
            pipe(pr);
            created=1;
            if(fork()==0){
                child(pr);
                return;
            }
            else{
                close(pr[READEND]);
            }
        }
        if(num%n!=0){
            write(pr[WRITEEND],&num,sizeof(int));
        }
    }
    close(pl[READEND]);
    close(pr[WRITEEND]);
    wait((int*)0);
}
