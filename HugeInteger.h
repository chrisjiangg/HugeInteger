#ifndef HUGEINTEGER_H_
#define HUGEINTEGER_H_
#include <iostream>
#include <string>

class HugeInteger
{
private:
	int* integer;
public:
	// Variables
	std::string neg;
	int arrsize;
	std::string revneg;
	// Required methods
	HugeInteger(const std::string& val);
	HugeInteger(int n);
	HugeInteger add(const HugeInteger& h);
	HugeInteger subtract(const HugeInteger& h);
	HugeInteger multiply(const HugeInteger& h);
	int compareAbs(const HugeInteger& h);
	int compareTo(const HugeInteger& h);
	std::string toString();
	double HugeIntTiming();
};

#endif /* HUGEINTEGER_H_ */
