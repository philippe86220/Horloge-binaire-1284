/*  les fonctions 'dizaine' et 'unite' déclarées en tant que 'constexpr' permettent d'extraire les dizaines et unités pour les heures minutes et secondes en provenance du module DS3231
    Ici on ne travaille qu'avec des chaines AZT pour en extraire entre autre des quartets.
    Toutes les secondes, les dizaines et unités des heures, minutes et secondes sont transformées en groupe de bits avec la fonction 'binaire'
    Enfin dans la loop (avec ma fonction : void concatener(), je concatène sous la forme d'une chaine AZT tous les bits constitutifs.
    Au final j'ai une chaine AZT 'binaire', l'opérateur ternaire fait le travail pour allumer ou éteindre les leds.
*/

#include <Wire.h>
#include "simpleRTC.h" // pin 16 : SCL - pin 17 SDA - sur uC SCL : 22 - SDA : 23

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


void concatener(char *dst, uint8_t dstSize,
                const char *a, const char *b, const char *c,
                const char *d, const char *e, const char *f)
{
  uint8_t i = 0;

  const char *srcs[6] = { a, b, c, d, e, f };

  for (uint8_t s = 0; s < 6; s++) {
    const char *p = srcs[s];
    while (*p) {
      if (i + 1 >= dstSize) {  // garder une place pour '\0'
        dst[i] = '\0';
        return;
      }
      dst[i++] = *p++;
    }
  }
  dst[i] = '\0';
}

uint8_t w_from_bits(uint8_t bits) {
  // bits = 2,3,4
  // bits ecrits = 7 - w  =>  w = 7 - bits
  return (uint8_t)(7 - bits);
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

  tempsPrecedent = millis();

}

void loop() {


    if (millis() - tempsPrecedent >= 1000UL) {

      if (RTC.actualiser()) {

      uint8_t h = RTC.heure();
      uint8_t m = RTC.minute();
      uint8_t s = RTC.seconde();
      uint8_t heureDizaine = dizaine(h);
      uint8_t heureUnite = unite(h);
      uint8_t minuteDizaine = dizaine(m);
      uint8_t minuteUnite = unite(m);
      uint8_t secondeDizaine = dizaine(s);
      uint8_t secondeUnite = unite(s);

      binaire(heureDizaine,  w_from_bits(2), tabHeuresDizaine);
      binaire(heureUnite,    w_from_bits(4), tabHeuresUnite);
      binaire(minuteDizaine, w_from_bits(3), tabMinutesDizaine);
      binaire(minuteUnite,   w_from_bits(4), tabMinutesUnite);
      binaire(secondeDizaine,w_from_bits(3), tabSecondesDizaine);
      binaire(secondeUnite,  w_from_bits(4), tabSecondesUnite);


      concatener(tabConcatenation, sizeof(tabConcatenation),
                 tabHeuresDizaine, tabHeuresUnite,
                 tabMinutesDizaine, tabMinutesUnite,
                 tabSecondesDizaine, tabSecondesUnite);

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
      
      //tempsPrecedent = millis();
      tempsPrecedent += 1000; 

    }
  }
}
