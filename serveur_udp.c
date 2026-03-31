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
#include <fcntl.h>
#include <sys/select.h>
#include <errno.h>

//Définition de la structure pour stocker des informations sur une personne.

typedef struct Personne 
{
    char Nom[10];
    int NombreDeBillets;
} Personne;

// Effacer le contenu du fichier HTML
void EffacerHTML()
{
    FILE *dossier = fopen("RéservationUDP.html", "w");
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
    EffacerHTML();
    // Ouvrerture du socket serveur
    int socketServer = socket(AF_INET, SOCK_DGRAM, 0);
    // Vérification ouverture socket
    if (socketServer == -1)
    {
        perror("Erreur d'ouverture du socket");
        exit(EXIT_FAILURE);
    }
    
    struct sockaddr_in addrServer;
    addrServer.sin_addr.s_addr = inet_addr("127.0.0.1");
    addrServer.sin_port = htons(9606);
    addrServer.sin_family = AF_INET;
    // Adresse locale du socket du serveur
    if (bind(socketServer, (const struct sockaddr *)&addrServer, sizeof(addrServer)) == -1)
    {
        perror("Erreur liaison socket");
        exit(EXIT_FAILURE);
    }
    
    printf("bind: %d\n", socketServer);

    char message[100];
    Personne personne;
    
    struct sockaddr_in addrClient;
    socklen_t clientAddrLen = sizeof(addrClient);
    // Procédure du socket pour fonctionner en mode non bloquant.
    if (fcntl(socketServer, F_SETFL, O_NONBLOCK) < 0)
    {
        perror("Une erreur s'est produite lors de la configuration du socket en mode non bloquant.");
        exit(EXIT_FAILURE);
    }

    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(socketServer, &fds);

    while (1)
    {
        
        int pret = select(socketServer + 1, &fds, NULL, NULL, NULL);

        if (pret == -1)
        {
            perror("Erreur");
            exit(EXIT_FAILURE);
        }

        if (FD_ISSET(socketServer, &fds))
        {
            // Recueillir les informations du client
            ssize_t receivedBytes = recvfrom(socketServer, message, sizeof(message), 0, (struct sockaddr *)&addrClient, &clientAddrLen);

            if (receivedBytes == -1)
            {
                if (errno != EAGAIN && errno != EWOULDBLOCK)
                {
                    perror("Erreur");
                    break;
                }
            }
            else if (receivedBytes > 0)
            {
                message[receivedBytes] = '\0'; // Dernier caractère de la chaîne est nul.
                printf("Nom du client (%s:%d): %s\n", inet_ntoa(addrClient.sin_addr), ntohs(addrClient.sin_port), message);
                // Vérifier si le message est "exit"
                if (strncmp(message, "exit", 4) == 0)
                {
                    printf("Le serveur a reçu la commande 'exit 0'. 3 2 1 Fermeture proche...\n");
                    break; 
                }
                // Réponse au client
                strcpy(message, "Merci, vos informations vont etre enregistrées.");
                sendto(socketServer, message, strlen(message) + 1, 0, (struct sockaddr *)&addrClient, sizeof(addrClient));
                // Réponse du client
                receivedBytes = recvfrom(socketServer, &personne, sizeof(personne), 0, (struct sockaddr *)&addrClient, &clientAddrLen);
                if (receivedBytes == -1)
                {
                    if (errno != EAGAIN && errno != EWOULDBLOCK)
                    {
                        perror("Erreur lors de la réception des infos de la personne");
                        break;
                    }
                }
            }
        }
    }
    // Fermeture du socket du serveur
    close(socketServer);
    printf("Serveur fermé.\n");
    return 0;
}
