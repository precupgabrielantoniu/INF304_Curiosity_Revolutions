#include "environnement.h"
#include "interprete.h"
#include "programme.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void gestion_erreur_terrain(erreur_terrain e) {
  switch (e) {
  case OK:
    break;
  case ERREUR_FICHIER:
    printf("Erreur lecture du terrain : erreur d'ouverture du fichier\n");
    exit(1);
  case ERREUR_LECTURE_LARGEUR:
    printf("Erreur lecture du terrain : erreur de lecture de la largeur\n");
    exit(1);
  case ERREUR_LECTURE_HAUTEUR:
    printf("Erreur lecture du terrain : erreur de lecture de la hauteur\n");
    exit(1);
  case ERREUR_LARGEUR_INCORRECTE:
    printf("Erreur lecture du terrain : largeur incorrecte\n");
    exit(1);
  case ERREUR_HAUTEUR_INCORRECTE:
    printf("Erreur lecture du terrain : hauteur incorrecte\n");
    exit(1);
  case ERREUR_CARACTERE_INCORRECT:
    printf("Erreur lecture du terrain : caractère incorrect\n");
    exit(1);
  case ERREUR_LIGNE_TROP_LONGUE:
    printf("Erreur lecture du terrain : ligne trop longue\n");
    exit(1);
  case ERREUR_LIGNE_TROP_COURTE:
    printf("Erreur lecture du terrain : ligne trop courte\n");
    exit(1);
  case ERREUR_LIGNES_MANQUANTES:
    printf("Erreur lecture du terrain : lignes manquantes\n");
    exit(1);
  case ERREUR_POSITION_ROBOT_MANQUANTE:
    printf(
        "Erreur lecture du terrain : position initiale du robot manquante\n");
    exit(1);
  }
}

void affichage_position_programme(erreur_programme e) {
  int i;
  printf("Ligne %d, colonne %d :\n", e.num_ligne, e.num_colonne);
  printf("%s\n", e.ligne);
  /* Impression de e.num_colonne-1 espaces */
  for (i = 1; i < e.num_colonne; i++) {
    printf(" ");
  }
  /* Impression d'un curseur de position */
  printf("^\n");
}

void gestion_erreur_programme(erreur_programme e) {
  switch (e.type_err) {
  case OK_PROGRAMME:
    break;
  case ERREUR_FICHIER_PROGRAMME:
    printf("Erreur lecture du programme : erreur d'ouverture du fichier\n");
    exit(2);
  case ERREUR_BLOC_NON_FERME:
    printf("Erreur lecture du programme : bloc non fermé\n");
    exit(2);
  case ERREUR_FERMETURE_BLOC_EXCEDENTAIRE:
    printf("Erreur lecture du programme : fermeture de bloc excédentaire\n");
    affichage_position_programme(e);
    exit(2);
  case ERREUR_COMMANDE_INCORRECTE:
    printf("Erreur lecture du programme : commande incorrecte\n");
    affichage_position_programme(e);
    exit(2);
  }
}

void test(char *nom_fich)
{
    Environnement envt;
    Programme prog;
    erreur_terrain errt;
    erreur_programme errp;
    etat_inter etat;
    resultat_inter res, res_attendu;

    char f_terrain[50], f_prog[50], e;
    int nb_pas;
    int x, y;
    char or;
    Orientation o;

    FILE *f = fopen(nom_fich, "r");
    fgets(f_terrain, 100, f);
    f_terrain[strlen(f_terrain)-1] = '\0';
    fgets(f_prog, 100, f);
    f_prog[strlen(f_prog)-1] = '\0';
    fscanf(f, "%d", &nb_pas);
    fscanf(f, "\n");
    fscanf(f, "%c", &e);
    fscanf(f, "\n");

    if (e == 'N' || e == 'F')
    {
        fscanf(f, "%d %d", &x, &y);
        fscanf(f, "\n");
        fscanf(f, "%c", &or);
        switch(or)
        {
            case 'N':
                o = Nord;
                break;
            case 'S':
                o = Sud;
                break;
            case 'O':
                o = Ouest;
                break;
            case 'E':
                o = Est;
                break;
        }
    }
    fclose(f);

    switch (e)
    {
    case 'S':
        res_attendu = SORTIE_ROBOT;
        break;
    case 'O':
        res_attendu = CRASH_ROBOT;
        break;
    case 'P':
        res_attendu = PLOUF_ROBOT;
        break;
    case 'N':
        res_attendu = OK_ROBOT;
        break;
    case 'F':
        res_attendu = ARRET_ROBOT;
        break;
    default:
        break;
    }

    errt = initialise_environnement(&envt, f_terrain);
    gestion_erreur_terrain(errt);

    errp = lire_programme(&prog, f_prog);
    gestion_erreur_programme(errp);

    init_etat(&etat);
    int i = 0;
    do
    {
        res = exec_pas(&prog, &envt, &etat);
        /* Affichage du terrain et du robot */
        afficher_envt(&envt);
        i++;
    } while (res == OK_ROBOT && i < nb_pas);
    int ok = 1;
    if(res == res_attendu)
    {
        if(res == OK_ROBOT || res == ARRET_ROBOT)
        {
            if(envt.r.o != o)
            {
                printf("envt.r.o: %d, o: %d\n", envt.r.o, o);
                printf("Orientation ne correspond pas\n");
                ok = 0;
            }
            if(envt.r.x != x || envt.r.y != y)
            {
                printf("Position ne correspond pas\n");
                ok = 0;
            }
        }
    }
    else
    {
        printf("Evenement ne correspond pas\n");
        ok = 0;
    }
    if(ok)
    {
        printf("OK\n");
    }
}

int main(int argc, char **argv)
{
    if(argc < 2)
    {
        printf("Usage: ./%s <fichier-test>\n", argv[0]);
        exit(1);
    }
    test(argv[1]);
    return 0;
}