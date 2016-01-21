#include "api.hpp"
#include "aes.hpp"
#include <cstdio>

void print_16char_as_hex(const uint8_t in[16]) {
  for (int i = 0; i < 16; i++) {
    printf("%02x", in[i]);
  }
  printf("\n");
}

int main()
{
  uint8_t clef[16];
  uint8_t plain[16];
  uint8_t cipher[16];

  
  API api("data/default.api");
  api.init();
  printf("----login(\"admin\") ----\n");
  api.login("admin");
  api.newKey(clef);
  printf("Veuillez entrer la donnée à crypter :\n");
  scanf("%16s",plain);
  printf("----chiffrement en cours ----\n");
  print_16char_as_hex(plain);
  api.encryptData(plain,cipher,clef);
  printf("----chiffrement terminé ----\n");
  print_16char_as_hex(cipher);

  /* 
 *** TP : PREMIERE PARTIE ***
    Dans la première partie du TP on ne se préoccupe pas de la sécurité locale. Cela implique
      1. que les données dans le fichier ne sont pas protégées,
      2. que les utilisateurs n'ont pas de mot de passe.

    L'objectif est donc d'implanter des fonctions permettant d'utiliser cette API. Afin de simplifier les choses, l'utilisation d'une primitive cryptographique n'est pas obligatoire : dans un premier temps on se contentera d'afficher à l'écran les calculs qui auraient du être effectués.

    Par exemple, la fonction de chiffrement de données pourrait ressembler à :
    error_t encryptData( key, data )
    {
      printf("Chiffrement de la donnée %s avec la clef %s\n",data,key);
    }

    Le but de ce TP est de se préoccuper de la politique de sécurité de notre API.

    Il faudra réaliser les fonctions suivantes.

      1. login() qui permet de se connecter
      2. createAdmin() qui permet de créer un compte administrateur avec un login donné
      3. addUSer() qui permet de créer un compte utilisateur avec un login donné
      4. newKey() qui permet de créer une nouvelle clef
      5. encryptData() qui permet de chiffrer des données
      6. wrapKey() qui permet de sortir une clef de la mémoire (en la protégeant bien sûr).

      Notez que vous avez le droit de rajouter des membres aux classes ou d'autres fonctions. 
      Attention à bien effectuer les tests nécesssaires !

      Enfin, les vecteurs d'attributs ont été créés pour pouvoir automatiser l'export dans un fichier afin de ne pas perdre de temps avec cela. Maintenant, si cette façon de représenter les attributs ne vous convient pas il est possible de la modifier mais il faudra alors modifier les fonctions dump() et parse() en conséquence.

 *** TP : SECONDE PARTIE ***

      1. Le code fourni ainsi que les fonctions codées par vos soins ne respectent pas toutes les bonnes pratiques présentées en cours. Dans un premier temps il est question pour vous de faire les modifications nécessaires au code pour qu'il s'approche le plus possible des préconisations listées dans le cours (tests, retours, effacements ...).

      2. Le second point de cette partie (si le temps le permet) consiste à utiliser la primitive de chiffrement disponible afin de commencer à mettre en place un petit peu de sécurité locale. Étant données la durée du TP, on essaiera juste de protéger en confidentialité les clefs des utilisateurs en chiffrant/déchiffrant leur valeur à l'aide d'une clef dérivée, au moins, du mot de passe utilisateur.

  */
  
  return 0;
}
