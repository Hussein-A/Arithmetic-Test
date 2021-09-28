#pragma once
#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<random>
#include<vector>
#include<time.h>

class ArithmeticQuestion {
	//arithmetic questions revolve strictly around two variables in a given range
protected:
	std::string operation = "Undefined"; //used when writing to file which op was used
	std::uniform_int_distribution<int> range{ 0,1 };
	int lastUsedOperandX = 0;
	int lastUsedOperandY = 0;//for question selection
	std::random_device rd;
	std::mt19937 rng{ rd() };

	ArithmeticQuestion(int correct, int incorrect) : correctAnsPoints(correct), incorrectAnsPoints(incorrect) {}

public:
	const int correctAnsPoints;
	const int incorrectAnsPoints;
	virtual std::string getRandQuestion() = 0;
	virtual int getSoln() const = 0;
	void setRange(int min, int max) { this->range = std::uniform_int_distribution<int>(min, max); }
	std::uniform_int_distribution<int> getRange() const { return range; }
	std::string getOp() const { return this->operation; }
	bool operator==(const ArithmeticQuestion& q) const;

};


class Addition : public ArithmeticQuestion {
#define CORRECTPOINTS 1
#define INCORRECTPOINTS -2
public:
	Addition() : ArithmeticQuestion(CORRECTPOINTS, INCORRECTPOINTS)
	{
		this->setRange(0, 100);
		this->operation = "Addition";
	}

	Addition(int min, int max) : ArithmeticQuestion(CORRECTPOINTS, INCORRECTPOINTS) {
		this->setRange(min, max);
		this->operation = "Addition";
	}

	std::string getRandQuestion() override;
	int getSoln() const override;
};

class Subtraction : public ArithmeticQuestion {
#define CORRECTPOINTS 1
#define INCORRECTPOINTS -2
public:
	Subtraction() : ArithmeticQuestion(CORRECTPOINTS, INCORRECTPOINTS) {
		this->setRange(0, 100);
		this->operation = "Subtraction";
	}

	Subtraction(int min, int max) : ArithmeticQuestion(CORRECTPOINTS, INCORRECTPOINTS) {
		this->setRange(min, max);
		this->operation = "Subtraction";
	}

	std::string getRandQuestion()  override;
	int getSoln() const override;
};

class Multiplication : public ArithmeticQuestion {
#define CORRECTPOINTS 1
#define INCORRECTPOINTS -2
public:
	Multiplication() : ArithmeticQuestion(CORRECTPOINTS, INCORRECTPOINTS) {
		this->setRange(0, 12);
		this->operation = "Multiplication";
	}

	Multiplication(int min, int max) : ArithmeticQuestion(CORRECTPOINTS, INCORRECTPOINTS) {
		this->setRange(min, max);
		this->operation = "Multiplication";
	}

	std::string getRandQuestion()  override;
	int getSoln() const override;
};

class Division : public ArithmeticQuestion {
#define CORRECTPOINTS 1
#define INCORRECTPOINTS -2
public:
	Division() : ArithmeticQuestion(CORRECTPOINTS, INCORRECTPOINTS) {
		this->setRange(1, 100);
		this->operation = "Division";
	}

	Division(int min, int max) : ArithmeticQuestion(CORRECTPOINTS, INCORRECTPOINTS) {
		this->setRange(min, max);
		this->operation = "Division";
	}

	std::string getRandQuestion() override;
	int getSoln() const override;
};