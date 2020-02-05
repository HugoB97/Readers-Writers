#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <string>
using namespace std;
#define SEMKEY 151
#define SEMKEY2 152


struct sembuf Ssembuf,Wsembuf,S1sembuf,W1sembuf;

main()
{
    int semid, shmid, *ptr;
    char line[256];

    // Marks a piece of physical memory as shareable and returns an ID that can be identified.
    semid=semget(SEMKEY,1,0777|IPC_CREAT);
    semid=semget(SEMKEY2,1,0777|IPC_CREAT);

    // Intialise and set value for semaphores.
    semctl(semid, 0, SETVAL, 1);
    semctl(semid, 1, SETVAL, 1);

    // The below 4 sets of 3 initialises structure members.
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

    ofstream myfile("file.txt");
    
    while(1){
        
        // Create a wait operation.
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
        // End of critical section
        
        // Create a signal operation.
        semop(semid,&Ssembuf,1);
    }
    return 0;
}
