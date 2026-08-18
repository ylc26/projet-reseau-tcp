# Communication client-serveur TCP et UDP en C

Projet pédagogique comparant deux modes de communication réseau à travers un cas simple de réservation. Les clients transmettent un nom et une quantité au serveur, tandis qu’une page HTML présente les informations enregistrées.

## Objectifs

- créer et configurer des sockets IPv4 en C ;
- comparer une communication orientée connexion (TCP) et une communication par datagrammes (UDP) ;
- manipuler `bind`, `listen`, `accept`, `connect`, `send`, `recv`, `sendto` et `recvfrom` ;
- gérer plusieurs clients TCP avec des threads POSIX ;
- expérimenter un serveur UDP non bloquant avec `select` ;
- produire une restitution HTML simple.

## Architecture

| Mode | Adresse | Port | Particularité |
|---|---|---:|---|
| TCP | `127.0.0.1` | 9065 | Serveur multithread limité à trois clients |
| UDP | `127.0.0.1` | 9606 | Socket non bloquant et arrêt par commande `exit` |

## Structure du dépôt

```text
.
├── examples/
│   ├── reservations_tcp.html   # Exemple de restitution TCP
│   └── reservations_udp.html   # Exemple de restitution UDP
├── src/
│   ├── client_tcp.c
│   ├── client_udp.c
│   ├── serveur_tcp.c
│   └── serveur_udp.c
├── .gitignore
├── Makefile
└── README.md
```

Les exécutables compilés ne sont pas versionnés : ils sont régénérés dans `bin/`.

## Compilation

Prérequis : GCC, GNU Make et un environnement compatible avec les sockets POSIX.

```bash
make
```

## Démonstration TCP

Dans un premier terminal :

```bash
./bin/serveur_tcp
```

Dans un autre terminal, lancez jusqu’à trois clients :

```bash
./bin/client_tcp
```

## Démonstration UDP

Dans un premier terminal :

```bash
./bin/serveur_udp
```

Dans un autre terminal :

```bash
./bin/client_udp
```

Les fichiers `RéservationTCP.html` et `RéservationUDP.html` produits pendant l’exécution sont ignorés par Git. Des exemples sont conservés dans `examples/`.

## Limites

Ce projet illustre des concepts réseau dans un environnement local. Les structures échangées, les tailles de chaînes et le protocole applicatif sont volontairement simples ; le code n’est pas destiné à une utilisation en production.

## Contexte

Projet universitaire réalisé dans le cadre de la Licence Informatique à l’Université Paris-Saclay.
