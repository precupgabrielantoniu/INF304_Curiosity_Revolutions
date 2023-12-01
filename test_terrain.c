#include "terrain.h"
#include <stdio.h>

int main(int argc, char **argv) {
  FILE *f;
  Terrain t;
  erreur_terrain e;
  int x, y;

  if (argc < 2) {
    printf("Usage : %s <fichier>\n", argv[0]);
    return 1;
  }

  f = fopen(argv[1], "r");
  e = lire_terrain(f, &t, &x, &y);
  afficher_erreur(e);
  while(e != OK) //== ERREUR_FICHIER)
  {
    printf("Le fichier n'a pas pu etre ouvert. Donnez un autre: ");
    scanf("%s", argv[1]);
    f = fopen(argv[1], "r");
    e = lire_terrain(f, &t, &x, &y);
    afficher_erreur(e);
  }
  fclose(f);
  if(e == OK)
    afficher_terrain(&t);

  printf("Position initiale du robot : (%d, %d)\n", x, y);
}
