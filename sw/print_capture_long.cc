#include <vector>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

int main(int argc, char ** argv) {

  string infilename = argv[1];
  
  // Assume input file has name *.log. Replace extention to .txt
  string outfilename = infilename.substr(0,infilename.length()-3) + "txt";
  
  cout << endl;
  cout << "  <>  Processing file: " << infilename << endl;
  cout << endl;

  ifstream ifs(infilename.c_str());
  string line("");
  
  // divider line in log file after which buffer contents follow
  string divider("---------------------");
  while(line.compare(divider)!=0) {
    getline(ifs, line);
  }

  // KS !!!  read 4 lines because TrackletParameters print out starts with 
  //         word with all zeroes
  for(int i=0; i<4; i++) {
    getline(ifs, line);
  }

  vector<uint32_t> ipbwords;
  uint32_t testword[8] = {0,0,0,0,0,0,0,0};
  int idx=0;
  while(getline(ifs, line)) {
    stringstream ss(line);
    string tmp1, tmp2, tmp3;
    ss >> tmp1 >> tmp2 >> tmp3 >> hex >> testword[idx];
    
    if(idx==7 && testword[idx] >= 0x30000) {
      ipbwords.push_back(testword[7]);
      ipbwords.push_back(testword[6]);
      ipbwords.push_back(testword[5]);
      ipbwords.push_back(testword[4]);
      ipbwords.push_back(testword[3]);
      ipbwords.push_back(testword[2]);
      ipbwords.push_back(testword[1]);
      ipbwords.push_back(testword[0]);
    }

    idx = (idx + 1) % 8;
  }

  cout << internal << setfill('0');
  cout << hex;

  ofstream ofs(outfilename.c_str());
  ofs << internal << setfill('0');
  ofs << hex;
  ofs << "  <>  Log file: " << infilename << endl;
  ofs << endl;

  for(unsigned int i=0; i<ipbwords.size(); i+=8) {
//    cout << setw(4) << (ipbwords[i] & 0xFFFF) << " ";
//    cout << setw(4) << ipbwords[i+1] << " ";
    cout << setw(4) << ipbwords[i+2] << " ";
    cout << setw(4) << ipbwords[i+3] << " ";
    cout << setw(4) << (ipbwords[i+4] & 0xFFFF) << " ";
    cout << setw(4) << ipbwords[i+5] << " ";
    cout << setw(4) << ipbwords[i+6] << " ";
    cout << setw(4) << ipbwords[i+7];
    cout << endl;

//    ofs << setw(4) << (ipbwords[i] & 0xFFFF) << " ";
//    ofs << setw(4) << ipbwords[i+1] << " ";
    ofs << setw(4) << ipbwords[i+2] << " ";
    ofs << setw(4) << ipbwords[i+3] << " ";
    ofs << setw(4) << (ipbwords[i+4] & 0xFFFF) << " ";
    ofs << setw(4) << ipbwords[i+5] << " ";
    ofs << setw(4) << ipbwords[i+6] << " ";
    ofs << setw(4) << ipbwords[i+7];
    ofs << endl;
  }
  ofs.close();

  cout << endl;
  cout << "  <> Output file: " << outfilename << endl;
  cout << endl;

  return 0;
}
