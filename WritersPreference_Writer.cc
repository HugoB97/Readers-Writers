#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <string>
using namespace std;

// Arbitrary number used to identify the semaphore
#define SEMKEY 151
#define SEMKEY2 152
#define SEMKEY3 153
#define SEMKEY4 154

struct sembuf Ssembuf,Wsembuf,S1sembuf,W1sembuf,S2sembuf,W2sembuf,S3sembuf,W3sembuf;

main()
{
    
    int semid, semid2, semid3, semid4, write_count = 0;
    char line[256];
    char answer;

    // Marks a piece of physical memory as shareable and returns an ID
    semid=semget(SEMKEY,1,0777|IPC_CREAT);
    semid2=semget(SEMKEY2,1,0777|IPC_CREAT);
    semid3=semget(SEMKEY3,1,0777|IPC_CREAT);
    semid4=semget(SEMKEY4,1,0777|IPC_CREAT);

    // The below 8 sets of 3 initialises structure members.
    Wsembuf.sem_num=0;
    Wsembuf.sem_op=-1;
    Wsembuf.sem_flg=SEM_UNDO;

    Ssembuf.sem_num=0;
    Ssembuf.sem_op=1;
    Ssembuf.sem_flg=SEM_UNDO;

    W1sembuf.sem_num=1;
    W1sembuf.sem_op=-1;
    W1sembuf.sem_flg=SEM_UNDO;

    S1sembuf.sem_num=1;
    S1sembuf.sem_op=1;
    S1sembuf.sem_flg=SEM_UNDO;

    W2sembuf.sem_num=2;
    W2sembuf.sem_op=-1;
    W2sembuf.sem_flg=SEM_UNDO;

    S2sembuf.sem_num=2;
    S2sembuf.sem_op=1;
    S2sembuf.sem_flg=SEM_UNDO;

    W3sembuf.sem_num=3;
    W3sembuf.sem_op=-1;
    W3sembuf.sem_flg=SEM_UNDO;

    S3sembuf.sem_num=3;
    S3sembuf.sem_op=1;
    S3sembuf.sem_flg=SEM_UNDO;

    ofstream myfile("file.txt");

    while(1){

        // Create a wait operation.
        semop(semid3,&W2sembuf,1);
        
        // Get value the semaphore.
        write_count = semctl(semid3,0,GETVAL);
        
        // Create a wait queue.
        if(write_count == 0){
            semop(semid4, &W3sembuf, 1);
        }

        // Create a signal and then wait operation.
        semop(semid3,&S2sembuf,1);
        semop(semid,&Wsembuf,1);
        
        // Start of critical section
        myfile.open("file.txt", ios::app);
        if(myfile.is_open())
        {
            cin.getline(line,256);
            cout << “Please write to the file.” << endl;
            myfile << line << endl;
        }
        else{
            cout << "File cannot be opened." << endl;
        }
        myfile.close();
        // End of critical section.
        
        // Create a signal operation.
        semop(semid,&Ssembuf,1);
        semop(semid3,&W2sembuf,1);
        
        // Get value the semaphore.
        write_count = semctl(semid3,0,GETVAL);
        if(write_count == 0){
            semop(semid4, &S3sembuf, 1);
        }

        // Create a signal operation.
        semop(semid3,&S2sembuf,1);

    }
    return 0;
}
