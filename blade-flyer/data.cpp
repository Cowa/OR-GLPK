typedef struct {
	int nblieux;
	int capacite;
	int *demande; // Demande de chaque lieu (la case 0 est inutilisée car le dépôt n'a aucune demande à voir satisfaire)
	int **C; // distancier (les lignes et colonnes 0 correspondent au dépôt)
} donnees;

void lecture_data(char *file, donnees *p)
{
	int i,j;
	FILE *fin;

	int val;
	fin = fopen(file,"rt");

	/* Lecture du nombre de villes */

	fscanf(fin,"%d",&val);
	p->nblieux = val;

	/* Allocation mémoire pour la demande de chaque ville, et le distancier */

	p->demande = (int *) malloc (val * sizeof(int));
	p->C = (int **) malloc (val * sizeof(int *));
	for(i = 0;i < val;i++) p->C[i] = (int *) malloc (val * sizeof(int));

	/* Lecture de la capacité */

	fscanf(fin,"%d",&val);
	p->capacite = val;

	/* Lecture des demandes des clients */

	for(i = 1;i < p->nblieux;i++)
	{
		fscanf(fin,"%d",&val);
		p->demande[i] = val;
	}

	/* Lecture du distancier */

	for(i = 0; i < p->nblieux; i++)
		for(j = 0; j < p->nblieux; j++)
		{
			fscanf(fin,"%d",&val);
			p->C[i][j] = val;
		}

	fclose(fin);
}

/* Fonction de libération mémoire des données */

void free_data(donnees *p)
{
	int i;
	for(i = 0;i < p->nblieux;i++) free(p->C[i]);
	free(p->C);
	free(p->demande);
}
