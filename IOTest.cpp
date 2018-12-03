#include "UnboundedInt.h"

#include <limits.h>
#include <iostream>



int main(){
	UnboundedInt a;
	
	int rSize;
	
	std::cout << "Input Number: ";
	std::cin >> a;
	std::cout << "Number in decimal: " << a << std::endl;
	std::cout << "Number in binary: " << a.to_binary() << std::endl << std::endl;
	
	std::cout << "Block size for random number: ";
	std::cin >> rSize; 
	a.rfill(rSize);
	std::cout << "Number in decimal: " << a << std::endl;
	std::cout << "Number in binary: " << a.to_binary() << std::endl << std::endl;
	
	int z;
	std::cin >> z;
	
}