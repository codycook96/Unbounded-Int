#ifndef UNBOUNDEDINT_H
#define UNBOUNDEDINT_H

#define _CRT_RAND_S  

#include <stdlib.h>
#include <vector>
#include <iostream>
#include <sstream>
#include <random>
#include <time.h>

class UnboundedInt{
	private:
		std::vector<unsigned int> A;	//Storage vector for bits
		bool negative;					//Sign flag
		
		//.........INTERNAL ACCESS FUNCTIONS..........//
		unsigned int blockAt(unsigned int i) const;	//Get integer at certain block
		
		
		//........INTERNAL ARITHMETIC FUNCTIONS.......//
		//ROTATE
		void rotate(const UnboundedInt& B, unsigned int size = 0);	//Rotate number, ie first digit becomes last
		
		//LEFT SHIFT
		void shiftL(unsigned int &carry); 							//Shift left once, include carry in 
		void shiftL(); 												//Shift left once 
		void shiftBlockL(unsigned int shift, bool cutoff = false);	//Shift left by whole block "shift" number of times, optional cutoff ie don't add new blocks
		void shiftBlockL();											//Shift left by whole block once
		
		//RIGHT SHIFT
		void shiftR(unsigned int &carry);		//Shift right once, pass carry out
		void shiftR();							//Shift right once
		void shiftBlockR(unsigned int shift);	//Shift right by whole block "shift" number of times
		void shiftBlockR();						//Shift right by whole block once
		
		//ADDITION
		void add(const UnboundedInt &b, unsigned int &carry, bool neg = false); //Add UnboundedInt B, include carry in and pass carry out, optional add negative number
		void add(const UnboundedInt &b, bool neg = false);						//Add UnboundedInt B, optional add negative number
		void add(unsigned int b, unsigned int &carry, bool neg = false);		//Add unsigned int b, include carry in and pass carry out, optional add negative number
		void add(unsigned int b, bool neg = false);								//Add unsigned int b, optional add negative number
			
		//NEGATIVE
		void negate();								//Negate number using 2's Compliment
		void negativeAdjust(const UnboundedInt &B);	//Make negative this same size as UnboundedInt B
		
		//MULTIPLY
		void multiply(const UnboundedInt &B);											//Multiply this by UnboundedInt B
		
		//DIVIDE
		void inverse(unsigned int refSize = 0);											//Invert this with size up to refSize
		void divide(const UnboundedInt &B, UnboundedInt &Rem, unsigned int divSize);	//Divide this by B, pass out remainder
		
		//MODULAR
		void modulo(const UnboundedInt &B, const UnboundedInt &invB = UnboundedInt());	//Ramainder after this divided by UnboundedInt B, optional pass in inverse of UnboundedInt B
		
	public:
		//................CONSTRUCTORS................//
		UnboundedInt();							//Standard Constructor
		UnboundedInt(std::string s);			//String Constructor
		UnboundedInt(const UnboundedInt &b);	//Copy Constructor
		UnboundedInt(unsigned int n);			//Single Integer Constructor
		
		//.........EXTERNAL ACCESS FUNCTIONS..........//
		bool isNegative() const;	//Get sign
		unsigned int blockSize() const;				//Get number of blocks
		
		//........EXTERNAL OPERATOR FUNCTIONS.......//
		UnboundedInt& operator=(const UnboundedInt &b);
		
		UnboundedInt operator+(const UnboundedInt& B) const;
		
		UnboundedInt operator-() const;
		
		UnboundedInt operator-(const UnboundedInt& B) const;
		
		UnboundedInt operator*(const UnboundedInt& B) const;
		
		UnboundedInt operator/(const UnboundedInt& B) const;
		
		UnboundedInt operator%(const UnboundedInt& B) const;

		UnboundedInt operator++() const;
		
		UnboundedInt operator--() const;
		
		UnboundedInt operator<<(const UnboundedInt& B) const;
		
		UnboundedInt operator>>(const UnboundedInt& B) const;
		
		bool operator<(const UnboundedInt& B) const;
		
		bool operator>(const UnboundedInt& B) const;
		
		bool operator<=(const UnboundedInt& B) const;
		
		bool operator>=(const UnboundedInt& B) const;
		
		bool operator==(const UnboundedInt& B) const;
		
		bool operator!=(const UnboundedInt& B) const;
		
		friend std::ostream& operator<<(std::ostream& os, const UnboundedInt &B);
		
		friend std::istream& operator>>(std::istream& is, UnboundedInt &B);
		
		void print();
		
		//.........AUXILIARY FUNCTIONS........//
		//Modulo using precalculated inverse
		void mod(const UnboundedInt &B, const UnboundedInt &invB);
		
		//External Invert
		void invert();
		
		//Fill with random numbers "blocks" number of blocks long
		void rfill(unsigned int blocks, unsigned int salt = 0);
		
		//Output bits to string
		std::string to_binary();	
};




#endif