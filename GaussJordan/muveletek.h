#pragma once

typedef struct pivot_tar{
	int *eredeti_oszlopok;
	int *cserelt_oszlopok;
	int meret;
}pivot_tar;

extern void kiiro(matrix *m);
extern void reszleges_pivot(matrix *vektor, matrix *m, int sor, int oszlop);

double maxkereso_sorban(matrix *m, int hanyadik)
{
	double max = m->matrixt[hanyadik * m->oszlopok_szama];
	for (int i = 0; i < m->oszlopok_szama; ++i)
	{
		if (fabs(m->matrixt[hanyadik * m->oszlopok_szama + i]) > fabs(max))
		{
			max = m->matrixt[hanyadik * m->oszlopok_szama + i];
		}
	}
	return fabs(max);
}

void sornormalo(matrix *m, matrix *n, int hanyadik)
{
	double max = maxkereso_sorban(m, hanyadik);
	if (!max) { return; }
	for (int k = 0; k < m->oszlopok_szama; ++k)
	{
		m->matrixt[hanyadik * m->oszlopok_szama + k] = fabs(m->matrixt[hanyadik * m->oszlopok_szama + k] / max);
		n->matrixt[hanyadik * n->oszlopok_szama + k] = fabs(n->matrixt[hanyadik * n->oszlopok_szama + k] / max);
	}
}

void normalo_soronkent(matrix *m, matrix *n)
{
	for (int i = 0; i < m->sorok_szama; ++i)
	{
		sornormalo(m, n, i);
	}
}

int maxkereso_oszlopban(matrix *m, int sor, int oszlop)
{
	int max = sor;
	for (int i = sor; i < m->sorok_szama; ++i)
	{
		if (fabs(m->matrixt[(i * m->oszlopok_szama) + oszlop]) > fabs(m->matrixt[(max * m->oszlopok_szama) + oszlop]))
		{
			max = i;
		}
	}
	return max;
}

int zerus_oszlop(matrix *m, int sor, int oszlop)
{
	int max = 0;
	for (int i = sor; i < m->sorok_szama; ++i)
	{
		if (fabs(m->matrixt[(i * m->oszlopok_szama) + oszlop]) > fabs(m->matrixt[(max * m->oszlopok_szama) + oszlop]))
		{
			max = i;
		}
	}
	return max;
}

void sorcserelo(matrix *m, int sor, int melyik_sorral)
{
	double tmp;
	for (int i = 0; i < m->oszlopok_szama; ++i)
	{
		tmp = m->matrixt[(sor * m->oszlopok_szama) + i];
		m->matrixt[(sor * m->oszlopok_szama) + i] = m->matrixt[(melyik_sorral * m->oszlopok_szama) + i];
		m->matrixt[(melyik_sorral * m->oszlopok_szama) + i] = tmp;
	}
}

int max_almatrixban(matrix *m, int sor, int oszlop, pivot_tar *tar)
{
	int max = (sor * m->oszlopok_szama) + oszlop;
	int max_sor = sor;
	int max_oszlop = oszlop;
	for (int i = sor; i < m->sorok_szama; ++i)
	{
		for (int j = oszlop; j < m->oszlopok_szama; j++)
		{
 			if (fabs(m->matrixt[(i * m->oszlopok_szama) + j]) > fabs(m->matrixt[max]))
			{
				max = (i * m->oszlopok_szama) + j;
				max_sor = i;
				max_oszlop = j;
			}
		}
	}
	if (max_oszlop != oszlop)
	{
		tar->meret++;
		tar->eredeti_oszlopok[tar->meret] = oszlop;
		tar->cserelt_oszlopok[tar->meret] = max_oszlop;
	}
	return max_sor;
}

void oszlopcserelo(matrix *m, int oszlop1, int oszlop2)
{
	double tmp;
	for (int i = 0; i < m->sorok_szama; i++)
	{
		tmp = m->matrixt[(i * m->oszlopok_szama) + oszlop1];
		m->matrixt[(i * m->oszlopok_szama) + oszlop1] = m->matrixt[(i * m->oszlopok_szama) + oszlop2];
		m->matrixt[(i * m->oszlopok_szama) + oszlop2] = tmp;
	}
}


void teljes_pivot(matrix *m, matrix *vektor, int sor, int oszlop, pivot_tar *tar)
{
	int eredeti = tar->meret;
	int uj_sor = max_almatrixban(m, sor, oszlop, tar);
	if (tar->meret != eredeti)
	{
		oszlopcserelo(m, tar->eredeti_oszlopok[tar->meret], tar->cserelt_oszlopok[tar->meret]);
	}
	if (uj_sor != sor)
	{
		sorcserelo(m, sor, uj_sor);
		sorcserelo(vektor, sor, uj_sor);
	}
}

void visszacserelo(matrix *m, matrix *n, pivot_tar *tar)
{
	for (int i = tar->meret; i > 0; i--)
	{
		oszlopcserelo(m, tar->cserelt_oszlopok[i], tar->eredeti_oszlopok[i]);
	}
}

void ellen_matrix(matrix *m, matrix *n)
{
	n->matrixt = (double*)calloc(m->aktualis_meret, sizeof(double));
	n->aktualis_meret = m->aktualis_meret;
	n->oszlopok_szama = m->oszlopok_szama;
	n->sorok_szama = m->sorok_szama;
	for (int i = 0; i < m->sorok_szama; ++i)
	{
		for (int j = 0; j < m->oszlopok_szama; ++j)
		{
			if (i == j)
			{
				n->matrixt[(i * m->oszlopok_szama) + j] = 1;
			}
		}
	}
}

void sor_szorzasa(matrix *m, matrix *n, int sor, double mivel)
{
	for (int i = 0; i < m->oszlopok_szama; ++i)
	{
		m->matrixt[(sor * m->oszlopok_szama) + i] *= mivel;
		n->matrixt[(sor * n->oszlopok_szama) + i] *= mivel;

	}
}

void also_haromszog(matrix *m, matrix *n, pivot_tar *tar)
{
	for (int i = 0; i < m->sorok_szama; ++i)
	{
		if (!zerus_oszlop(m, i, i)) { exit(-1); }
		teljes_pivot(m, n, i, i, tar);
		for (int j = i + 1; j < m->sorok_szama; ++j)
		{
			for (int k = 0; k < m->oszlopok_szama; ++k)
			{
				m->matrixt[(j * m->oszlopok_szama) + k] -= m->matrixt[(j * m->oszlopok_szama) + i] * m->matrixt[(i * m->oszlopok_szama) + k];
				n->matrixt[(j * n->oszlopok_szama) + k] -= n->matrixt[(j * n->oszlopok_szama) + i] * m->matrixt[(i * m->oszlopok_szama) + k];
			}
		}
	}
}

void felso_haromszog(matrix *m, matrix *n)
{
	for (int i = m->sorok_szama - 1; i >= 0; --i)
	{
		for (int j = 0; j < i; ++j)
		{
			for (int k = 0; k < m->oszlopok_szama; ++k)
			{
				m->matrixt[(j * m->oszlopok_szama) + k] -= m->matrixt[(i * m->oszlopok_szama) + k] * m->matrixt[(j * m->oszlopok_szama) + i];
				n->matrixt[(j * n->oszlopok_szama) + k] -= n->matrixt[(i * n->oszlopok_szama) + k] * n->matrixt[(j * n->oszlopok_szama) + i];
			}
		}
	}
}

void gauss_jordan(matrix *m, matrix *n, pivot_tar *tar)
{
	normalo_soronkent(m, n);
	also_haromszog(m, n, tar);
	felso_haromszog(m, n);

}