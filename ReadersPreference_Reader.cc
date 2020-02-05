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
    
    int semid, semid2, read_count = 0;
    string line;
    char input;
    
    // Marks a piece of physical memory as shareable and returns an ID that can be identified.
    semid=semget(SEMKEY,1,0777|IPC_CREAT);
    semid2=semget(SEMKEY2,1,0777|IPC_CREAT);

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

    ifstream myfile("file.txt", ios::in);

    while(1){
        cout << "Press 'y' to read: ";
        cin >> input;
        
        // Begin the read.
        if(input == 'y'){
            // Create a wait operation.
            semop(semid2,&W1sembuf,1);
            
            // Get value of the semaphore.
            read_count = semctl(semid2,0,GETVAL);
            
            // Open semaphore, create a wait operation.
            if(read_count == 0){
                semop(semid, &Wsembuf, 1);
            }
            // Create a signal operation.
            semop(semid2,&S1sembuf,1);
            
            // Start of critical section
            myfile.open("file.txt");
            if(myfile.is_open())
            {
                while(getline(myfile, line))
                {
                    cout << line << endl;
                }
                myfile.close();
            }
            // End of critical section

            // Create a wait operation.
            semop(semid2,&W1sembuf,1);
            
            // Get value of the semaphore.
            read_count = semctl(semid2,0,GETVAL);
            
            // Create a signal operation.
            if(read_count == 0){
                semop(semid, &Ssembuf, 1);
            }
            // Create a signal operation.
            semop(semid2,&S1sembuf,1);
        }
        input = 'quit';
    }
    return 0;
}
