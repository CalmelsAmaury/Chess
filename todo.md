# TODO LIST

# 1 - Définir le format du fichier définissant l'état de l'échiquier
    Ce fichier permet de stocker sous forme textuelle la position de toutes les pièces.
    1.1 définir le format pour une pièce 
    1.2 définir le format général du fichier
    1.3 écrire un exemple d'échiquier dans : https://github.com/CalmelsAmaury/Chess/blob/main/test/Chess.cfg

Attendre la validation de cette étape.

# 2 - créer dans la class Model un fonction permettant de charger le fichier échiquier
     2.1 écrire une fonction de chargement d'un fichier text Model.load(path)
     2.2 interpreter le contenue du fichier et pour toutes les pièces, instancier une classe dérivant de la classe Piece
     2.3 ajouter l'instance à la liste des pieces du modèle : Model.pieces_
     2.4 tester le bon fonctionnement du load dans https://github.com/CalmelsAmaury/Chess/blob/main/test/test_algo.cpp
          pour mémoire : Model c;
                     auto loaded = c.load("Chess.cfg");
                     assert(loaded==true);

Attendre la validation de cette étape.
