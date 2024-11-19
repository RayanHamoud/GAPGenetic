#pragma once
#include <vector>
#include <algorithm>
#include <limits.h>
#include "Instance.h"
#include <random>
#include<chrono>
#include "thread"

class Solution
{
public:
    GAPInstance *instance;
    // Two different representatiosn of the solution
    vector<vector<bool>> x;      // Binary matrix for the solution (n x m)
    vector<int> task_assignment; // task_assignment[j] = agent qui fait le job j

    int fitness; //Profit généré par la solution
    int unfitness; //somme des violations de ressources

    int delta_objective_cost;         // Total profit of the solution
    int penalized_objective_cost;     // Penalized profit of the solution
    vector<int> agent_resource_usage; // Resources used by each agent
    vector<int> agent_ressource_violations;

    bool operator==(const Solution& other) const {
        // Comparaison d'exemple, à adapter selon tes critères
        return task_assignment == other.task_assignment;
    }

    // Default constructor
    Solution() : instance(nullptr)
    {                            // You can initialize instance to nullptr or some default value
        task_assignment.clear(); // Initialize task_assignment if needed
    }

    // Copy constructor (already used in your code)
    Solution(const Solution &other)
    {   
        x = other.x;
        fitness=other.fitness;
        unfitness = other.unfitness;
        task_assignment = other.task_assignment; // Deep copy the vector
        agent_resource_usage = other.agent_resource_usage;
        agent_ressource_violations = other.agent_ressource_violations;
        instance = other.instance; // Shallow copy the instance pointer (both point to the same instance)
    }
    Solution(Solution* &other)
    {   
        x = other->x;
        fitness=other->fitness;
        unfitness = other->unfitness;
        task_assignment = other->task_assignment; // Deep copy the vector
        agent_resource_usage = other->agent_resource_usage;
        agent_ressource_violations = other->agent_ressource_violations;
        instance = other->instance; // Shallow copy the instance pointer (both point to the same instance)
    }

    Solution(GAPInstance *inst) : instance(inst)
    {
        // Initialize solution matrices
        x = vector<vector<bool>>(inst->M, vector<bool>(inst->T, 0));
        fitness=0;
        unfitness=0;
        task_assignment = vector<int>(inst->T, 0);
        penalized_objective_cost = 0;
        delta_objective_cost = 0;
        agent_resource_usage = vector<int>(inst->M, 0);
        agent_ressource_violations = vector<int>(inst->M, 0);
    }

    void computeRessourceViolations();
    int computeProfit();
    bool isFeasible();
    void display();
    void assign(int task, int new_agent);
    void mutation();
    void mutation_heuri();
    void reparation(int max_repa);

};

int getRandomInt(int T);