/*
    fonctions.c - Définitions des fonctions pour le jeu Memoryx
    Auteur : Ahmet BASBUNAR
 */

#include "fonctions.h"

/*
   ============================================================
   UTILITAIRES
   ============================================================
*/

// Pause le programme pendant un nombre de secondes
void attendre(int secondes) { SLEEP(secondes); }

// Efface l'écran du terminal (en fait, scroll pour garder l'historique)
void effacer_ecran(void) {
    for (int i = 0; i < 0; i++) printf("\n");
}

// Vide le buffer d'entrée pour éviter les caractères restants
void vider_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    // vider le buffer d'entrée
}

/*
   ===========================================================
    SAISIE ENTIER
   ===========================================================
*/
int saisir_entier(const char *message, int min, int max) {
    char buffer[100];
    int valeur;
    while (1) {
        if (message) printf("%s", message);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            // Fin de saisie (ex : Ctrl-D) : on quitte proprement au lieu de boucler
            printf(RED "\n  Fin de saisie, arret du jeu.\n" RESET);
            exit(EXIT_SUCCESS);
        }
        // Supprimer le \n
        buffer[strcspn(buffer, "\n")] = 0;
        // Trim espaces
        char *start = buffer;
        while (*start == ' ' || *start == '\t') start++;
        char *end = start + strlen(start) - 1;
        while (end > start && (*end == ' ' || *end == '\t')) end--;
        *(end + 1) = 0;
        // Si vide, afficher message et reprompt
        if (*start == 0) {
            printf(RED "  Entree vide. Entrez un nombre entier.\n" RESET);
            continue;
        }
        // Parser
        if (sscanf(start, "%d", &valeur) == 1) {
            if (valeur < min || valeur > max) {
                printf(RED "  Erreur : entrez un nombre entre %d et %d.\n" RESET, min, max);
                continue;
            }
            return valeur;
        } else {
            printf(RED "  Entree invalide. Entrez uniquement un nombre entier.\n" RESET);
        }
    }
}

/*
   ============================================================
   AFFICHAGE
   ============================================================
*/

// Affiche la bannière du jeu avec le titre et les règles
void afficher_banniere(void) {
    printf(BLUE);
    printf("  __  __  ______  __  __   ____   _____  __   __ __   __\n");
    printf(" |  \\/  ||  ____||  \\/  | / __ \\ |  __ \\ \\ \\ / / \\ \\ / /\n");
    printf(" | \\  / || |__   | \\  / || |  | || |__) | \\ V /   \\ V / \n");
    printf(" | |\\/| ||  __|  | |\\/| || |  | ||  _  /   | |     > <  \n");
    printf(" | |  | || |____ | |  | || |__| || | \\ \\   | |    / . \\ \n");
    printf(" |_|  |_||______||_|  |_| \\____/ |_|  \\_\\  |_|   /_/ \\_\\\n");
    printf(RESET);
    printf(CYAN "          ============================================\n" RESET);
    printf(BOLD "                 Le jeu de memoire avec Joker\n" RESET);
    printf(CYAN "          ============================================\n\n" RESET);
    printf(GREEN "  >>> " RESET BOLD "Votre but" RESET " : Trouvez toutes les " YELLOW "paires de cartes" RESET " identiques !\n");
    printf(RED "  >>> ATTENTION !!!" RESET MAGENTA " Le JOKER [0]" RESET " vous fait passer et " RED "permute" RESET " avec une carte !\n\n");
}

// Fonction fusionnée pour afficher le plateau en mode normal ou triche
// mode_triche = 0 : affichage normal ( cartes retournées / positions visibles )
// mode_triche = 1 : affichage triche ( toutes les cartes visibles  )
void afficher_plateau(Jeu *jeu, int pos1, int pos2, int mode_triche) {
    int i, j, k;
    // i = ligne, j = colonne, k = position dans T

    // si mode triche, afficher Message MODE TRICHE ACTIVE
    if (mode_triche) {
        printf("\n" RED "      *** MODE TRICHE ***\n" RESET);
    } else {
        printf("\n");
    }

    // Affichage du plateau avec bordures et indices
    printf("      ");
    for (j = 0; j < jeu->C; j++) printf(CYAN "%3d " RESET, j);
    printf("\n     +");
    for (j = 0; j < jeu->C; j++) printf("----");
    printf("+\n");

    // boucle d'affichage des lignes
    for (i = 0; i < jeu->L; i++) {
        printf(CYAN "  %2d " RESET "|", i);
        for (j = 0; j < jeu->C; j++) {
            k = convertir_ij_en_k(i, j, jeu->C);
            if (jeu->T[k] == CARTE_RETIREE) {
                printf("    ");
            } else if (mode_triche) {
                if (k == pos1 || k == pos2) {
                    if (jeu->T[k] == JOKER) printf(BOLD RED " JK " RESET);
                    else printf(BOLD GREEN "%3d " RESET, jeu->T[k]);
                } else {
                    if (jeu->T[k] == JOKER) printf(RED " JK " RESET);
                    else printf(YELLOW "%3d " RESET, jeu->T[k]);
                }
            } else if (k == pos1 || k == pos2) {
                if (jeu->T[k] == JOKER) printf(RED " JK " RESET);
                else printf(YELLOW "%3d " RESET, jeu->T[k]);
            } else {
                printf("%3d ", k);
            }
        }
        printf("|\n");
    }

    // Affichage de la bordure inférieure
    printf("     +");
    for (j = 0; j < jeu->C; j++) printf("----");
    printf("+\n");
}
/*
exemple :
        0   1   2   3   4
      +-------------------+
    0 |  0  12  5  JK  7  |
    1 | 11   3  8   2 10  |
    2 |  4   6  9  1 13   |
      +-------------------+
*/

// Affichage des scores et du temps écoulé
void afficher_scores(Jeu *jeu, int modeSolitaire) {
    int n = (jeu->L * jeu->C - 1) / 2;
    long tempsEcoule, pairesTotales;
    struct timeval end;
    gettimeofday(&end, NULL);
    tempsEcoule = time_diff(jeu->tempsDebut, end);
    printf("\n--------------------------------------------------\n");
    if (modeSolitaire) {
        printf(" Mode Solitaire | %s | Score: %d/%d\n", jeu->joueurs[0].pseudo, jeu->joueurs[0].score, n);
        printf(" Coups: %d | Temps: %02ld:%02ld\n", jeu->coups, tempsEcoule / 60, tempsEcoule % 60);
    } else {
        pairesTotales = jeu->joueurs[0].score + jeu->joueurs[1].score;
        printf(" %s: " GREEN "%d" RESET "  VS  %s: " GREEN "%d" RESET " | Paires: %ld/%d\n",
               jeu->joueurs[0].pseudo, jeu->joueurs[0].score, jeu->joueurs[1].pseudo, jeu->joueurs[1].score, pairesTotales, n);
        printf(" Coups: %d | Temps: %02ld:%02ld\n", jeu->coups, tempsEcoule / 60, tempsEcoule % 60);
    }
    printf("--------------------------------------------------\n");
}
/*

si modeSolitaire
    afficher "Mode Solitaire | Pseudo | Score: X/Y
    afficher "Coups: Z | Temps: MM:SS
sinon
    afficher "Pseudo1: X  VS  Pseudo2: Y | Paires: A/B
    afficher "Coups: Z | Temps: MM:SS"
fi

exemple :
1/ mode solitaire
--------------------------------------------------
 Mode Solitaire | Solo-Player | Score: 5/8
 Coups: 12 | Temps: 03:25
--------------------------------------------------
2/ mode deux joueurs
--------------------------------------------------
 Player1: 3  VS  Player2: 4 | Paires: 7/8
 Coups: 15 | Temps: 04:10
--------------------------------------------------
*/

// Affichage de l'écran de fin de partie
void afficher_fin_partie(Jeu *jeu, int modeSolitaire) {
    int n = (jeu->L * jeu->C - 1) / 2;
    struct timeval end;
    gettimeofday(&end, NULL);
    long tempsTotal = time_diff(jeu->tempsDebut, end);

    printf("\n" BOLD "========================================\n");
    printf("          FIN DE PARTIE !               \n");
    printf("========================================\n" RESET "\n");

    // Affichage des résultats
    if (modeSolitaire) {
        // affichage simple pour le mode solitaire / BRAVO + score
        printf("Bravo " GREEN "%s" RESET " !\n", jeu->joueurs[0].pseudo);
        printf("Paires trouvees : " YELLOW "%d/%d" RESET "\n", jeu->joueurs[0].score, n);
    } else {
        // affichage détaillé pour le mode deux joueurs / scores + gagnant
        printf("%s : " GREEN "%d" RESET " paires\n", jeu->joueurs[0].pseudo, jeu->joueurs[0].score);
        printf("%s : " GREEN "%d" RESET " paires\n", jeu->joueurs[1].pseudo, jeu->joueurs[1].score);
        printf("\n");
        // si joueur1.score > joueur2.score
        if (jeu->joueurs[0].score > jeu->joueurs[1].score)
            printf(GREEN "*** %s GAGNE ! ***\n" RESET, jeu->joueurs[0].pseudo);
        // sinon si joueur2.score > joueur1.score
        else if (jeu->joueurs[1].score > jeu->joueurs[0].score)
            printf(GREEN "*** %s GAGNE ! ***\n" RESET, jeu->joueurs[1].pseudo);
        // sinon égalité
        else
            printf(YELLOW "*** EGALITE ! ***\n" RESET);
    }
    // nb coups + temps total
    printf("Nombre de coups : " YELLOW "%d" RESET "\n", jeu->coups);
    printf("Temps total     : " YELLOW "%02ld:%02ld" RESET "\n", tempsTotal / 60, tempsTotal % 60);
}
/*
exemple :
========================================
          FIN DE PARTIE !
========================================
Bravo Solo-Player !
Paires trouvees : 8/8
Nombre de coups : 20
Temps total     : 05:30
*/

/*
    ============================================================
   GESTION DU TABLEAU / PLATEAU
   ============================================================
*/

// Prépare la structure du jeu avec allocation mémoire et vérifications des dimensions
void preparer_jeu(Jeu *jeu, int L, int C) {
    if (L % 2 == 0 || C % 2 == 0 || (L == 1 && C == 1) || (L * C < 3)) {
        printf(RED "Erreur : dimensions invalides !\n" RESET);
        printf("  - L et C doivent etre impairs\n");
        printf("  - L et C != 1\n");
        printf("  - Plateau : au moins 3 cases\n");
        exit(EXIT_FAILURE);
    }
    jeu->L = L;
    jeu->C = C;
    jeu->R = L * C;
    jeu->coups = 0;
    jeu->tour = 0;
    gettimeofday(&jeu->tempsDebut, NULL);
    jeu->T = malloc(jeu->L * jeu->C * sizeof(int));
    jeu->P = malloc(jeu->L * jeu->C * sizeof(int));
    if (!jeu->T || !jeu->P) {
        printf(RED "Erreur d'allocation memoire.\n" RESET);
        exit(EXIT_FAILURE);
    }
    bot_initialiser(&jeu->bot, jeu->L * jeu->C);
}

// Initialisation du plateau avec mélange et placement des cartes
void initialiser_plateau(Jeu *jeu) {
    // taille total = L * C
    int taille_totale = jeu->L * jeu->C;
    int i, j, temp, valeur_carte;

    // 1. Remplir P avec les indices 0 à N-1
    for (i = 0; i < taille_totale; i++)
        jeu->P[i] = i;

    // 2. Mélanger P avec l'algorithme de Fisher-Yates
    for (i = taille_totale - 1; i > 0; i--) {
        j = rand() % (i + 1);
        temp = jeu->P[i];
        jeu->P[i] = jeu->P[j];
        jeu->P[j] = temp;
    }

    // 3. Mettre toutes les cases du plateau à "retirée"
    for (i = 0; i < taille_totale; i++)
        jeu->T[i] = CARTE_RETIREE;

    // 4. Placer le Joker à la première position mélangée
    jeu->T[jeu->P[0]] = JOKER;

    // 5. Placer les paires de cartes sur les autres positions
    valeur_carte = 1;
    for (i = 1; i < taille_totale; i += 2) {
        jeu->T[jeu->P[i]] = valeur_carte;      // Première carte de la paire
        jeu->T[jeu->P[i + 1]] = valeur_carte;  // Deuxième carte de la paire
        valeur_carte++;
    }
}

// Libération de la mémoire allouée pour le jeu / free
void liberer_jeu(Jeu *jeu) {
    if (jeu->T) { free(jeu->T); jeu->T = NULL; }
    if (jeu->P) { free(jeu->P); jeu->P = NULL; }
    if (jeu->bot.memoire) { free(jeu->bot.memoire); jeu->bot.memoire = NULL; }
}

// Conversion de k en (i,j)
void convertir_k_en_ij(int k, int C, int *i, int *j) {
    *i = k / C;  /* i = k div C */
    *j = k % C;  /* j = k mod C */
}
// Conversion de (i,j) en k
int convertir_ij_en_k(int i, int j, int C) {
    return i * C + j;  /* k = i × C + j */
}

// Vérifie si une position existe encore dans P (carte non retirée)
// P = tableau des positions disponibles
int est_dans_P(Jeu *jeu, int pos) {
    int i;
    for (i = 0; i < jeu->R; i++)
        if (jeu->P[i] == pos)
            return 1;
    return 0;
}

// Gère l'effet du Joker : permutation aléatoire avec une autre carte et oubli par le bot
void gestion_joker(Jeu *jeu, int positionJoker) {
    int r = rand() % jeu->R;
    int position_aleatoire = jeu->P[r];

    /* Permutation dans T uniquement */
    int temp = jeu->T[positionJoker];
    jeu->T[positionJoker] = jeu->T[position_aleatoire];
    jeu->T[position_aleatoire] = temp;

    printf(MAGENTA ">>> Le Joker a ete deplace en position %d ! <<<\n" RESET, position_aleatoire);

    /* Le bot invalide ces positions car elles ont changé */
    bot_oublier_carte(&jeu->bot, positionJoker);
    bot_oublier_carte(&jeu->bot, position_aleatoire);
}

// Met à jour les tables T et P après avoir trouvé une paire, et fait oublier au bot
void mettre_a_jour_tables(Jeu *jeu, int p1, int p2) {
    int i, nouvelle_taille;

    /* Marquer les cartes comme retirées dans T */
    jeu->T[p1] = CARTE_RETIREE;
    jeu->T[p2] = CARTE_RETIREE;

    /* Retirer p1 et p2 de la table P */
    nouvelle_taille = 0;
    for (i = 0; i < jeu->R; i++) {
        if (jeu->P[i] != p1 && jeu->P[i] != p2) {
            jeu->P[nouvelle_taille] = jeu->P[i];
            nouvelle_taille++;
        }
    }

    /* Mettre à jour R (nombre de cartes restantes) */
    jeu->R = nouvelle_taille;

    /* Le bot oublie ces cartes */
    bot_oublier_carte(&jeu->bot, p1);
    bot_oublier_carte(&jeu->bot, p2);
}

/* Saisie sécurisée d'une position par un joueur humain */
int saisir_position(Jeu *jeu) {
    int pos; // position saisie
    char message[100];
    sprintf(message, "Entrez une " CYAN "POSITION" RESET " (0 a %d) : ", jeu->L * jeu->C - 1);
    while (1) {
        pos = saisir_entier(message, 0, jeu->L * jeu->C - 1);
        if (jeu->T[pos] == CARTE_RETIREE) {
            printf(RED "Cette carte a deja ete retiree.\n" RESET);
            continue;
        }
        if (!est_dans_P(jeu, pos)) {
            printf(RED "Cette position n'est plus disponible.\n" RESET);
            continue;
        }
        return pos;
    }
}

/*
   ============================================================
    BOT ++
   ============================================================
*/

/*
 * Le bot utilise un tableau indexé par position (0 à L*C-1)
 * Chaque case contient : position, valeur, estValide
 *
 * Cette structure permet un accès O(1) pour mémoriser/oublier
 * mais nécessite O(n²) pour chercher les paires
 */
void bot_initialiser(MemoireBot *mem, int capacite) {
    int i;
    mem->capaciteMax = capacite;
    mem->memoire = (CarteConnue *)malloc(capacite * sizeof(CarteConnue));
    if (mem->memoire == NULL) {
        printf(RED "Erreur d'allocation memoire pour le bot.\n" RESET);
        exit(EXIT_FAILURE);
    }
    // Initialiser toutes les cartes comme invalides
    for (i = 0; i < capacite; i++) {
        mem->memoire[i].position = i;
        mem->memoire[i].valeur = -1;
        mem->memoire[i].estValide = 0;
    }
}

// Le bot mémorise une carte (sauf le Joker)
// Doit être appelé à CHAQUE révélation de carte (par n'importe quel joueur)
// Le bot a la particularité de mémoriser tous les coups joués lors d'une partie.
void bot_memoriser(MemoireBot *mem, int position, int valeur) {
    if (valeur == JOKER) return;  /* Le bot ne mémorise pas le Joker */
    if (position >= 0 && position < mem->capaciteMax) {
        mem->memoire[position].position = position;
        mem->memoire[position].valeur = valeur;
        mem->memoire[position].estValide = 1;
    }
}

// Le bot oublie une carte (après paire trouvée ou Joker déplacé)
void bot_oublier_carte(MemoireBot *mem, int position) {
    if (position >= 0 && position < mem->capaciteMax)
        mem->memoire[position].estValide = 0;
}

/*
 *  NIVEAU 1 : Chercher une paire complète en mémoire
 *
 * Le bot parcourt sa mémoire pour trouver deux cartes :
 * - De même valeur
 * - Toujours présentes sur le plateau (vérification avec "est_dans_P")
 *
 * Retourne 1 si une paire est trouvée, 0 sinon
 */
int bot_chercher_paire(MemoireBot *mem, Jeu *jeu, int *p1, int *p2) {
    int i, j;
    for (i = 0; i < mem->capaciteMax; i++) {
        if (mem->memoire[i].estValide && est_dans_P(jeu, i)) {
            for (j = i + 1; j < mem->capaciteMax; j++) {
                if (mem->memoire[j].estValide && est_dans_P(jeu, j)) {
                    if (mem->memoire[i].valeur == mem->memoire[j].valeur) {
                        *p1 = i;
                        *p2 = j;
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}

/*
 *  NIVEAU 2 : Chercher une carte de valeur donnée
 *
 * Après avoir révélé la première carte :
 * Le bot cherche si cette valeur existe dans sa mémoire
 * (pour former une paire avec la carte qu'il vient de voir)
 *
 * exclure : position à exclure (généralement la première carte révélée)
 */
int bot_chercher_valeur(MemoireBot *mem, Jeu *jeu, int valeur, int exclure) {
    int i;
    for (i = 0; i < mem->capaciteMax; i++) {
        if (mem->memoire[i].estValide &&
            mem->memoire[i].valeur == valeur &&
            i != exclure &&
            est_dans_P(jeu, i))
            return i;
    }
    return -1;
}

/*
 * Le bot choisit ses deux cartes au début de son tour
 *
 * - il NE TRICHE PAS
 * - Il n'a pas accès à T (table des cartes)
 * - Il utilise seulement P (table des positions disponibles)
 * - Il se base uniquement sur sa mémoire des coups précédents
 *
 * Si le bot trouve une paire en mémoire : il la joue directement
 * Sinon : il choisit aléatoirement et pos2 = -1 (sera choisi après avoir vu pos1)
 */
void bot_jouer_tour(Jeu *jeu, int *pos1, int *pos2) {
    int idx1;

    // NIVEAU 1 : Chercher une paire complète
    if (bot_chercher_paire(&jeu->bot, jeu, pos1, pos2)) {
        printf(YELLOW "Bot: J'ai trouve une paire complete en memoire !\n" RESET);
        return;
    }

    // NIVEAU 3 : Choix aléatoire de la première carte
    idx1 = rand() % jeu->R;
    *pos1 = jeu->P[idx1];

    // La deuxième carte sera choisie APRÈS avoir vu la première
    // (pour respecter le fait que le bot ne triche pas)
    *pos2 = -1;
}

/*
 * Le bot choisit sa deuxième carte APRÈS avoir vu la première
 *
 * Cette fonction est appelée après la révélation de la première carte
 * ici le bot applique sa stratégie de niveau 2
 */
void bot_choisir_deuxieme_carte(Jeu *jeu, int pos1, int valeur1, int *pos2) {
    if (valeur1 == JOKER) {
        *pos2 = -1;
        return;
    }
    // Niveau 2 : Chercher la paire en mémoire
    *pos2 = bot_chercher_valeur(&jeu->bot, jeu, valeur1, pos1);
    if (*pos2 != -1) {
        printf(YELLOW "Bot: Je connais la paire de cette carte !\n" RESET);
        return;
    }
    // Niveau 3 : Choix aléatoire simplifié (choisir directement, risque faible de collision)
    int idx2 = rand() % jeu->R;
    *pos2 = jeu->P[idx2];
    if (*pos2 == pos1) {  // Rare, mais gérer si nécessaire
        idx2 = (idx2 + 1) % jeu->R;
        *pos2 = jeu->P[idx2];
    }
}

// différence entre deux temps en sec
long time_diff(struct timeval start, struct timeval end)
{   // tv_sec est de type long
    return end.tv_sec - start.tv_sec;
}

/* ============================================================
   FONCTIONS MENU ET CONFIGURATION
   ============================================================ */

/*
 * Affiche le menu principal et gère les choix
 * Retourne le choix_menu (1-4 pour jouer, 5 pour quitter)
 * Modifie modeTriche si activé/désactivé
 */
int afficher_menu_principal(int *modeTriche) {
    int choix_menu;
    do {
        effacer_ecran();
        //afficher_banniere();
        printf(CYAN "  ╔══════════════════════════════════════╗\n" RESET);
        printf(CYAN "  ║" RESET BOLD "          MENU PRINCIPAL              " RESET CYAN "║\n" RESET);
        printf(CYAN "  ╚══════════════════════════════════════╝\n" RESET);
        if (*modeTriche) printf(RED "\n       >>> Mode Triche ACTIVE <<<\n" RESET);
        printf("\n  " BOLD "Choisissez votre mode de jeu :\n\n" RESET);
        if (*modeTriche) printf(GREEN "    [0]" RESET " Desactiver le Mode Triche\n");
        else printf(RED "    [0]" RESET " Activer le Mode Triche\n");
        printf(YELLOW "    [1]" RESET " Duel : " CYAN "Humain" RESET " vs " CYAN "Humain\n" RESET);
        printf(YELLOW "    [2]" RESET " Duel : " CYAN "Humain" RESET " vs " MAGENTA "Bot\n" RESET);
        printf(GREEN "    [3]" RESET " Solitaire : " CYAN "Humain" RESET " seul\n");
        printf(GREEN "    [4]" RESET " Solitaire : " MAGENTA "Bot" RESET " seul (Demo)\n");
        printf(RED "    [5]" RESET " Quitter\n");
        printf("\n  Votre choix : " BOLD);
        choix_menu = saisir_entier(NULL, 0, 5);
        printf(RESET);
        if (choix_menu == 5) {
            printf(GREEN "\n  Merci d'avoir joue ! A bientot !\n\n" RESET);
            return 5;
        }
        if (choix_menu == 0) {
            *modeTriche = !(*modeTriche);
            printf(*modeTriche ? RED "\n  >>> Mode Triche ACTIVE ! <<<\n" RESET : GREEN "\n  >>> Mode Triche DESACTIVE ! <<<\n" RESET);
            attendre(1);
        }
    } while (choix_menu < 1 || choix_menu > 4);
    return choix_menu;
}

/*
 * Configure les dimensions du plateau (L et C)
 * Assure que L et C sont impairs et >= 3
 */
void configurer_plateau(int *L, int *C) {
    printf(CYAN "  ╔══════════════════════════════════════╗\n" RESET);
    printf(CYAN "  ║" RESET BOLD "       CONFIGURATION DU PLATEAU        " RESET CYAN "║\n" RESET);
    printf(CYAN "  ╚══════════════════════════════════════╝\n" RESET);
    printf("  " YELLOW "Rappel" RESET " : L et C doivent etre " BOLD "impairs" RESET " (3, 5, 7, ...)\n\n");
    do {
        *L = saisir_entier("  Nombre de lignes " CYAN "L" RESET " (impair, >= 3) : ", 1, 99);
        *C = saisir_entier("  Nombre de colonnes " CYAN "C" RESET " (impair, >= 3) : ", 1, 99);
        if (*L % 2 == 0 || *C % 2 == 0)
            printf(RED "  Erreur : L et C doivent etre impairs.\n" RESET);
        else if (*L < 3 || *C < 3)
            printf(RED "  Erreur : L et C doivent etre >= 3.\n" RESET);
    } while (*L % 2 == 0 || *C % 2 == 0 || *L < 3 || *C < 3);

    printf("\n  " GREEN "✓" RESET " Plateau : " BOLD "%d x %d" RESET " = " YELLOW "%d cases" RESET "\n", *L, *C, *L * *C);
    printf("  " GREEN "✓" RESET " Paires : " YELLOW "%d" RESET " + " RED "1 Joker" RESET "\n", (*L * *C - 1) / 2);
}

/*
 * Configure les joueurs en fonction du choix_menu
 * Définit les pseudos, types (bot/humain), scores, et l'ordre de passage
 */
void configurer_joueurs(Jeu *jeu, int choixMenu, int *modeSolitaire) {
    *modeSolitaire = (choixMenu >= 3);
    jeu->nbJoueurs = *modeSolitaire ? 1 : 2;

    switch (choixMenu) {
        case 1:
            strcpy(jeu->joueurs[0].pseudo, "Joueur 1");
            jeu->joueurs[0].estBot = 0;
            jeu->joueurs[0].score = 0;
            strcpy(jeu->joueurs[1].pseudo, "Joueur 2");
            jeu->joueurs[1].estBot = 0;
            jeu->joueurs[1].score = 0;
            break;
        case 2:
            strcpy(jeu->joueurs[0].pseudo, "Joueur");
            jeu->joueurs[0].estBot = 0;
            jeu->joueurs[0].score = 0;
            strcpy(jeu->joueurs[1].pseudo, "Bot-Memoryx");
            jeu->joueurs[1].estBot = 1;
            jeu->joueurs[1].score = 0;
            break;
        case 3:
            strcpy(jeu->joueurs[0].pseudo, "Solo-Player");
            jeu->joueurs[0].estBot = 0;
            jeu->joueurs[0].score = 0;
            break;
        case 4:
            strcpy(jeu->joueurs[0].pseudo, "Bot-Demo");
            jeu->joueurs[0].estBot = 1;
            jeu->joueurs[0].score = 0;
            break;
    }

    jeu->joueurActuel = 0;
    if (!(*modeSolitaire)) {
        int choixPremier;
        printf(CYAN "  ╔══════════════════════════════════════╗\n" RESET);
        printf(CYAN "  ║" RESET BOLD "          ORDRE DE PASSAGE             " RESET CYAN "║\n" RESET);
        printf(CYAN "  ╚══════════════════════════════════════╝\n" RESET);
        printf(YELLOW "    [1]" RESET " %s commence\n", jeu->joueurs[0].pseudo);
        printf(YELLOW "    [2]" RESET " %s commence\n", jeu->joueurs[1].pseudo);
        printf("\n  Votre choix : ");
        choixPremier = saisir_entier(NULL, 1, 2);
        if (choixPremier == 2) jeu->joueurActuel = 1;
    }
}

/*
 * Gère la fin de partie : affichage des résultats, libération mémoire, et demande de rejouer
 */
void gerer_fin_partie(Jeu *jeu, int modeSolitaire, int *rejouer) {
    /* FIN DE PARTIE */
    effacer_ecran();
    //afficher_banniere();
    afficher_fin_partie(jeu, modeSolitaire);
    liberer_jeu(jeu);

    /* Demander si le joueur veut rejouer */
    printf("\n" BOLD "=== QUE VOULEZ-VOUS FAIRE ? ===\n" RESET);
    printf("1. Retour au menu principal\n");
    printf("2. Quitter le jeu\n");
    printf("Votre choix : ");
    *rejouer = saisir_entier(NULL, 1, 2);
}

/*
 * Gère un tour complet de jeu
 */
void jouer_tour(Jeu *jeu, int modeSolitaire, int modeTriche) {
    Joueur *j;
    int p1, p2, tourTermine;

    effacer_ecran();
    //afficher_banniere();

    j = &jeu->joueurs[jeu->joueurActuel];
    tourTermine = 0;
    jeu->tour++;
    jeu->coups++;

    afficher_scores(jeu, modeSolitaire);
    printf("\n" BOLD "  Tour %d" RESET " - C'est au tour de : " CYAN BOLD "%s" RESET "\n", jeu->tour, j->pseudo);

    afficher_plateau(jeu, -1, -1, modeTriche);

    /* === CHOIX DE LA PREMIÈRE CARTE === */
    if (j->estBot) {
        printf(YELLOW "\n%s reflechit...\n" RESET, j->pseudo);
        attendre(1);
        bot_jouer_tour(jeu, &p1, &p2);
        printf("%s choisit la position : %d\n", j->pseudo, p1);
        attendre(1);
    } else {
        printf("\nCarte 1 : ");
        p1 = saisir_position(jeu);
    }

    printf("\n=> Carte revelee : ");
    if (jeu->T[p1] == JOKER)
        printf(RED "[JOKER]" RESET "\n");
    else
        printf(YELLOW "[%d]" RESET "\n", jeu->T[p1]);

    // Le bot mémorise cette carte révélée
    bot_memoriser(&jeu->bot, p1, jeu->T[p1]);

    if (jeu->T[p1] == JOKER) {
        afficher_plateau(jeu, p1, -1, modeTriche);
        printf(RED "\n!!! JOKER !!! %s passe son tour !\n" RESET, j->pseudo);
        gestion_joker(jeu, p1);
        attendre(1);
        tourTermine = 1;
    }

    /* === CHOIX DE LA DEUXIÈME CARTE === */
    if (!tourTermine) {
        afficher_plateau(jeu, p1, -1, modeTriche);

        if (j->estBot) {
            /* Le bot choisit sa 2ème carte APRÈS avoir vu la première (pas de triche) */
            bot_choisir_deuxieme_carte(jeu, p1, jeu->T[p1], &p2);
            if (p2 != -1) {
                printf(YELLOW "\n%s choisit sa 2eme carte...\n" RESET, j->pseudo);
                attendre(1);
                printf("%s choisit la position : %d\n", j->pseudo, p2);
                attendre(1);
            }
        } else {
            printf("\nCarte 2 : ");
            do {
                p2 = saisir_position(jeu);
                if (p1 == p2)
                    printf(RED "Vous avez deja retourne cette carte !\n" RESET);
            } while (p1 == p2);
        }

        printf("\n=> Carte revelee : ");
        if (jeu->T[p2] == JOKER)
            printf(RED "[JOKER]" RESET "\n");
        else
            printf(YELLOW "[%d]" RESET "\n", jeu->T[p2]);

        // Le bot mémorise cette carte révélée
        bot_memoriser(&jeu->bot, p2, jeu->T[p2]);

        if (jeu->T[p2] == JOKER) {
            afficher_plateau(jeu, p1, p2, modeTriche);
            printf(RED "\n!!! JOKER !!! %s passe son tour !\n" RESET, j->pseudo);
            gestion_joker(jeu, p2);
            attendre(1);
            tourTermine = 1;
        }
    }

    /* === VÉRIFICATION PAIRE === */
    if (!tourTermine) {
        afficher_plateau(jeu, p1, p2, modeTriche);

        if (jeu->T[p1] == jeu->T[p2]) {
            printf(GREEN "\n>>> PAIRE TROUVEE ! (+1 Point) <<<\n" RESET);
            j->score++;
            mettre_a_jour_tables(jeu, p1, p2);
            printf("Vous rejouez !\n");
            attendre(1);
        } else {
            printf(RED "\nRate..." RESET " Retenez bien les positions.\n");
            attendre(1);
            if (!modeSolitaire)
                jeu->joueurActuel = (jeu->joueurActuel + 1) % MAX_JOUEURS;
        }
    } else {
        if (!modeSolitaire)
            jeu->joueurActuel = (jeu->joueurActuel + 1) % MAX_JOUEURS;
    }
}
