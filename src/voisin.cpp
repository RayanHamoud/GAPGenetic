#include<voisin.h>

int agent(int t , Solution sol){ //Renvoie l'agent assigné à la tâche t dans la solution sol !! a changer avec la nouvelle implémentation
    int M = sol.instance->M;
    int index = 0;
    for (int i = 0; i<M; i++){
        if (sol.x[i][t]){
            index = i;
        }
    }
    return index;
}


vector<Solution> Vois_1::generate(){
    vector<Solution> voisins;
    int M = instance->M;   
    int T = instance->T;
    
    //On parcourt la sol et pour chaque tâche on crée les (M-1)xT solutions possibles
    //On ne les push que si elles sont faisables

    for (int t = 0; t < T; t++){
        //int idx = agent(t, sol);
        int idx = sol.task_assignment[t];
        for (int i = 0; i<M; i++){
            if (i != idx){
                Solution new_sol(sol);
                new_sol.assign(t,i);
                // new_sol.task_assignment[t] = i;
                // new_sol.computeRessourceViolations();
                // new_sol.computeProfit();

                if (new_sol.unfitness == 0){
                    voisins.push_back(new_sol);
                }
            }
        }        
    }
    return voisins;
}