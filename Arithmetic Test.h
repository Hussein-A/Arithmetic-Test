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
	std::uniform_int_distribution<int> range{0,1};
	int lastUsedOperandX = 0;
	int lastUsedOperandY = 0;

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

class Multiplication : public ArithmeticQuestion{
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




class Test {
private:
	#define TIMELIMIT 120
	int score = 0;
	std::chrono::seconds timeLimitSeconds{ TIMELIMIT };
	std::chrono::high_resolution_clock::time_point startTime{std::chrono::high_resolution_clock::now()};
	std::string saveFileName;
	std::vector<std::unique_ptr<ArithmeticQuestion>> questionBank;

public:
	//time limit manipulation
	bool isPastTimeLimit() const;
	std::chrono::seconds getTimeLimitSeconds() const { return this->timeLimitSeconds; }
	void setTimeLimitSeconds(int seconds) { if (seconds <= 0) std::cout << "Time limit cannot be <= 0. Default limit of 120 seconds applied.\n"; else this->timeLimitSeconds = std::chrono::seconds(seconds); }
	void setStartTime() { startTime = std::chrono::high_resolution_clock::now(); }

	//score manipulation
	int getScore() const { return this->score; }
	void addToScore(int x) { this->score += x; }

	//File saving settings
	std::string getSaveFileName() const { return this->saveFileName; }
	bool setSaveFileName(std::string fileName) { this->saveFileName = fileName; return true; }
	bool saveSettingsAndScore();

	//Question bank manipulation
	bool addQuestion(std::unique_ptr<ArithmeticQuestion>& questionuptr) { this->questionBank.push_back(std::move(questionuptr)); return true; }
	//bool removeQuestion(ArithmeticQuestion& question);
	void setQuestionBank(std::vector<std::unique_ptr<ArithmeticQuestion>>& qBank) { this->questionBank = move(qBank);}
	const std::vector<std::unique_ptr<ArithmeticQuestion>>& getQuestionVec() const { return questionBank; }

	//Output Messages
	void correctAnsMsg(int points = 1) { std::cout << "Correct! " << "+" << points << "\n"; }
	void incorrectAnsMsg(int points = -1) { std::cout << "Incorrect! " << points << "\n"; }
};

class TestUI {
public:
	int getNum();
	std::uniform_int_distribution<int> getRange();
	bool getYesNo();
	void getUserOps(Test& t);
	bool getFileSettings(Test& t); //assumes a save file exists, returns true if settings from file were copied to test object
};