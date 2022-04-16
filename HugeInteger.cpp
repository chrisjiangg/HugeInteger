#include "HugeInteger.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <stdexcept>
#include<algorithm>
#include <boost/algorithm/string.hpp>
#include <boost/multiprecision/cpp_int.hpp>
#include <chrono>

using namespace std::chrono;
using namespace std;

HugeInteger::HugeInteger(const std::string& val) {
	neg = "+"; //sets neg to '+' at the very start
	revneg = "";

	string copyval = ""; //initialize empty string to copy val to

	if (val[0] == '-') { //if the index 0 is '-'
		neg = "-"; //set neg to '-' to keep track of the negative 
		copyval.reserve(val.size());
		for (size_t i = 1; i < val.size(); i++) { //starts from the first digit thats not the '-' and loops through copying val to copyval
			copyval += val[i];
		}
	}
	else
		copyval = val;

	bool isnumber = true;

	if (copyval == "") { //checks if copyval is empty
		throw std::invalid_argument("invalid input");
	}

	for (char const& ch : copyval) { //checks if the number is valid and doesnt contain characters or symbols
		if (std::isdigit(ch) == 0) {
			isnumber = false;
			throw std::invalid_argument("invalid input");
		}
	}
	integer = (int*)malloc(size(copyval) * sizeof(int)); //allocates enough memory to store val

	for (int i = 0; i < size(copyval); i++) { //loops through the string and splits it up into digits
		integer[i] = (int)copyval[i] - '0'; //puts it into an array integer
	}
	arrsize = size(copyval); //sets arrsize to the size of copyval

}

HugeInteger::HugeInteger(int n) {
	neg = "+"; //sets neg to '+' at the very start
	revneg = "";

	if (n <= 0) { //thorw an exception if n is 0 or negative
		throw std::invalid_argument("invalid input");
	}

	integer = (int*)malloc(n * sizeof(int)); //allocates enough memory to integer
	integer[0] = (rand() % 9) + 1; //first digit 1-9

	for (int i = 1; i < n; i++) { //rest of the n digits 0-9
		integer[i] = (rand() % 10);
	}
	arrsize = n;
}

HugeInteger HugeInteger::add(const HugeInteger& h) {
	// TODO
	int carryover = 0;
	int i = arrsize - 1; //gets the size of this array, -1 because we want i to start at the final index and not beyond it 
	int j = h.arrsize - 1; //gets the size of the array in h, -1 because we want j to start at the final index and not beyond it 
	int* result;
	int k;
	int* larger;
	int largersize;
	string sign = "";

	if (compareAbs(h) >= 0) { //checking which integer is bigger
		if (neg == "-" && h.neg == "+" && revneg == "") { //checks signs to see if it can be sent to the subtract function
			revneg = "-";
			return subtract(h);
		}
		if (neg == "+" && h.neg == "-" && revneg == "") {
			revneg = "+";
			return subtract(h);
		}
		result = (int*)malloc((i + 2) * sizeof(int)); //allocates enough space in terms of i, +2 because we want one more than the size of the larger array and i is one less than size
		k = i; //substitute for i or j depending which is larger
		larger = integer; //larger number
		largersize = arrsize; //larger size
		if (revneg == "") {
			sign = neg;
		}
		else {
			sign = revneg;
		}
	}
	else {
		if (neg == "-" && h.neg == "+" && revneg == "") { //checks signs to see if it can be sent to the subtract function
			revneg = "+";
			return subtract(h);
		}
		if (neg == "+" && h.neg == "-" && revneg == "") {
			revneg = "-";
			return subtract(h);
		}
		result = (int*)malloc((j + 2) * sizeof(int)); //allocates enough space in terms of j, +2 because we want one more than the size of the larger array and j is one less than size
		k = j; //substitute for i or j depending which is larger
		larger = h.integer; //larger number
		largersize = h.arrsize; //larger size
		if (revneg == "") {
			sign = h.neg;
		}
		else {
			sign = revneg;
		}
	}

	revneg = "";

	while (i >= 0 && j >= 0) { //loops until one of the sizes of the arrays are 0
		result[k + 1] = integer[i] + h.integer[j] + carryover; //adds matching digits of the integers together
		if (result[k + 1] > 9) { //if the sum if the same index integer is bigger than 9 subtract 10 and carry over 1
			result[k + 1] -= 10; //subtract 10 from the digit that needs to be carried over
			carryover = 1;
		}
		else
			carryover = 0;
		i--; //deincriments counters
		j--;
		k--;
	}
	for (k; k >= 0; k--) { //handles the digits of the larger number that dont get added to anything
		result[k + 1] = larger[k] + carryover;
		if (result[k + 1] > 9) {
			result[k + 1] -= 10;
			carryover = 1;
		}
		else
			carryover = 0;
	}
	result[0] = carryover; //in the case the left most digit needs to get carried over

	string hugeinteger = "";

	if (sign == "-") { //inserts negative sign when needed
		hugeinteger += '-';
	}

	for (int a = 0; a < largersize + 1; a++) { //converts integer to char and adds to a empty string
		if (a != 0 || result[0] != 0) {
			hugeinteger += ((char)(result[a] + '0')); // + '0' to convert properly to ascii
		}
	}

	return HugeInteger(hugeinteger);
}

HugeInteger HugeInteger::subtract(const HugeInteger& h) {
	// TODO
	int carryover = 0;
	int i = arrsize - 1; //gets the size of this array, -1 because we want i to start at the final index and not beyond it 
	int j = h.arrsize - 1; //gets the size of the array in h, -1 because we want j to start at the final index and not beyond it 
	int* result;
	int L;
	int s;
	int* larger;
	int* smaller;
	string sign = "";
	int largersize;

	if (compareAbs(h) > 0) { //checking which integer is bigger
		if (neg == "-" && h.neg == "+" && revneg == "") { //checks signs
			revneg = "-";
			return add(h);
		}
		if (neg == "+" && h.neg == "-" && revneg == "") {
			revneg = "+";
			return add(h);
		}
		result = (int*)malloc((i + 1) * sizeof(int)); //allocates enough space in terms of i, +2 because we want one more than the size of the larger array and i is one less than size
		L = i; //substitute for i or j depending which is larger
		s = j;
		larger = integer; //larger number
		smaller = h.integer;//smaller number
		largersize = arrsize; //larger size
		if (revneg == "") {
			sign = neg;
		}
		else {
			sign = revneg;
		}
	}
	else if (compareAbs(h) < 0) {
		if (neg == "-" && h.neg == "+" && revneg == "") { //checks signs
			revneg = "-";
			return add(h);
		}
		if (neg == "+" && h.neg == "-" && revneg == "") {
			revneg = "+";
			return add(h);
		}
		result = (int*)malloc((j + 1) * sizeof(int)); //allocates enough space in terms of j, +2 because we want one more than the size of the larger array and j is one less than size
		L = j;; //substitute for i or j depending which is larger
		s = i;
		larger = h.integer; //larger number
		smaller = integer; //smaller number
		largersize = h.arrsize; //larger size
		if (revneg == "") {
			if (neg == "+") {
				sign = "-";
			}
			if (neg == "-") {
				sign = "+";
			}
		}
		else {
			sign = revneg;
		}
	}
	else {
		if (neg == "-" && h.neg == "+" && revneg == "") { //checks signs
			revneg = "-";
			return add(h);
		}
		else if (neg == "+" && h.neg == "-" && revneg == "") {
			revneg = "+";
			return add(h);
		}
		else {
			return HugeInteger("0");
		}
	}

	revneg = "";

	while (s >= 0 && L >= 0) { //loops until one of the sizes of the arrays are 0
		result[L] = larger[L] - smaller[s] - carryover; //adds matching digits of the integers together
		if (result[L] < 0) { //if the sum if the same index integer is bigger than 9 subtract 10 and carry over 1
			result[L] += 10; //subtract 10 from the digit that needs to be carried over
			carryover = 1;
		}
		else
			carryover = 0;
		s--; //deincriments counters
		L--;
	}
	for (L; L >= 0; L--) { //handles the digits of the larger number that dont get added to anything
		result[L] = larger[L] - carryover;
		if (result[L] < 0) {
			result[L] += 10;
			carryover = 1;
		}
		else
			carryover = 0;
	}

	string hugeinteger = "";

	if (sign == "-") { //inserts negative sign when needed
		hugeinteger += '-';
	}

	bool zero = false;

	for (int a = 0; a < largersize; a++) { //converts integer to char and adds to a empty string
		if (result[a] != 0 || zero) {
			hugeinteger += ((char)(result[a] + '0')); // + '0' to convert properly to ascii
			zero = true;
		}
	}

	return HugeInteger(hugeinteger);
}

HugeInteger HugeInteger::multiply(const HugeInteger& h) {
	// TODO
	int carryover = 0;
	int i = arrsize - 1; //gets the size of this array, -1 because we want i to start at the final index and not beyond it 
	int j = h.arrsize - 1; //gets the size of the array in h, -1 because we want j to start at the final index and not beyond it 
	string sign = "";

	if (neg == "-" && h.neg == "+") { //checks signs
		sign = "-";
	}
	else if (neg == "+" && h.neg == "-") {
		sign = "-";
	}
	else {
		sign = "";
	}

	int** result = (int**)malloc((h.arrsize) * sizeof(int*)); //allocating enough space for 2d array

	for (int a = 0; a < h.arrsize; a++) {
		result[a] = (int*)malloc((arrsize + 1) * sizeof(int));
	}

	for (int a = h.arrsize - 1; a >= 0; a--) { //multiplying the inputs together digit by digit , -1 arrsize because we want the final index
		for (int b = arrsize; b >= 1; b--) { //save additonal space to use carry over at the end , 
			result[a][b] = (h.integer[a] * integer[b - 1]) + carryover;
			carryover = result[a][b] / 10;
			result[a][b] = result[a][b] - (carryover * 10);
		}
		result[a][0] = carryover;
		carryover = 0;
	}

	char** copyresult = (char**)malloc((h.arrsize) * sizeof(char*)); //allocating enough space

	for (int a = 0; a < h.arrsize; a++) {
		copyresult[a] = (char*)malloc((arrsize + 2) * sizeof(char)); //+2 because the max size is 1 more digit of the first copy result and need addtional space for the null to convert back to a string
	}

	for (int a = 0; a < h.arrsize; a++) { //converting result to a string by copying it over to copyresult
		for (int b = 0; b < arrsize + 1; b++) {
			copyresult[a][b] = ((char)(result[a][b] + '0'));
		}
		copyresult[a][arrsize + 1] = '\0';
	}

	string zero = "";
	zero.append(h.arrsize - 1, '0'); //creates number of zeros based on number of digits of the second number which is why we using h.arrsize

	HugeInteger hugeresult = HugeInteger(sign + copyresult[0] + zero); //initiallizing hugeresult as copyresult[0] with the appropiate number of zeros at the end

	for (int a = 1; a < h.arrsize; a++) { //adds all the copyresults together with the zeros on the end
		hugeresult = hugeresult.add(HugeInteger(sign + copyresult[a] + zero.substr(0, zero.length() - a))); //we substring because we want to decrease the number of zeros each time
	}

	return hugeresult;
}

int HugeInteger::compareAbs(const HugeInteger& h) {
	if (arrsize > h.arrsize) { //checking which integer is bigger
		return 1;
	}

	else if (arrsize < h.arrsize) {
		return -1;
	}

	else {
		for (int i = 0; i < arrsize; i++) {
			if (integer[i] > h.integer[i]) { //checking individual digits starting from the left if they are the same size
				return 1;
			}
			if (integer[i] < h.integer[i]) {
				return -1;
			}
		}
	}
	return 0;
}

int HugeInteger::compareTo(const HugeInteger& h) {
	// TODO
	if (neg == "+" && h.neg == "-") { //if one number is positive and the other is negative we know which number is bigger
		return 1;
	}
	if (neg == "-" && h.neg == "+") {
		return -1;
	}
	if (arrsize > h.arrsize) { //checking which integer is bigger if both are the same sign and determines which is less negative or more positive to be the bigger number
		if (neg == "+") {
			return 1;
		}
		if (neg == "-") {
			return -1;
		}
	}

	else if (arrsize < h.arrsize) {
		if (neg == "+") {
			return -1;
		}
		if (neg == "-") {
			return 1;
		}
	}

	else { //going through individual digits starting from the left if same size
		if (neg == "+") {
			for (int i = 0; i < arrsize; i++) {
				if (integer[i] > h.integer[i]) {
					return 1;
				}
				if (integer[i] < h.integer[i]) {
					return -1;
				}
			}
		}
		if (neg == "-") {
			for (int i = 0; i < arrsize; i++) {
				if (integer[i] > h.integer[i]) {
					return -1;
				}
				if (integer[i] < h.integer[i]) {
					return 1;
				}
			}
		}
	}
	return 0;
}

std::string HugeInteger::toString() {
	// TODO
	string hugeinteger = "";

	if (neg == "-") { //inserts negative sign when needed
		hugeinteger += '-';
	}

	bool zero = false;

	for (int a = 0; a < arrsize; a++) { //converts integer to char and adds to a empty string
		if (integer[a] != 0 || zero) {
			hugeinteger += ((char)(integer[a] + '0')); // + '0' to convert properly to ascii
			zero = true;
		}
	}

	if (hugeinteger == "-" || hugeinteger == "" || hugeinteger == "-0") { //makes zero is always positive and empty hugeinteger is also zero
		hugeinteger = "0";
	}
	
	
	return hugeinteger;
}

double HugeIntTiming() {
	system_clock::time_point startTime, endTime;
	double runTime = 0.0;
	double durationMs = 0.0;
	int n = 500;
	int MAXNUMINTS = 100; //at least 100
	int MAXRUN = 100; //set to make durationMs at least 500
	for (int numInts = 0; numInts < MAXNUMINTS; numInts++) {
		HugeInteger huge1(n); //creates a ramdon integer of n digits
		HugeInteger huge2(n);
		startTime = system_clock::now();
		for (int numRun = 0; numRun < MAXRUN; numRun++) {
			HugeInteger huge3 = huge1.add(huge2);
		}
		endTime = system_clock::now();
		durationMs = (duration<double, std::milli>(endTime - startTime)).count();
		runTime += durationMs / ((double)MAXRUN);
	}
	runTime += runTime / ((double)MAXNUMINTS);
	return runTime;
}

