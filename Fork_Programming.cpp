//Steven Centeno

// affiliated programs:
//kill_patch.h
//kill_patch64.o

//fork programming

//This program is supposed to:
//
//	I). print 2 options to choose from 1. Run Program, 2. Exit Program
//
// II). ask the user for a letter to search for in the size of the array.
//
//III). ask the user for the size of the array
//
// IV). spawns child process (after asking for the size fo the new array), 
//		to fill the array with raw bytes of upper Random capital letters
//
//  V). child process searches the array for the inputted letter and output 
//		the number of occurrances of said letter. Each child should only 
//		read 1 letter and terminate after reading said letter. THen after outputting
//		the total number of letters terminate the whole program.
//
// VI). when the software (child process) cannot read any number of occurrences, 
//		it restarts the search in the array and as a result, needs another terminal to
//		enter a kill command to kill the child
//

#include <unistd.h>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <memory>
#include <random>
#include <algorithm>
#include <cstdlib>
#include <string>
#include <cstring>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "kill_patch.h"
using namespace std;


//initialize methods for the program
bool menuSelection();
void createChild();
char* createRandArr(int arrSize);

//global variables
char searchChar;
int arrSize;
int len;


//the buffer that is used for the force terminate
struct buf {
    long mtype;
    char greeting[50]; 
};

//initializing the buffer
buf msg;





/*
* Creates a random capital character bewteen A and Z and returns an array holding these random characters
**/
char* createRandArr(int arrSize) {
	
	int randNum;
	char randChar;
	char *array = new char[arrSize];

	//iterates up to the size of the array and 
	for (int Itr = 0; Itr < arrSize; Itr++)
	{
		randNum = rand() % 26;
		randChar = 'A' + randNum;
		array[Itr] = randChar;
	}

	return array;
};





/*
* creates the child to both create the array and search for the first occurance of 
* the letter in the array, once the first occurrence is found, that child will make 
* a new child to find the next occurrence of the letter and so on. This will happen until
* every element in the array is searched through. Additionally, if no letter is found it will
* loop until a new terminal executes the child process, so the parent process may continue
**/
void createChild() {

	//retrieves the mesage queue ID that is active, will return a -1 if its not found
	//this is so the child will be able to be terminated later
	int qid = msgget(ftok(".", 'u'), 0);

	cout << "Child will be created to make the array" << "\n" << endl;
	long childPID;

	//need these two as they will be used to read the child memories later
	int numOfDesiredLetter = 0;
	int *PointerLetterCount;

	//points to address of the int
	PointerLetterCount = &numOfDesiredLetter;

	//the child process is made
	childPID = fork();

	//this is the parent process
	if(childPID > 0) 
	{

		cout << "Parent process; pid: " << getpid()
			<< ", ppid (parent ID): " << getppid()
			<< ", child will be: " << childPID << "\n" << endl;
		
		//should wait until the child process is terminated
		wait(NULL);

		cout << "all children terminated" << "\n" << endl;
	} 
	
	//this is the child process
	//this process will be used to create an array and then search for the first occurrence
	//it will also be terminated if a kill command is used in another terminal
	//also creates new children if the first occurrence is found, the new children searches 
	//for the next occurrence also counting the total number of ocurrences and continuously does so until the 
	//end of the array is reached
	else if (childPID == 0) 
	{

		cout << "Child process; pid: " << getpid()
			<< ", ppid " << getppid()
			<< ", child will be (should be 0 because hasnt run):  " 
			<< childPID << "\n" << endl;
		
		//creates the array using the inputs given earlier by the user and saves it into a variable
		char* userArr = createRandArr(arrSize);
		cout << "the array is: " << userArr << "\n" << endl;

		//now looks through the array to get the letter the user wants
		//if it does not exist, will loop until a kill command is used to end the process of the child

		//uses the kill patch to end the process if it is manually cut off from
		//another terminal
		buf exitMsg;
    	len = sizeof(msg)-sizeof(long);
    	exitMsg.mtype = 1;
    	strcpy(exitMsg.greeting, "C_Leaves");
    	kill_patch(qid, (struct msgbuf *)&exitMsg, len, 1);

		//the loop if there isnt a first occurrence (hasnt found the letter)
		while(*PointerLetterCount < 1) 
		{

			//to ensure the last child is the only one displaying the total number of occurrences, 
			//will have this flag that only allows the last child that actually exits the for loop correctly, 
			//to display the total number of occurrences
			bool processFoundElement = false;
			
			//loops through the array, searching for a character
			for(int i = 0; i < arrSize; i++) 
			{

				//if the child finds a character, it will create a new child to find the next character
				//with this the pointer will add to the num 
				if(char(userArr[i]) == searchChar) 
				{

					//changes the count of letters by incrementing it by one, since its an address in memory
					//despite being different processes, it should add to the total num of letters
					*PointerLetterCount += 1;
					
					//forks another child into existence to find the next occurrence of the letter
					long childsChildPID = 0;
					childsChildPID = fork();

					//parent process
					//will wait for child proces to finish then exit and exit the process
					if(childsChildPID > 0) 
					{

						cout << "New Parent process (child); pid: " << getpid()
							<< ", ppid (parent ID): " << getppid()
							<< ", child will be: " << childsChildPID << "\n" << endl;

						//waits for other children processes to end
						wait(NULL);

						//delcares that the process found a new child so that this parent will not display
						//the total number of occurrences, only the child that exits the loop naturally will
						//display the total count of occurrences
						processFoundElement = true;

						//now leaves the while loop
						break;
					}

					//child process
					else if(childsChildPID == 0) 
					{

						cout << " New Child process (child of child); pid: " << getpid()
							<< ", ppid (parent ID): " << getppid()
							<< ", child will be (should be 0 because hasnt run):  " 
							<< childsChildPID  << "\n" << endl;
						
						//just continue with the loop so doent do anything
					}

					else 
					{
						cout << "error child of child not made" << endl;
					}
				}
			}

			//displays the amount of elements only when its the last child process made that exited the for loop
			//naturally, so that the true number of elements is displayed before exiting
			if(!processFoundElement and *PointerLetterCount > 0)
			{
				//displays the letter count before termination
				cout << "the total number of the letter " << searchChar 
				<< " in the array is: " << *PointerLetterCount << "\n" << endl;
			}
		}

		//child process exits
		exit(0);
	} 
	
	else 
	{
		cout << "Spawn error - no child process" << endl;
	}
};





/*
* prints the two options to select from and if the user selects to exit the program
* the method will return true to tell the program to quit, otherwise continue
**/
bool menuSelection() {
	
	int option;
	cout << "Please select an option (enter the number): \n\n1. Run the program\n2. Exit the program\n" << endl;
	cin >> option;

	//if the user runs the job, they will be prompted to enter both the array size and the letter to search for
	//these variables are saved globally, so that every proceeding function will easily access these static variables
	if(option == 1) 
	{
		cout << "\nenter the Letter to search for in the array:\n" << endl;
		cin >> searchChar;
		cout << "\nenter the size of the array to search through" << endl;
		cin >> arrSize;
		return false;
	}

	else
	{
		return true;
	}
};





int main() {

	//program continuously repeats until the user exits the program
	bool userHasntExit = true;
	while (userHasntExit) 
	{
		
		//checks to see if the user exited, if not will get the array size, and the letter to search
		//then makes child processes to search the array for the amount of letters existing in the array
		bool TerminateTheProcess = menuSelection();

		if(TerminateTheProcess) 
		{ 
			userHasntExit = false;
		}

		//searches the array if the user has not exit the program
		else 
		{
			createChild();
		}
	}
};