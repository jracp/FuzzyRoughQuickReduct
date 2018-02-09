// Programmed by Javad Rahimipour Anaraki on 22/08/16 updated on 25/01/18
// Ph.D. Candidate
// Department of Computer Science
// Memorial University of Newfoundland
// jra066 [AT] mun [DOT] ca | www.cs.mun.ca/~jra066

// This is an implementation of Delta Quick Reduct algorithm
// Input: dataset name in the same path
// Output: is a subset of selected features
// More info: https://link.springer.com/chapter/10.1007/978-3-662-53611-7_1

#include <iostream>
#include <stdio.h>
#include <cmath>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <vector>
#include <time.h>

using namespace std;
double dependency(int r, int c, int nCls, int cls[], double v[], vector<vector<double> >& data, int maxF[], int lF);
int linear_search(int arr[], int n, int val);

int main(int argc, char* argv[])
{
    clock_t t1,t2;
    t1=clock();
    int r = 0, c = 0, pos = 0, i = 0, j = 0, pStart = 0, pEnd = 0;
    string value, elmnt, temp;
    string datasetName = argv[1];
    string path = datasetName;
    ifstream dataset (path);
    ifstream tData (path);
    
    //Calcultating number of rows and columns
    getline (dataset, value);
    pos = value.find(",");
    while(pos != -1)
    {
        value.replace(pos, 1, "0");
        pos = value.find(",");
        c++;
    }
    c++;
    
    while(dataset.good()) {
        getline (dataset, value);
        r++;
    }
    
    //Converting dataset into matrix
    vector<double> tempData(c);
    vector< vector<double> > data(r, tempData);
    
    for (i=0; i<r; i++) {
        getline (tData, elmnt);
        pStart = 0;
        pEnd = elmnt.find(",");
        j = 0;
        
        while(pEnd != -1) {
            temp = elmnt.substr(pStart, pEnd - pStart);
            data[i][j] = atof(temp.c_str());
            elmnt.replace(pEnd, 1, "_");
            pStart = pEnd + 1;
            pEnd = elmnt.find(",");
            j++;
        }
        temp = elmnt.substr(pStart, 1);
        data[i][j] = atof(temp.c_str());
    }
    
    //Finding number of classes in dataset
    int cls[c];
    double clsTmp;
    int nCls = 1;
    j = 0;
    clsTmp = data[0][c-1];
    cls[j] = 0;
    
    for (i=1; i<r; i++) {
        if (clsTmp != data[i][c-1]) {
            j++;
            cls[j] = i;
            clsTmp = data[i][c-1];
            nCls++;
        }
    }
    
    for (j=0; j<nCls-1; j++) {cls[j] = cls[j+1];}
    cls[nCls-1] = r;
    
    //Calculating variance of feature
    double sum = 0.0;
    double mean = 0.0, v[c-1];
    
    for (j=0; j<c-1 ;j++) {
        sum = 0.0;
        for (i=0; i<r; i++) {sum += data[i][j];}
        mean = sum / r;
        v[j] = 0.0;
        for (i=0; i<r; i++) {v[j] = v[j] + pow((data[i][j] - mean), 2.0);}
        v[j] = pow(v[j]/(r - 1), 0.5);
    }
    
    int selF[c], allF[c];
    double maxDD = 0.0, cDD = 0.0, preDD = 1.0;
    int cnt = 0, maxF = 0;
    for (int l=0; l<c-1; l++) {allF[l] = l;}
    
    while((maxDD != 1) && (cnt != c-1) && (preDD != maxDD)) {
        for (int f=0; f<c-1; f++) {
            if(linear_search(selF, cnt+1, f) > 0) {continue;}
            selF[cnt] = f;
            cDD = dependency(r, c, nCls, cls, v, data, selF, cnt+1);
            if (cDD > maxDD) {
                preDD = maxDD;
                maxDD = cDD;
                maxF = f;
            }
        }
        selF[cnt] = maxF;
        cnt++;
    }
    
    t2 = clock();
    cout << "[ ";
    for (int it=0;it<cnt;it++) {cout << selF[it] + 1 << " ";}
    cout << "]" << endl <<"DD = " << maxDD << endl;
    cout << "Processing time = " << ((float)t2-(float)t1) / CLOCKS_PER_SEC << " s" << endl;
    
    return 0;
}

double dependency(int r, int c, int nCls, int cls[], double v[], vector<vector<double> >& data, int maxF[], int lF)
{
    //Calculating dependency degree
    int mF = 1, h = 0, k = 0, s = 0, i = 0, j = 0;
    int lMoD = nCls;
    double fterm1 = 0.0, fterm2 = 0.0, moRp = 0.0, out = 0.0;
    double tmp[c], moRa[lF], supMat[r];
    double moX[r][2 * lMoD];
    
    for (int a=0; a<r; a++) {
        for (int b=0; b<(2*lMoD); b++) {moX[a][b] = 0.0;}
    }
    
    for (nCls=0; nCls<lMoD; nCls++) {
        for (s; s<cls[nCls]; s++) {moX[s][nCls] = 1.0;}
        s = cls[nCls];
    }
    
    for (int x1=0; x1<r; x1++) {
        for (int x2=0; x2<r; x2++) {
            for (h=0; h<lF; h++) {
                mF = maxF[h];
                fterm1 = (data[x2][mF] - (data[x1][mF] - v[mF])) / (data[x1][mF] - (data[x1][mF] - v[mF]));
                fterm2 = ((data[x1][mF] + v[mF]) - data[x2][mF]) / (data[x1][mF] + v[mF] - data[x1][mF]);
                moRa[h] = max(min(fterm1, fterm2), 0.0);
            }
            
            if (lF > 1) {
                moRp = max(moRa[0] + moRa[1] - 1.0, 0.0);
                for(int nMoRmF=2; nMoRmF<lF; nMoRmF++) {
                    moRp = max(moRp + moRa[nMoRmF] - 1.0, 0.0);
                }
            } else {
                moRp = moRa[0];
            }
            
            for (int j=lMoD; j<(2*lMoD); j++) {
                moX[x2][j] = min(1- moRp + moX[x2][j-lMoD], 1.0);
            }
        }
        
        k = 0;
        
        for (j=lMoD; j<(2*lMoD); j++) {
            tmp[k] = moX[0][j];
            
            for (i=1; i<r; i++) {
                tmp[k] = min(moX[i][j], tmp[k]);
            }
            k++;
        }
        
        supMat[x1] = tmp[0];
        
        for (k=1; k<lMoD; k++) {
            supMat[x1] = max(tmp[k], supMat[x1]);
        }
    }
    
    for (k=0; k<r; k++) {
        out += supMat[k];
        supMat[k] = 0.0;
    }
    
    out = out / r;
    return out;
}

int linear_search(int arr[], int n, int val)
{
    for(int i = 0; i < n; i++) {if (arr[i] == val) {return i;}}
    return -1;
}
