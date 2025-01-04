# TODO LIST

# 1 - Définir le format du fichier définissant l'état de l'échiquier
    Ce fichier permet de stocker sous forme textuelle la position de toutes les pièces.
    1.1 définir le format pour une pièce 
    1.2 définir le format général du fichier
    1.3 écrire un exemple d'échiquier dans : https://github.com/CalmelsAmaury/Chess/blob/main/test/Chess.cfg

 > décrire ce format dans un nouveau fichier format.md.
Attendre la validation de cette étape. VALIDE le 1/12/2024 

# 2 - créer dans la class Model une fonction permettant de charger le fichier échiquier
     2.1 écrire une fonction de chargement d'un fichier text Model.load(path)
     2.2 interpreter le contenue du fichier et pour toutes les pièces, instancier une classe dérivant de la classe Piece
     2.3 ajouter l'instance à la liste des pieces du modèle : Model.pieces_
     2.4 tester le bon fonctionnement du load dans https://github.com/CalmelsAmaury/Chess/blob/main/test/test_algo.cpp
          pour mémoire : Model c;
                     auto loaded = c.load("Chess.cfg");
                     assert(loaded==true);

Attendre la validation de cette étape. VALIDE le 1/12/2024 

# 3 - ajouter un identifiant unique pour chaque pièce de l’échiquier.

Spécification d’une ligne : <id>,<Type Pièce>,<Couleur>,<colonne>,<rang>

Par exemple :

1,Knight,b,b,8
2,Knight,b,g,8
3,Pawn,w,a,2
4,Pawn,w,b,2
5,Pawn,w,c,2
6,Pawn,w,d,2


De cette façon nous pouvons par la suite analyser le mouvement d’une pièce sur toute une partie. Sinon ce n’est pas possible.

    3.1 modifier le fichier de config de l'échiquier
    3.2 modifier le load
    3.3 ajouter une fonction de recherche d'un pièce par son ID (un peux comme getPiece)

Attendre la validation de cette étape. VALIDE LE 1/12/2024

# 4 - Créer une fonction save qui créer un nouveaau fichier contenant les pièces du modèle en mémoire de la classe Model.
  4.1 Créer la fonction Model::save\
  4.2 ajouter cette fonction au fichier de test test_algo

Attendre la validation de cette étape. VALIDE LE 2/12/2024

# 5 - Définir l'algorithme qui calcul le prochain coup de l'échiquier pour une couleur\
  5.1 écrire cet algorithme en pseudo code dans un fichier texte (ne pas écrire de code c++)\

  METHODOLOGIE\
  M1. ecrire le titre des grands chapitres de l'algo (choisir le plus simple)\
  M2. pour chaque chapitre, écrire le titre des sections signifiantes\
  M3. pour chaque section écrire le pseudo code\
  
 

# 6 - Ecrire le nextPossibleMoves et le bestMove (ainsi que les tests unitaires correspondant) pour chaque piece de l'échiquier : 

6.1 Roi (1 pièce)/ 
6.2 Dame ou Reine (1 pièce)/
6.3 Tours (2 pièces)/ 
6.4 Fous (2 pièces)/  
6.5 Cavaliers (2 pièces)/
6.7 Pions (8 pièces)/ 

  
# 7 - Créer un fonction qui dessine l'echiquier comme ci-dessous
 

    0  1  2  3  4  5  6  7
    A  B  C  D  E  F  G  H
  +------------------------+
0 | R  K  B  Q ^K ^.  K  . | 0
1 | P  .  P  P *.  P  P  P | 1
2 | .  p  .  .  .  .  .  p | 2
3 | .  .  .  .  .  .  .  R | 3
4 | .  p  P  p  .  B  .  . | 4
5 | .  .  k  .  .  q  .  . | 5
6 | .  .  p  .  p  p  p  . | 6
7 | r  P  b  .  k  b  k  r | 7
  +------------------------+
    A  B  C  D  E  F  G  H
    0  1  2  3  4  5  6  7

VALIDE LE 27/12/2024

# 8 - Refactoriser l'application avec un fichier par piece, un fichier pour le modèle
VALIDE LE 3/1/2025

# 9 - Application Front (partie A)

Le jeux sera jouable depuis une page internet via une interface utilisateur écrite en React Typescript Css et utilisant NextJs.
Le jeux permet à deux utilisateur de commencer et de jouer une partie ensemble. L'IA (fonction bestMove c++) sera appelée pour donner une 
indication à l'utilisateur de la meilleur pièce à jouer.

L'application Front doit être dans le dossier Presentation.
Utiliser un fichier css pour le design (pas de Tailwind)

1 - créer une appli NextJS en mode Typescript 

2 - créer un composant React functional pour afficher une board 





# N - exercice d'explication oral à Fabrice & Marylise du code (à la Feynman)

 

