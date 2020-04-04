//Evan Perry
//Extra Credit 1
//CSCI 6632

#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>
#include <climits>
#include <math.h>
#define FN = "output.txt"

//function stubs
using namespace std;
void populateArray(vector <int>& arrayToBeSorted);
void printArray(vector <int>& arrayToBeSorted, bool state, bool isSorted, int whichSort = 0);
void insertionSort(vector <int>& arrayToBeSorted);
void Merge(vector <int>& arrayToBeSorted, int p, int q, int r);
void Mergesort(vector <int>& arrayToBeSorted, int p, int r);
void printHeader(bool state, bool isSorted);
void printSortHeader(int, bool state);
void fileSortHeader(int, bool state, ofstream&);
void MergesortRun(vector <int>& arrayToBeSorted, vector<chrono::high_resolution_clock::time_point>& timepoints);
void InsertionSortRun(vector <int>& arrayToBeSorted, vector<chrono::high_resolution_clock::time_point>& timepoints);
void UNHsort(vector <int>& arrayToBeSorted);
void UNHsortRun(vector <int>& arrayToBeSorted, vector<chrono::high_resolution_clock::time_point>& timepoints);
void printTimes(vector<chrono::high_resolution_clock::time_point>& timepoints);
chrono::high_resolution_clock::time_point timer();
double printTimesHelper(chrono::high_resolution_clock::time_point, chrono::high_resolution_clock::time_point);
void writeToFile(vector <int>& arrayToBeSorted, int);

//--------------------------------------
/*main controls entire execution*/
int main(void) {
	vector <int> arrayToBeSorted;
	vector <chrono::high_resolution_clock::time_point> timepoints;
	MergesortRun(arrayToBeSorted, timepoints);
	InsertionSortRun(arrayToBeSorted, timepoints);
	UNHsortRun(arrayToBeSorted, timepoints);
	printTimes(timepoints);
	return 0;
}
//-------------------------------------------
/*This function populates the array with 10000 items. It first checks
if the array is empty, if it is not empty it erases all elements in the vector,
we also ensure no duplicate numbers.*/
void populateArray(vector <int>& arrayToBeSorted) {
	if (!arrayToBeSorted.empty()) {
		arrayToBeSorted.erase(arrayToBeSorted.begin(), arrayToBeSorted.end());
	}
	srand(int(time(NULL)));
	int randomNumber = 0;
	for (int i = 10000; i > 0; i--) {
		randomNumber = rand();
		vector<int>::iterator noDuplicates = find(arrayToBeSorted.begin(), arrayToBeSorted.end(), randomNumber);
		while (noDuplicates != arrayToBeSorted.end()){
			randomNumber = rand();
			noDuplicates = find(arrayToBeSorted.begin(), arrayToBeSorted.end(), randomNumber);
		}
		arrayToBeSorted.push_back(randomNumber);
	}
}

//-------------------------------------------------
/*We print out the contects of the vector which prints that to the
screen, we then call writeToFile whichs writes those to a file
called output.txt*/
void printArray(vector <int>& arrayToBeSorted, bool state, bool isSorted, int whichSort) {
	int j = 1;
	printHeader(state, isSorted);
	bool flip = !state;
	cout << "---------------\n";
	printSortHeader(whichSort, true);
	cout << "---------------\n";
	for (int i : arrayToBeSorted) {
		cout << j << ": " << i << endl;
		j++;
	}
	cout << "--------------\n";
	printSortHeader(whichSort, false);
	cout << "--------------\n";
	printHeader(flip, isSorted);
	writeToFile(arrayToBeSorted, whichSort);
}

//--------------------------------------------------
/*We write contents of the vectors to a file.*/
void writeToFile(vector <int>& arrayToBeSorted, int whichSort) {
	ofstream output;
	int num = 1;
	output.open("output.txt", ios_base::app);
	if (output.is_open()) {
		output << "-------------Begin of Output----------------\n";
		fileSortHeader(whichSort, true, output);
		for (int i : arrayToBeSorted) {
			output << num << " : ";
			output << i << endl;
			num++;
		}
		fileSortHeader(whichSort, false, output);
		output << "---------End of Output----------------------\n";
	}
	output.close();
}
//-----------------------------------------------
/*The logic for insertionsort*/
void insertionSort(vector <int>& arrayToBeSorted) {
	int x, j = 0;
	for (int i = 0; i < int(arrayToBeSorted.size()); i++) {
		x = arrayToBeSorted[i];
		j = i - 1;
		while ((j >= 0) && (x < arrayToBeSorted[j])) {
			arrayToBeSorted[j + 1] = arrayToBeSorted[j];
			j--;
		}
		arrayToBeSorted[j + 1] = x;
	}
}

//-----------------------------------------------------------
/*Controller for insertion sort, first populates the array, prints the array, then we take note of the time,
sort the array, and take take note of the time again, and lastly print out the sorted array.*/
void InsertionSortRun(vector <int>& arrayToBeSorted, vector<chrono::high_resolution_clock::time_point>& timepoints) {
	populateArray(arrayToBeSorted);
	printArray(arrayToBeSorted, true, false);
	timepoints.push_back(timer());
	insertionSort(arrayToBeSorted);
	timepoints.push_back(timer());
	printArray(arrayToBeSorted, true, true, 2);
}

//-------------------------------------------------
/*The logic for putting the elements back together for merge sort.*/
void Merge(vector <int>& arrayToBeSorted, int startIndex, int midPoint, int endIndex) {
	int numberOfLeftElements = midPoint - startIndex + 1;
	int numberOfRightElements = endIndex - midPoint;
	vector <int> left;
	vector <int> right;
	for (int i = 0; i < numberOfLeftElements; i++) {
		left.push_back(arrayToBeSorted[startIndex + i]);
	}
	for (int j = 0; j < numberOfRightElements; j++) {
		right.push_back(arrayToBeSorted[midPoint + 1 + j]);		
	}
	left.push_back(INT_MAX);
	right.push_back(INT_MAX);
	int i = 0;
	int j = 0;
	for (int k = startIndex; k < endIndex + 1; k++) {
		if (left[i] <= right[j]) {
			arrayToBeSorted[k] = left[i];
			i++;
		}
		else {
			arrayToBeSorted[k] = right[j];
			j++;
		}
	}
}

//-----------------------------------------------
/*Divides up the array recursively calls itself to divide up array and then
calls merge to put the numbers back together.*/
void Mergesort(vector <int>& arrayToBeSorted, int startIndex, int endIndex) {
	int midPoint;
	if (startIndex < endIndex) {
		midPoint = int(floor((startIndex + (endIndex-1)) / 2));
		Mergesort(arrayToBeSorted, startIndex, midPoint);
		Mergesort(arrayToBeSorted, midPoint + 1, endIndex);
		Merge(arrayToBeSorted, startIndex, midPoint, endIndex);
	}
}

//----------------------------------------------------------
/*Controller for merge sort, first we populate the array, print the headers and unsorted numbers,
then we check the time, sort the array, check the time again. We then print out the sorted array.*/
void MergesortRun(vector <int>& arrayToBeSorted, vector<chrono::high_resolution_clock::time_point>& timepoints) {
	populateArray(arrayToBeSorted);
	printArray(arrayToBeSorted, true, false);
	timepoints.push_back(timer());
	Mergesort(arrayToBeSorted, 0, arrayToBeSorted.size() - 1);
	timepoints.push_back(timer());
	printArray(arrayToBeSorted, true, true, 1);
}

//------------------------------------------------------------
/*Controller for UNH sort, first we populate the array, print out the array, take note of the time, then
sort the array, and then take note of the time again, and lastly we print out the sorted array.*/
void UNHsortRun(vector <int>& arrayToBeSorted, vector<chrono::high_resolution_clock::time_point>& timepoints) {
	populateArray(arrayToBeSorted);
	printArray(arrayToBeSorted, true, false);
	timepoints.push_back(timer());
	UNHsort(arrayToBeSorted);
	timepoints.push_back(timer());
	printArray(arrayToBeSorted, true, true, 3);
}

//-------------------------------------------------------
/*Logic for UNHsort*/
void UNHsort(vector <int>& arrayToBeSorted) {
	for (int i = arrayToBeSorted.size() - 1; i > 0; i--) {
		for (int j = 0; j < i; j++) {
			if (arrayToBeSorted[j] > arrayToBeSorted[j + 1]) {
				int temp = arrayToBeSorted[j];
				arrayToBeSorted[j] = arrayToBeSorted[j + 1];
				arrayToBeSorted[j + 1] = temp;
			}
		}
	}
}

//---------------------------------------------------------
/*Source: https://en.cppreference.com/w/cpp/chrono/high_resolution_clock/now
We need the current time in order to measure how long the sort take, this function
returns the current time when it is called.*/
chrono::high_resolution_clock::time_point timer() {
	chrono::high_resolution_clock::time_point currentTime = chrono::high_resolution_clock::now();
	return (currentTime);
}

//-------------------------------------------------------
/*Prints the times at the end of execution.*/
void printTimes(vector<chrono::high_resolution_clock::time_point>& timepoints) {
	double executionTime[3];
	int endTimeElement = 1;
	int startTimeElement = 0;
	for (int i = 0; i < 3; i++) {
		executionTime [i] = printTimesHelper(timepoints[endTimeElement], timepoints[startTimeElement]);
		endTimeElement = endTimeElement + 2;
		startTimeElement = startTimeElement + 2;
	}
	cout << "Merge sort took " << executionTime[0]*1000 << " milliseconds to complete.\n";
	cout << "Insertion sort took " << executionTime[1] * 1000 << " milliseconds to complete.\n";
	cout << "UNH sort took " << executionTime[2] * 1000 << " milliseconds to complete.\n";
	ofstream output;
	output.open("output.txt", ios_base::app);
	if (output.is_open()) {
		output << "Executions Times are as follows: \n";
		output << "Merge sort took " << executionTime[0] * 1000 << " milliseconds to complete.\n";
		output << "Insertion sort took " << executionTime[1] * 1000 << " milliseconds to complete.\n";
		output << "UNH sort took " << executionTime[2] * 1000 << " milliseconds to complete.\n";
		output << "End of file.\n";
	}
	output.close();
	
}

//-------------------------------------------------------
/*Source: https://en.cppreference.com/w/cpp/chrono/duration
This is a helper function for printTimes, it subtracts start from end time, casts to double and returns
the total time, it is converted to milliseconds in printTimes to ease readibility.*/
double printTimesHelper(chrono::high_resolution_clock::time_point end, chrono::high_resolution_clock::time_point start) {
	chrono::duration <double> executionTime =
		chrono::duration_cast<chrono::duration<double>>
		(end - start);
	return(executionTime.count());
}

//--------------------------------------------------------
/*Helper function to print the correct header before
outputting the array.*/
void printSortHeader(int whichSort, bool state) {
	if (state) {
		if (whichSort == 1) {
			cout << "Beginning of Merge Sort Print Out\n";
		}
		else if (whichSort == 2) {
			cout << "Beginning of Insertion Sort Print Out\n";
		}
		else if (whichSort ==3) {
			cout << "Beginning of UNH Sort Print Out\n";
		}
	}
	else {
		if (whichSort == 1) {
			cout << "End of Merge Sort Print Out\n";
		}
		else if (whichSort == 2) {
			cout << "End of Insertion Sort Print Out\n";
		}
		else if (whichSort ==3) {
			cout << "End of UNH Sort Print Out\n";
		}
	}
}

//--------------------------------------------------------
/*Helper function for printing headers to file*/
void fileSortHeader(int whichSort, bool state, ofstream& output) {
	if (state) {
		if (whichSort == 1) {
			output << "Beginning of Merge Sort Print Out\n";
		}
		else if (whichSort == 2) {
			output << "Beginning of Insertion Sort Print Out\n";
		}
		else if (whichSort == 3) {
			output << "Beginning of UNH Sort Print Out\n";
		}
	}
	else {
		if (whichSort == 1) {
			output << "End of Merge Sort Print Out\n";
		}
		else if (whichSort == 2) {
			output << "End of Insertion Sort Print Out\n";
		}
		else if (whichSort == 3) {
			output << "End of UNH Sort Print Out\n";
		}
	}
}

//-------------------------------------------------------------
/*Prints headers on top of both unsorted and sort lists.*/
void printHeader(bool state, bool isSorted) {
	if (isSorted) {
		if (state) {
			cout << "-----------------------------------------------------------\n";
			cout << "Sorted list generated by pseudo random number generator\n";
			cout << "-----------------------------------------------------------\n";
		}
		else {
			cout << "----------------------------------------------------------\n";
			cout << "End of sorted list generated by pseudo random number generator\n";
			cout << "-----------------------------------------------------------\n";
		}
	}
	else if (!isSorted) {
		if (state) {
			cout << "-----------------------------------------------------------\n";
			cout << "Unsorted list generated by pseudo random number generator\n";
			cout << "-----------------------------------------------------------\n";

		}
		else {
			cout << "-----------------------------------------------------------\n";
			cout << "End of unsorted list generated by pseudo random number generator\n";
			cout << "-----------------------------------------------------------\n";

		}
	}
}

/*Sample Output of times
----------------------------------------------------------
End of sorted list generated by pseudo random number generator
-----------------------------------------------------------
Merge sort took 93.2439 milliseconds to complete.
Insertion sort took 1056.42 milliseconds to complete.
UNH sort took 2895.24 milliseconds to complete.*/