void CREATE_PARTITION (char *part, FILE *g)
{
	int nrpartitii, MAXF2, MAXD2;
	int MAXD, MAXF, poz, adresa, i, e, y, anterior;
	char nume[10];
	FILE *f;

	f = fopen ("mbr.bin", "r+");

	fread (& nrpartitii, sizeof (int), 1, f);

	i = 1;
	e = 0;

    fscanf (g, "%d%d", & MAXD2, & MAXF2);

	while (!e && i <= nrpartitii)
	{
		fseek (f, 4 + (i - 1) * 4, SEEK_SET);
		fread (& adresa, sizeof (int), 1, f); // indexul fiecarei partitii

        if (adresa == 0)
        {
            fseek (f, 4 + (i - 2) * 4, SEEK_SET); // mergem la partitia anterioara
            fread (& anterior, sizeof (int), 1, f);
            fseek (f, anterior + 12, SEEK_SET);

            // cate directoare si fisiere are partitia anterioara sa stim unde se termina
            fread (& MAXD, sizeof (int), 1, f);
			fread (& MAXF, sizeof (int), 1, f);

			poz = ((MAXF + 1) * MAXD + MAXF) * 12 + anterior + 20;
			fseek (f, poz, SEEK_SET); // inceputul urmatoarei partitii goale
			fseek (f, 12, SEEK_CUR);
			// citim cate fisiere si directoare are partitia stearsa pt a vedea daca avem loc
			fread (& MAXD, sizeof (int), 1, f);
			fread (& MAXF, sizeof (int), 1, f);

			if ((MAXF + 1) * MAXD + MAXF >= (MAXF2 + 1) * MAXD2 + MAXF2)
			{
                fseek (f, poz, SEEK_SET);

                y = strlen (part);
                fwrite (& y, 4, 1, f); // scriem cat de mare e numele
                fwrite (part, y, 1, f); // scriem numele
                fseek (f, 8 - y, SEEK_CUR);

                fwrite (& MAXD2, 4, 1, f); // scriem nr directoarelor
                fwrite (& MAXF2, 4, 1, f); // scriem nr fisierelor

                // modificam indexul partitiilor
                fseek (f, 4 + (i - 1) * 4, SEEK_SET);
                fwrite (& poz, 4, 1, f);
               // fseek (f, -4, SEEK_CUR);
                //fread(& y, 4, 1, f);
                printf("%d\n", poz);
                e = 1;
			}
        }

        i++;
    }

    if (!e) printf("INVALID\n");
    fclose (f);
}

void PARTITION_OP (char *part, int x, FILE *g)
{
	int nrpartitii, MAXF2, MAXD2;
	int MAXD, MAXF, poz, adresa, i, e, y, anterior;
	char nume[10];
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

            if (strstr (part, nume) && x == 2 && adresa != 0) 	// stergere
            {
                fseek (f, 4 + (i - 1) * 4, SEEK_SET);
                y = 0;
                fwrite (& y, 4, 1, f);

                e = 1;
            }

            if (strstr (part, nume) && x == 3 && adresa != 0) 	// format
            {
                fseek (f, adresa + 12, SEEK_SET);
                fread (& MAXD, sizeof (int), 1, f);
                fread (& MAXF, sizeof (int), 1, f);

                y = 0;
                fwrite(& y, 4, (MAXF + 1) * MAXD * 3, f);

                e = 1;
            }

		i++;
	}

	fclose (f);

	if (e == 1) printf("SUCCES\n");
		else if (e == 0)
            printf("INVALID\n");
}

void TASK3 ()
{
	char s1[20], s2[30];
	FILE *g;

	g = fopen ("mbr.op", "r");

	while (fscanf (g, "%s%s", s1, s2) != EOF)
        {

            if (strcmp (s1, "CREATE_PARTITION") == 0)
                CREATE_PARTITION(s2, g);
            if (strcmp (s1, "DELETE_PARTITION") == 0)
                PARTITION_OP (s2, 2, g);
            if (strcmp (s1, "FORMAT_PARTITION") == 0)
                PARTITION_OP (s2, 3, g);
        }


	fclose (g);
}
