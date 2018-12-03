#include "UnboundedInt.h"




//Number of bits in unsigned integer, may vary
const unsigned int UINT_SIZE = (sizeof(unsigned int) * 8);

//Return bit at location index in integer a
bool getBit(unsigned int a, unsigned int index){
	return (bool) (a & (1<<index));
}

unsigned int flip(const unsigned int& b){
	unsigned int result; 
	for(int i = 0; i < UINT_SIZE; i++){
		if(getBit(b,(UINT_SIZE - 1 - i)))
			result += 1 << i;
	} 
	return result;
}

//................CONSTRUCTORS................//
UnboundedInt::UnboundedInt(){
	A.push_back(0);
	negative = false;
}

UnboundedInt::UnboundedInt(std::string s){
	negative = false;
	
	UnboundedInt digit; 
	UnboundedInt ten(10);
	
	for(int i = 0; i < s.size(); i++){
		digit = UnboundedInt(s.at(i) - '0');
		for(int  j = s.size() - i; j > 1; j--){
			digit.multiply(ten);
		}
		add(digit, false);
	}
}

UnboundedInt::UnboundedInt(const UnboundedInt &B){
	for(int i = 0; i < B.blockSize(); i++){
		A.push_back(B.blockAt(i));
	}
	negative = B.isNegative();
}

UnboundedInt::UnboundedInt(unsigned int n){
	A.push_back(n);
	negative = false;
}

//..............ACCESS FUNCTIONS..............//
unsigned int UnboundedInt::blockAt(unsigned int i) const {
	return A.at(i);
}

unsigned int UnboundedInt::blockSize() const {
	return A.size();
}

bool UnboundedInt::isNegative() const {
	return negative;
}

//........INTERNAL ARITHMETIC FUNCTIONS.......//
//Rotate entire bitsring
void UnboundedInt::rotate(const UnboundedInt& B, unsigned int size){
	A.clear();
	
	if(size < B.blockSize())
		size = B.blockSize();
	
	for(int i = 0; i < size; i++){
		A.push_back(0);
	}
	
	for(int i = 0; i < B.blockSize(); i++){
		for(int j = 0; j < UINT_SIZE; j++){
			A[size - 1 - i] += ((((B.blockAt(i) & (1 << j)) >> j )<< (UINT_SIZE - 1 - j)));
		}
	}
}

//Shift entire bit string to the left with carry in 
void UnboundedInt::shiftL(unsigned int &carry){	
	unsigned int temp = 0;
	for(int i = 0; i < A.size(); i++){
		temp = getBit(A[i], UINT_SIZE - 1);
		A[i] = (A[i] << 1) + carry;
		carry = temp;
	}
	if(carry)
		A.push_back(carry);
}

//Shift entire bit string to the left without carry in
void UnboundedInt::shiftL(){
	unsigned int carry = 0;
	shiftL(carry);
}

//Shift entire bitstring left by "shift" number of blocks, optionally cutoff data pushed past original size 
void UnboundedInt::shiftBlockL(unsigned int shift, bool cutoff){	
	if(shift == 0){
		return;
	}
	if(cutoff){
		if(shift >= A.size()){
			A.clear();
			A.push_back(0);
		}
	}
	else{
		for(int i = 0; i < shift; i++){
			A.push_back(0);
		}
	}
	
	for(int i = (A.size() - 1); i >= shift; i--){
		A[i] = A[i - shift];
	}
	
	for(int i = 0; i < shift; i++){
		A[i] = 0;
	}
}

//Shift entire bitstring left by one block
void UnboundedInt::shiftBlockL(){
	shiftBlockL(1);
}

//Shift entire bit string to the right with carry out
void UnboundedInt::shiftR(unsigned int &carry){
	carry = 0;
	unsigned int temp = 0;
	if(A.size() == 1 && A[0] == 1){
		A[0] = 0;
		return;
	}
	else if(A[A.size() - 1] == 1){
		carry = 1;
		A.pop_back();
	}
	for(int i = A.size() - 1; i >= 0; i--){
		temp = getBit(A[i], 0);
	
		A[i] = (A[i] >> 1) + (carry << (UINT_SIZE - 1));
		carry = temp;
	}
	
}

//Shift entire bit string to the right without carry out
void UnboundedInt::shiftR(){
	unsigned int carry = 0;
	shiftR(carry);
}

//Shift entire bit string to the right by "shift" blocks
void UnboundedInt::shiftBlockR(unsigned int shift){	
	if(shift >= A.size()){
		A.clear();
		A.push_back(0);
		return;
	}

	for(int i = 0; i < (A.size() - shift); i++){
		A[i] = A[i + shift];
	}
	
	for(int i = 0; i < shift; i++){
		A.pop_back();
	}
}

//Shift entire bit string to the right by one block
void UnboundedInt::shiftBlockR(){
	shiftBlockR(1);
}

//Add second unbounded integer B, carry in/out, negative
void UnboundedInt::add(const UnboundedInt &B, unsigned int &carry, bool neg){	
	unsigned int size = B.blockSize();
	
	while(A.size() < size){
		A.push_back(0);
	}
	
	for(int i = 0; i < size; i++){
		A[i] = A[i] + B.blockAt(i) + carry;
	
		carry = (A[i] < B.blockAt(i)); //If sum is less than b = 1, there was overflow so carry = 1
	}	
	unsigned int prev;
	for(int i = size; i < A.size(); i++){
		prev = A[i];
		A[i] = A[i] + carry;
	
		carry = (A[i] < prev);
	}
	if(neg){
		if(carry){
			negative = false;
			
			for(int i = (A.size() - 1); i > 0; i--){
				if(A[i] == 0){
					A.pop_back();
				}
				else{
					break;
				}
			}
			
		}
		else{
			negative = true;
		}
	}
	else{
		if(carry){
			A.push_back(carry);
		}
	}
}

//Add second unbounded integer B without carry, optional false
void UnboundedInt::add(const UnboundedInt &B, bool neg){	
	unsigned int carry = 0;
	
	add(B, carry, neg);
}

//Add unsigned int, with carry, and negative
void UnboundedInt::add(unsigned int b, unsigned int &carry, bool neg){	
	unsigned int old;
	bool negat = false;
	
	if(b < 0){
		std::cout << "yes" << std::endl;
		carry = -b;
		negat = true;
	}
	else{
		carry = b;
	}
	
	for(int i = 1; i < A.size(); i++){
		old = A[i];
		A[i] = A[i] + carry;
	
		carry = (A[i] < old); //If sum is less than b = 1, there was overflow so carry = 1
		if(carry == 0)
			break;
	}	
	
	if(negat)
		negate();
	
}

//Add unsigned int without carry
void UnboundedInt::add(unsigned int b, bool neg){
	unsigned int carry = 0;
	add(b, neg);
}


//Negate number using two's compliment
void UnboundedInt::negate(){
	for(int i = 0; i < A.size(); i++){
		A[i] = A[i] ^ UINT_MAX; //XOR with all 1's 
	}
	
	unsigned int size = A.size();
	unsigned int carry = 1;
	
	for(int i = 0; i < size; i++){
		A[i] = A[i] + carry;
	
		carry = (!A[i]); //If sum is 0, then there was overflow so carry = 1
		if(!carry)
			break; //If theres no carry then theres nothing to propogate forward
	}
	
	//Dont't add new block
	negative = !negative;
}

//Adjust negative number's size
void UnboundedInt::negativeAdjust(const UnboundedInt &B){
	while(B.blockSize() > A.size()){
		A.push_back(UINT_MAX);
	}
}

//Multiply number by unbounded int b
void UnboundedInt::multiply(const UnboundedInt &B){
	UnboundedInt mult(B);
	UnboundedInt shift(*this);
	
	if(mult.blockSize() == 1 && mult.blockAt(0) == 0){
		A.clear();
		A.push_back(0);
		return;
	}
	
	
	A.clear();
	negative = false;
	
	bool neg = false;
	
	if(shift.isNegative()){
		shift.negate();
		neg = !neg;
	}
	
	if(mult.isNegative()){
		mult.negate();
		neg = !neg;
	}

	if(getBit(B.blockAt(0), 0))
			add(shift,false);
	
	int j = 1;
	
	for(int i = 0; i < B.blockSize(); i++){

		while(j < UINT_SIZE){
			shift.shiftL();

			if(getBit(B.blockAt(i), j)){	
				add(shift, false);
			}
			
			j++;
		}
		j = 0;
	}	
	
	if(neg){
		negate();
	}
}

//inverse number to size "refSize"
void UnboundedInt::inverse(unsigned int refSize){
	UnboundedInt x(1);
	UnboundedInt digit(1);
	UnboundedInt denom(*this);
	UnboundedInt numer(1);
	
	if(refSize == 0)
		refSize = A.size();

	A.clear();

	while(A.size() <= refSize){				
		digit = UnboundedInt(1);

		while(x < denom){
			x.shiftL();
			digit.shiftL();
		}
	
		add(digit, false);
		if(x == denom){
			break;
		}
		
		numer = x - denom;
		denom = denom * digit;
		x = numer;
	}
	shiftR();	
}

//Divide number by B (where B is inverseed), remainder out, divSize corresponding to fraction size
void UnboundedInt::divide(const UnboundedInt &B, UnboundedInt &Rem, unsigned int divSize){
	UnboundedInt mult(B);
	UnboundedInt base(*this);
	
	A.clear();
	negative = false;
	
	bool neg = false;
	
	if(base.isNegative()){
		base.negate();
		neg = !neg;
	}
	
	if(mult.isNegative()){
		mult.negate();
		neg = !neg;
	}
	
	
	unsigned int empty = 0;
	for(int i = 0; i < mult.blockSize(); i++){
		if(mult.blockAt(i) == 0){
			empty++;
		}
		else{
			break;
		}
	}
	mult.shiftBlockR(empty);
	
	UnboundedInt shift;

	for(int i = 0; i < mult.blockSize(); i++){
		
		for(int j = 0; j < UINT_SIZE; j++){
			shift.shiftL();
			if(getBit(mult.blockAt(i), j)){	
				shift.add(base, false);
			}
		}
	}	
		
	bool adjust = true;
	
	UnboundedInt frac(shift);

	shift.shiftBlockR(mult.blockSize() + (divSize - 1));
	
	add(shift, false);
	
	frac.shiftBlockL(shift.blockSize(), true);
	
	Rem.rotate(frac);

	for(int k = 0; k < base.blockSize(); k++){
		if(Rem.blockAt(k) != UINT_MAX)
			adjust = false;
	}
	
	if(adjust){
		add(UnboundedInt(1), false);
		Rem = UnboundedInt(0);
	}
	
	if(neg){
		negate();
	}
}

void UnboundedInt::modulo(const UnboundedInt &B, const UnboundedInt &invB){
	UnboundedInt mult(B);
	UnboundedInt invMult(invB);
	UnboundedInt base(*this);
	UnboundedInt remFrac;
	
	if(*this < B){
		return;
	}
	
	if(*this == B){
		A.clear();
		A.push_back(0);
		return;
	}
		
	
	if(invMult == UnboundedInt()){	
			invMult = mult;
			invMult.inverse(B.blockSize() + 10);
	}

	base.divide(invMult, remFrac, B.blockSize());
	base.multiply(B);
	base.negate();
	add(base, true);
}

//........EXTERNAL ARITHMETIC FUNCTIONS.......//
//Assignement Operator
UnboundedInt& UnboundedInt::operator=(const UnboundedInt &B){
	A.clear();
	
	for(int i = 0; i < B.blockSize(); i++){
		A.push_back(B.blockAt(i));
	}
	
	negative = B.isNegative();
	
	return *this;
}

//Addition Operator
UnboundedInt UnboundedInt::operator+(const UnboundedInt& B) const{
	UnboundedInt result;
	
	bool neg = negative ^ B.isNegative();
	
	if(neg){
		if(negative){
			UnboundedInt newA = *this;
			newA.negativeAdjust(B);
			result = B;
			result.add(newA, true);
			return result;
		}
		if(B.isNegative()){
			UnboundedInt newB(B);
			newB.negativeAdjust(*this);
			result = *this;
			result.add(newB, true);
			return result;
		}
	}
	else{
		result.add(B, false);
		result.add(*this, false);
		return result;
	}
}

//Negation Operator
UnboundedInt UnboundedInt::operator-() const{
	UnboundedInt neg(*this);
	
	neg.negate();
	
	return neg;
}

//Subraction Operator
UnboundedInt UnboundedInt::operator-(const UnboundedInt& B) const{
	UnboundedInt result(*this);
	UnboundedInt neg(B);
	
	neg.negate();
	
	return result+ neg;
}

//Multiplication Operator
UnboundedInt UnboundedInt::operator*(const UnboundedInt& B) const{
	UnboundedInt result(*this);
	
	result.multiply(B);
	
	return result;
}

//Division Operator
UnboundedInt UnboundedInt::operator/(const UnboundedInt& B) const{
	UnboundedInt result(*this);
	UnboundedInt invB(B);
	
	if(invB == UnboundedInt(1))
		return result;
	
	unsigned int divSize = invB.blockSize();
	invB.inverse(result.blockSize() + 10);
	
	UnboundedInt Rem;
	result.divide(invB, Rem, divSize);
	
	return result;
}

//Modulo Operator
UnboundedInt UnboundedInt::operator%(const UnboundedInt& B) const{
	UnboundedInt result(*this);
	UnboundedInt invB(B);
	
	if(invB == UnboundedInt(1))
		return UnboundedInt(1);
	
	unsigned int divSize = invB.blockSize();
	invB.inverse(result.blockSize() + 10);
	result.modulo(B);
	return result;
}

//Incremental Operator
UnboundedInt UnboundedInt::operator++() const{
	UnboundedInt result(*this);
	UnboundedInt one(1);
	result.add(one, false);
	return result;
}

//Decremental Operator
UnboundedInt UnboundedInt::operator--() const{
	UnboundedInt result(*this);
	UnboundedInt one(1);
	one.negate();
	result.add(one, false);
	return result;
}

//Shift Left Operator
UnboundedInt UnboundedInt::operator<<(const UnboundedInt& B) const{
	UnboundedInt result(*this);
	
	result.shiftL();
	
	return result;
}

//Shift Right Operator
UnboundedInt UnboundedInt::operator>>(const UnboundedInt& B) const{
	UnboundedInt result(*this);
	
	result.shiftR();
	
	return result;
}

//Less Than Operator
bool UnboundedInt::operator<(const UnboundedInt& B) const{
	unsigned int size = 0;
	
	if(!negative && !B.isNegative()){
		if(A.size() > B.blockSize()){
				return false;
		}
		else if(A.size() < B.blockSize()){
				return true;
		}

		
		for(int i = (A.size() - 1); i >= 0; i--){
			if(A[i] > B.blockAt(i)){
				return false;
			}
			else if(A[i] < B.blockAt(i)){
				return true;
			}
		}
		
	}
	else if(negative && B.isNegative()){
		size = A.size();
		if(B.blockSize() > A.size())
			size = B.blockSize();
		
		for(int i = (size - 1); i >= 0; i--){
			if(A[i] > B.blockAt(i)){
				return true;
			}
			else if(A[i] < B.blockAt(i)){
				return false;
			}
		}
	}
	else{
		return negative;
	}
}

//Greater Than Operator 
bool UnboundedInt::operator>(const UnboundedInt& B) const{
	unsigned int size = 0;
	
	if(!negative && !B.isNegative()){
		if(A.size() > B.blockSize()){
				return true;
		}
		else if(A.size() < B.blockSize()){
				return false;
		}
		
		for(int i = (A.size() - 1); i >= 0; i--){
			if(A[i] > B.blockAt(i)){
				return true;
			}
			else if(A[i] < B.blockAt(i)){
				return false;
			}
		}
		
	}
	else if(negative && B.isNegative()){
		size = A.size();
		if(B.blockSize() > A.size())
			size = B.blockSize();
		
		for(int i = (size - 1); i >= 0; i--){
			if(A[i] > B.blockAt(i)){
				return false;
			}
			else if(A[i] < B.blockAt(i)){
				return true;
			}
		}
	}
	else{
		return !negative;
	}
}

//Less Than or Equal Operator
bool UnboundedInt::operator<=(const UnboundedInt& B) const{
	return !(*this > B);
}

//Greater Than or Equal Operator
bool UnboundedInt::operator>=(const UnboundedInt& B) const{
	return !(*this < B);
}


//Equal Operator
bool UnboundedInt::operator==(const UnboundedInt& B) const{
	if(A.size() != B.blockSize()){
		return false;
	}
	for(int i = 0; i < A.size(); i++){
		if(A[i] != B.blockAt(i)){
			return false;
		}
	}
	
	return true;
}

//Unequal Operator
bool UnboundedInt::operator!=(const UnboundedInt& B) const{
	return !(*this == B);
}

//Insertion Operator
std::ostream& operator<<(std::ostream& os, const UnboundedInt &B){
	std::vector<int> output(B.blockSize() << 5, 0);
	bool neg = false;
	
	UnboundedInt x(B);

	if(x.isNegative()){
		x.negate();
		//x = -x;
		neg = true;
	}
 
	UnboundedInt zero(0);
	UnboundedInt one(1);
	UnboundedInt nine(9);
	UnboundedInt ten(10);
	
	int digit = 0;
	UnboundedInt d1(1);
	UnboundedInt d2(1);
	
	UnboundedInt v1(0);
	UnboundedInt v2(0);
	
	while (x > nine){ //nine
		digit = 0;
		d1 = one;
		d2 = one;
		while(d1 <= x){
			
			d2 = d1;
			d1 = d1 * ten;
			digit++;
		}
		
		digit -= 1;
		
		v1 = d2;
		v2 = d2;

		for(int value = 1; value <= 9; value++){
			v1 = v1 + d2;
			if(v1 > x){
				x = x - v2;
				output.at(digit) = value; 
				break;
			}
			v2 = v1;
		}

	}
	
	output.at(0) = x.blockAt(0);
	
	
	if(neg)
		os << "-";
	
	bool leadingZero = true;
	
	if(B.blockSize() == 1 && B.blockAt(0) == 0){
		os << 0;
	}
	else{
		for(int i = ((B.blockSize() << 5) -1 ); i >= 0; i--){
			if(output.at(i) != 0){
				leadingZero = false;
			}
			
			if(!leadingZero){
				os << output.at(i);
			}
		}
	}
	return os;
	//std::cout << "out[0] = " << output.at(0);
}

//Extraction Operator
std::istream& operator>>(std::istream& is, UnboundedInt &B){
	std::string s;
	is >> s;
	B = UnboundedInt(s);
}

//.........AUXILIARY FUNCTIONS........//
//Modulo with precalculated inverse
void UnboundedInt::mod(const UnboundedInt &B, const UnboundedInt &invB){
	modulo(B, invB);
}

//External Invert
void UnboundedInt::invert(){
	inverse(A.size() + 2);
}

//Fill with random numbers with "blocks" number of blocks long
void UnboundedInt::rfill(unsigned int blocks, unsigned int salt){
	A.clear();
	unsigned int randNum;
	
	std::default_random_engine generator;
	generator.seed(time(NULL) + salt);
	std::uniform_int_distribution<int> distr(1,UINT_MAX);

	
	for(int i = 0; i < blocks; i++){
		randNum =  distr(generator); 
		A.push_back(randNum);
	}
}

//Output bits to string
std::string UnboundedInt::to_binary(){
	std::stringstream ss;
	for(int i = A.size() - 1; i >= 0; i--){
		for(int j = (UINT_SIZE - 1); j >= 0; j--){
			ss << getBit(A[i],j);
		}
	}	
	return ss.str();
}










