// ======================================================================= 
//   Programme : Etude de Cas Mis a jour Compte bancaire
//   Version 1 : Execution de deux opérations en LINEAIRE
//   Version 2 : Execution de deux opérations en PARALLELE
//				 3 threads exécutés en //  ( main  + 2 threads )
//	une fonction Operation avec 1 parametre est exécuté par chaque thread 
// ----------------------------------------------------------------------- 
//     Ecrit le								par :
// ----------------------------------------------------------------------- 
//     Modifié le							par :
// ========================================================== data C ===== 
// ----------------------------------------------------------------------- 
//     Include                      
// ----------------------------------------------------------------------- 
#include <windows.h>
#include <stdio.h>
#include <stdbool.h>
#include "Fonctions_TD5_7.h"


// ----------------------------------------------------------------------- 
//     define                      
// ----------------------------------------------------------------------- 
// ---- Commandes pour le Préprocessor
//#define PROGRAMMATION_LINEAIRE 
#define PROGRAMMATION_PARALLELE
//#define DEKKER = "DEKKER"
//#define SEMAPHORE = "SEMAPHORE"

// ----------------------------------------------------------------------- 
//     Structure                       
// ----------------------------------------------------------------------- 

// ---- Structure pour passer les paramètres à la fonction Operation
struct ST_Operation {
	int MontantOperation;
	int IndexClient;
	int bMessage;
};


// ----------------------------------------------------------------------- 
//     Espace de données Global accessible par tous les threads                    
// ----------------------------------------------------------------------- 
char NomFichierBanque[64];

#ifdef DEKKER

#endif

#ifdef SEMAPHORE

#endif

// ----------------------------------------------------------------------- 
//     Prototye des Fonctions 
// ----------------------------------------------------------------------- 
void Operation(struct ST_Operation*);

// ======================================================================= 
//     Main                      
// ======================================================================= 
int main()
{

	// ---- Declarations des variables locales	
	DWORD ThreadId1, ThreadId2;									// necessaire pour la creation d'un thread
	HANDLE hdl_thread0 = 0;
	HANDLE hdl_thread1 = 0;
	struct ST_Operation Operation1, Operation2;
	struct ST_Compte MonCompte;

	int IndexClientA;
	int bMessage = false;

	// ---- Initialisations 
#ifdef DEKKER
	printf("\n ---------------------------- Version DEKKER ---------------------------- \n");

#endif

#ifdef SEMAPHORE
	printf("\n ---------------------------- Version SEMAPHORE\n");

#endif

	// ----------------------------------------------------------------------- 
	//     Traitement Principal : Lancement des Operations à réaliser
	// ----------------------------------------------------------------------- 
	while (true) {
		// ----    Initialisation du fichier                     
		strcpy(NomFichierBanque, "C:\\Users\\n.paiva\\source\\repos\\TD7\\ProgC_SystemeWindows_TD7\\TD7\\Banque.txt");

		// ---- Lecture du Compte du Client 0 sur lequel on va opérer
		IndexClientA = 0;
		LectureCompte(NomFichierBanque, &MonCompte, IndexClientA, bMessage);
		AfficherCompte(&MonCompte);

		// ---- Initialisation des 2 Opérations a effectuer 
		Operation1.MontantOperation = 1000;			// on credite de 1000
		Operation1.IndexClient = IndexClientA;
		Operation1.bMessage = bMessage;

		Operation2.MontantOperation = -2000;		// on debite de 2000
		Operation2.IndexClient = IndexClientA;
		Operation2.bMessage = bMessage;

#ifdef PROGRAMMATION_LINEAIRE
		// ----------------------------------------------------------------------- 
		//     On appelle les deux operations a effectuer linéairement                       
		// ----------------------------------------------------------------------- 
		printf("---------------- VERSION PROGRAMMATION LINEAIRE --------------------------------\n");
		Operation(&Operation1);

		Operation(&Operation2);
#endif

#ifdef PROGRAMMATION_PARALLELE

		// ----------------------------------------------------------------------- 
		//     Démarrage des opérations en Thread                       
		// ----------------------------------------------------------------------- 
		printf("---------------- VERSION PROGRAMMATION PARALLELE ------------------------------\n");
		// ---- Creation du Thread 1
		hdl_thread0 = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Operation, &Operation1, NULL, &ThreadId1);
		if (hdl_thread0 == NULL)
		{
			printf(" le thread 0 n'a pu etre lance\n");
			break;
		}
		// ---- Creation du Thread 2
		hdl_thread1 = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Operation, &Operation2, NULL, &ThreadId2);
		if (hdl_thread1 == NULL)
		{
			printf(" le thread 1 n'a pu etre lance\n");
			break;
		}

		// ----------------------------------------------------------------------- 
		//     Attente de la Fin des activités                       
		// ----------------------------------------------------------------------- 
		WaitForSingleObject(hdl_thread0, INFINITE);
		WaitForSingleObject(hdl_thread1, INFINITE);

		printf("\n\n\tLes 2 Threads ont termines leur execution\n\n");

#endif

		// ---- Lecture du Compte du Client 0 
		LectureCompte(NomFichierBanque, &MonCompte, IndexClientA, bMessage);
		AfficherCompte(&MonCompte);
		break;
	}

	// ---- Fin du Processus maître
	printf("\n\n\n\n");

	getchar();

	ExitProcess(0);

	return 0;
}


// ================================================================================== 
//     Fonction Operation (Crediter : Montant Positif  ou Debiter : Montant Négatif)               
// ================================================================================== 
void Operation(struct ST_Operation* p_Param)
{
	struct ST_Compte w_Compte;
	int w_Montant = 0;
	char LibOperation[20];
	char Position[20];

#ifdef DEKKER							//    Initialisation DEKKER

#endif

#ifdef SEMAPHORE						//    Initialisation SEMAPHORES

#endif

	// ******************************************************************************************
	// ----- Reperer la Section Critique
	// ----- Encadrer la section Critique de son Prologue et Epilogue pour chaque Version
	// ******************************************************************************************
	while (true) {
		// ---- Mis a jour du libelle de l'opération lancée
		w_Montant = p_Param->MontantOperation;
		if (w_Montant < 0) {
			strcpy(Position, "\t\t\t\t\t\t");				// pour afficher les debits sur la droite de l'ecran
			strcpy(LibOperation, "Debit");
			printf("\n%sOperation de %s (%d) lancee pour le client %d - \n", Position, LibOperation, w_Montant, p_Param->IndexClient);
		}
		else {
			strcpy(Position, "");
			strcpy(LibOperation, "Credit");
			printf("\n%sOperation de %s (%d) lancee pour le client %d - \n", Position, LibOperation, w_Montant, p_Param->IndexClient);
		}

		// ---- Lecture des infos actuelles du Compte du Client 
		if (!(LectureCompte(NomFichierBanque, &w_Compte, p_Param->IndexClient, p_Param->bMessage))) break;

		// ---- Traitement de l'Opération
		printf("\n%sSolde Depart = %d ----- Calcul en cours...\n", Position, w_Compte.Solde);
		w_Compte.Solde += w_Montant;
		w_Compte.NbreOperations++;
		printf("\n%sNouveau Solde = %d \n", Position, w_Compte.Solde);

		Sleep(2000);	// On genere une E/S en espérant de rendre actif l'autre thread 

		// ---- Ecriture dans le Fichier de Sortie des Infos du Compte Maj
		if (!(ModifieCompte(NomFichierBanque, &w_Compte, p_Param->IndexClient, p_Param->bMessage))) break;

		printf("\n%sOperation de %s terminee - \n", Position, LibOperation);
		break;
	}
}