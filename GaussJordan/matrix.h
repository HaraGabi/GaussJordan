#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


typedef struct matrix{
	double *matrixt;
	int sorok_szama;
	int oszlopok_szama;
	int aktualis_meret;
}matrix;

void meretezo(matrix *m, int meret)
{
	m->matrixt = (double*)malloc(meret * sizeof(double));
	m->aktualis_meret = meret;
}

void atmeretezo(matrix *m, int ujmeret)
{
	double *ptr = (double*)realloc(m->matrixt, ujmeret * sizeof(double));
	if (!ptr)
	{
		printf("Vegzetes hiba történt!\nA program kilep.\n");
		exit(-2);
	}
	else
	{
		m->matrixt = ptr;
		m->aktualis_meret = ujmeret;
	}
}

void meret_vizsgalat(int beolvasott_szamok, matrix *m)
{
	int uj_meret;
	if (beolvasott_szamok >= m->aktualis_meret)
	{
		uj_meret = beolvasott_szamok * 10;
		atmeretezo(m, uj_meret);
	}
}

void beolvaso_ismeretlenre(matrix*m, FILE *f)
{
	int jelenlegi_karakter;
	int elozo_karakter;
	char szamjegyek[20];
	int index = 0;
	int segedindex = 0;
	int beolvasott_szamok = 0;
	int in_file = 1;

	while (in_file)
	{
		jelenlegi_karakter = fgetc(f);

		meret_vizsgalat(beolvasott_szamok, m);

		if (isspace(jelenlegi_karakter) && !isspace(elozo_karakter))
		{
			if (m->sorok_szama <= 1)
			{
				m->oszlopok_szama++;
			}
			m->matrixt[index] = atof(szamjegyek);
			beolvasott_szamok++;
			segedindex = 0;
			index++;
		}
		else
		{
			szamjegyek[segedindex] = jelenlegi_karakter;
			segedindex++;
			szamjegyek[segedindex] = '\0';
		}

		if (jelenlegi_karakter == '\n')
		{
			while (!feof(f))
			{
				if (EOF != fscanf(f, "%lf", &m->matrixt[index]))
				{
					beolvasott_szamok++;
				}
				index++;
				meret_vizsgalat(beolvasott_szamok, m);
			}
			m->sorok_szama = (beolvasott_szamok) / m->oszlopok_szama;
			break;

		}

		if (jelenlegi_karakter == EOF)
		{
			if (!isspace(elozo_karakter))
			{
				if (m->sorok_szama <= 1)
				{
					m->oszlopok_szama++;
				}
				m->matrixt[index] = atof(szamjegyek);
				beolvasott_szamok++;
				segedindex = 0;
				index++;
			}
			in_file = 0;
		}
		elozo_karakter = jelenlegi_karakter;
	}
}