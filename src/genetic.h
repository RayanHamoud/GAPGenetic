#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <Instance.h>
#include <Solution.h>
#include <map>
#include <greedy.h>
#include <random>
#include <voisin.h>
#include<montee.h>
#include<chrono>
#include "thread"


using namespace std;
const int max_repa = 100;
const int timeLimitMs = 1*1000;//Temps max en millisecondes

class Population {
    public : 
    GAPInstance* inst;
    int n;
    vector<Solution> vec;
    Solution best;
    
    Population (){}
    //On crée le constructeur qui va prendre un vecteur de solutions
    Population (vector<Solution> solutions, GAPInstance* inst){
        n = solutions.size();
        vec = solutions;
        best = greedy(inst);
        vec.push_back(best);
        for (Solution sol : solutions){
            if (sol.fitness > best.fitness and sol.unfitness == 0){
                best = sol;
            }
        }      
    }
    pair<Solution,Solution> selection();
    void remplacement(Solution enfant);
    void evolution(int N_max);

};

Solution random_init(GAPInstance *inst);

Solution croisement(pair <Solution,Solution> parents);

vector <Solution> init_population(int pop_size, GAPInstance *inst);

Solution genetic_algo_naif(int N_max, int pop_size, GAPInstance* inst);
