
#include "pch.h"
#include <iostream>
#include <fstream>
#include <tchar.h>
#include <stdio.h>
#include <string>
#include <algorithm>
#include <vector>

using namespace std;

const string inputFileName = "magic.txt"; //defining what file we're looking for while reading it

class MagicSquare {

private:
	int magicSum;
	int squareSize;
	vector< vector<int> > magicSquare;	
	//These 4 boolean member functions are going to be used in function isMagic later
	bool hasSameSum(); //Checks to make sure all rows, columns, and diagonals equal the magic sum
	bool hasUniqueValues(); //Checks the make sure no values repeat in the square
	bool hasAllNumbersInRange(); //Makes sure every value in the range (1 - 25, {1,2,3,4,5...25} are present in the square
	bool hasNoNumOutOfRange(); //Checks for any numbers that are larger than the valid range for the magic square

	/*	hasAllNumbersInRange and hasNoNumOutOfRange are very similar, but are different
	since the former is specifically checking to make sure that none of the values that
	are supposed to be there are missing. The latter identifies the acutal values that
	are there instead of the supposed value and prints the message that that particular
	value is outside of the range. Seperating the functions makes it easier
	to understand.	*/

public:
	int getMagicSum() { return magicSum; }
	int getSquareSize() { return squareSize; }
	bool readSquareFromFile(ifstream &); //Takes in the values of the file and puts it in our magic square class
	void displayMagicSquare();
	bool isMagic();


};

int main()
{
	MagicSquare square;

	ifstream inputFileStream(inputFileName);
	try {
		if (!inputFileStream) {
			throw system_error(errno, system_category(), "unable to open " + inputFileName + ".txt\n\n"); //error checking if unable to open file
		}
	}
	catch (const system_error& e) {
		cout << (e.what()) << " (" << (e.code()) << ")\n\n";
		cerr << "Press any key to end the program\n";
		getchar();
		exit(EXIT_FAILURE);
	} //Lines 38-48 is an exception that checks that we were able to open the magic.txt file correctly


	bool isDone;
	do {
		if (!(isDone = square.readSquareFromFile(inputFileStream))) {
			square.displayMagicSquare(); //keep showing the square as long as you can still read from the file
			if (!(square.isMagic())) {
				cout << endl << "Not a magic square." << endl << endl;
			}
			else
				cout << endl << "This is a magic square with size " << square.getSquareSize() << " and sum " << square.getMagicSum() << endl << endl;
		}

		system("pause");
		system("cls");
	} while (!isDone); //stop running when isDone is false from the readSquareFromFile function
}

bool MagicSquare::readSquareFromFile(ifstream & fileStream) {

	if (!(fileStream >> squareSize)) {
		cout << "End of file\n";
		return true;
	}

	//Swap trick
	magicSquare.clear();
	magicSquare.swap(magicSquare);	//Making the vector empty then filling it back up with more input

	magicSquare.resize(squareSize, vector<int>(squareSize, 0));

	for (int i = 0; i < squareSize; i++) {
		for (int j = 0; j < squareSize; j++) {
			fileStream >> magicSquare[i][j];
		}
	}

	magicSum = (squareSize * ((squareSize * squareSize) + 1)) / 2; //calculating Magic Sum through formula [n (n^2 + 1)] / 2


	return false;

}

void MagicSquare::displayMagicSquare() {

	for (int i = 0; i < squareSize; i++) {
		for (int j = 0; j < squareSize; j++) {

			if (squareSize > 9 && magicSquare[i][j] < 100 && magicSquare[i][j] > 9)
				cout << "0" << magicSquare[i][j] << " ";		//If the values are 3 digits, 2 digit numbers must have 1 zero in front of it for the sake of nice formatting
			else if (squareSize > 9 && magicSquare[i][j] < 10)
				cout << "00" << magicSquare[i][j] << " ";		//If the values are 3 digits, 1 digit numbers must have 2 zeroes in front of it
			else if (squareSize > 3 && magicSquare[i][j] < 10) {
				/*I have this if statement here to account for if there are single digit numbers in a magic square
				and to put a 0 in front of them to make them line up with the rest of the two digit numbers*/
				cout << "0" << magicSquare[i][j] << " ";	
			}
			else {
				cout << magicSquare[i][j] << " ";
			}
		}
		cout << endl;
	}
	cout << endl;
}

bool MagicSquare::hasSameSum() {
	//First check rows & columns
	int rowSum = 0;
	int colSum = 0;

	for (int i = 0; i < squareSize; i++) {
		for (int j = 0; j < squareSize; j++) {
			
			rowSum += magicSquare[i][j];
			colSum += magicSquare[j][i];

		}
		if (rowSum != magicSum || colSum != magicSum)
			return false;
		rowSum = 0, colSum = 0; //reset the values for the next iteration of the for loop
	}
	//Now check diagonals
	int leftRightDiagSum = 0;
	int rightLeftDiagSum = 0;
	for (int i = 0; i < squareSize; i++) {
		leftRightDiagSum += magicSquare[i][i];
		rightLeftDiagSum += magicSquare[i][squareSize - i - 1];
	}
	if (leftRightDiagSum != magicSum || rightLeftDiagSum != magicSum)	//Note: only need to check this once since there are only 2 diagonals in a square
		return false;
	
	return true;
}

bool MagicSquare::hasUniqueValues() {
	for (int i_1 = 0; i_1 < squareSize; i_1++) {
		for (int j_1 = 0; j_1 < squareSize; j_1++) {
			
			int frequencyCounter = 1;
			for (int i_2 = 0; i_2 < squareSize; i_2++) {
				for (int j_2 = 0; j_2 < squareSize; j_2++) {
					/*I am writing 4 for loops to compare all the values of the magicSquare to each other
					If any one of the values are the same and are not at the same index, we return false.*/
					if (magicSquare[i_2][j_2] == magicSquare[i_1][j_1] && ((i_1 != i_2) || (j_1 != j_2))) {
						frequencyCounter++;
					}
				}
			}
			if (frequencyCounter > 1) {
				cout << "This square is not unique, it has " << frequencyCounter << " elements with value " << magicSquare[i_1][j_1] << endl;
				return false;
			}

		}
	}
	return true;
}

bool MagicSquare::hasAllNumbersInRange() {

	bool hasAllNumbers = true;
	for (int number = 1; number < squareSize * squareSize + 1; number++) { //checking all numbers with every value in the magic square
		bool hasNumber = false;
		for (int i = 0; i < squareSize; i++) {
			for (int j = 0; j < squareSize; j++) {
				if (magicSquare[i][j] == number) {
					hasNumber = true;
				}
			}
		}
		if (!hasNumber) { //If a number is missing hasAllNumbers fails
			cout << "The square does not have a " << number << endl;
			hasAllNumbers = false;
		}
	}

	return hasAllNumbers;

}
bool MagicSquare::hasNoNumOutOfRange() {
	bool allInRange = true;
	for (int i = 0; i < squareSize; i++) {
		for (int j = 0; j < squareSize; j++) { //checking all values of magic square and making sure they are within the valid range
			if (magicSquare[i][j] > squareSize * squareSize || magicSquare[i][j] < 1) { 
				allInRange = false;
				cout << "The square's entry " << magicSquare[i][j] << " is more than " << squareSize << " squared which is " << squareSize * squareSize << endl;
			}
		}
	}
	return allInRange;
}

bool MagicSquare::isMagic() {
	//Running through all the "checking" functions
	bool inRange = this->hasAllNumbersInRange();
	bool sameSum = this->hasSameSum();
	bool uniqueValues = this->hasUniqueValues();
	bool notOutRange = this->hasNoNumOutOfRange();

	bool isMagic = (inRange && sameSum && uniqueValues && notOutRange); //All must be true to be a magic square
	if (isMagic) {
		return true;
	}
	return false;
}