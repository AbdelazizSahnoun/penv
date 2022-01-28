/* ********************* *
 * TP0 INF3173 H2021
 * Code permanent: SAHA01119808
 * Nom: Sahnoun
 * Prénom: Abdelaziz
 * ********************* */



#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAILLE_MAX 5000

//Fonction qui retourne vrai ou faux si un string est un nombre positif ou non
int isNumber(char *argv) {
    char *endptr;
    long int argument = strtol(argv, &endptr, 0);
    return !(*argv == '\0' || *endptr != '\0' || argument <= 0);
}


int main(int argc, char *argv[]) {


    char pid[TAILLE_MAX];
    char cheminEnviron[TAILLE_MAX];
    char variable[TAILLE_MAX];


    if (argc < 3) {
        fprintf(stderr, "penv [-p] PID NOM\n");
        return 2;
    }

    if (strcmp("-p", argv[1]) == 0) {

        if (argc < 4) {
            fprintf(stderr, "penv [-p] PID NOM\n");
            return 2;
        }

        FILE *fileStat;
        char ligne[TAILLE_MAX];
        char PPID[TAILLE_MAX];
        char PID[TAILLE_MAX];
        char cheminStat[TAILLE_MAX];

        strcpy(variable, argv[3]);

        if (isNumber(argv[2])) {
            strcpy(PID, argv[2]);
        } else {
            fprintf(stderr, "Le deuxième argument n'est pas un chiffre. Veuillez entrez un PID valide.\n");
            return 2;
        }

        strcpy(cheminStat, "/proc/");
        strcat(cheminStat, PID);
        strcat(cheminStat, "/stat");


        fileStat = fopen(cheminStat, "r");
        if (!fileStat) {
            perror(cheminStat);
            return 2;
        }
        if (fgets(ligne, TAILLE_MAX, fileStat) != NULL) {
            //Dans le man de proc, le PPID d'un processus est le quatrième string du fichier stat
            sscanf(ligne, "%*s %*s %*s %s", PPID);
        }
        strcpy(pid, PPID);
        fclose(fileStat);


    } else if (!isNumber(argv[1])) {
        fprintf(stderr, "Le premier argument n'est pas un chiffre. Veuillez entrez un PID valide.\n");
        return 2;
    } else {
        strcpy(pid, argv[1]);
        strcpy(variable, argv[2]);
    }


    strcpy(cheminEnviron, "/proc/");
    strcat(cheminEnviron, pid);
    strcat(cheminEnviron, "/environ");

    FILE *fileEnviron = fopen(cheminEnviron, "r");
    if (!fileEnviron) {
        perror(cheminEnviron);
        return 2;
    }
    char **tableau_lignes = NULL;
    char *phrase_tmp = malloc(sizeof(char));
    int nb_lignes = 0;
    int nb_mots = 0;
    char caractere;

    //On lit caractère par caractère, quand on rencontre un caractère null c'est qu'il sagit d'une entrée
    //On la stocke dans tableau_lignes qui contient toutes les entrées dans environ
    //Ainsi tableau_lignes contiendera toutes les entrées de environ

    do {

        caractere = fgetc(fileEnviron);
        phrase_tmp[nb_mots] = caractere;;
        nb_mots++;
        phrase_tmp = realloc(phrase_tmp, sizeof(char) * nb_mots + 1);


        if (caractere == '\0') {
            tableau_lignes = realloc(tableau_lignes, sizeof(*tableau_lignes) * (nb_lignes + 1));
            *(tableau_lignes + nb_lignes) = malloc(sizeof(char) * strlen(phrase_tmp) + 1);
            for (int i = 0; i < strlen(phrase_tmp); i++) {
                tableau_lignes[nb_lignes][i] = phrase_tmp[i];
            }
            nb_lignes++;
            free(phrase_tmp);
            phrase_tmp = malloc(sizeof(char));
            nb_mots = 0;
        }

    } while (caractere != EOF);

    phrase_tmp = NULL;


    char temp[TAILLE_MAX];
    char resultat[TAILLE_MAX];
    int trouve = 0;
    unsigned long long longeur = strlen(variable);
    for (int i = 0; i < nb_lignes; i++) {

        strncpy(temp, tableau_lignes[i], longeur);

        if (strcmp(temp, variable) == 0) {

            trouve = 1;
            for (int j = 0; j < strlen(tableau_lignes[i]) - longeur; j++) {
                resultat[j] = tableau_lignes[i][longeur + j + 1];
            }
            break;

        }

    }

    free(tableau_lignes);
    tableau_lignes = NULL;
    fclose(fileEnviron);

    if (trouve == 1) {
        printf("%s\n", resultat);
        return 0;
    } else {
        return 1;
    }


}
