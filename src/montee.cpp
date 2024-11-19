#include "montee.h"

Solution montee(Solution sol, GAPInstance *inst , bool maximisation){
    //Algorithme de montée, on part d'une solution courante
    //On construit un voisinnage selon une méthode donnée (a compléter plus tard)
    Solution sol_max = sol;

    //Condition d'arrêt : nombre d'appels à generate dans notre cas 
    int k_max = 100; //Pour tester, cond d'arrêt sur le nombre de voisinages générés
    int k = 0;
    while (k<k_max){
        Solution current = sol_max;
        Vois_1 V_current("current"+to_string(k), current, inst) ; //On construit le voisinnage de current
        vector<Solution> current_voisins = V_current.generate();
        for (Solution s : current_voisins) {
            if (maximisation){
                if (s.fitness > sol_max.fitness) {
                    sol_max = s; 
                }
            }
            else {
                if (s.fitness < sol_max.fitness) {
                    sol_max = s; 
                }
            }                
        }
        if (current.x != sol_max.x){
            k += 1; //On a trouvé une meilleure solution
        }
        else {
            //cout<<"Optimum local atteint pour k = "<<k<<endl;
            k = k_max+1; //On a déjà un optimum local (un voisinage qui n'améliore pas)
        }
    }
    return sol_max;
}   