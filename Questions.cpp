#include"Questions.h"

using namespace std;

//ArithmeticQuestion Implementations
bool ArithmeticQuestion::operator==(const ArithmeticQuestion& q) const {
	return this->getOp() == q.getOp() && this->getRange().a() == q.getRange().a()
		&& this->getRange().b() == q.getRange().b();
}

//Operation Implementations
std::string Addition::getRandQuestion() {
	this->lastUsedOperandX = this->range(rng);
	this->lastUsedOperandY = this->range(rng);

	return std::to_string(this->lastUsedOperandX) + "+" + std::to_string(this->lastUsedOperandY) + "?\n";
}

int Addition::getSoln() const {
	return this->lastUsedOperandX + this->lastUsedOperandY;
}

std::string Subtraction::getRandQuestion() {
	this->lastUsedOperandX = this->range(rng);
	this->lastUsedOperandY = this->range(rng);

	return std::to_string(this->lastUsedOperandX) + "-" + std::to_string(this->lastUsedOperandY) + "?\n";
}

int Subtraction::getSoln() const {
	return this->lastUsedOperandX - this->lastUsedOperandY;
}

std::string Multiplication::getRandQuestion() {
	this->lastUsedOperandX = this->range(rng);
	this->lastUsedOperandY = this->range(rng);

	return std::to_string(this->lastUsedOperandX) + "*" + std::to_string(this->lastUsedOperandY) + "?\n";
}

int Multiplication::getSoln() const {
	return this->lastUsedOperandX * this->lastUsedOperandY;
}

std::string Division::getRandQuestion() {
	//answer must be an int hence given x/y, x must be a multiple of y
	this->lastUsedOperandY = this->range(rng);
	const int& denom = this->lastUsedOperandY;
	const int maxMultipleOfDenom = this->range.b() / denom; //if denom is 3 and max range is 12 then the highest multiple is 4

	uniform_int_distribution<int> newDist(1, maxMultipleOfDenom);
	this->lastUsedOperandX = newDist(rng)*denom;

	return std::to_string(this->lastUsedOperandX) + "/" + std::to_string(this->lastUsedOperandY) + "?\n";
}

int Division::getSoln() const {
	return this->lastUsedOperandX / this->lastUsedOperandY;
}