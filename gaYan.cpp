#include <iostream>
#include <vector>
#include <random>

#include "Cube.h"

using namespace std;
using namespace chrono;

mt19937 rng((int) chrono::steady_clock::now().time_since_epoch().count());

// Parameters of the Genetic Algorithm
// You only need to change stuff here to test

int childPopulationSize = 120, percentageParent = 10;
int numSeconds = 60, populationSize = 100, mutationProbability = 20, crossoverProbability = 80, crossoverPoints = 1;

struct Solution 
{
    vector<int> x;

    static int N;
    static Cube c;

    int curFitness;

    Solution() : x(N), curFitness(100) {}

    bool operator == (Solution other) const { return x == other.x; }

    void change(int i) {
        while (true) {
            int move = rng() % 18;
            if (i > 2 && move == this->x[i - 1] && move == this->x[i - 2] && move == this->x[i - 3]) continue;
            if (i > 0 && ((move % 2 == 0 && move == this->x[i - 1] - 1 ) || (move % 2 == 1 && move == this->x[i - 1] + 1))) continue;
            this->x[i] = move;
            break;
        }

        curFitness = 100;
    }

    int fitness() {
        if( curFitness == 100 )
            curFitness = c.fitness(this->x);
        return curFitness;
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

void crossover(Solution &child1, Solution &child2) {
    child1.curFitness = child2.curFitness = 100;
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

// Aqui tem q mudar pq o novo fitness vai de 0 a 14
// Provavelmente o ideal é fazer algo não linear, tipo proporcional ao quadrado de 15 - fitness
vector<Solution> generateChildren(vector<Solution> population, int populationSize)
{
    vector<int> slicesSize, psSlicesSize;
    shuffle( population.begin() , population.end() , rng );

    for(int i = 0 ; i < populationSize ; i++)
    {
        int curSlice = 15 - population[i].fitness();

        slicesSize.emplace_back( curSlice );

        if( i == 0 )
            psSlicesSize.emplace_back( curSlice );
        else
            psSlicesSize.emplace_back( psSlicesSize.back() + curSlice );
    }

    auto findIdSlice = [&](int num) -> int {
        return lower_bound( psSlicesSize.begin() , psSlicesSize.end() , num ) - psSlicesSize.begin();
    };

    vector<Solution> newPopulation;
    int totalSize = psSlicesSize.back();

    while( newPopulation.size() < childPopulationSize )
    {
        int x = rng()%(totalSize/2);
        int parent1 = findIdSlice(x), parent2 = findIdSlice(x + totalSize/2);

        generate_children(newPopulation, population[parent1], population[parent2]);
    }

    return newPopulation;
}

vector<Solution> getBestCandidates(vector<Solution> newPopulation, int populationSize)
{
    sort(newPopulation.begin(), newPopulation.end(), [&](Solution a, Solution b){
        return a.fitness() < b.fitness();
    });

    vector<Solution> population;

    for(int i = 0 ; i < populationSize ; i++)
        population.push_back( newPopulation[i] );

    return population;
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
        gen++;
        if (gen % 10 == 0) cout << "generation " << gen << endl;

        vector<Solution> newPopulation = generateChildren(population, populationSize);

        sort(population.begin(), population.end(), [&](Solution a, Solution b){
            return a.fitness() < b.fitness();
        });

        for(int i = 0 ; i*100 < (int)population.size()*percentageParent ; i++)
            newPopulation.push_back( population[i] );

        population = getBestCandidates(newPopulation, populationSize);

        for (int i = 0; i < populationSize; i++)
        {
            if( answer.fitness() > population[i].fitness() )
            {
                auto elapsedTime = duration_cast<seconds>(high_resolution_clock::now() - start).count();

                cout << "Found new answer after " << elapsedTime << " seconds with value " << population[i].fitness() << endl;
            
                answer = population[i];

            }
        }
    }

    return answer;
}

int main()
{
    Solution::c.scramble();
    Solution::N = 50;

    Solution answer = genetic_algorithm();
    cout << answer.fitness() << endl;
    return 0;
}