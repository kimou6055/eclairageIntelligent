#include <string.h>

#define tentatives 4;
#define messageErreur "999999999";
#define id "103";
int compteurAux = 0;
int AccuseReception = 0;
char *requete;
int condition; // condition de vérification du voltix
typedef struct Meteo
{
  int temperature;
  int humidite;
  int luminosite;
  int brouillard;

} Meteo;

typedef struct Lampe
{
  int luminosite;
  int capteurMouvement;
  int capteurBruit;

} Lampe;

int etatActivationCapteurs=0;
Meteo meteo;
Lampe lampe;
void setup()
{
}

void loop()
{
  interrupt(etatActivationCapteurs);
  
  do
  {
    requete = recepteionRequete();
    compteurAux++;
  } while (!verifSyntax(requete) || compteurAux == tentatives);
  compteurAux = 0;
  switch (verifTypeRqt(requete))
  {
  case (1): // retour
  {
    if (isMessageErreur(requete))
    {
      envoiePere(requete);
      break;
    }
    if (iAmVoltix(condition))
    {
      char *DonneSupplementaire = CollecteDonneSupplementaire(meteo);
      // DonneSupplementaire codée sur 6 bits
      strcat(requete, DonneSupplementaire);
      // envoyer vers la constante pere
    }
    do
    {
      AccuseReception = envoiePere(requete);
      compteurAux++;

    } while (!AccuseReception || compteurAux == tentatives);

    if (compteurAux == tentatives)
      envoiePere(id + messageErreur);
    compteurAux = 0;
    break;
  }
  case (2): // envoie
  {
    if (!verifATransmettre(requete))

      decouperEtEnvoyer(requete);

    supprimerEnTete(requete);

    if (verifFinRequete(requete))
      break;

    TransmettreRequete(requete);
    break;
  }
  case (3): // ordre
  {
    if (verifOrdreAllumage(requete))

    {
      lampe.luminosite = allumageLampe(requete);
      char *retourDonnes = collecteDonnesLampe(lampe, retourCapteurs());  //il manque un control saisie

      do
      {
        AccuseReception = envoiePere(retourDonnes);
        compteurAux++;

      } while (!AccuseReception || compteurAux == tentatives);

      if (compteurAux == tentatives)
        envoiePere(id + messageErreur);
      compteurAux = 0;
    }
    if (verifOrdreCapteurs(requete))

    {
        etatActivationCapteurs=activationCapteurs(requete);
      
    }

    break;
  }
  }
}
