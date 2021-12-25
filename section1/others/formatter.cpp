#include <iostream>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <string>
#include <vector>
#include <limits>
#include <cstdlib>
#include <algorithm>
#include <bits/stdc++.h>
#include <sstream>

double getMean(std::string riga){
    unsigned int pos;
    if(riga.length()!=0){
        for(unsigned int i=0;i < 2;i++){
            //cancella nella stringa riga il primo campo appena visualizzato, compreso il ;
            riga = riga.erase(0, riga.find(",",0)+1);
        }
    }
    pos = riga.find(',', 0);
    riga = riga.erase(pos);
    return std::stod(riga);
}

unsigned int max_index(double arr[], unsigned int n)
{
    int i, count=0;
    double max = arr[0];

    for (i = 1; i < n; i++)
        if (arr[i] > max){
            max = arr[i];
            count++;
        }

    return count;
}

int main(int argc, char* argv[]) {

  unsigned int it = 0;
  unsigned int max_i;
  char* filename;
  double mean_v[48];
  std::string s;
  std::string data[48];

  if (argc > 1) {
    filename = argv[1];
  } else {
    std::cout <<"Insert filename: ./exe filename" <<std::endl;
    return -1;
  }

  std::ifstream fin(filename);
  std::ofstream fout;
  fout.open("result.csv", std::ios_base::app);

  if(filename[2] == '1' && filename[3] == '.' )
    fout << "rank,Np,mean,mean_permsg,var,s2,N,dev,s"<<std::endl;
  if(!fin){
    std::cout <<"Error while opening file "<< filename <<std::endl;
    return -1;
  }

  getline(fin, s); //legge la prima riga del file CSV

  while(!fin.eof()){
      mean_v[it] = getMean(s);
      data[it] = s;
      getline(fin, s); //legge la riga successiva del file CSV
      it++;
  }

  max_i = max_index(mean_v, it);
  fout << data[max_i] << '\n';

  fin.close();
  fout.close();   

  return 0;
}