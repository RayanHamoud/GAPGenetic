#pragma once

#include <genetic.h>
#include <vector>
#include <thread>
#include <mutex>
#include <random>

using namespace std;

class island {
    public:
    island() = default;
    Population population;
    
    int pop_size;
    int N_max;
    GAPInstance *inst;
    void evolve();
    void migrateFrom( vector<Solution> migrants);
    vector<Solution> selectMigrants();
    // autres fonctions pour mutation, croisement, etc.
    island(int pop_size, int N_max, GAPInstance *inst): pop_size(pop_size), N_max(N_max), inst(inst) 
        {
        // Initialiser la population aléatoirement
        vector<Solution> sols = init_population(pop_size, inst);
        Population pop(sols,inst);
        population = pop;
        }
    
};
vector<island> implantation(vector<island> islands, GAPInstance* inst);
Solution run(int numIslands, int generations, int pop_size, int N_max, GAPInstance* inst, bool implant = false);