#include <papi.h>
#include <iostream>

using namespace std;

#define SYSTEMTIME clock_t

void print_matrix(double *m, int dim, int n) {
    [&] {
        for (int i = 0; i < dim; i++) {
            for (int j = 0; j < dim; j++, n--) {
                cout << m[i * dim + j] << " ";
                if (n <= 1) return;
            }
            cout << endl;
        }
    }();
    cout << endl;
}

void print_matrix(double *m, int dim) { print_matrix(m, dim, dim * dim); }

void OnMult(int m_ar, int m_br) {
    double *pha, *phb, *phc;

    pha = (double *)malloc((m_ar * m_ar) * sizeof(double));
    phb = (double *)malloc((m_ar * m_ar) * sizeof(double));
    phc = (double *)malloc((m_ar * m_ar) * sizeof(double));

    for (int i = 0; i < m_ar; i++)
        for (int j = 0; j < m_ar; j++) pha[i * m_ar + j] = 1;

    for (int i = 0; i < m_br; i++)
        for (int j = 0; j < m_br; j++) phb[i * m_br + j] = (double)(i + 1);

    SYSTEMTIME Time1, Time2;
    Time1 = clock();

    for (int i = 0; i < m_ar; i++) {
        for (int j = 0; j < m_br; j++) {
            double temp = 0;
            for (int k = 0; k < m_ar; k++) {
                temp += pha[i * m_ar + k] * phb[k * m_br + j];
            }
            phc[i * m_ar + j] = temp;
        }
    }

    Time2 = clock();
    fprintf(stdout, "Time: %3.3f seconds\n",
            (double)(Time2 - Time1) / CLOCKS_PER_SEC);

    cout << "Result matrix: " << endl;
    print_matrix(phc, m_ar, 10);

    free(pha);
    free(phb);
    free(phc);
}

void OnMultLine(int m_ar, int m_br) {
    double *pha, *phb, *phc;
    pha = (double *)malloc((m_ar * m_ar) * sizeof(double));
    phb = (double *)malloc((m_ar * m_ar) * sizeof(double));
    phc = (double *)malloc((m_ar * m_ar) * sizeof(double));

    for (int i = 0; i < m_ar; i++)
        for (int j = 0; j < m_ar; j++) pha[i * m_ar + j] = (double)1.0;

    for (int i = 0; i < m_br; i++)
        for (int j = 0; j < m_br; j++) phb[i * m_br + j] = (double)(i + 1);

    for (int i = 0; i < m_ar * m_ar; i++) phc[i] = 0;

    SYSTEMTIME Time1, Time2;
    Time1 = clock();

    for (int i = 0; i < m_ar; i++) {
        for (int j = 0; j < m_br; j++) {
            for (int k = 0; k < m_ar; k++) {
                phc[i * m_ar + k] += pha[i * m_ar + j] * phb[j * m_br + k];
            }
        }
    }

    Time2 = clock();
    fprintf(stdout, "Time: %3.3f seconds\n",
            (double)(Time2 - Time1) / CLOCKS_PER_SEC);

    cout << "Result matrix: " << endl;
    print_matrix(phc, m_ar, 10);

    free(pha);
    free(phb);
    free(phc);
}

void OnMultBlock(int m_ar, int m_br, int blk) {
    double *pha, *phb, *phc;
    pha = (double *)malloc((m_ar * m_ar) * sizeof(double));
    phb = (double *)malloc((m_ar * m_ar) * sizeof(double));
    phc = (double *)malloc((m_ar * m_ar) * sizeof(double));

    for (int i = 0; i < m_ar; i++)
        for (int j = 0; j < m_ar; j++) pha[i * m_ar + j] = (double)1.0;

    for (int i = 0; i < m_br; i++)
        for (int j = 0; j < m_br; j++) phb[i * m_br + j] = (double)(i + 1);

    for (int i = 0; i < m_ar * m_ar; i++) phc[i] = 0;

    SYSTEMTIME Time1, Time2;
    Time1 = clock();

    for (int i = 0; i < m_ar / blk; i++) {
        for (int j = 0; j < m_br / blk; j++) {
            for (int k = 0; k < m_ar / blk; k++) {
                for (int line = 0; line < blk; line++) {
                    for (int col = 0; col < blk; col++) {
                        for (int kk = 0; kk < blk; kk++) {
                            int iLine = i * blk + line;
                            int iCol = j * blk + col;
                            int ik = k * blk + kk;
                            phc[iLine * m_ar + ik] += pha[iLine * m_ar + iCol] * phb[iCol * m_ar + ik];
                        }
                    }
                }
            }
        }
    }

    Time2 = clock();
    fprintf(stdout, "Time: %3.3f seconds\n",
            (double)(Time2 - Time1) / CLOCKS_PER_SEC);

    cout << "Result matrix: " << endl;
    print_matrix(phc, m_ar, 10);

    free(pha);
    free(phb);
    free(phc);
}

int main(int argc, char *argv[]) {
    int lin, col, blockSize;

    int EventSet = PAPI_NULL;
    long long values[2];

    int ret = PAPI_library_init(PAPI_VER_CURRENT);
    if (ret != PAPI_VER_CURRENT) std::cerr << "FAIL" << endl;

    ret = PAPI_create_eventset(&EventSet);
    if (ret != PAPI_OK) cerr << "ERROR: create eventset" << endl;

    ret = PAPI_add_event(EventSet, PAPI_L1_DCM);
    if (ret != PAPI_OK) cerr << "ERROR: PAPI_L1_DCM" << endl;

    ret = PAPI_add_event(EventSet, PAPI_L2_DCM);
    if (ret != PAPI_OK) cerr << "ERROR: PAPI_L2_DCM" << endl;

    int op;
    do {
        cout << endl << "1. Multiplication" << endl;
        cout << "2. Line Multiplication" << endl;
        cout << "3. Block Multiplication" << endl;
        cout << "Selection?: ";
        cin >> op;
        if (op == 0) break;
        printf("Dimensions: lins=cols ? ");
        cin >> lin;
        col = lin;

        // Start counting
        ret = PAPI_start(EventSet);
        if (ret != PAPI_OK) cout << "ERROR: Start PAPI" << endl;

        switch (op) {
            case 1:
                OnMult(lin, col);
                break;
            case 2:
                OnMultLine(lin, col);
                break;
            case 3:
                cout << "Block Size? ";
                cin >> blockSize;
                OnMultBlock(lin, col, blockSize);
                break;
        }

        ret = PAPI_stop(EventSet, values);
        if (ret != PAPI_OK) cout << "ERROR: Stop PAPI" << endl;
        printf("L1 DCM: %lld \n", values[0]);
        printf("L2 DCM: %lld \n", values[1]);

        ret = PAPI_reset(EventSet);
        if (ret != PAPI_OK) std::cerr << "FAIL reset" << endl;

    } while (op != 0);

    ret = PAPI_remove_event(EventSet, PAPI_L1_DCM);
    if (ret != PAPI_OK) std::cerr << "FAIL remove event" << endl;

    ret = PAPI_remove_event(EventSet, PAPI_L2_DCM);
    if (ret != PAPI_OK) std::cerr << "FAIL remove event" << endl;

    ret = PAPI_destroy_eventset(&EventSet);
    if (ret != PAPI_OK) std::cerr << "FAIL destroy" << endl;
}