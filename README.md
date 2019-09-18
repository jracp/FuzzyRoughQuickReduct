---
title: "FuzzyRoughQuickReduct"
author: "Javad Rahimipour Anaraki"
date: '08/02/18'
---

## Use case
To determine the most important features using the algorithm described in [New Approaches to Fuzzy-Rough Feature Selection](http://ieeexplore.ieee.org/document/4505335/) by Richard Jensen and Qiang Shen

## Compile
To compile the C++ code follow these steps:

1. Be sure that you have the latest [GCC/G++ compiler](https://gcc.gnu.org/) installed
2. Use `g++ -o FRQR FRQR.cpp -std=c++11` to compile the program
3. To improve its performance one can use `-O1` or `-O2` or `-O3`
4. Ignore the following warning message:

        FRQR.cpp:238:14: warning: expression result unused [-Wunused-value]
        for (s;s<cls[nCls];++s) {
        ^
        1 warning generated.

For the MATLAB code, simply copy `FRQR.m` and `IND.m` to a folder containing a sub-folder called `Data`. Place your dataset in that folder and add the name of the dataset to `FRQR.m` file and run the code.

## Run
To run the program use `./FRQR {a dataset name}`

## Note
The classification outcome column of dataset should be sorted ascendingly
