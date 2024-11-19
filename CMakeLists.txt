cmake_minimum_required(VERSION 3.10)  # Version minimale requise de CMake
project(GAPGenetic)       # Nom du projet

# Spécifie les normes C++ à utiliser (optionnel)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Indiquer le répertoire contenant les fichiers d'en-tête
include_directories(src)

# Récupérer tous les fichiers source dans le dossier src
file(GLOB_RECURSE SOURCES 
    src/*.cpp     # Inclut tous les fichiers .cpp dans le dossier src
    src/*.h       # Inclut tous les fichiers .h dans le dossier src
)

# Créer l'exécutable avec les fichiers sources
add_executable(main.exe main.cpp ${SOURCES}) # Inclut main.cpp et tous les fichiers source
