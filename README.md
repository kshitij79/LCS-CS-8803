# **LCS_Project_1**
CS 8803: Logic in CS, Fall 2023 Project I : Kshitij Pathania

This code presents a systematic approach to solving the riddle " Who owns the fish?" using propositional logic, CNF (Con-junctive Normal Form) encoding, and the DPLL (Davis–Putnam–Logemann–Loveland) algorithm. The riddle involves five houses, each with distinct properties such as color, nationality, beverage preference, cigar brand, and pet ownership, and challenges the solver to determine who owns the fish among these individuals. The code creates encoding of the riddle's information and constraints into a CNF representation and the imple-mentation of the DPLL algorithm. 

The repositry consists of:
- Project.cpp: This files outputs the cnf form of the problem and the propostion whose assignments are true.
- cnf.txt: It contains the encoding of the riddle in the cnf form obtained from the output of Project.cpp
- answer.txt: This is the list of propostions having assignment true for the riddle.
- DPLL.cpp
- DPLL.h
- experiment.cpp
- visualize.py
- result100.csv
- result150.csv

How to Compile and Run
1. Compile and Run the Experiment
To compile the experiment files, use the following command:

```g++ -o experiment experiment.cpp dpll.cpp
After compilation, run the experiment with:

```./experiment.exe
2. Generate Graph
To generate a graph after running the experiments, execute the Python visualization script:

```python visualize.py
3. Solve Einstein's Problem
To compile the files for solving Einstein's problem, use the command:

```g++ -o Project1 Project1.cpp dpll.cpp
Then, run the executable:

```./Project1
