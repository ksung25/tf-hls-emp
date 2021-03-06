#include <vector>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cassert>
#include <iostream>

using namespace std;


#define BUFFER_LEN 1000

/*
 * header:
 * [63:60]: "1001"
 * [59:56]: memory bank
 * [55:49]: nentries page 0
 * [48:42]: nentries page 1
 * [41:35]: nentries page 2
 * [34:28]: nentries page 3
 * [27:21]: nentries page 4
 * [20:14]: nentries page 5
 * [13: 7]: nentries page 6
 * [ 6: 0]: nentries page 7
 *
 * data:
 * [63:60]: "0000" = subword 0, "0001" = subword 1 (if needed)
 * [59:48]: memory address
 * [47: 0]: data
 */

//
// helper function to split data fields in a line
//
vector<string> split(const string& s, char delimiter)
{
  vector<string> tokens;
  string token;
  istringstream ss(s);

  while(getline(ss,token,delimiter)) {
    tokens.push_back(token);
  }

  return tokens;
}


int main(int argc, char ** argv) {

  //
  // Parse config file
  // Assumed format:
  //   <output file name>
  //   <number of events>
  //   <memory module print file 1>
  //   <memory module print file 2>
  //   ...
  //   <memory module print file n>
  //
  ifstream cfgfile(argv[1]);
  assert(cfgfile.is_open());
  string cfgline;
  getline(cfgfile, cfgline);
  string outfilename = cfgline;
  getline(cfgfile, cfgline);
  const uint32_t NEVENTS = atoi(cfgline.c_str());
  vector<ifstream*> datafiles;
  while(getline(cfgfile, cfgline)) {
    datafiles.push_back(new ifstream(cfgline.c_str()));
    assert(datafiles.back()->is_open());
  }
  const uint32_t NMODULES = datafiles.size();
  cfgfile.close();


  //
  // main processing code
  //

  ofstream* memfile = new ofstream(outfilename.c_str());
  
  // base header word template for each memory module
  // - memory bank number written now, nentries content updated later
  //   while processing data from input files
  vector<uint32_t> htmp[4];
  for(uint32_t ifile=0; ifile<NMODULES; ifile++) {
    uint32_t membank = ( (ifile+1) << 8 );
    htmp[0].push_back(0x0);
    htmp[1].push_back(0x0);
    htmp[2].push_back(0x0);
    htmp[3].push_back(0x39000 | membank);
  }
  
  // cache cout defaults
  ios_base::fmtflags oldFlags = cout.flags();
  streamsize         oldPrec  = cout.precision();
  char               oldFill  = cout.fill();

  // fill between prefix and number with 0s
  cout << std::internal << std::setfill('0');


  string line;
  vector<uint32_t> rxdata0;
  for(uint32_t ifile=0; ifile<NMODULES; ifile++) {

    ifstream *datafile = datafiles.at(ifile);

    uint32_t ievt=0;
    uint32_t ihead = rxdata0.size();
    bool eof=false;

    rxdata0.push_back(htmp[0][ifile]);
    rxdata0.push_back(htmp[1][ifile]);
    rxdata0.push_back(htmp[2][ifile]);
    rxdata0.push_back(htmp[3][ifile]);

    uint32_t ndata=0;
    while(ievt<=NEVENTS && !eof) {
      if(getline(*datafile, line)) {
        if(line.find("Event") != string::npos) {
          // update header word with number of data
          if(ievt==1) {
            rxdata0[ihead+3] = (rxdata0[ihead+3] | (ndata << 1));
          } else if(ievt==2) {
            rxdata0[ihead+3] = (rxdata0[ihead+3] | (ndata >> 6));
            rxdata0[ihead+2] = (rxdata0[ihead+2] | (ndata << 10));
          }

          ievt++;
          ndata=0;

        } else {
          // Parse the line in the data file
          uint64_t indata = strtoul(split(line,' ').back().c_str(),NULL,0);
          uint64_t addr = (ievt-1)*128 + ndata;
          rxdata0.push_back(indata & 0xFFFF);
          rxdata0.push_back((indata >> 16) & 0xFFFF);
          rxdata0.push_back(indata >> 32);
          rxdata0.push_back(0x30000 | addr);
          ndata++;
        }
      } else {
        eof = true;
        // update header word with number of data
        if(ievt==1) {
          rxdata0[ihead+3] = (rxdata0[ihead+3] | (ndata << 1));
        } else if(ievt==2) {
          rxdata0[ihead+3] = (rxdata0[ihead+3] | (ndata >> 6));
          rxdata0[ihead+2] = (rxdata0[ihead+2] | (ndata << 10));
        }
      }
    }
  }

  for(uint32_t i=0; i<rxdata0.size()/4; i++) {
    cout << std::hex << std::setw(4) << rxdata0[4*i+3];
    cout << " "      << std::setw(4) << rxdata0[4*i+2];
    cout << " "      << std::setw(4) << rxdata0[4*i+1];
    cout << " "      << std::setw(4) << rxdata0[4*i];
    cout << endl;

    *memfile << std::hex << rxdata0[4*i]   << std::endl;
    *memfile << std::hex << rxdata0[4*i+1] << std::endl;
    *memfile << std::hex << rxdata0[4*i+2] << std::endl;
    *memfile << std::hex << rxdata0[4*i+3] << std::endl;
  }

  memfile->close();

  // restore cout defaults
  cout.flags(oldFlags);
  cout.precision(oldPrec);
  cout.fill(oldFill);
}
