#include <iostream>
#include "src/Instance.h"
#include "src/Solution.h"
#include "src/greedy.h"
#include "voisin.h"
#include "montee.h"
#include "genetic.h"
#include "iles.h"
#include <iostream>
#include <chrono>
#include <benchmark.h>


int main()
{   
    GAPInstance gap("../data/gap12.txt",0, true);
    bool maximisation = gap.max;
    //Display the instance data
    //gap.display();
    // cout<<"----------------------------------------------------"<<endl;

    // int N_max = 500000;
    // int pop_size = 10;
    
    // auto start = std::chrono::high_resolution_clock::now();

    // Solution naif_genetic = genetic_algo_naif(N_max, pop_size, &gap);

    // // Fin du chronométrage
    // auto end = std::chrono::high_resolution_clock::now();

    // // Calcul de la durée en microsecondes
    // std::chrono::duration<double, std::milli> duration = end - start;
    // std::cout << "Temps d'exécution : " << duration.count() << " millisecondes" << std::endl;
    
    // int N_max = 200; //Nombre de fois qu'on s'autorise à ne pas améliorer la solution
    // int pop_size = 100; 
    // int numIslands = 8;
    // int generations = 200;
    
    // auto start = std::chrono::high_resolution_clock::now();
    // Solution sol_islands = run(numIslands, generations, pop_size, N_max, &gap,false);
    // // Fin du chronométrage
    // auto end = std::chrono::high_resolution_clock::now();

    // // Calcul de la durée en millisecondes
    // std::chrono::duration<double, std::milli> duration = end - start;
    // std::cout << "Temps d'execution : " << duration.count() << " millisecondes" << std::endl;
    // cout<<"La meilleure solution admissible trouvee est de fitness "<<sol_islands.fitness<<"-"<<sol_islands.isFeasible()<<endl;


    //Ecriture des résultats dans un ficher csv
    benchmark("../data", "../benchmark/benchmark_results_bd_.csv");
    processBenchmarkFiles("../benchmark/benchmark_results_bd_.csv", "../benchmark/best_known_values.csv","../benchmark/benchmark_results_with_gap_bd_.csv");

    return 0;
}
