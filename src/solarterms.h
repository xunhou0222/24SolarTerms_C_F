// APIs that can be found in "SOFA" package, or libsofa_c_20210512.a equivalently.
int iauCal2jd(int iy, int im, int id, double *djm0, double *djm);

double iauObl06(double date1, double date2);

void iauAb(double pnat[3], double v[3], double s, double bm1, double ppr[3]);

void iauPnm06a(double date1, double date2, double rbpn[3][3]);

int iauJd2cal(double dj1, double dj2, int *iy, int *im, int *id, double *fd);

int iauUtctai(double utc1, double utc2, double *tai1, double *tai2);

int iauTaitt(double tai1, double tai2, double *tt1, double *tt2);


// Self-defined APIs.
void utc2tdb(double JD, double *TT);

void sun2earth(int set, double AU, double clight, double JD, double pos[3], double vol[3]);

int termscheck(int *k, double JD, double lambda, double date[24], double angle[24]);

void termsprinter(double date[24], double angle[24]);

void JD2CST(double JD, int CST[6]);


// APIs that can be found in "testeph", or in "libtesteph.a" more precisely.
void split_(double *TT, double FR[2]);

void const_(char *NAM, double *VAL, double *SSS, int *N);

void pleph_(double *ET, int *NTARG, int *NCENT, double *RRD );