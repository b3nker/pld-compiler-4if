# Présentation

Ce répertoire rassemble l'ensemble des sources et des tests pour le Projet Longue Durée "Compilateur" de l'hexanôme H4232.

# L'équipe

-   Guillaume Berthomet
-   Corentin Forler
-   Benjamin Kermani
-   Yohan Meyer
-   Nicolas Trouin
-   Erwan Versmée

# Structure du code

## `compiler`

Le dossier `compiler` contient l'ensemble des sources et fichiers utiles à la compilation de notre compilateur (ainsi qu'un Makefile, en plus du Makefile racine). Il est organisé en arborescence, avec les sources entièrement contenues dans le dossier `src`. Dans ce sous-dossier, la racine contient les fichiers de bases de notre compilateur, `ast` contient l'ensemble des classes qui nous ont permis d'implémenter un Abstract Syntax Tree, `types` contient tous les fichiers de gestion des types, `symbols` contient tout le nécessaire pour gérer les tables des symboles et `utils` contient des fichiers de fonctions utilitaires.

La documentation des classes de `compiler` se trouve dans le dossier `compiler/doc`, accessible en ligne à l'adresse <https://cogk.gitlab.io/pld-compilateur-4if/hierarchy.html>.

## `tests`

Le dossier `tests` contient notre framework de test (`pld-test.py`) qu'il est possible d'appeler plus simplement grâce à la commande `./test.sh [sous-dossier]`. De base, la commande `./test.sh` effectue l'ensemble des tests contenus dans le sous-dossier `tests`, mais il est possible de lui indiquer en argument un sous-dossier de cette base de test pour que le framework ne teste que les fichiers de ce sous-dossier (par exemple, `./test.sh CompExpr` n'effectuera que les tests contenus dans `tests/CompExpr`).

Par ailleurs, le framework de test que nous utilisons est une version amélioré du framework qui nous a été fourni au départ : séparant les tests en groupes (selon leur dossier parent) et utilisant des couleurs et un résumé d'exécution. Il nous a beaucoup aidé à garantir la qualité de l'ensemble des fonctionnalités que nous implémentions, autant en tests de nouvelles fonctionnalités qu'en tests de non-régression.

À l'heure actuelle, notre framework contient 210 tests différents répartis en 17 catégories.

Néanmoins, ces tests ayant été majoritairement écrits manuellement, ils ne couvrent pas tous les cas possibles, qu'il s'agisse de tests "positifs" (qui viennent valider une fonctionnalité), ou "négatifs" (qui viennent tester des cas d'erreurs ou de syntaxe invalide). Des tests de fuzzing, c'est-à-dire générés automatiquement, ont aussi été intégrés à notre suite de test (à l'aide des scripts python `fuzz.py`, `nest.py` et `100.py`), mais ne sont que des tests "positifs".

# Fonctionnalités implémentées

## Fonctionnalités présentes lors de la soutenance

Voici ci-dessous une liste succinte (car déjà présentée) des fonctionnalités présentes au moment de la présentation du Mercredi 7 Avril.

-   La grammaire que nous acceptons est visible dans le fichier `compiler/src/ifcc.g4`.
    -   Un seul fichier est accepté, qui peut contenir un sous-ensemble du langage C ainsi que des commentaires (single and multi lines) et des directives préprocesseurs qui sont ignorées.
-   Variables : nous acceptons des variables pouvant être déclarées n'importe où dans une fonction, de type `int` ou `int[]`. Ces variables sont stockées dans la table des symboles de la fonction où elles sont déclarées.
    -   À noter : il était déjà possible d'utiliser des constantes caractères _(ex. : `'a'` ou `'\n'` ou `'\127'`)_.
-   Opérations arithmétiques et logiques :
    -   Moins unaire `-`, Not logique `!`, Not bit-à-bit `~`
    -   Addition `+`, Soustraction `-`, Multiplication `*`, Division `/`, Modulo `%`
    -   Et bit-à-bit `&`, Ou bit-à-bit `|`, Ou exclusif bit-à-bit `^`
    -   Et logique paresseux `&&`, Ou logique paresseux `||`
    -   Affectation avec valeur de retour _(ex. : `a = b = 2` fonctionne)_.
-   Structures de contrôle :
    -   `if` et `if else`
    -   `while`
    -   `for`
-   Fonctions présentant un nombre arbitraire d'arguments
    -   Possibilité de séparer déclaration et définition de fonction
    -   Possibilité de déclarer et appeler des fonctions sans corps (par exemple pour déclarer une fonction de la bibliothèque standard comme `int putchar(int c);`)
-   AST
    -   Nous avons implémenté un AST sous la forme d'une hiérarchie de classe C++. Cet AST nous a par la suite servi pour effectuer plusieurs optimisations de type "simplification de noeud" et quelques analyses statiques de plus qu'un simple visiteur.
-   Analyses statiques
    -   Quelques simples analyses statiques : variable utilisé sans être déclarée, variable jamais utilisée, variable utilisée sans jamais être initialisée, etc...

## Fonctionnalités implémentées après la soutenance

Voici ci-dessous une liste des fonctionnalités (hors optimisation d'assembleur) que nous avons implémentées après la soutenance et qui sont désormais fonctionnelles dans notre projet :

-   Opérations arithmétiques : Ajout de l'incrémentation et de la décrémentation préfixe (`++a`, `--a`).
-   Gestion de l'hexadécimal : les constantes `0x` et les caractères échappés hexadécimaux _(ex. : `'\0x0F'`)_ sont acceptés par notre compilateur.
-   Les blocs sont enfin correctement gérés comme des statements.
-   Quelques corrections ont été apportées à la gestion des tableaux, mais le support reste partiel.

Voici ensuite les optimisations via les expressions que nous avons apportées à l'assembleur généré :

-   Les expressions s'évaluent correctement à des constantes le cas échéant (ex: `0 + 3 * 5` est remplacé par `15`). Le cas particulier incluant une division ou un modulo par 0 conserve la division / le modulo afin de continuer à projeter une erreur à l'exécution.
-   Les expressions reconnaissent la présence d'éléments neutres _(ex. : `1 * a` est réduit à `a`)_.
-   Les structures de contrôles s'optimisent dans certain cas de conditions constantes :
    -   Si la condition d'un `if` est constante, il se réduit à son seul statement `then` (conditon vraie) ou son seul statement `else` (condition fausse) en conséquence. S'il n'a pas de `else`, il est entièrement retiré à la place en cas de condition fausse.
    -   Si la condition d'un `while` est constante, il est retiré de l'assembleur si elle est toujours fausse ou il émet un "warning, boucle infinie" si elle est toujours vraie.
    -   Si la condition d'un `for` est constante, il est réduit à son simple statement d'initialisation si elle est toujours fausse ou il émet un warning si elle est toujours vraie.
-   Les statements se réduisant à une expression constante ou à une variable n'apparaissent pas dans l'assembleur _(ex: `3 + 5;` ou `a;`)_.

Enfin, voici les optimisations plus générales que nous avons apportées à notre compilateur :

-   Le nombre de variables temporaires utilisées a été grandement réduits, à une quantité très proche du minimum nécessaire via un système de réutilisation des variables temporaires.
-   De même, le nombre de `mov` inutiles a été autant que possible réduit (exemple simple de `mov` inutile : `mov %eax, -4(%rbp)` suivi de `mov -4(%rbp), %eax`).
-   Enfin, une optimisation "_Tail Call_" a été ajoutée pour réutiliser la même _Stack Frame_ si une fonction retourne l'appel d'une autre fonction _(ex. : `... return f2();`)_.

# Conclusion

Ce projet fut long, et parfois difficile (notamment aux moments charnières où nous étions loins d'être sûrs de ce que nous devions et pouvions implémenter), mais il nous a permis de beaucoup apprendre sur les compilateurs, ainsi que de perfectionner notre capacité à coder et correctement tester en équipe : il a été une réelle expérience positive pour notre hexanôme.
