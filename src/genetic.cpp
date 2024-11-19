#include <genetic.h>

// bool callWithProbability(double probability) {
//     // Initialiser le générateur de nombres aléatoires
//     std::random_device rd;
//     std::mt19937 gen(rd());
//     std::uniform_real_distribution<> dis(0.0, 1.0); // Distribution continue entre 0.0 et 1.0

//     // Retourner true avec la probabilité spécifiée
//     return dis(gen) < probability;
// }


// int getRandomInt(int T) {
//     // Initialiser le générateur de nombres aléatoires
//     // Générateur de nombre aléatoire basé sur Mersenne Twister
//     std::uniform_int_distribution<> dis(0, T - 1); // Distribution entre 0 et T-1
//     std::random_device rd;
//     std::seed_seq seed{rd(), static_cast<unsigned int>(std::chrono::steady_clock::now().time_since_epoch().count())};
//     std::mt19937 gen(seed);

//     return dis(gen); // Génère un entier entre 0 et T-1
// }


//Fonction qui va nous donner deux couples d'indice à tester dans la selection binare
pair<pair<int, int>,pair<int, int>> getTwoPairs(int n) {
    std::random_device rd;
    std::seed_seq seed{rd(), static_cast<unsigned int>(std::chrono::steady_clock::now().time_since_epoch().count())};
    std::mt19937 gen(seed);
    // Créer un vecteur contenant les entiers de 1 à n
    std::vector<int> numbers(n);
    for (int i = 0; i < n; ++i) {
        numbers[i] = i;
    }

    // Mélanger le vecteur de façon aléatoire
    std::shuffle(numbers.begin(), numbers.end(), gen);

    // Créer deux couples avec les quatre premiers entiers du vecteur mélangé
    std::pair<int, int> pair1 = {numbers[0], numbers[1]};
    std::pair<int, int> pair2 = {numbers[2], numbers[3]};

    return {pair1, pair2};
}


Solution random_init(GAPInstance *inst){
    //Fonction qui va initier une solution aléatoirement
    //On va respecter la contrainte à chaque tâche un agent
    //La seule contrainte qu'on va rompre c'est 
    Solution sol(inst);
    for (int t = 0 ; t < inst->T; t++){
        int i = getRandomInt(inst->M); //On tire au sort l'agent
        sol.task_assignment[t] = i;
    }
    
    sol.computeProfit(); //fitness
    sol.computeRessourceViolations(); //unfitness
    return sol;
}

vector <Solution> init_population(int pop_size, GAPInstance *inst){
    vector <Solution> population; 
    //On va initialiser une population de solution aléatoires
    for (int iter = 0; iter < pop_size; iter++){
        Solution new_sol = random_init(inst);
        population.push_back(new_sol);
    } 
    return population;  
}

Solution mutation (Solution *sol){//On permute les agents de deux tâches aléatoires
    Solution new_sol(sol);
    int T = new_sol.instance->T;
    //Shift d'agents entre deux tâches prises au hasard
    int t1 = getRandomInt(T);//On tire au sort la 1 ere tâche
    int i1 = new_sol.task_assignment[t1];

    int t2 = getRandomInt(T);//On Tire au sort la seconde tâche
    
    while (t2 == t1 ){
        t2 = getRandomInt(T);
    }
    int i2 = new_sol.task_assignment[t2];

    swap(new_sol.task_assignment[t1], new_sol.task_assignment[t2]);
    new_sol.computeProfit();
    new_sol.computeRessourceViolations();
    
    return new_sol;
}

Solution mutation_heuri(Solution* sol){
    Solution new_sol(sol);
    vector<int> agents;

    //On va ranger les agents dans un ordre aléatoire (recherche de diversité)
    vector<int> to_shuffle;
    for(int i = 0; i< sol->instance->M; i++){
        to_shuffle.push_back(i);
    }
    std::random_device rd;
    std::seed_seq seed{rd(), static_cast<unsigned int>(std::chrono::steady_clock::now().time_since_epoch().count())};
    std::mt19937 gen(seed);
    std::shuffle(to_shuffle.begin(), to_shuffle.end(), gen);
    agents = to_shuffle;
    
    for (int i : agents){
        if (sol->agent_ressource_violations[i] > 0){
            vector<int> tasks_i; // tâches effectuées par l'agent t
            for (int t = 0; t<sol->instance->T ; t++){
                if (sol->task_assignment[t] == i){
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

            for(int agent = 0 ; agent<sol->instance->M; agent++){
                int agent_usage = sol->agent_resource_usage[agent];
                int agent_task_resource = sol->instance->resource[agent][task];
                int agent_capa = sol->instance->capacities[agent];
                
                if (agent_usage+agent_task_resource<=agent_capa){
                    free_agents_task.push_back(agent);
                }
                if (agent_usage+agent_task_resource - agent_capa < sol->agent_ressource_violations[i]){
                    better_agents.push_back(agent);
                }                
            }
            //Si on a la liste d'agents capables de l'effectuer 
            if (free_agents_task.size()>0){
                int best_agent = free_agents_task[0];
                for (int agent : free_agents_task){
                    if (sol->instance->profit[agent][task]>sol->instance->profit[best_agent][task]){
                        best_agent = agent;
                    }
                }
                new_sol.assign(task, best_agent);
                // new_sol.task_assignment[task] = best_agent;
                // new_sol.computeProfit();
                // new_sol.computeRessourceViolations();
                return new_sol;
            }

            //Si on a au moins un meilleur agent
            else if (better_agents.size() >0){
                int best_agent = better_agents[0];
                for (int agent : better_agents){
                    if (sol->instance->profit[agent][task]>sol->instance->profit[best_agent][task]){
                        best_agent = agent;
                    }
                }
                new_sol.assign(task, best_agent);
                // new_sol.task_assignment[task] = best_agent;
                // new_sol.computeProfit();
                // new_sol.computeRessourceViolations();
                return new_sol;
            }        
        }
    }
    return new_sol;
}

pair <Solution,Solution> Population::selection(){
    
    int N = vec.size();
    pair <pair<int,int>,pair<int,int>> contests = getTwoPairs(N);
    pair<int,int> pair1 = contests.first;
    pair<int,int> pair2 = contests.second;
    
    Solution parent1;
    Solution parent2;
    if (vec[pair1.first].fitness >vec[pair1.second].fitness){
        parent1 = vec[pair1.first];

    }
    else {
        parent1 = vec[pair1.second];
    }

    if (vec[pair2.first].fitness >vec[pair2.second].fitness){
        parent2 = vec[pair2.first];
    }
    else {
        parent2 = vec[pair2.second];
    }
    pair <Solution, Solution> parents = {parent1, parent2};
    
    return parents;
}

Solution croisement(pair <Solution,Solution> parents){
    Solution parent1(parents.first);
    Solution parent2(parents.second);

    //On utiliser un single crossover point
    Solution enfant(parent1);
    int T = enfant.instance->T;
    int p = getRandomInt(T);
    //cout<<"le single cross se fait a l'indice "<<p<<endl;

    if (p > T/2){
        for (int _t =0; _t < p; _t ++){
            enfant.task_assignment[_t] = parent1.task_assignment[_t];
        }
        for (int _t =p; _t < T; _t ++){
            enfant.task_assignment[_t] = parent2.task_assignment[_t];
        }
    }
    else{
        for (int _t =0; _t < p; _t ++){
            enfant.task_assignment[_t] = parent2.task_assignment[_t];
        }
        for (int _t =p; _t < T; _t ++){
            enfant.task_assignment[_t] = parent1.task_assignment[_t];
        }
    }
    enfant.computeProfit();
    enfant.computeRessourceViolations();
    enfant.reparation(max_repa);
    return enfant;
}

void Population::remplacement(Solution enfant){
    
    Solution worst = vec[0];
    for (Solution sol:vec){
        if(sol.unfitness > worst.unfitness){
            worst = sol;
        }
    } 
    if (worst.unfitness>0){
        vec.erase(remove(vec.begin(), vec.end(), worst), vec.end());
    }
    if (worst.unfitness == 0 and enfant.unfitness == 0){
        for (Solution sol : vec){
            if (sol.fitness<worst.fitness){
                worst = sol;
            }
        }
        vec.erase(remove(vec.begin(), vec.end(), worst), vec.end());
        vec.push_back(enfant);
    }
    else{
        vec.push_back(enfant);
    } 
    if (enfant.unfitness == 0 and enfant.fitness>best.fitness){
        best = enfant;
    }
}

void Population::evolution(int N_max){
    int non_ameliorant = 0;
    int nb_enfants_generes = 0;
    auto start = std::chrono::high_resolution_clock::now();
    auto current = std::chrono::high_resolution_clock::now();

    while (non_ameliorant < N_max ){
        current = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = current - start;
        if (elapsed.count() >= timeLimitMs) {
            //std::cout << "Temps limite atteint (" << timeLimitMs << " ms)" << std::endl;
            //cout<<non_ameliorant<<endl;
            break; // Interrompre l'exécution si le temps limite est atteint
        }
        //Gnération d'enfants (on génère jusqu'à ce qu'on ait un enfant nouveau dans la population)
        pair<Solution, Solution> parents = selection();
        Solution enfant = croisement(parents);
        auto it = std::find(vec.begin(), vec.end(), enfant);
        int k = 0;
        while (it != vec.end() and k < 1000){
            k+=1;
            enfant.mutation();
            enfant.reparation(max_repa);
            it = std::find(vec.begin(), vec.end(), enfant);
        }
        if (enfant.fitness>best.fitness and enfant.unfitness == 0){
            best = enfant;
        }
        else{
            non_ameliorant+=1;
        }
        remplacement(enfant);
        nb_enfants_generes+=1;
        //cout<<elapsed.count()<<endl;
    }
}

Solution genetic_algo_naif(int N_max, int pop_size, GAPInstance* inst){
    
    vector<Solution> solutions = init_population(pop_size, inst);
    //cout<<solutions.size()<<endl; 
    Population population(solutions,inst);
    population.evolution(N_max);
    cout<<"La meilleure solution generee a une fitness de "<<population.best.fitness<<" et unfitness de "<<population.best.unfitness<<endl;
    //population.best.display();

    return population.best;
}