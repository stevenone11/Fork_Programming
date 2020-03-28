//Steven Centeno
//None
//fork programming

#include <unistd.h>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <memory>
#include <random>
#include <algorithm>
#include <string>
using namespace std;

//This program is supposed to:
//
//	I). print 2 options to choose from 1. Run Program, 2. Exit Program
// II). ask the user for a letter to search for in the size of the array.
//III). ask the user for the size of the array
// IV). spawns child process (after asking for the size fo the new array), 
//		to fill the array with raw bytes of upper Random capital letters
//  V). child process searches the array for the inputted letter and output 
//		the number of occurrances of said letter. Each child should only 
//		read 1 letter and terminate after reading said letter. THen after outputting
//		the total number of letters terminate the whole program.
// VI). when the software (child process) cannot read any number of occurrences, 
//		it restarts the search in the array and as a result, needs another terminal to
//		enter a kill command to kill the child


//initialize methods for the program
bool menuSelection();
void createChild();
vector<char> createRandArr(int arrSize);

//global variables
string searchChar;
int arrSize;


/*
* Creates a random capital character bewteen A and Z and returns it
**/
vector<char> createRandArr(int arrSize) {
	
	int randNum;
	String randChar;
	vector<char> vec;

	//iterates up to the size of the array and 
	for (int Itr = 0; Itr < arrSize; Itr++)
	{
		randNum = rand() % 26;
		randChar = 'A' + randNum;
		vec.push_back(randChar);
	}

	return vec;
};


/*
* creates the child to both create the array and search for the first occurance of 
* the letter in the array and once that occurs, continuously
**/
void createChild(){

	cout << "Child will be created to make the array";
	long childPID;

	//need these two as they will be used to read the child memories later
	int numOfDesiredLetter = 0;
	int *PointerLetterCount;

	//points to address of the int
	PointerLetterCount = &numOfDesiredLetter;

	childPID = fork();

	//this is the parent process
	if(childPID > 0) {

		cout << "Parent process; pid: " << getpid()
			<< ", ppid (parent ID): " << getppid()
			<< ", child will be: " << childPID << endl;
		
		//should wait until the child process is terminated
		wait(NULL);
	} 
	
	//this is the child process
	//this process will be used to create an array and then search for 
	else if (childPID == 0) {

		cout << "Child process; pid: " << getpid()
			<< ", ppid " << getppid()
			<< ", child will be (should be 0 because hasnt run yet):  " << childPID << endl;

		
		//creates the array using the inputs given earlier by the user and saves it into a variable
		vector<char> userArr = createRandArr(arrSize);
		cout << "the array is: " << userArr << endl;

		//now looks through the array to get the letter the user wants
		//if it does not exist, will wait for a kill command to end the process of the child

		//kill porcess command should be here


		while(*PointerLetterCount < 1) {
			
			//loops through the array, searching for a character
			for(auto i: userArr) {

				//if the child finds a character, it will create a new child to find the next character
				//with this the pointer will add to the num 
				if(userArr[i] == searchChar) {

					//changes the count of letters by incrementing it by one, since its an address in memory
					//despite being different processes, it should add to the total num of letters
					*PointerLetterCount += 1;
					
					//forks another child into existence
					long childsChildPID = 0;
					childsChildPID = fork();

					//parent process
					//will wait for child proces to finish then exit and exit the process
					if(childsChildPID > 0) {

					cout << "New Parent process (child); pid: " << getpid()
						<< ", ppid (parent ID): " << getppid()
						<< ", child will be: " << childsChildPID << endl;

						//waits for other children processes to end
						wait(NULL);
						//now leaves the while loop
						break;
					}

					//child process
					else if(childsChildPID == 0) {

						cout << " New Child process (child of child); pid: " << getpid()
							<< ", ppid (parent ID): " << getppid()
							<< ", child will be (should be 0 because hasnt run yet):  " << childsChildPID << endl;
						
						//just continue with the loop so doent do anything
					}

					else{
						cout << "error child of child not made" << endl;
					}
				}
			}
		}

		//child process exits
		exit(0);
	} 
	
	else {
		cout << "Spawn error - no child process" << endl;
	}

	//displays the letter count
	cout << "the number of the letter " << searchChar << " in the array is: " << numOfDesiredLetter << endl;
};


/*
* prints the two options to select from and if the user selects to exit the program
* the method will return true to tell the program to quit, otherwise continue
**/
bool menuSelection(){
	
	int option;
	cout << "Please select an option (enter the number): \n\n1. Run the program\n2. Exit the program\n" << endl;
	cin >> option;

	//if the user runs the job, they will be prompted to enter both the array size and the letter to search for
	if(option == 1) {
		cout << "\nenter the Letter to search for in the array:\n" << endl;
		cin >> searchChar;
		cout << "\nenter the size of the array to search through" << endl;
		cin >> arrSize;
		return false;
	}

	else{
		return true;
	}
};


int main() {

	//program continuously repeats until the user exits the program
	bool userHasntExit = true;
	while (userHasntExit) {
		
		if(menuSelection()) { 
			userHasntExit = false;
		}

		else {
			createChild();
		}
	}
}