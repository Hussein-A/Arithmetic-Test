# Arithmetic-Test

## Description
In C++, a program to test and train mental arithmetic in chosen operators (+,-,*,/) and chosen time limit. Operations are currently on integers only. For each operation user may input desired range for operands. For each question operation is chosen randomly then operands randomly based on given range. Default range for each operation is provided.  Score is recorded to a results file (if given by user) and outputed at end of session. 1 mark for correct and -2 marks for incorrect answers. Operands may be negative.


## Motivation
This program was created with the goal of improving mental arithmetic whether it be for arithmetic challenges faced on a daily basis or in preparation for job assessments/interviews in, say, trading, etc.

## Installation
Windows:
1. Open developer command prompt. To do this type developer command prompt into start menu search bar. Ensure that you have Microsoft Visual C++ Build Tools for 2015 or later installed.
2. Change directory to the location where the program was downloaded. 
3. To compile type and enter: `cl /EHsc "Arithmetic Test".cpp` (or whatever you changed the name of the program to, still in double quotes).
4. Once it has successfuly compiled type and enter: `"Arithmetic Test".exe` to run the program.

For more specific instructions please click this [link](https://msdn.microsoft.com/en-ca/library/ms235639.aspx).

## Change Log
* 1.3.0
  * Completely redesigned codebase to be more inline with modern coding practices and OOP principles.
  * Operations no longer take in two ranges (one for the left operand and one for the right) but rather just one range for both.
  * Saving was changed to reflect the above change. (Note if attempting to use old save settings with new redesign it will only take the first range (ie. apply the left operand range for both operands) It is not recommended to do this).
  * Although highly unlikely to have been an issue users may now find that their random numbers are more "truly" random due to change in RNG (rand() to random_device, mt19937 & uniform_int_distribution).

* 1.2.1
  * Added the ability to restart directly after completion of session to avoid tedious restarting of program. Session choices do
 not need to be re-entered.

* 1.2.0
  * In addition to timestamping results all settings are saved to a file called "results.txt". This includes the operations used
  previously, their associated ranges and the time limit.

  * Settings saved can be used for prompt use without having to input settings manually each time. Only the most recent settings
  are used.
  
  * Answers and operations can now be negative ex. 75+-99 =  ? Chance for a negative operand is (1/5).
  
  * Fixed a bug where operands were not following their user inputted ranges.

* 1.1.0
  * Added ability to write time stamped results to a given file after the end of a session.
  * Incorrect scores now remove two points (-2) while correct points give one (+1).
  
* 1.0.0
  * First proper release

## License and Author Information
Hussein Ahmad  
Email: <hussein.tec101@gmail.com>  
This software is distributed under the MIT License. See `License` for more information.
