# Multiprise Pilotée
### API de contrôle

Version 1.0

----
### Présentation
Le bloc d'alimentation pilotable "Multiprise Pilotée" se commande via le connecteur USB-C. Le contrôleur interne implémente une liaison série virtuelle (classe USB-CDC). Sous Windows, la liaison série sera vue comme un port COM, sous Linux comme un device ttyUSB dans /dev.

La liaison série se configure comme une liaison série normale à ceci prés que ce qui relève du baudrate ou du format des données n'a pas d'importance ici et peut prendre n'importe quelle valeur.

Exemple de configuration en python sous Unix :
```
import serial

# Configuration de /dev/ttyUSB0 pour un baudrate de 115200 bauds
s = serial.Serial("/dev/ttyUSB0", 115200)
```

Le bloc d'alimentation pilotable fourni les éléments suivants, tous commandables individuellement :
* 4 prises 230V
* 3 prises 3V
* 1 prise 5V
* 1 prise 5V réversible (la polarité peut être changée)
* 1 prise variable 2V ↔ 5V

De ce fait, l'API décrit ci-après propose des jeux de commande par groupe d'élément. Le format retenu est globalement celui de commandes AT, utilisées généralement dans les équipements réseaux.
    
### Description des commandes
#### Syntaxe des commandes AT

Les commandes peuvent être écrites en majuscule ou minuscule ou un mélange des deux. Les commandes doivent être terminées par "\n" (0x0A) ou "\r" (0x0D).

Les réponses se terminent par "\n" (0x0A).

```
Commande                    Réponse

ATAC1=1\n                   OK\n
ATVER?\n                    1.0\n
```

#### Commandes générales

| Commande | Description                     | Réponse |
| -------- | --------                        | ------- |
| AT       | Ping de l'équipement            | OK      |
| ATE=n    | Active (n=1) ou désactive (n=0) l'écho de la commande envoyée | OK si n=0 ou 1 ERROR sinon |
| ATE?     | Récupère l'état de l'écho       | "1" ou "0" selon l'activation de l'écho                  |
| ATR      | Remise à zéro de toutes les sorties | OK si succès <br> ERROR en cas d'erreur              |
| ATVER?   | Récupère la version du firmware | MAJOR_VER.MINOR_VER                                      |

#### Commandes pour les prises 230V (AC)

| Commande | Description                     | Réponse |
| -------- | --------                        | ------- |
| ATAC=n   | Active (n=1) ou désactive (n=0) toutes les sorties AC | OK si succès <br> ERROR en cas d'erreur |
| ATAC?    | Récupère l'état des sorties AC  | V correspondant au masque des prises actives                  |
| ATACx=n  | Active (n=1) ou désactive (n=0) la sortie AC (x) | OK si succès <br> ERROR en cas d'erreur      |
 
#### Commandes pour les prises 3V

| Commande | Description                     | Réponse |
| -------- | --------                        | ------- |
| AT3V=n   | Active (n=1) ou désactive (n=0) toutes les sorties 3V | OK si succès <br> ERROR en cas d'erreur |
| AT3V?    | Récupère l'état des sorties 3V | V correspondant au masque des prises actives                   |
| AT3Vx=n  | Active (n=1) ou désactive (n=0) la sortie 3V (x) | OK si succès <br> ERROR en cas d'erreur      |

#### Commandes pour les prises 5V

| Commande | Description                     | Réponse |
| -------- | --------                        | ------- |
| AT5V=n   | Active (n=1) ou désactive (n=0) toutes les sorties 5V | OK si succès <br> ERROR en cas d'erreur |
| AT5V?    | Récupère l'état des sorties 5V | V correspondant au masque des prises actives                   |

#### Commandes pour la prise 5V inversable

| Commande | Description                     | Réponse |
| -------- | --------                        | ------- |
| ATVPOL=n | Active la sortie en positif (n = +), <br> négatif (n = -) <br> ou désactive (n = 0) | OK si succès <br> ERROR en cas d'erreur       |
| ATVPOL?  | Récupère l'état de la sortie 5V inversable | VPOS si actif positif <br> VNEG si actif négatif <br> OFF si désactivé |
    
#### Commandes pour la prise variable

| Commande | Description                     | Réponse |
| -------- | --------                        | ------- |
| ATVAR=n  | Active (2.0 ≤ n ≤ 5.0) ou désactive (n=0.0) la sortie variable | OK si succès <br> ERROR en cas d'erreur     |
| ATVAR?   | Récupère l'état de la sortie variable | V correspondant à la valeur, en flottant, de la sortie               |

