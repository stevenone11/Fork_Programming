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
	string randChar;
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
* the letter in the array
**/
void createChild(){

}


/*
* prints the two options to select from and if the user selects to exit the program
* the method will return true to tell the program to quit, otherwise continue
**/
bool menuSelection(){
	
	int option;
	cout << "Please select an option (enter the number): \n\n1. Run the program\n2. Exit the program\n" << endl;
	cin >> option;

	//if the user runs the job, they will be prompted to enter both the array size and the letter to search for
	if(option == 1){
		cout << "\nenter the Letter to search for in the array:\n" << endl;
		cin >> searchChar;
		cout << "\nenter the size of the array to search through" << endl;
		cin >> arrSize;
		return false;
	}

	else{
		return true;
	}
}


int main() {

	//program continuously repeats until the user exits the program
	bool userHasntExit = true;
	while (userHasntExit){
		
		if(menuSelection()){
			userHasntExit = false;
		}

		else{
			createChild();
		}
	}
}