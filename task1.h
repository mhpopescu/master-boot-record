void T1_AFISARE(FILE *f) // functia de afisare
{
	int MAXF, MAXD, mxf, nrpartitii, adresa, bytes_ocupati = 0, sizenume_director, sizenume_fisier, i;
	char nume[8];

	fread (& nrpartitii, sizeof(int), 1, f);

	i = 1;
	while (i <= nrpartitii)
	{
		bytes_ocupati = 0;
		
		fseek (f, 4 + (i - 1) * 4, SEEK_SET);
		fread (& adresa, sizeof(int), 1, f);

		fseek (f, adresa + 4, SEEK_SET);
		fread (nume, 8, 1, f);

		printf("%s ", nume);

		fread (& MAXD, sizeof(int), 1, f);
		fread (& MAXF, sizeof(int), 1, f);

		printf("%d ", 12 * (MAXF * MAXD + MAXF + MAXD) );

		while (MAXD > 0) // parcurge toate directoarele
		{
			fread (& sizenume_director, sizeof(int), 1, f);

			if (sizenume_director > 0)
			{
				bytes_ocupati += 12;
				fseek (f, 8, SEEK_CUR);

				mxf = MAXF;
				while (mxf > 0)
				{
					fread (& sizenume_fisier, sizeof(int), 1, f);
					if (sizenume_fisier > 0) bytes_ocupati += 12;

					fseek (f, 8, SEEK_CUR);
					mxf --;
				}
			}

			MAXD --;
		}

		mxf = MAXF;
		while (mxf > 0) // parcurge toate fisierele
		{
			fread (& sizenume_fisier, sizeof(int), 1, f);
			if (sizenume_fisier > 0) bytes_ocupati += 12;

			fseek (f, 8, SEEK_CUR);
			mxf --;
		}

		printf("%d\n", bytes_ocupati);

		i++;
	}
}


void TASK1 () // aceasta functie este pentru validare
{
	int nrpartitii, adresa1, adresa, inainte, i, e = 1, MAXF, MAXD, bytes;
	FILE *f;

	f = fopen ("mbr.bin", "r+");

	fread (& nrpartitii, sizeof(int), 1, f);

	fread (& adresa1, sizeof(int), 1, f);

	if (adresa1 != (nrpartitii + 1) * sizeof (int))		e = 0; // verificarea pentru prima partitie

	i = nrpartitii;
	inainte = adresa1;

    fseek (f, adresa1, SEEK_SET);

	while (i > 1)
	{
		fseek (f, 12, SEEK_CUR);
		fread (& MAXD, sizeof(int), 1, f);
		fread (& MAXF, sizeof(int), 1, f);

		fseek (f, adresa1 - (i - 1) * 4, SEEK_SET);
		fread (& adresa, sizeof(int), 1, f);

		bytes = (MAXF + 1) * MAXD * 12 + MAXF * 12;

		if (adresa != inainte + bytes + 20) 	e = 0;

		inainte += bytes + 20;
		fseek (f, inainte, SEEK_SET);
		i --;
	}

	if (!e) printf("INVALID\n");
	else 
		{
			printf("SUCCES\n");
			fseek (f, 0, SEEK_SET);
			T1_AFISARE (f); // functia de afisare
		}

	fclose (f);
}
