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
![rendu](/docs/IMG_2888.jpg)


---

## 🧰 Matériel utilisé (BOM)

### Composants principaux
- ATmega1284 (DIP40)
- Support de circuit intégré DIP-40 (socket pour ATmega1284, recommandé pour faciliter le remplacement éventuel)
- Module RTC DS3231 (Adafruit ou équivalent)
-  Barrette femelle sécable 2,54 mm (pour enficher le module RTC DS3231 et l'enlever pour mise à l'heure)
- Régulateur de tension **L7805ABP (TO-220)**
- LEDs rouges et vertes
  - par exemple https://www.gotronic.fr/art-led-5-mm-rouge-l53lid-2091.htm
  - https://www.gotronic.fr/art-led-5-mm-verte-l53lgd-2093.htm
- Resistances 470 Ω (selon LED et luminosite voulue)
- Diode 1N4007
- Condensateurs de filtrage
- Connecteur jack DC

### Alimentation
- Entrée DC (jack)
- Régulation **5 V**
- Le projet fonctionne intégralement en 5 V

---

## 🔌 Alimentation et horloge

- **Alimentation** : adaptateur 9 V DC, capable de fournir jusqu’a 0,5 A (ou plus).
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

![cablage](/docs/IMG_2920.jpg)

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
Programmateur	→ ATmega1284
```
+---------------+---------------------+
| Programmateur | ATmega1284 (DIP40) |
+---------------+---------------------+
| MOSI          | pin 6               |
| MISO          | pin 7               |
| SCK           | pin 8               |
| RESET         | pin 9               |
| VCC           | pin 10 & AVCC pin 30|
| GND           | pin 11 & 31         |
+---------------+---------------------+
```
⚠️ Les broches SPI correspondent au PORT B du microcontrôleur :
```
PB5 = MOSI
PB6 = MISO
PB7 = SCK
```
![standard](/docs/standard.png)
![cablage](/docs/IMG_2921.jpg)

---

🔥 **Gravure de la séquence d’initialisation**

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

⚙️ **Téléversement du fichier `.ino`**

![televersement](/docs/IMG_2926.jpg)

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
RTC.actualiser();
RTC.heure();
RTC.minute();
RTC.seconde();
```
- Le DS3231 est la référence temporelle.

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
👉 w n’est pas le nombre de bits à écrire : 
👉 c’est l’indice à partir duquel on commence à écrire, donc le nombre de positions ignorées au début.

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

Imaginez une règle de 8 cases (les 8 bits) :

```
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

🧮 **fonction d'aide au calcul de w : `uint8_t w_from_bits(uint8_t bits)`**
```cpp
uint8_t w_from_bits(uint8_t bits) {
  return (uint8_t)(7 - bits);
}
```
Pour plus de lisibilité, on ne manipule pas directement w.  

On indique simplement le nombre de bits nécessaires :

```cpp
binaire(valeur, w_from_bits(3), buffer);
```
Formule interne :
```cpp
w = 7 - nombre_de_bits
```
--- 

4️⃣ Fonction concatener() — explication pédagogique 


```cpp
char tabConcatenation[20 + 1];
```
-> concatènes dans l’ordre :

HHd | HHu | MMd | MMu | SSd | SSu

via la fonction :

```cpp
void concatener(char *dst, uint8_t dstSize,
                       const char *a, const char *b, const char *c,
                       const char *d, const char *e, const char *f)
{
```
🎯 Objectif
La fonction `concatener()` construit **une chaîne binaire globale unique** à partir de plusieurs petites  
chaînes contenant les bits des heures, minutes et secondes.  

Elle remplace volontairement l’usage des fonctions classiques comme `strcat()` afin de :  
- garder un contrôle total sur la mémoire,
- éviter tout dépassement de buffer,
- rester pédagogique.

---

🧩 **Les paramètres transmis**

L’appel de la fonction est :  

```cpp
concatener(tabConcatenation, sizeof(tabConcatenation),
           tabHeuresDizaine, tabHeuresUnite,
           tabMinutesDizaine, tabMinutesUnite,
           tabSecondesDizaine, tabSecondesUnite);
```

On transmet donc :

- `dst` : le tableau final qui recevra la chaîne complète
- `dstSize` : sa taille maximale
- `a..f` : pointeurs vers 6 C-strings sources

⚠️ En langage C :
> Quand on passe un tableau à une fonction, son nom est automatiquement converti en pointeur vers son premier élément (équivalent à &tab[0]).

Ainsi :
```cpp
tabHeuresDizaine
```
est équivalent à :

```cpp
&tabHeuresDizaine[0]
```
---

🧠 **Le principe clé : un tableau de pointeurs**

Au lieu de copier chaque tableau séparément, la fonction crée un tableau de pointeurs :  

```cpp
const char *srcs[6] = { a, b, c, d, e, f };
```

Ce tableau contient simplement :  

👉 les adresses des six chaînes à copier.  

Cela permet de :  

- parcourir toutes les sources avec une seule boucle,
- éviter la répétition de code,
- rendre la fonction générique.
  
---

🔁 **Étape 1 — Parcours des chaînes source**
Une boucle for parcourt les six pointeurs :

```
for (uint8_t s = 0; s < 6; s++)
```

À chaque itération :

```cpp
const char *p = srcs[s];
```

Le pointeur p reçoit l’adresse du premier caractère de la chaîne courante.

---

🔁 **Étape 2 — Copie caractère par caractère**
Une boucle interne copie les caractères tant que le zéro terminal n’est pas atteint :

```cpp
while (*p)
```
À chaque tour :
```cpp
dst[i++] = *p++;
```
Cette instruction compacte réalise plusieurs actions en une seule opération :

- `*p` : lit le caractère pointé dans la chaîne source
-  ce caractère est immédiatement copié dans `dst[i]`
- `p++` : avance le pointeur vers le caractère suivant
- `i++` : avance l’indice dans la chaîne destination

👉 Autrement dit :  
> On copie un caractère, puis on avance simultanément dans la chaîne source et dans la chaîne destination.

---

🧠 Important : gestion du zéro terminal  

Lorsque `*p == '\0'` :  

👉 la boucle s’arrête automatiquement  
👉 on passe à la chaîne suivante.  

---

🔒 Protection contre le dépassement mémoire
Avant chaque écriture, la fonction vérifie :

```cpp
if (i + 1 >= dstSize)
```

Cela garantit :

- qu’il reste toujours une place pour '\0'
- qu’aucune écriture hors buffer n’est possible.

Si la taille maximale est atteinte :

👉 la fonction termine immédiatement la chaîne et sort.

---

🏁 Étape finale — Fin de la chaîne globale
Une fois toutes les copies terminées :

```cpp
dst[i] = '\0';
```
On ajoute le marqueur de fin de chaîne.  

Le tableau dst devient alors une C-string valide contenant tous les bits.

---

🎯 **Résumé en une phrase **
> La fonction concatener() copie successivement plusieurs chaînes binaires dans un tableau unique,  
> caractère par caractère, en utilisant un tableau de pointeurs pour parcourir automatiquement toutes les   
> sources, tout en garantissant l’absence de dépassement mémoire.

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
if (millis() - tempsPrecedent >= 1000UL) {
  ...
  tempsPrecedent += 1000;
}
```

- += 1000 évite la dérive (cadence stable à 1 Hz)
- Pas de delay()


## 👤 Auteur

Projet conçu et réalisé par **Philippe86220**  
2024 — usage personnel et pédagogique

---

## 🤝 Remerciements

Ce projet a bénéficié d’échanges pédagogiques avec ChatGPT (OpenAI), utilisé comme assistant technique pour la réflexion algorithmique, 
l’optimisation du code et la rédaction de la documentation.

---

## 📄 Licence

MIT


