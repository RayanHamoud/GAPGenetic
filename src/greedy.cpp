#include<greedy.h>

std::vector<int> getKeysSortedByValueDescending(const std::map<int, float>& inputMap) {
    // Créer un vecteur de paires pour stocker les paires clé-valeur
    vector<std::pair<int, float>> vec(inputMap.begin(), inputMap.end());
    
    sort(vec.begin(), vec.end(), [](const auto& a, const auto& b) {
        return a.second > b.second; // Comparaison des valeurs   
        }
        );


    // // Trier le vecteur par valeur en ordre décroissant
    // sort(vec.begin(), vec.end(), [](const auto& a, const auto& b) {
    //         return a.second > b.second; // Comparaison des valeurs   
    // });

    // Extraire les clés dans un vecteur
    vector<int> sortedKeys;
    for (const auto& pair : vec) {
        sortedKeys.push_back(pair.first);
    }

    return sortedKeys;
}

vector<int> best_ratio(int t, GAPInstance *inst){
    //t : task at study
    //inst: our current working instance
    //return a vector of agent's indexes by ratio profit/ressource (ordre décroissant)
    map<int,float> ratios; 
    for (int i = 0; i < inst->M; i++){
        if (inst->max){
            ratios[i] = float(inst->profit[i][t])/float(inst->resource[i][t]);
        }
        else {
            ratios[i] = float(inst->resource[i][t]) * float((inst->profit[i][t]));
        }
    }
    vector<int> best_ratio = getKeysSortedByValueDescending(ratios);
    return best_ratio;
}

Solution greedy(GAPInstance *inst){
    Solution sol(inst);

    for (int t = 0 ; t < inst->T; t++){
        vector<int> ratio_idx = best_ratio(t, inst); // ordered vector of agents for the task t by better ratio

        for (int i : ratio_idx){
            int ressource = inst->resource[i][t];
            int capa = inst->capacities[i];
            int utilisation = sol.agent_resource_usage[i];
            
            if (utilisation + ressource <= capa){
                sol.x[i][t] = true;
                sol.agent_resource_usage[i] = utilisation + ressource; 
                sol.task_assignment[t] = i; 
                break;
            } // On peut l'assigner à l'agent i
                
        }
    }   
    sol.computeProfit();
    sol.computeRessourceViolations();
    return sol;
}
