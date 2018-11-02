# Arithmetic-Test

## Description
In C++, a program to test and train mental arithmetic in chosen operators (+,-,*,/) and chosen time limit. Operations are currently on integers only. For each operation user may input desired range for operands. For each question operation is chosen randomly then operands randomly based on given range. Default range for each operation is provided.  Score is recorded to a results file (if given by user) and outputed at end of session. 1 mark for correct and -2 marks for incorrect answers.


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
*1.1.0
  * Added ability to write time stamped results to a given file after the end of a session.
  * Incorrect scores now remove two points (-2) while correct points give one (+1).
* 1.0.0
  * First proper release

## License and Author Information
Hussein Ahmad  
Email: <hussein.tec101@gmail.com>  
This software is distributed under the MIT License. See `License` for more information.
