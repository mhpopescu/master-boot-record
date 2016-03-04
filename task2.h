int FILE_OP_FUNCTIONS (int MAXF, int x, char *file, FILE *f, FILE *g)
{
    int mxf, sizenume_fisier, j, y, poz;
    char nume_fisier[10], new_name[10];

    mxf = MAXF;

    while (mxf > 0) // cand gaseste un spatiu liber sau fisierul
    {
        poz = ftell (f);

        fread (& sizenume_fisier, sizeof (int), 1, f);
        fread (nume_fisier, 1, sizenume_fisier, f);
        nume_fisier[sizenume_fisier] = '\0';

        fseek (f, poz, SEEK_SET);

        if (x == 1 && sizenume_fisier == 0) // creare
        {
            y = strlen (file);

            fwrite (&y, sizeof (int), 1, f);
            fwrite (file, 8, 1, f);

            return 1;
        }

        if (x == 2 && strcmp (nume_fisier, file) == 0) // redenumire
        {
            fscanf (g, "%s", new_name);
            y = strlen (new_name);

            fwrite (& y, sizeof (int), 1, f);
            fwrite (new_name, 8, 1, f);

            return 1;
        }

        if (x == 3 && sizenume_fisier == 0) // stergere
        {
            y = 0;
            for (j = 1; j <= 3; j++)
                fwrite (&y, sizeof (int), 1, f);

            return 1;
        }

        mxf --;
        fseek (f, 12, SEEK_CUR);
    }

    return 0;
}

void FILE_OP (char *path, int x, FILE *g)
{
	int y, poz, nrpartitii, sizenume_director, sizenume_fisier, MAXD, MAXF, mxf, adresa, i, e, j;
	char nume[10], *file, new_name[10], nume_fisier[10], nume_director[10], *dir, aux[20];
	FILE *f;

	f = fopen ("mbr.bin", "r+");

	fread (& nrpartitii, sizeof (int), 1, f);

	i = 1;
	e = 0;

	while (!e && i <= nrpartitii)
	{
		fseek (f, 4 + (i - 1) * 4, SEEK_SET);
		fread (& adresa, sizeof (int), 1, f);

		fseek (f, adresa + 4, SEEK_SET);
		fread (nume, 8, 1, f);
		nume[8] = '\0';

		if (strstr (path, nume)) 	// partitia in care va fi salvat redenumit sau sters fisierul
		{
			strcpy (path, path + 9); // in path ramane doar calea din interiorul partitiei
			strcpy (aux, path);

			fread (& MAXD, sizeof (int), 1, f);
            fread (& MAXF, sizeof (int), 1, f);

            // daca strchr (path + 1, '/') returneaza NULL inseamna ca fisierul nu se afla intr-un director
			if (strchr (path + 1, '/')) // daca se va face o operatie intr-un director
			{
                file = strchr (path + 1, '/');
                strcpy (file, file + 1); // numele fisierului

                dir = strtok (aux, "/"); // numele directorului in care va fi prelucrat fisierul

                while (MAXD > 0 && !e) // parcurge toate directoarele
                {
                    fread (& sizenume_director, sizeof (int), 1, f);
                    fread (nume_director, 1, sizenume_director, f);
                    nume_director[sizenume_director] = '\0';

                    fseek (f, 8 - sizenume_director, SEEK_CUR);

                    if (strcmp (nume_director, dir) == 0) // directorul in care se va face o operatie asupra unui fisier
                        e = FILE_OP_FUNCTIONS (MAXF, x, file, f, g); // se executa operatia

                    MAXD --;
                }
            }
                else  // daca se va face o operatie in fisierele unei partitii
                {
                    fseek (f, (MAXF + 1) * 12, SEEK_CUR); // sare peste toare directoarele
                    file = path;
                    e = FILE_OP_FUNCTIONS (MAXF, x, file, f, g);
                }
		}

		i++;
	}

	fclose (f);

	if (e) printf("SUCCES\n");
		else printf("INVALID\n");
}


void DIR_OP (char *path, int x, FILE *g)
{
	int nrpartitii, sizenume_director, sizenume_fisier;
	int MAXD, MAXF, mxf, poz, adresa, i, e, y;
	char nume[10], new_name[10], nume_fisier[10], nume_director[10], j;
	FILE *f;

	f = fopen ("mbr.bin", "r+");

	fread (& nrpartitii, sizeof (int), 1, f);

	i = 1;
	e = 0;

	while (!e && i <= nrpartitii)
	{
		fseek (f, 4 + (i - 1) * 4, SEEK_SET);
		fread (& adresa, sizeof (int), 1, f);

		fseek (f, adresa + 4, SEEK_SET);
		fread (nume, 8, 1, f);
		nume [8] = '\0';

		if (strstr (path, nume)) 	// partitia in care se va prelucra
		{
			strcpy (path, path + 9);  // in path va ramane numele directorului

			fread (& MAXD, sizeof (int), 1, f);
			fread (& MAXF, sizeof (int), 1, f);

			while (MAXD > 0 && !e) // parcurge toate directoarele
			{
                poz = ftell (f);

				fread (& sizenume_director, sizeof (int), 1, f);
				fread (nume_director, 1, sizenume_director, f);

                nume_director[sizenume_director] = '\0';

                //fseek (f, 8 - sizenume_director, SEEK_CUR);
                fseek (f, poz, SEEK_SET);

				if (x == 1 && sizenume_director == 0) // creare
				{
					e = 1;

					y = strlen (path);

					fwrite (& y, sizeof (int), 1, f);
					fwrite (path, 1, y, f);
				}

				if (x == 2 && strcmp (nume_director, path) == 0) // redenumire
				{
					e = 1;

					fscanf (g, "%s", new_name);

					y = strlen (new_name);

					fwrite (& y, sizeof (int), 1, f);
					fwrite (new_name, 1, y, f);
				}

				if (x == 3 && strcmp (nume_director, path) == 0) // stergere
				{
					e = 1;

					y = 0;

					for (j = 1; j <= (MAXF + 1) * 3; j++)
                        fwrite (& y, 4, 1, f);
				}

				// daca operatia nu corespunde acestui director trece la urmatorul,
				// deci trebuie sa treaca si peste fisierele fiecarui director
				if (!e) fseek (f, MAXF * 12, SEEK_CUR); // asa trece peste fisiere

				MAXD --;
			}
		}

		i++;
	}

	fclose (f);

	if (e) printf("SUCCES\n");
		else printf("INVALID\n");
}


void PRINT (char *path)
{
	int nrpartitii, sizenume_director, sizenume_fisier, MAXD, MAXF, mxf, adresa, i, e;
	char nume[10], nume_fisier[10], nume_director[10];
	FILE *f;

	f = fopen ("mbr.bin", "r+");

	fread (& nrpartitii, sizeof (int), 1, f);

	i = 1;
	e = 0;

	while (!e && i <= nrpartitii)
	{
		fseek (f, 4 + (i - 1) * 4, SEEK_SET);
		fread (& adresa, sizeof (int), 1, f);

		fseek (f, adresa + 4, SEEK_SET);
		fread (nume, 8, 1, f);
		nume [8] = '\0';

		if (strstr (path, nume)) 	// partitia in care se afla directorul
		{
			strcpy (path, path + 9); // numele directorului

			fread (& MAXD, sizeof (int), 1, f);
			fread (& MAXF, sizeof (int), 1, f);

			while (MAXD > 0 && !e) // parcurge toate directoarele
			{
				fread (& sizenume_director, sizeof (int), 1, f);
				fread (nume_director, 1, sizenume_director, f);
				nume_director[sizenume_director] = '\0';

                fseek (f, 8 - sizenume_director, SEEK_CUR);

				if (strcmp (nume_director, path) == 0) // directorul cel bun
				{
					mxf = MAXF;
					e = 1;

					while (mxf > 0) // cat timp gaseste un fisier
					{
						fread (& sizenume_fisier, sizeof (int), 1, f);
						fread (nume_fisier, 1, sizenume_fisier, f);
						nume_fisier[sizenume_fisier] = '\0';

						fseek (f, 8 - sizenume_fisier, SEEK_CUR);

                        if (sizenume_fisier != 0)
                            printf("%s\n", nume_fisier);

						mxf --;
					}
				}

				MAXD --;
			}
		}

		i++;
	}

	fclose (f);

	if (!e) printf("INVALID\n");
}


void TASK2 ()
{
	char s1[15], s2[30];
	FILE *g;

	g = fopen ("mbr.op", "r");

	while (fscanf (g, "%s%s", s1, s2) != EOF)
        if (strlen (s2) == 8) printf ("INVALID\n");
            else
        {

            if (strcmp (s1, "CREATE_FILE") == 0)
                FILE_OP (s2, 1, g);
            if (strcmp (s1, "RENAME_FILE") == 0)
                FILE_OP (s2, 2, g);
            if (strcmp (s1, "DELETE_FILE") == 0)
                FILE_OP (s2, 3, g);

            if (strcmp (s1, "CREATE_DIR") == 0)
                DIR_OP (s2, 1, g);
            if (strcmp (s1, "RENAME_DIR") == 0)
                DIR_OP (s2, 2, g);
            if (strcmp (s1, "DELETE_DIR") == 0)
                DIR_OP (s2, 3, g);

            if (strcmp (s1, "PRINT") == 0)
                PRINT (s2);
        }


	fclose (g);
}
