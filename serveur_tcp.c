/* 
   L'objectif du projet est : créer une page HTML dynamique adaptée à notre protocole UDP qui se met automatiquement à jour lors du lancement du serveur. Le scénario implique une communication entre le client et
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
#include <pthread.h>
//Définition de la structure pour stocker des informations sur une personne.
typedef struct Personne 
{
    char Nom[10];
    int NombreDeBillets;
} Personne;

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

// Fonction exécutée par chaque thread client
void *function(void *arg)
{
    int socket = *(int *)arg;
    char msg[] = "Quel est votre nom et votre âge ?";
    Personne personne;
    
    // Question au client
    send(socket, msg, strlen(msg) + 1, 0);
    
    // Réponse du client
    recv(socket, &personne, sizeof(personne), 0);
    
    // Informations reçues
    printf("Le client s'appelle %s et a %d ans\n", personne.Nom, personne.NombreDeBillets);
    
    // Connexion et libérer la mémoire
    close(socket);
    free(arg);
    
    pthread_exit(NULL);
}

int main(void)
{
    EffacerHTML();
    // Ouverture socket serveur
    int socketServer = socket(AF_INET, SOCK_STREAM, 0);
    // Socket ouvert ou non
    if (socketServer == -1)
    {
        perror("Erreur d'ouverture du socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in addrServer;
    addrServer.sin_addr.s_addr = inet_addr("127.0.0.1");
    addrServer.sin_port = htons(9065);
    addrServer.sin_family = AF_INET;
    // Adresse locale du socket du serveur
    if (bind(socketServer, (const struct sockaddr *)&addrServer, sizeof(addrServer)) == -1)
    {
        perror("Erreur lors de la liaison du socket");
        exit(EXIT_FAILURE);
    }
    printf("bind: %d\n", socketServer);
    //Socket en attente de connexions
    listen(socketServer, 5);
    printf("listen\n");
    // Compteur de clients
    int compteur = 0; 
    pthread_t threads[3];
    // Boucle infinie pour accepter les connexions
    while (1)
    {
        if (compteur == 3) {
            break;
        }
         
        struct sockaddr_in addrClient;
        socklen_t csize = sizeof(addrClient);
        // Accepter la connexion d'un client
        int socketClient = accept(socketServer, (struct sockaddr *)&addrClient, &csize);
        printf("accept\n");

        printf("client: %d\n", socketClient);

        int *arg = malloc(sizeof(int));
        *arg = socketClient;   
        //Thread pour traiter la requête du client
        pthread_create(&threads[compteur], NULL, function, arg);
        
        compteur++;
    } 
    for (int i = 0; i < 3; i++)
    {
        pthread_join(threads[i], NULL);
    }
    // Fermer le socket du serveur
    close(socketServer);
    printf("close\n");
    return 0;
}
