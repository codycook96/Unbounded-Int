#include "UnboundedInt.h"

#include <limits.h>
#include <iostream>



int main(){
	int blocks_p;
	int blocks_q;
	
	UnboundedInt p;
	UnboundedInt invp;
	
	UnboundedInt q;
	std::cout << "Speed Test of q mod p" << std::endl;
	std::cout << "Enter aproximate numuber of blocks in p: ";
	std::cin >> blocks_p;
	std::cout << "Calculating inverse...";
	p.rfill(blocks_p);
	invp = p;
	invp.invert();
	std::cout << "Done" << std::endl;
	std::cout << "Enter aproximate numuber of blocks in q: ";
	std::cin >> blocks_q;
	
	for(int i = 0; i < 100; i++){
		std::cout << "Calculated mod " << i << std::endl;
		q.rfill(blocks_q, i);
		q.mod(p, invp);
	}
	
	
	
	int z;
	std::cin >> z;
	
}