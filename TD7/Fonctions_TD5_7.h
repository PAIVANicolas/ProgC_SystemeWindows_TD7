#pragma once
#include <windows.h>
#include <stdio.h>

// ======================================================================= 
//   Biblioth�que des fonctions : Fiche TD 5.7
//		
// ----------------------------------------------------------------------- 
//     Ecrit le								par :
// ----------------------------------------------------------------------- 
//     Modifi� le							par :
// ========================================================== data C ===== 
// ----------------------------------------------------------------------- 
//     Include                      
// ----------------------------------------------------------------------- 
// ----------------------------------------------------------------------- 
//     define                      
// ----------------------------------------------------------------------- 
// ----------------------------------------------------------------------- 
//     Structure                       
// ----------------------------------------------------------------------- 
struct ST_Compte {
	char NomClient[32];
	int  Solde;
	int	 NbreOperations;
};

// ----------------------------------------------------------------------- 
//     Prototye des Fonctions 
// ----------------------------------------------------------------------- 
void	AfficherCompte(struct ST_Compte*);
FILE* OpenFichier(char*, char*);
void	SaisieCompte(struct ST_Compte*);

int		AjoutCompte(char*, struct ST_Compte*, int);
int		LectureCompte(char*, struct ST_Compte*, int, int);
int		LireFichierComptes(char*, struct ST_Compte*, int);
int		ModifieCompte(char*, struct ST_Compte*, int, int);
// ---- Le dernier int dans ces fonctions corresponde
//      � une valeur vrai ou faux qui conditionne l'affichage des messages

// ======================================================================= 
//     Fonction Afficher Compte
// ======================================================================= 
void AfficherCompte(struct ST_Compte* p_Compte)
{
	printf("\n ----------------------------------------------------------\n");
	printf("\n\tCLIENT : %s  -  SOLDE =  %d avec %d Operations \n", p_Compte->NomClient, p_Compte->Solde, p_Compte->NbreOperations);
	printf("\n ----------------------------------------------------------\n");
}

// ======================================================================= 
//     Fonction AjoutCompte 
//								retourne vrai si l'ajout s'est effectu�               
// ======================================================================= 
int AjoutCompte(char* NomFichier, struct ST_Compte* p_Compte, int bMessage)
{
	FILE* pFichier;
	bool bOK = true;
	int NbRetry = 0;

	while (true) {
		if (bMessage) printf("\nAjout d'un enregistrement au fichier ");

		// ---- Open du Fichier en mode a
		pFichier = OpenFichier(NomFichier, (char*)"a");
		if (pFichier == NULL) break;										// On sort en cas d'Erreur de l'open

		// ---- Ecriture de la ligne du compte dans le fichier
		if (fprintf(pFichier, "%s %d %d\n", p_Compte->NomClient, p_Compte->Solde, p_Compte->NbreOperations) == 0) {
			printf("\nErreur : Ecriture dans Fichier %s", NomFichier);
			bOK = false;
		}
		else {
			if (bMessage) printf("\nEcriture dans Fichier OK %s ", NomFichier);
		}
		// ---- Close du fichier
		fclose(pFichier);
		break;
	}

	return(bOK);
}

// ======================================================================= 
//     Fonction LireFichierComptes                
// ======================================================================= 
int LireFichierComptes(char* NomFichier, struct ST_Compte* p_TbCompte, int bMessage)
{
	FILE* pFichier;
	bool bOK = false;
	struct ST_Compte w_Compte;
	int NbEnreg = 0;

	while (true) {
		if (bMessage) printf("\nLecture du fichier des Comptes et mis en tableau");

		// ---- Open du Fichier en mode r
		pFichier = OpenFichier(NomFichier, (char*)"r");
		if (pFichier == NULL) break;									// On sort en cas d'Erreur de l'open

		// ---- Lecture du Fichier	ligne par ligne 	
		while (fscanf(pFichier, "%s %d %d", w_Compte.NomClient, &w_Compte.Solde, &w_Compte.NbreOperations) == 3) {
			strcpy(p_TbCompte->NomClient, w_Compte.NomClient);
			p_TbCompte->Solde = w_Compte.Solde;
			p_TbCompte->NbreOperations = w_Compte.NbreOperations;
			NbEnreg++;
			p_TbCompte++;
		}

		fclose(pFichier);
		break;
	}
	return(NbEnreg);
}
// ======================================================================= 
//     Fonction ModifieCompte dans le fichier 
// ======================================================================= 
int ModifieCompte(char* NomFichier, struct ST_Compte* p_Compte, int Index, int bMessage)
{
	FILE* pFichier;
	bool bOK = false;
	struct ST_Compte w_TbCompte[8];
	struct ST_Compte* wp_Compte;
	int NbEnreg = 0;

	while (true) {

		if (bMessage) printf("\nModification du Compte a l'index %d ", Index);
		// ---- On lit l'integralit� du fichier des comptes	
		//		et on r�cup�re les Infos des comptes dans un tableau w_TbCompte
		NbEnreg = LireFichierComptes(NomFichier, w_TbCompte, bMessage);

		// ---- on met a jour le compte w_TbCompte[Index] 
		//		avec le compte pass� en param�tre p_Compte
		strcpy(w_TbCompte[Index].NomClient, p_Compte->NomClient);
		w_TbCompte[Index].Solde = p_Compte->Solde;
		w_TbCompte[Index].NbreOperations = p_Compte->NbreOperations;

		// ---- On efface le contenu du fichier avec un open en mode w 
		pFichier = OpenFichier(NomFichier, (char*)"w");
		if (pFichier == NULL) break;							// On sort en cas d'Erreur de l'open
		fclose(pFichier);

		// ---- On ecrit tous les enreg du tableau w_TbCompte dans le fichier
		wp_Compte = w_TbCompte;
		for (int w_i = 0; w_i < NbEnreg; w_i++) {
			AjoutCompte(NomFichier, wp_Compte, bMessage);
			wp_Compte++;
		}

		bOK = true;
		//		printf("\n-------------------------\n ");
		break;
	}
	return(bOK);
}
// ======================================================================= 
//     Fonction LectureCompte dans le fichier du compte situ� a l'index               
// ======================================================================= 
int LectureCompte(char* NomFichier, struct ST_Compte* p_Compte, int Index, int bMessage)
{
	bool bOK = false;
	struct ST_Compte w_TbCompte[8];
	int NbEnreg = 0;

	while (true) {
		if (bMessage) printf("\nLecture du Compte a l'index %d", Index);
		// ---- On met en tableau tous les comptes du fichier
		NbEnreg = LireFichierComptes(NomFichier, w_TbCompte, bMessage);

		// ---- on met a jour le compte p_Compte 
		strcpy(p_Compte->NomClient, w_TbCompte[Index].NomClient);
		p_Compte->Solde = w_TbCompte[Index].Solde;
		p_Compte->NbreOperations = w_TbCompte[Index].NbreOperations;

		bOK = true;

		break;
	}
	return(bOK);
}
// ======================================================================= 
//     Fonction OpenFichier				Retourne l'ID du fichier ouvert
// ======================================================================= 
FILE* OpenFichier(char* NomFichier, char* Mode)
{
	FILE* pFichier;
	int NbRetry = 0;

	// ---- Open du Fichier en mode Mode
	while (true) {
		pFichier = fopen(NomFichier, Mode);			// ouverture du fichier
		// on boucle si l'open ne s'est pas bien pass� car le fopen n accepte pas le partage de fichier
		// donc on attend si il est occup�
		if (pFichier != NULL) break;						// On sort de l'open OK

		printf("\nErreur a l'open - retry en cours %s", NomFichier);
		if (NbRetry == 5) break; 							// On sort de l'open apres 5 echecs
	}
	return pFichier;
}
// ======================================================================= 
//     Fonction SaisieCompte                
// ======================================================================= 
void SaisieCompte(struct ST_Compte* p_Compte)
{
	printf("\nSaisie d'un Nom de client ");
	scanf("%s", p_Compte->NomClient);
	printf("\nSaisie du Solde ");
	scanf("%d", &p_Compte->Solde);
	printf("\nSaisie du Nbre Operations ");
	scanf("%d", &p_Compte->NbreOperations);
	printf("\n-------------------------\n ");
}
