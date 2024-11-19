#include "Solution.h"
int getRandomInt(int T) {
    // Initialiser le générateur de nombres aléatoires
    // Générateur de nombre aléatoire basé sur Mersenne Twister
    std::uniform_int_distribution<> dis(0, T - 1); // Distribution entre 0 et T-1
    std::random_device rd;
    std::seed_seq seed{rd(), static_cast<unsigned int>(std::chrono::steady_clock::now().time_since_epoch().count())};
    std::mt19937 gen(seed);

    return dis(gen); // Génère un entier entre 0 et T-1
}


int Solution::computeProfit()
{   
    int profit = 0;
    fitness = 0;
     for (int t = 0; t < instance->T; t++)
    {
        int agent_idx = task_assignment[t];
        profit += instance->profit[agent_idx][t];
    }
    fitness = profit;
    return profit;
}

void Solution::computeRessourceViolations()
{   
    unfitness = 0;
    for (int i = 0; i < instance->M; i++)
    {
        agent_ressource_violations[i] = 0;
        agent_resource_usage[i] = 0;
    }

    // Step 2: Calculate total resource used by agent i
    for (int j = 0; j < instance->T; ++j)
    {
        int agent_idx = task_assignment[j];
        agent_ressource_violations[agent_idx] += instance->resource[agent_idx][j];
        agent_resource_usage[agent_idx] += instance->resource[agent_idx][j];
    }
    for (int i = 0; i < instance->M; i++)
    {
        // Step 3: Calculate violation for agent i
        agent_ressource_violations[i] = max(0, agent_ressource_violations[i] - instance->capacities[i]);
        unfitness+= agent_ressource_violations[i];
    }
    
}

void Solution::assign(int task, int new_agent){
    //Nous allons assigner la tache task a l'agent new_agent
    int old_agent = task_assignment[task];
    fitness -= instance->profit[old_agent][task];
    unfitness -= agent_ressource_violations[old_agent];

    agent_resource_usage[old_agent] -= instance->resource[old_agent][task];
    agent_ressource_violations[old_agent] = max(0, agent_resource_usage[old_agent]-instance->capacities[old_agent]);
    unfitness += agent_ressource_violations[old_agent];

    unfitness -= agent_ressource_violations[new_agent];
    task_assignment[task] = new_agent;
    fitness += instance->profit[new_agent][task];
    agent_resource_usage[new_agent] += instance->resource[new_agent][task];
    agent_ressource_violations[new_agent] = max(0, agent_resource_usage[new_agent]-instance->capacities[new_agent]);
    unfitness += agent_ressource_violations[new_agent];
}

bool Solution::isFeasible()
{
    computeRessourceViolations();
    return unfitness ==0;
}

void Solution::display()
{
    for (int t = 0; t < instance->T; ++t)
    {
        std::cout << " Task " << t << " affected to worker " << task_assignment[t] << endl;
    }
}

void Solution::mutation(){
    int T = instance->T;
    //Shift d'agents entre deux tâches prises au hasard
    int t1 = getRandomInt(T);//On tire au sort la 1 ere tâche
    int i1 = task_assignment[t1];

    int t2 = getRandomInt(T);//On Tire au sort la seconde tâche
    
    while (t2 == t1 ){
        t2 = getRandomInt(T);
    }
    int i2 = task_assignment[t2];
    assign(t1,i2);
    assign(t2,i1);
}
void Solution::mutation_heuri(){
    vector<int> agents;

    //On va ranger les agents dans un ordre aléatoire (recherche de diversité)
    vector<int> to_shuffle;
    for(int i = 0; i< instance->M; i++){
        to_shuffle.push_back(i);
    }
    std::random_device rd;
    std::seed_seq seed{rd(), static_cast<unsigned int>(std::chrono::steady_clock::now().time_since_epoch().count())};
    std::mt19937 gen(seed);
    std::shuffle(to_shuffle.begin(), to_shuffle.end(), gen);
    agents = to_shuffle;
    
    for (int i : agents){
        if (agent_ressource_violations[i] > 0){
            vector<int> tasks_i; // tâches effectuées par l'agent t
            for (int t = 0; t<instance->T ; t++){
                if (task_assignment[t] == i){
                    tasks_i.push_back(t);
                }
            }
            if (tasks_i.size()==0){
                break;
            }
            //On va choisir une tâche aléatoirement parmi elles, et créer une liste
            //d'agents qui ont la capacité de l'effectuer (
            //Si un tel agent existe :
            int task = tasks_i[getRandomInt(tasks_i.size())];

            vector<int> free_agents_task;
            vector<int> better_agents; //Vecteur d'agents pour qui ce serait mieux d'assigner la tâche (même s'ils sont déja surchargés)

            for(int agent = 0 ; agent<instance->M; agent++){
                int agent_usage = agent_resource_usage[agent];
                int agent_task_resource = instance->resource[agent][task];
                int agent_capa = instance->capacities[agent];
                
                if (agent_usage+agent_task_resource<=agent_capa){
                    free_agents_task.push_back(agent);
                }
                if (agent_usage+agent_task_resource - agent_capa < agent_ressource_violations[i]){
                    better_agents.push_back(agent);
                }                
            }
            //Si on a la liste d'agents capables de l'effectuer 
            if (free_agents_task.size()>0){
                int best_agent = free_agents_task[0];
                for (int agent : free_agents_task){
                    if (instance->profit[agent][task]>instance->profit[best_agent][task]){
                        best_agent = agent;
                    }
                }
                assign(task, best_agent);
                break;
            }

            //Si on a au moins un meilleur agent
            else if (better_agents.size() >0){
                int best_agent = better_agents[0];
                for (int agent : better_agents){
                    if (instance->profit[agent][task]>instance->profit[best_agent][task]){
                        best_agent = agent;
                    }
                }
                assign(task, best_agent);
                break;
            }        
        }
    }
    
}

void Solution::reparation(int repa_max){
    int n = getRandomInt(repa_max);
    int i =0;
    while(!isFeasible() and i < n){
        i+=1;
        mutation_heuri();
    }
}

