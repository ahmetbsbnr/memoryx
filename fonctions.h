/*
    fonctions.h - Déclarations fonctions et structures
    Auteur : Ahmet BASBUNAR
 */

/* ==== BIBLIOTHEQUE ====*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

#include <unistd.h>
#define SLEEP(x) sleep(x)

/* ==================== COULEURS ==================== */
#define RESET   "\033[0m"
#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define BLUE    "\033[1;34m"
#define CYAN    "\033[1;36m"
#define MAGENTA "\033[1;35m"
#define BOLD    "\033[1m"

/* ==================== CONSTANTES ==================== */
#define JOKER 0
#define CARTE_RETIREE -1
#define MAX_PSEUDO 50
#define MAX_JOUEURS 2

/* ==================== STRUCTURES ==================== */

// structure pour une carte connue par le bot
typedef struct {
    int position;
    int valeur;
    int estValide;
} CarteConnue;

// structure pour la mémoire du bot
typedef struct {
    CarteConnue *memoire; // tableau des cartes connues
    int capaciteMax; // capacité maximale de la mémoire
} MemoireBot;

// structure pour un joueur
typedef struct {
    char pseudo[MAX_PSEUDO]; // pseudo du joueur
    int score;              // score du joueur
    int estBot;           // 1 si c'est un bot, 0 sinon
} Joueur;

// structure principale du jeu
typedef struct {
    int L; // lignes
    int C; // colonnes
    int R; // cartes restantes
    int *T; // tableau des cartes
    int *P; // tableau des positions disponibles
    Joueur joueurs[MAX_JOUEURS]; // tableau des joueurs
    int joueurActuel; // index du joueur actuel
    int nbJoueurs; // nombre de joueurs
    MemoireBot bot; // mémoire du bot
    int coups; // nombre de coups joués
    int tour; // numéro du tour actuel
    struct timeval tempsDebut; // temps de début de la partie
} Jeu;

/* ==================== En-têtes Fonctions ==================== */

void attendre(int secondes);
void effacer_ecran(void);
void vider_buffer(void);
int saisir_entier(const char *message, int min, int max);
void afficher_banniere(void);
void afficher_plateau(Jeu *jeu, int pos1, int pos2, int mode_triche);
void afficher_scores(Jeu *jeu, int modeSolitaire);
void afficher_fin_partie(Jeu *jeu, int modeSolitaire);
void preparer_jeu(Jeu *jeu, int L, int C);
void initialiser_plateau(Jeu *jeu);
void liberer_jeu(Jeu *jeu);
void convertir_k_en_ij(int k, int C, int *i, int *j);
int convertir_ij_en_k(int i, int j, int C);
int est_dans_P(Jeu *jeu, int pos);
void gestion_joker(Jeu *jeu, int positionJoker);
void mettre_a_jour_tables(Jeu *jeu, int p1, int p2);
int saisir_position(Jeu *jeu);
void bot_initialiser(MemoireBot *mem, int capacite);
void bot_memoriser(MemoireBot *mem, int position, int valeur);
void bot_oublier_carte(MemoireBot *mem, int position);
int bot_chercher_paire(MemoireBot *mem, Jeu *jeu, int *p1, int *p2);
int bot_chercher_valeur(MemoireBot *mem, Jeu *jeu, int valeur, int exclure);
void bot_jouer_tour(Jeu *jeu, int *pos1, int *pos2);
void bot_choisir_deuxieme_carte(Jeu *jeu, int pos1, int valeur1, int *pos2);

// différence entre deux temps en sec
long time_diff(struct timeval start, struct timeval end);

// Fonctions pour le menu et la configuration (déplacées depuis main)
int afficher_menu_principal(int *modeTriche);
void configurer_plateau(int *L, int *C);
void configurer_joueurs(Jeu *jeu, int choixMenu, int *modeSolitaire);
void gerer_fin_partie(Jeu *jeu, int modeSolitaire, int *rejouer);
void jouer_tour(Jeu *jeu, int modeSolitaire, int modeTriche);
