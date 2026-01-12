# Memoryx

Un jeu de Memory dans le terminal, en C : retrouver toutes les paires de cartes identiques sur le plateau… en se méfiant du Joker, qui fait passer son tour et permute avec une autre carte.

## Modes
1. **Duel** Humain vs Humain
2. **Duel** Humain vs Bot (le bot mémorise toutes les cartes révélées)
3. **Solitaire**
4. **Démo** : le bot joue seul

Un mode triche (affichage des cartes) est disponible depuis le menu.

## Compiler et lancer
```bash
gcc -Wall -Wextra -o memoryx memoryx.c fonctions.c
./memoryx
```
Fonctionne sous Linux et macOS (couleurs ANSI dans le terminal).

### Tests
```bash
gcc -Wall -Wextra -o test_memoryx test_memoryx.c fonctions.c
./test_memoryx
```

## Contenu
| Chemin | Rôle |
|---|---|
| `memoryx.c` | Programme principal |
| `fonctions.c`, `fonctions.h` | Structures et fonctions du jeu (plateau, joker, bot, affichage) |
| `test_memoryx.c` | Tests unitaires |

## Auteur
Ahmet BASBUNAR
