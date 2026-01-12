/*
    memoryx.c - Programme principal du jeu Memoryx
    Auteur : Ahmet BASBUNAR
 */

#include "fonctions.h"

/* ============================================================
   PROGRAMME PRINCIPAL
   ============================================================ */

// Fonction principale : gère le menu, la configuration, la boucle de jeu et la fin
int main(void) {
    Jeu jeu;
    int L, C, choix_menu, modeTriche = 0, modeSolitaire = 0, rejouer;
    srand((unsigned int)time(NULL));

    afficher_banniere();

    while (1) {
        rejouer = 0;

        choix_menu = afficher_menu_principal(&modeTriche);
        if (choix_menu == 5) return 0;

        configurer_plateau(&L, &C);

        preparer_jeu(&jeu, L, C);
        initialiser_plateau(&jeu);

        configurer_joueurs(&jeu, choix_menu, &modeSolitaire);

        gettimeofday(&jeu.tempsDebut, NULL);
        printf(GREEN "\n  >>> Partie lancee ! Bonne chance ! <<<\n" RESET);
        attendre(2);

        /* BOUCLE PRINCIPALE DE JEU */
        while (jeu.R > 1) {
            jouer_tour(&jeu, modeSolitaire, modeTriche);
        }

        gerer_fin_partie(&jeu, modeSolitaire, &rejouer);
        if (rejouer != 1) {
            printf(GREEN "\nMerci d'avoir joue ! A bientot !\n" RESET);
            break;
        }
    } /* Fin boucle while(1) */

    return 0;
}
