#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "matrix.h"
#include "muveletek.h"


void reszleges_pivot(matrix *vektor, matrix *m, int sor, int oszlop)
{
	int melyik_sorral = maxkereso_oszlopban(m, sor, oszlop);
	sorcserelo(m, sor, melyik_sorral);
	sorcserelo(vektor, sor, melyik_sorral);
}


void kiiro(matrix *m)
{
	for (int i = 0; i < 9; ++i)
	{
		printf("%f ", m->matrixt[i]);
	}
	printf("\n");
}

int main(int argc, char* argv[])
{
	matrix m, n;
	FILE *f = fopen(argv[1], "rt");
	if (!f) { exit(1); }
	meretezo(&m, 9);
	m.oszlopok_szama = 0;
	m.sorok_szama = 1;
	beolvaso_ismeretlenre(&m, f);
	kiiro(&m);
	pivot_tar tar;
	tar.cserelt_oszlopok = (int*)malloc(m.oszlopok_szama * sizeof(int));
	tar.eredeti_oszlopok = (int*)malloc(m.oszlopok_szama * sizeof(int));
	tar.meret = 0;
	ellen_matrix(&m, &n);
	gauss_jordan(&m, &n, &tar);
	kiiro(&m);
	kiiro(&n);
	return 0;
}