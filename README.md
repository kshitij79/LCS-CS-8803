# **LCS_Project_1**
CS 8803: Logic in CS, Fall 2023 Project I : Kshitij Pathania

This code presents a systematic approach to solving the riddle " Who owns the fish?" using propositional logic, CNF (Con-junctive Normal Form) encoding, and the DPLL (Davis–Putnam–Logemann–Loveland) algorithm. The riddle involves five houses, each with distinct properties such as color, nationality, beverage preference, cigar brand, and pet ownership, and challenges the solver to determine who owns the fish among these individuals. The code creates encoding of the riddle's information and constraints into a CNF representation and the imple-mentation of the DPLL algorithm. 

The repositry consists of:
- Project.cpp: This files outputs the cnf form of the problem and the propostion whose assignments are true.
- cnf.txt: It contains the encoding of the riddle in the cnf form obtained from the output of Project.cpp
- answer.txt: This is the list of propostions having assignment true for the riddle.
