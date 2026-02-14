/*  les fonctions 'dizaine' et 'unite' déclarées en tant que 'constexpr' permettent d'extraire les dizaines et unités pour les heures minutes et secondes en provenance du module DS3231
    (le temps pourrait provenir également d'un serveur...)
    Ici on ne travaille qu'avec des chaines AZT pour en extraire entre autre des quartets.
    Toutes les secondes, les dizaines et unités des heures, minutes et secondes sont transformées en groupe de bits avec la fonction 'binaire'
    Enfin dans la loop (avec ma fonction : void concatener(char *tab1, char *tab2, char *tab3, char *tab4, char *tab5, char *tab6), je concatène sous la forme d'une chaine AZT tous les bits constitutifs.
    Au final j'ai une chaine AZT 'binaire', l'opérateur ternaire fait le travail pour allumer ou éteindre les leds.
*/

#include <Wire.h>
#include "simpleRTC.h" // pin 16 : SCL - pin 17 SDA - sur uC SCL : 22 - SDA : 23 - Merci à Bricoleau pour sa librairie


constexpr  uint8_t dizaine(uint8_t x) {
  return  ((x) / 10);
}
constexpr  uint8_t unite(uint8_t x) {
  return ((x) % 10);
}

unsigned long tempsPrecedent;

char tabHeuresDizaine [2 + 1];
char tabHeuresUnite [4 + 1];
char tabMinutesDizaine [3 + 1];
char tabMinutesUnite [4 + 1];
char tabSecondesDizaine [3 + 1];
char tabSecondesUnite [4 + 1];
char tabConcatenation [20 + 1];


void concatener(char *tab1, char *tab2, char *tab3, char *tab4, char *tab5, char *tab6) {
  uint8_t i = 0;
  while (tab1[i]) i++; // on récupére la valeur de i à la fin du tableau
  while ( *tab2) { // on ajoute les caractères du deuxième tableau au premier
    tab1[i++] =  *tab2;
    tab2++;
  }
  while (*tab3) { // on ajoute les caractères du troisième tableau
    tab1[i++] =  *tab3;
    tab3++;
  }
  while (*tab4) { // on ajoute les caractères du quatrième tableau
    tab1[i++] =  *tab4;
    tab4++;
  }
  while (*tab5) { // on ajoute les caractères du cinquième tableau
    tab1[i++] =  *tab5;
    tab5++;
  }
  while (*tab6) { // on ajoute les caractères du sixième tableau
    tab1[i++] =  *tab6;
    tab6++;
  }
  tab1[i] = '\0';
}

char *binaire (uint8_t n, uint8_t w, char *tab) {
  uint8_t y = 0;

  for ( uint8_t x = 0 ; x < 8; x++) {
    if (x > w ) { // w = 3 donne 4 elements
      tab[y] = n & 0x80 ? '1' : '0';
      y++;
    }
    n <<= 1;
  }
  tab[y] = '\0';
  return tab;
}


void setup() {
  Wire.begin();
  for (uint8_t x = 8; x < 14; x++) {// HEURES
    pinMode(x, OUTPUT);
    digitalWrite(x, LOW);
  }
  for (uint8_t x = 18; x < 24; x++) {// Secondes
    pinMode(x, OUTPUT);
    digitalWrite(x, LOW);
  }
  for (uint8_t x = 24; x < 31; x++) {// Minutes
    pinMode(x, OUTPUT);
    digitalWrite(x, LOW);
  }

  pinMode(15, OUTPUT);
  digitalWrite(15, LOW); // dernière Led des secondes

}

void loop() {
  uint8_t h = RTC.heure();
  uint8_t m = RTC.minute();
  uint8_t s = RTC.seconde();

  if (RTC.actualiser()) {

    if (millis() - tempsPrecedent > 1000) {
      uint8_t heureDizaine = dizaine(h);
      uint8_t heureUnite = unite(h);
      uint8_t minuteDizaine = dizaine(m);
      uint8_t minuteUnite = unite(m);
      uint8_t secondeDizaine = dizaine(s);
      uint8_t secondeUnite = unite(s);

      binaire(heureDizaine, 5, tabHeuresDizaine); // deux bits pour les heures (dizaines)
      binaire(heureUnite, 3, tabHeuresUnite); // quatre bits pour les heures (unités)
      binaire(minuteDizaine, 4, tabMinutesDizaine); // trois bits pour les minutes (dizaines)
      binaire(minuteUnite, 3, tabMinutesUnite); // quatre bits pour les minutes (unités)
      binaire(secondeDizaine, 4, tabSecondesDizaine); // trois bits pour les secondes (dizaines)
      binaire(secondeUnite, 3, tabSecondesUnite); // quatre bits pour les secondes (unités)

      strcpy(tabConcatenation, tabHeuresDizaine);

      concatener (tabConcatenation, tabHeuresUnite, tabMinutesDizaine, tabMinutesUnite, tabSecondesDizaine, tabSecondesUnite ) ;

      for (uint8_t x = 0; x < 6; x++) {
        tabConcatenation[x] == '1' ? digitalWrite( x + 8, HIGH) : digitalWrite( x + 8, LOW) ; // pins 8 - 9 - 10 - 11 - 12 - 13 (HEURES)
      } // 14 - 15 - 16 - 17 - 18 - 19 sur uC
      for (uint8_t x = 6; x < 13; x++) {
        tabConcatenation[x] == '1' ? digitalWrite( x + 18, HIGH) : digitalWrite( x + 18, LOW) ; // pins 24 - 25 - 26 - 27 - 28 - 29 - 30 (Minutes)
      }// 40 - 39 - 38 - 37 - 36 - 35 - 34 sur uC
      for (uint8_t x = 13; x < 19; x++) {
        tabConcatenation[x] == '1' ? digitalWrite( x + 5, HIGH) : digitalWrite( x + 5, LOW) ; // pins 18 - 19 - 20 - 21 - 22 - 23  (Secondes)
      }// 24 - 25 - 26 - 27 - 28 - 29 sur uC
      tabConcatenation[19] == '1' ? digitalWrite( 15, HIGH) : digitalWrite( 15, LOW) ; // pin 15  (dernier bit des Secondes) - pin 21 sur uC

      *tabConcatenation = '\0'; // On repart sur une chaine vide
      tempsPrecedent = millis();
    }
  }
}
