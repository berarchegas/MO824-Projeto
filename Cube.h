#ifndef CUBE_H
#define CUBE_H

#include <bits/stdc++.h>

using namespace std;

struct Cube {

    static mt19937 rng;

    // All these static variables/functions are to calculate the Korf's pattern databases
    static vector<vector<array<int, 3>>> cornerPositions;
    static map<array<int, 3>, int> cornerToInt;
    static vector<vector<array<int, 3>>> edgePositions;
    static map<array<int, 2>, int> edgeToInt;
    static vector<int> eightMap; // Maps 8 digit permutations to integers
    static vector<int> sixMap; // Maps 6 digit permutations to integers
    static vector<char> cornerDistance; // Minimum number of moves to solve only the corners
    static vector<char> edgeDistance1; // Minimum number of moves to solve only the first set of edges
    static vector<char> edgeDistance2; // Minimum number of moves to solve only the second set of edges
    static bool initialized;
    static vector<int> factorial;
    static void initializeStatics();
    static void initializeEightMap(); 
    static void initializeSixMap(); 
    static void initializeCornerDistance();
    static void initializeEdgeDistance(); 

    vector<int> v;

    Cube();  // Constructor declaration
    Cube(const vector<int>& _v);
    Cube(const Cube& other);
    void set(int face, int i, int j, int cor);
    int getColor(int face, int i, int j) const;
    Cube copyCube() const;
    void print() const; // Method to print the cube
    void rightAntiClock(); // Rotate right face anticlockwise
    void rightClock(); // Rotate right face clockwise
    void upClock(); // Rotate up face clockwise
    void upAntiClock(); // Rotate up face anticlockwise
    void leftClock(); // Rotate left face clockwise
    void leftAntiClock(); // Rotate left face anticlockwise
    void downClock(); // Rotate down face clockwise
    void downAntiClock(); // Rotate down face anticlockwise
    void frontClock(); // Rotate front face clockwise
    void frontAntiClock(); // Rotate front face anticlockwise
    void backClock(); // Rotate back face clockwise
    void backAntiClock(); // Rotate back face anticlockwise
    void verticalClock(); // Combination of vertical rotations
    void verticalAntiClock(); // Combination of vertical rotations
    void horizontalClock(); // Combination of horizontal rotations
    void horizontalAntiClock(); // Combination of horizontal rotations
    void transversalClock(); // Combination of transversal rotations
    void transversalAntiClock(); // Combination of transversal rotations
    void performOperation(int operationIndex); // Perform operation based on index
    int countDif() const; // Count the number of tiles that have the wrong color
    int fitness(const vector<int> &x); // Calculates the fitness function, you can change the code so that it uses another fitness function
    int fitnessLowerBound(const vector<int> &x); // Fitness based on Korf's databases
    int fitnessWrongStickers(const vector<int> &x); // Fitness based on countDif()
    void scramble(); // Scrambles the cube
    void findBest(const vector<int> &x); // prints the cube at the lowest fitness point
    int cornerMapping() const; // Uses the corners of the cube to map it to an integer in the range [0, 3 ^ 7 * 8! - 1]
    pair<int, int> edgeMapping() const; // Uses the first and second set of edges to map the cube to a pair of integers, both in the range [0, 2 ^ 6 * 12!/6! - 1]
};

struct CubeInitializer {
    CubeInitializer() {
        Cube::initializeStatics();
    }
};

extern CubeInitializer cubeInitializer;

#endif // CUBE_H