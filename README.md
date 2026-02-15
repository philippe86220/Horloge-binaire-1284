# Horloge binaire – ATmega1284 + DS3231

**horloge binaire HH:MM:SS :**

Horloge binaire basée sur un
microcontrôleur **ATmega1284** et un module RTC **DS3231**.  
L’heure est affichée sous forme binaire (HH:MM:SS) à l’aide de LED.

Ce projet a été développé en 2024 dans un objectif à la fois
**fonctionnel**, **pédagogique** et **électronique**, en privilégiant :

- la compréhension du code,
- la maîtrise du matériel,
- et la simplicité des principes.

---

## 🧭 Objectif du projet

- Réaliser une horloge binaire autonome
- Comprendre le lien **temps → binaire → LEDs**
- Travailler sans `String`, uniquement avec des **`C-strings`**
- Exploiter un microcontrôleur AVR “hors Arduino Uno”
- Concevoir un circuit complet (alimentation, MCU, RTC, affichage)

---

## 🖼️ Rendu final

L’horloge affiche :
- **Heures** (dizaines + unités)
- **Minutes** (dizaines + unités)
- **Secondes** (dizaines + unités)

Chaque chiffre décimal est représenté en **binaire**, à l’aide de LED organisées par poids (1, 2, 4, 8).

![connexions](/docs/connexions.jpg)

---

## 🧰 Matériel utilisé (BOM)

### Composants principaux
- ATmega1284 (DIP40)
- Module RTC DS3231 (Adafruit ou équivalent)
- Régulateur de tension **L7805ABP (TO-220)**
- LEDs rouges et vertes
  par exemple https://www.gotronic.fr/account/orders/000605204  
- Resistances 470 ohm (selon LED et luminosite voulue)
- Diode 1N4007
- Condensateurs de filtrage
- Connecteur jack DC

### Alimentation
- Entrée DC (jack)
- Régulation **5 V**
- Le projet fonctionne intégralement en 5 V

---

## 🔌 Alimentation et horloge

- **Alimentation** : tension de 9 V en courant continu pouvant fournir jusqu’à 0,5 A
- **Tension** : 5 V régulée
- **Fréquence MCU** : oscillateur interne
- **Quartz externe** : ❌ non utilisé
- **Fréquence** : 8 MHz interne

Le choix d’un RTC **DS3231** garantit une excellente précision, indépendamment de l’horloge interne du microcontrôleur.  
De prèférence évitez le module de type aliexpress, utilisez par exemple :   
https://www.gotronic.fr/art-module-rtc-i2c-ds3231-ada3013-24708.htm

---

## 📐 Schéma de câblage

Le schéma électronique complet est fourni :

- ATmega1284 autonome
- RTC DS3231 via I2C
- Groupes de LED pour heures / minutes / secondes
- Régulation et filtrage d’alimentation

👉 Le Schéma est valable aussi bien pour un 1284 ou un 1284P :

![Schematic](/docs/Schematic_Horloge-Binaire-ATmega1284.png)



---

## 💾 Flash et configuration de l’ATmega1284

### Core Arduino utilisé

Ce projet utilise **MightyCore 3.0.3**, développé par MCUdude et l'**IDE Arduino 1.8.19** :

https://github.com/MCUdude/MightyCore

MightyCore permet d’utiliser l’ATmega1284 directement depuis l’IDE Arduino.

---

### Installation de MightyCore

1. Ouvrir **Fichier → Préférences**
2. Ajouter l’URL suivante :

https://mcudude.github.io/MiniCore/package_MCUdude_MiniCore_index.json

3. Ouvrir **Outils → Type de carte → Gestionnaire de cartes**
4. Rechercher **MightyCore**
5. Installer

---

⚙️ **Configuration dans Arduino IDE**

Sélectionner :

- Type de carte : ATmega1284
- Clock : Internal 8 MHz
- BOD : 2.7 V
- Variant : 1284
- Pinout : Standard
- Bootloader : No bootloader
- Programmer : AVRISP mkII

👉 Ces paramètres configurent automatiquement les fuses.  
L’utilisation d’un bootloader n’est pas nécessaire dans ce projet,   
car le microcontrôleur est programmé directement via l’interface ISP.

---

🔌 **Câblage ISP**
Connexion du programmateur AVRISP mkII :  
Programmateur	ATmega1284
```
+---------------+---------------------+
| Programmateur | ATmega1284 (DIP40) |
+---------------+---------------------+
| MOSI          | pin 6               |
| MISO          | pin 7               |
| SCK           | pin 8               |
| RESET         | pin 9               |
| VCC           | pin 10              |
| GND           | pin 11              |
+---------------+---------------------+
```
⚠️ Ces broches correspondent au PORT B du microcontrôleur :
```
PB5 = MOSI
PB6 = MISO
PB7 = SCK
```
![standard](/docs/standard.png)
![cablage](/docs/IMG_2921.jpg)

---

🔥 **Gravure de la séquence d’initialisation**

![cablage](/docs/IMG_2920.jpg)

Cliquer :
```
Outils → Graver la séquence d'initialisation
```
Cette étape :
- configure les fuses
- sélectionne l’horloge interne 8 MHz
- désactive le bootloader
  
👉 aucune autre manipulation n’est nécessaire.

---

## 🧠 Principe de fonctionnement du code

Chaque seconde, le programme effectue la séquence suivante :

1. Lecture de l’heure via le RTC
2. Séparation dizaines / unités
3. Conversion de chaque chiffre en binaire
4. Construction d’une chaîne binaire globale
5. Pilotage direct des LED à partir de cette chaîne

La chaîne binaire est la **source unique de vérité**.

---

## 🧩 Choix techniques importants

- ❌ Pas de classe `String`
- ✅ Utilisation exclusive de `char[]`
- ✅ Pas de `delay()`
- ✅ Code volontairement lisible et structuré
- ✅ Approche pédagogique assumée

---

## 📚 Pour aller plus loin

- Ajouter un réglage manuel de l’heure   
  (le module RTC est très performant donc ce n'est pas indispensable de plus  
  vous devez le rendre amovible et donc réglable via l'ide Arduino)
- Ajouter un affichage date
- Porter le projet en 3,3 V

---

## 🔍 Déroulé du programme 

1️⃣ Acquisition du temps

```cpp
uint8_t h = RTC.heure();
uint8_t m = RTC.minute();
uint8_t s = RTC.seconde();
```
- Le DS3231 est la source
- `RTC.actualiser()` sert de validation / synchro
---

2️⃣ Séparation dizaines / unités

```cpp
constexpr uint8_t dizaine(uint8_t x) { return x / 10; }
constexpr uint8_t unite(uint8_t x)   { return x % 10; }
```
---

3️⃣ Conversion décimale → binaire (explication ultra pédagogique)

🎯 **Objectif de la fonction**

La fonction `binaire()` transforme un **chiffre décimal** (par exemple `2`, `7`, `9`)
en une suite de LED allumées ou éteintes, représentée ici par une chaîne de caractères '0' et '1'.

Exemples :

- `2` → `"10"`
- `5` → `"101"`
- `9` → `"1001"`

---

🧠 **Principe fondamental (à comprendre en premier)**

> **L’ATmega1284 est un microcontrôleur 8 bits,**
> mais **nous n’avons pas toujours besoin des 8 bits** pour afficher un chiffre.

Exemples :

- les **dizaines d’heures** ne vont que de `0` à `2`
- afficher `2` avec 8 bits (`00000010`) n’a aucun intérêt
- **2 bits suffisent** : `10`

👉 Le rôle de la fonction binaire() est donc :
- de regarder **les 8 bits** du nombre,
- mais de **ne garder que les bits utiles.**

---

🔍 **Signature de la fonction**

```cpp
binaire(valeur, w, buffer);
```
- `valeur` : le nombre à convertir (ex: 2, 7, 9)
- `w` : indique **combien de bits on ignore au début**
- `buffer` : la chaîne `'0'/'1'` résultat

⚠️ **Point clé pour le débutant**
👉 `w` n’est pas le nombre de bits,
👉 c’est le nombre de positions ignorées avant de commencer à écrire.

---

🧩 **Comment la fonction travaille (pas à pas)**

La fonction :

1. regarde toujours le **bit de poids fort** (bit 7),
2. décale le nombre vers la gauche,
3. répète l’opération **8 fois**,
4. n’écrit les bits que lorsque `x > w`.

Autrement dit :

> “Je regarde les bits un par un, mais je commence à écrire seulement quand j’arrive dans la zone qui m’intéresse.”

---

🧠 **Visualisation simple (sans code)**

Imagine une règle de 8 cases (les 8 bits) :

```css
[ b7 ][ b6 ][ b5 ][ b4 ][ b3 ][ b2 ][ b1 ][ b0 ]

```

- La fonction parcourt toujours les 8 cases
- `w` dit :
👉 **“Ignore les cases de gauche jusqu’à celle-ci”**

---

🧮 **Pourquoi `w = 5` pour les dizaines d’heures**

**Valeurs possibles**

Les dizaines d’heures peuvent valoir :
- 0
- 1
- 2

En binaire :

- 0 → `00`
- 1 → `01`
- 2 → `10`

👉 **2 bits suffisent**

---

**Que fait `w = 5` ?**

Si `w = 5`, la condition `x > w` devient vraie seulement pour :

```
x = 6 et x = 7
```

➡️ donc **2 bits sont écrits**

C’est exactement ce que l’on veut.

```cpp
binaire(heureDizaine, 5, tabHeuresDizaine); // écrit 2 bits
```

---

🧮 **La formule (à donner seulement après)**

Une fois l’idée comprise, on peut ajouter la règle mathématique :  

> Nombre de bits écrits = `7 - w`

Donc :
- si on veut 2 bits → `7 - w = 2` → `w = 5`
- si on veut 4 bits → `7 - w = 4` → `w = 3`

---

📊 **Tableau récapitulatif**

| Groupe            | Valeur max | Bits nécessaires | Valeur de `w` |
| ----------------- | ---------- | ---------------- | ------------- |
| Heures dizaines   | 2          | 2 bits           | `5`           |
| Heures unités     | 9          | 4 bits           | `3`           |
| Minutes dizaines  | 5          | 3 bits           | `4`           |
| Minutes unités    | 9          | 4 bits           | `3`           |
| Secondes dizaines | 5          | 3 bits           | `4`           |
| Secondes unités   | 9          | 4 bits           | `3`           |

👉 **Le tableau montre qu’il n’y a rien à deviner**

> La fonction `binaire()` regarde toujours 8 bits, mais n’en conserve que le nombre strictement  
> nécessaire pour afficher le chiffre.  
> Le paramètre `w` sert simplement à indiquer **à partir de quel moment on commence à écrire les bits utiles.**

---

4️⃣ Construction d’une chaîne binaire globale  


```cpp
char tabConcatenation[20 + 1];
```
-> concatènes dans l’ordre :

HHd | HHu | MMd | MMu | SSd | SSu

via la fonction :

```cpp
void concatener(...)
```
- Pas de strcat
- Pas de buffer temporaire
- Une seule chaîne finale

🎯 **Idée de base**  

En langage C, une chaîne de caractères (`char[]`) n’est rien d’autre qu’une **suite de cases mémoire   
contiguës,** chacune contenant un caractère.
Par exemple, la chaîne `"10"` est stockée en mémoire comme ceci :

```
Adresse →   0     1     2
Contenu →  '1'   '0'   '\0'
```
Le `'\0'` (zéro terminal) indique la fin de la chaîne.

---

🧩 **Notre situation dans le programme**

Nous avons plusieurs petits tableaux mémoire :  

```
tabHeuresDizaine   = "10"
tabHeuresUnite     = "0011"
tabMinutesDizaine  = "101"
```

Et un grand tableau :

```
tabConcatenation = [cases mémoire vides]
```

Le rôle de `concatener()` est **de remplir ce grand tableau en copiant les cases des autres tableaux à la suite.**  

---

🔎 **Étape 1 — Trouver la fin du tableau principal**   
Au début, tab1 contient déjà le premier morceau.

Exemple :
```
tab1 (tabConcatenation)

Adresse →   0     1     2     3     4 ...
Contenu →  '1'   '0'   '\0'  ?     ?
```

La boucle : 

```cpp
while (tab1[i]) i++;
```

avance jusqu’au `'\0'`.

👉 À la fin :

- i pointe exactement **la première case libre.**

---

🔎 **Étape 2 — Copier un autre tableau**

Prenons `tab2 = "0011"`.

En mémoire :
```
Adresse →   0     1     2     3     4
Contenu →  '0'   '0'   '1'   '1'   '\0'

```

La boucle :

```cpp
while (*tab2) {
  tab1[i++] = *tab2;
  tab2++;
}
```

fait exactement ceci :  

**Copie case par case**
```
tab1 avant :
[ '1' ][ '0' ][ '\0' ][ ? ][ ? ][ ? ]

copie '0' →
[ '1' ][ '0' ][ '0' ][ ? ][ ? ]

copie '0' →
[ '1' ][ '0' ][ '0' ][ '0' ][ ? ]

copie '1' →
[ '1' ][ '0' ][ '0' ][ '0' ][ '1' ]

copie '1' →
[ '1' ][ '0' ][ '0' ][ '0' ][ '1' ][ '1' ]
```
Puis la boucle s’arrête quand elle rencontre le `'\0'`.

👉 On passe ensuite au tableau suivant (`tab3`, `tab4`, etc.).

---

🔎 **Étape finale — Marquer la fin de la chaîne**  

Après toutes les copies :
```cpp
tab1[i] = '\0';
```

On place le marqueur de fin :  

```css
[ '1' ][ '0' ][ '0' ][ '0' ][ '1' ][ '1' ][ '\0' ]
```

Maintenant, `tab1` est une chaîne valide.

---

🧠 **Que signifient exactement les deux lignes clés ?**  

Ces deux lignes sont le cœur de la fonction :
```cpp
tab1[i++] = *tab2;
tab2++;
```
👉 Traduction simple :  
- `*tab2` = “le caractère contenu dans la case pointée par tab2”
- `tab2++` = “déplace le pointeur pour qu’il pointe vers le caractère suivant”
- `tab1[i++]` = “écrit dans la case i de tab1, puis avance i”

Donc :  

> On copie le contenu d’une case mémoire dans une autre, puis on avance d’une case dans les deux 
> tableaux.

---

🎯 **Résumé en une phrase **
> La fonction concatener() copie simplement le contenu de plusieurs tableaux de caractères 
> dans un grand tableau, case mémoire par case mémoire, jusqu’à construire une seule chaîne continue.

---

5️⃣ Affichage LED = lecture directe de la chaîne

tabConcatenation[x] == '1'
  ? digitalWrite(pin, HIGH)
  : digitalWrite(pin, LOW);

Avec **trois plages :**
- Heures
- Minutes
- Secondes (+ bit final à part)

👉 La chaîne binaire est la source unique d’état
👉 Le hardware obéit

---

6️⃣ Gestion temporelle

```cpp
if (millis() - tempsPrecedent > 1000)
```

- Mise à jour toutes les secondes
- Pas de delay()


## 👤 Auteur

Projet conçu et réalisé par **Philippe86220**  
2024 — usage personnel et pédagogique

---

## 📄 Licence

MIT 


