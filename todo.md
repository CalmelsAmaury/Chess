# TODO LIST

# 1 - Définir le format du fichier définissant l'état de l'échiquier
    Ce fichier permet de stocker sous forme textuelle la position de toutes les pièces.
    1.1 définir le format pour une pièce 
    1.2 définir le format général du fichier
    1.3 écrire un exemple d'échiquier dans : https://github.com/CalmelsAmaury/Chess/blob/main/test/Chess.cfg

 > décrire ce format dans un nouveau fichier format.md.
Attendre la validation de cette étape. VALIDER le 1/12/2024 

# 2 - créer dans la class Model une fonction permettant de charger le fichier échiquier
     2.1 écrire une fonction de chargement d'un fichier text Model.load(path)
     2.2 interpreter le contenue du fichier et pour toutes les pièces, instancier une classe dérivant de la classe Piece
     2.3 ajouter l'instance à la liste des pieces du modèle : Model.pieces_
     2.4 tester le bon fonctionnement du load dans https://github.com/CalmelsAmaury/Chess/blob/main/test/test_algo.cpp
          pour mémoire : Model c;
                     auto loaded = c.load("Chess.cfg");
                     assert(loaded==true);

Attendre la validation de cette étape. VALIDER le 1/12/2024 

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

Attendre la validation de cette étape. 

# 4 - exercice d'exolication orale à Fabrice & Marylise du code (à la Feynman)

Attendre la validation de cette étape. 

