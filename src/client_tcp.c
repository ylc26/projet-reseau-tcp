/* 
   L'objectif du projet est : créer une page HTML dynamique adaptée à notre protocole TCP qui se met automatiquement à jour lors du lancement du serveur. Le scénario implique une communication entre le client et
   le serveur, où le client transmet le nom et le nombre de billets réservés pour un événement, par exemple, en utilisant les commandes : ./serveur_udp et./client_udp, et le serveur ferme tout seul au bout 3 clients.
   Cette interaction donne accès à une page HTML contenant la liste des personnes enregistrées. Le protocole TCP :
    Il gère la retransmission des paquets en cas de perte et garantit la livraison ordonnée des données.
    Il établit et rompt la connexion de manière fiable en utilisant un mécanisme de "handshake".
    En écoutant et en acceptant les connexions entrantes, le serveur peut gérer le nombre de clients.
    Le serveur peut identifier la fermeture du client en surveillant l'état de la connexion.
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//Définition de la structure pour stocker des informations sur une personne.
typedef struct Personne 
{
    char Nom[10];
    int NombreDeBillets;
} Personne;

void EnregistrementHTML(Personne *Personne)
{
    FILE *dossier = fopen("RéservationTCP.html", "a");
    if (dossier == NULL)
    {
        perror("Erreur d'ouverture de fichier");
        exit(EXIT_FAILURE);
    }
    // Écriture du début du fichier HTML si le fichier est vide
    if (ftell(dossier) == 0)
    {
        // Écriture du titre principal
        fprintf(dossier, "<html>\n<head>\n<title>Communication orientee flux</title>\n</head>\n<body>\n");
        // Ajout du titre principal en haut de la page
        fprintf(dossier, "<h1 style=\"text-align:center;\">Communication orientee flux</h1>\n");
        // Ajout du sous-titre centré juste en dessous du titre principal 
        fprintf(dossier, "<h2 style=\"text-align:center;\"><span style=\"font-weight:normal;\">Liste des clients actuels</span></h2>\n");
        // Explication de l'utilité du serveur
        fprintf(dossier, "<h2 style=\"text-align:center;\"><span style=\"font-weight:normal;\">Reservations de billets pour l'entrée d'un evenement par exemple</span></h2>\n");
    }
    // Écrire du tableau HTML avec les informations de personne
    fprintf(dossier, "<div style=\"text-align:center;\">\n");
    fprintf(dossier, "<table border=\"1\" style=\"margin: auto;\">\n");
    fprintf(dossier, "<tr><th>Nom</th><th>Nombre De Billets</th></tr>\n");
    fprintf(dossier, "<tr><td>%s</td><td>%d</td></tr>\n", Personne->Nom, Personne->NombreDeBillets);
    fprintf(dossier, "</table>\n");
    fprintf(dossier, "</div>\n");
    // Fermeture du fichier HTML
    fclose(dossier);
}

// Effacer le contenu du fichier HTML
void EffacerHTML()
{
    FILE *dossier = fopen("RéservationTCP.html", "w");
    if (dossier == NULL)
    {
        perror("Erreur d'ouverture de fichier");
        exit(EXIT_FAILURE);
    }
    // Titre principal
    fprintf(dossier, "<html>\n<head>\n<title>Communication orientee flux</title>\n</head>\n<body>\n");
    // Écriture du titre principal en haut de la page
    fprintf(dossier, "<h1 style=\"text-align:center;\">Communication orientee flux</h1>\n");
    // Ajout du sous-titre centré juste en dessous du titre principal
    fprintf(dossier, "<h2 style=\"text-align:center;\"><span style=\"font-weight:normal;\">Liste des clients actuels</span></h2>\n");
    // Explication de l'utilité du serveur
    fprintf(dossier, "<h2 style=\"text-align:center;\"><span style=\"font-weight:normal;\">Reservations de billets pour l'entrée d'un evenement par exemple</span></h2>\n");
    
    // Fermeture du fichier HTML
    fclose(dossier);
}


int main(void)
{
    // Ouvrerture socket client
    int socketClient = socket(AF_INET, SOCK_STREAM, 0);
    // Socket est ouvert ou nom
    if (socketClient == -1)
    {
        perror("Erreur d'ouverture du socket");
        exit(EXIT_FAILURE);
    }
    // Adresse du serveur
    struct sockaddr_in addrClient;
    addrClient.sin_addr.s_addr = inet_addr("127.0.0.1");
    addrClient.sin_port = htons(9065);
    addrClient.sin_family = AF_INET;
    // Connexion avec le serveur
    if (connect(socketClient, (const struct sockaddr *)&addrClient, sizeof(addrClient)) == -1)
    {
        perror("Erreur pendant connexion au serveur");
        EffacerHTML();
        exit(EXIT_FAILURE);
    }

    printf("connecté\n");
    // Structure pour stocker les informations personne
    Personne personne;
    char message[100];
    // Question du serveur
    recv(socketClient, message, sizeof(message), 0);
    printf("%s\n", message);
    // Informations de l'utilisateur
    printf("Votre nom et âge : ");
    scanf("%s %d", personne.Nom, &personne.NombreDeBillets);
    // Informations au serveur
    send(socketClient, &personne, sizeof(personne), 0);
    // Enregistrement des infos dans un fichier HTML.
    EnregistrementHTML(&personne);
    printf("Informations enregistrées dans ReservationTCP.html.\n");
    // Fermeture socket client
    close(socketClient);

    return 0;
}