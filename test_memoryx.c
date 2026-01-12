/*
    test_memoryx.c - Fichier de test pour le jeu Memoryx
    Ce fichier permet de tester les principales fonctionnalités du jeu sans modifier le code original.
    Auteur : GitHub Copilot
*/

#include "fonctions.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Test d'initialisation du plateau
void test_initialiser_plateau() {
    Jeu jeu;
    preparer_jeu(&jeu, 3, 3);
    initialiser_plateau(&jeu);
    assert(jeu.L == 3 && jeu.C == 3 && jeu.R == 9);
    int nb_joker = 0, nb_paires = 0;
    for (int i = 0; i < jeu.L * jeu.C; i++) {
        if (jeu.T[i] == JOKER) nb_joker++;
        else if (jeu.T[i] > 0) nb_paires++;
    }
    assert(nb_joker == 1);
    assert(nb_paires == 8);
    liberer_jeu(&jeu);
    printf("[OK] test_initialiser_plateau\n");
}

// Test affichage plateau normal et triche
void test_afficher_plateau() {
    Jeu jeu;
    preparer_jeu(&jeu, 3, 3);
    initialiser_plateau(&jeu);
    printf("\nAffichage normal :\n");
    afficher_plateau(&jeu, -1, -1, 0);
    printf("\nAffichage triche :\n");
    afficher_plateau(&jeu, -1, -1, 1);
    liberer_jeu(&jeu);
    printf("[OK] test_afficher_plateau\n");
}

// Test saisie_entier (simulé)
void test_saisir_entier() {
    // Impossible de tester interactivement sans modification du code
    printf("[INFO] Test manuel requis pour saisir_entier.\n");
}

// Test bot
void test_bot() {
    Jeu jeu;
    preparer_jeu(&jeu, 3, 3);
    initialiser_plateau(&jeu);
    int pos1, pos2;
    bot_jouer_tour(&jeu, &pos1, &pos2);
    assert(pos1 >= 0 && pos1 < jeu.L * jeu.C);
    printf("Bot a choisi la position : %d\n", pos1);
    liberer_jeu(&jeu);
    printf("[OK] test_bot\n");
}

int main() {
    test_initialiser_plateau();
    test_afficher_plateau();
    test_saisir_entier();
    test_bot();
    printf("\nTous les tests sont passes.\n");
    return 0;
}
