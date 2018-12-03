#include "UnboundedInt.h"

#include <limits.h>
#include <iostream>



int main(){
	UnboundedInt a;
	UnboundedInt b;
	
	UnboundedInt c;
	UnboundedInt d;
	
	int numTests;
	int size;
	
	
	std::cout << "Testing that (a / b) + (a % b) = a" << std::endl;
	
	std::cout << "Enter b: ";
	std::cin >> b;
	
	std::cout << "Number of tests: ";
	std::cin >> numTests;
	
	std::cout << "Size of a ( < 10 reccommended for speed): ";
	std::cin >> size;

	for(int i = 0; i < numTests; i++){
		a.rfill(size, i);
		
		std::cout << "Test " << i << "...";
		c = (a / b);
		c = c * b;
		d = (a % b);
		if((c + d) == a)
			std::cout << "passed" << std::endl;
		
	}
	
	
	
	int z;
	std::cin >> z;
}