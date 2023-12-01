#include "generation_terrains.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "programme.h"
#include "terrain.h"
#include "environnement.h"
#include "robot.h"
#include "interprete.h"
#include "type_pile.h"

void gestion_erreur_terrain(erreur_terrain e)
{
    switch (e)
    {
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

void affichage_position_programme(erreur_programme e)
{
    int i;
    printf("Ligne %d, colonne %d :\n", e.num_ligne, e.num_colonne);
    printf("%s\n", e.ligne);
    /* Impression de e.num_colonne-1 espaces */
    for (i = 1; i < e.num_colonne; i++)
    {
        printf(" ");
    }
    /* Impression d'un curseur de position */
    printf("^\n");
}

void gestion_erreur_programme(erreur_programme e)
{
    switch (e.type_err)
    {
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

int main(int argc, char ** argv)
{
    if (argc < 9)
    {
        printf("Usage: ./curiosity-perf fichier_programme N L H d graine nb_step_max fichier_res\n");
        exit(1);
    }

    int N, i, l, h, nb_step_max, graine;
    float dObst;
    FILE *resFile;
    Terrain T;
    Programme Prog;

    int nb_sorti = 0, nb_plouf = 0, nb_ecrase = 0, nb_bloque = 0;
    int nb_pas_pour_sortir = 0;

    N = strtol(argv[2], NULL, 10);
    l = strtol(argv[3], NULL, 10);
    h = strtol(argv[4], NULL, 10);
    dObst = strtof(argv[5], NULL);
    graine = strtol(argv[6], NULL, 10);
    nb_step_max = strtol(argv[7], NULL, 10);

    // test de l et h
    if (l > DIM_MAX || l % 2 == 0)
    {
        printf("Largeur incorrecte : doit être impaire et <= %d\n", DIM_MAX);
        return 1;
    }
    if (h > DIM_MAX || h % 2 == 0)
    {
        printf("Hauteur incorrecte : doit être impaire et <= %d\n", DIM_MAX);
        return 1;
    }
    if ((dObst > 1) || (dObst < 0))
    {
        printf("Densité incorrecte : doit être comprise entre 0 et 1\n");
        return 1;
    }

    // Ouverture du fichier programme et initialiser programme
    gestion_erreur_programme(lire_programme(&Prog, argv[1]));

    // Ouverture du fichier résultat
    resFile = fopen(argv[8], "w");

    // Initialisation de la fonction random
    srand(graine);


    /*------------------------------------------------------------------------------------------------------*/
    //Partie principale:
    //Initialiser environnement et le robot
    //Generer les terrains aleatoires
    //Executer le robot dans les terrains generes
    //Ecrire les resultats dans le fichier resFile

    etat_inter e;
    init_etat(&e);

    Robot R;
    init_robot(&R, l / 2, h / 2, Est);

    Environnement *E = malloc(sizeof(Environnement));
    E->r = R;

    resultat_inter resultat;
    fprintf(resFile, "%d", N);
    for (i = 0; i < N; i++)
    {
        generation_aleatoire(&T, l, h, dObst);
        E->t = T;
        resultat = exec_pas(&Prog, E, &e);
        switch (resultat)
        {
            case SORTIE_ROBOT:
                nb_sorti++;
                fprintf(resFile, "%d", 0); // A MODIFIER ***********
                break;
            case ARRET_ROBOT:
                nb_bloque++;
                fprintf(resFile, "%d", -1);
                break;
            case PLOUF_ROBOT:
                nb_plouf++;
                fprintf(resFile, "%d", -2);
                break;
            case CRASH_ROBOT:
                nb_ecrase++;
                fprintf(resFile, "%d", -3);
                break;
            
            default:
                break;
        }

    }


    fclose(resFile);
    return 0;
}