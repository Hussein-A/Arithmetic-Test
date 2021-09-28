#pragma once
#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<random>
#include<vector>
#include<time.h>
#include<chrono>
#include "Questions.h"

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