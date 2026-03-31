/* 
   L'objectif du projet est : créer une page HTML dynamique adaptée à notre protocole UDP qui se met automatiquement à jour lors du lancement du serveur. Le scénario implique une communication entre le client et
   le serveur, où le client transmet le nom et le nombre de billets réservés pour un événement, par exemple, en utilisant les commandes : ./serveur_udp et./client_udp, et le serveur ferme avec la commande exit 0.
   Cette interaction donne accès à une page HTML contenant la liste des personnes enregistrées. Le protocole UDP a l'avantage d'être rapide mais il est plus difficile d'instaurer une limite de client.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

//Définition de la structure pour stocker des informations sur une personne.

typedef struct Personne 
{
    char Nom[10];
    int NombreDeBillets;
} Personne;

void EnregistrementHTML(Personne *Personne)
{
    FILE *dossier = fopen("RéservationUDP.html", "a");
    if (dossier == NULL)
    {
        perror("Erreur d'ouverture de fichier");
        exit(EXIT_FAILURE);
    }
    // Écriture du début du fichier HTML si le fichier est vide
    if (ftell(dossier) == 0)
    {
        // Écriture du titre principal
        fprintf(dossier, "<html>\n<head>\n<title>Communication orientee datagramme</title>\n</head>\n<body>\n");
        // Ajout du titre principal en haut de la page
        fprintf(dossier, "<h1 style=\"text-align:center;\">Communication orientee datagramme</h1>\n");
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
    fprintf(dossier, "<html>\n<head>\n<title>Communication orientee datagramme</title>\n</head>\n<body>\n");
    // Écriture du titre principal en haut de la page
    fprintf(dossier, "<h1 style=\"text-align:center;\">Communication orientee datagramme</h1>\n");
    // Ajout du sous-titre centré juste en dessous du titre principal
    fprintf(dossier, "<h2 style=\"text-align:center;\"><span style=\"font-weight:normal;\">Liste des clients actuels</span></h2>\n");
    // Explication de l'utilité du serveur
    fprintf(dossier, "<h2 style=\"text-align:center;\"><span style=\"font-weight:normal;\">Reservations de billets pour l'entrée d'un evenement par exemple</span></h2>\n");
    
    // Fermeture du fichier HTML
    fclose(dossier);
}

int main(int argc, char *argv[])
{
    // Ouvrerture du socket client
    int socketClient = socket(AF_INET, SOCK_DGRAM, 0);
    // Vérification ouverture socket
    if (socketClient == -1)
    {
        perror("Erreur d'ouverture du socket");
        exit(EXIT_FAILURE);
    }
    
    struct sockaddr_in addrServer;
    addrServer.sin_addr.s_addr = inet_addr("127.0.0.1");
    addrServer.sin_port = htons(9606);
    addrServer.sin_family = AF_INET;
    // Adresse locale du socket du serveur
    bind(socketClient, (const struct sockaddr *)&addrServer, sizeof(addrServer));

    char message[100];
    Personne personne;
    // Lire les infos du client (read)
    printf("Votre nom et nombre de billets réservés : ");
    scanf("%s %d", personne.Nom, &personne.NombreDeBillets);
    // Envoyer infos au serveur
    sendto(socketClient, &personne, sizeof(personne), 0, (struct sockaddr *)&addrServer, sizeof(addrServer));
    // Recevoir la question du serveur
    recvfrom(socketClient, message, sizeof(message), 0, NULL, NULL);
    printf("%s\n", message);
    // Enregistrement des infos dans un fichier HTML.
    EnregistrementHTML(&personne);
    printf("Informations enregistrées dans ReservationUDP.html.\n");
    // Fermeture socket client
    close(socketClient);

    return 0;
}




