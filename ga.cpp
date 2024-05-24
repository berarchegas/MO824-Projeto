#include <bits/stdc++.h>
#include "Cube.h"

using namespace std;
using namespace chrono;

mt19937 rng((int) chrono::steady_clock::now().time_since_epoch().count());

// Parameters of the Genetic Algorithm
// You only need to change stuff here to test
int numSeconds = 1000, populationSize = 200, mutationProbability = 20, crossoverProbability = 80, crossoverPoints = 2;

struct Solution 
{
    vector<int> x;
    int fitnessValue;

    static int N;
    static Cube c;

    Solution() : x(N), fitnessValue(-1) {}

    bool operator == (Solution other) const { return x == other.x; }

    void change(int i) {
        this->fitnessValue = -1;
        while (true) {
            int move = rng() % 18;
            if (i > 2 && move == this->x[i - 1] && move == this->x[i - 2] && move == this->x[i - 3]) continue;
            if (i > 0 && ((move % 2 == 0 && move == this->x[i - 1] - 1 ) || (move % 2 == 1 && move == this->x[i - 1] + 1))) continue;
            this->x[i] = move;
            break;
        }
    }

    int fitness() {
        if (this->fitnessValue >= 0) return this->fitnessValue;
        return this->fitnessValue = c.fitness(this->x);
    }
};

// Struct static variables
int Solution::N = 50;
Cube Solution::c = Cube();

// Sets every gene to a random operator [0, 18)
// Checks for 4 consecutive elements in a row
// Checks for two consecutive inverse moves
Solution generateRandomSolution()
{
    Solution curSolution;
    for (int i = 0; i < Solution::N; i++)
    {
       curSolution.change(i);
    }
    return curSolution;
}

// Given 2x Children, it randomly sorts the children 
// and then selects the best one out of every two consecutive children
void select_best(vector<Solution> &population, vector<Solution> &newPopulation)
{
    vector<int> id(newPopulation.size()), randomValues(newPopulation.size());
    iota(id.begin(), id.end(), 0);
    for (int i = 0; i < newPopulation.size(); i++) {
        randomValues[i] = rng();
    }
    sort(id.begin(), id.end(), [&](int i, int j) {
        return randomValues[i] > randomValues[j];
    });
    for (int i = 0; i < populationSize; i++) {
        int id1 = id[2 * i];
        int id2 = id[2 * i + 1];
        if (newPopulation[id1].fitness() < newPopulation[id2].fitness())
            population[i] = newPopulation[id1];
        else
            population[i] = newPopulation[id2];
    }
}

void crossover(Solution &child1, Solution &child2) {
    child1.fitnessValue = child2.fitnessValue = -1;
    vector<bool> crossover(Solution::N);
    vector<int> points(crossoverPoints);
    for (int i = 0; i < crossoverPoints; i++)
        points[i] = rng() % Solution::N;
    sort(points.begin(), points.end()); 
    for (int i = 0, id = 0; i < Solution::N; i++)
    {
        if (id < crossoverPoints && i == points[id])
        {
            id++;
        }
        crossover[i] = id % 2;
    }
    for (int i = 0; i < Solution::N; i++)
    {
        if (crossover[i])
        {
            swap(child1.x[i], child2.x[i]);
        }
    }
}

void mutate(Solution &child) {
    int id = rng() % Solution::N;
    child.change(id);
}

// Applies crossover and mutation to the children
void generate_children(vector<Solution> &newPopulation, Solution child1, Solution child2) {
    
    if (rng() % 100 < crossoverProbability)
        crossover(child1, child2);
    
    if (rng() % 100 < mutationProbability)
        mutate(child1);
    
    if (rng() % 100 < mutationProbability)
        mutate(child2);

    newPopulation.push_back(child1);
    newPopulation.push_back(child2);
}

Solution genetic_algorithm()
{
    Solution answer;
    auto start = high_resolution_clock::now();
    vector<Solution> population(populationSize);
    for (int i = 0; i < populationSize; i++)
        population[i] = generateRandomSolution();

    int gen = 0;
    while( duration_cast<seconds>(high_resolution_clock::now() - start).count() < numSeconds )
    {
        vector<Solution> newPopulation;

        while (newPopulation.size() < 4 * populationSize) {
            int parent1 = rng() % populationSize;
            int parent2 = parent1;
            while (parent2 == parent1) {
                parent2 = rng() % populationSize;
            }
            generate_children(newPopulation, population[parent1], population[parent2]);
        }

        vector<Solution> intermediate(2 * populationSize);
        select_best(intermediate, newPopulation);  
        select_best(population, intermediate);  

        for (int i = 0; i < populationSize; i++)
        {
            if( answer.fitness() > population[i].fitness() )
            {
                auto elapsedTime = duration_cast<seconds>(high_resolution_clock::now() - start).count();

            
                answer = population[i];

                answer.c.findBest(answer.x);

                cout << "Found new answer after " << elapsedTime << " seconds with value " << population[i].fitness() << endl;

            }
        }
        gen++;
        if (gen % 10 == 0) cout << "generation " << gen << endl;
    }

    return answer;
}

int main()
{
    Solution::c.scramble();
    Solution::N = 100;

    Solution answer = genetic_algorithm();
    cout << answer.fitness() << endl;
    return 0;
}