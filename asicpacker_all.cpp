#include "TFile.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TString.h"
#include "TTree.h"
#include "TEntryList.h"
#include "Riostream.h"
#include "TCut.h"
#include "TH1.h"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
#include <utility>
#include <sstream>
#include <cmath>
#include <signal.h>

#include "AIDAraw.h"

using namespace std;

int main (int argv, char* argc[]){

	const int maxmulti=16;
	TString fin;
	char fi[128];
	if(argv==3){
		int mainrunnum = atoi(argc[1]);
		int subrunnum = atoi(argc[2]);

		sprintf(fi,"$AIDADIR/R%4d_%d.root",mainrunnum,subrunnum);
		fin=Form("$AIDADIR/R%4d_%d.root",mainrunnum,subrunnum);
	}else if (argv==2){
		int mainrunnum = atoi(argc[1]);	
		sprintf(fi,"$AIDADIR/R%4d.root",mainrunnum);	
		fin=Form("$AIDADIR/R%4d.root",mainrunnum);
	}else {std::cout<<"USAGE: ./hitbuild AIDARUN# AIDASUBRUN# or ./hitbuild AIDARUN#"<<endl; return -1;}

	//reading raw file
	AIDAraw rawaida(fi);

	int nl = fin.Length();
	fin.Remove(nl-5);
	fin+="_packed_all.root";
	//if(TFile::Open(fin)==1)
	std::cout<<"File output : "<<fin<<std::endl;

	TFile* fo = new TFile(fin,"RECREATE");

	int gap_time = 200;
	int max_window = 3700;				// 37us window (If all ch's in ASIC fired, it will took 32us);
	int asicNo,asicNo0,multinum,adcSum;
	int ion_flag;

	int xtemp, ytemp, zpre;
	int i=0,j=0,ii,i0,nextfirst,inext;
	//int start_entry;
	Long64_t opening_time,t0,extT0,tprev,nlist;
	Long64_t timestamp[maxmulti],extTimestamp[maxmulti];
	int stripNo[maxmulti],rangeType[maxmulti],adcData[maxmulti],dssdNo[maxmulti],feeNo[maxmulti],chNo[maxmulti];
	double cal0[6][256],cal1[6][256],thresh[6][256],c0,c1,tempEX,tempEY;
	int dssd_index, strip_index;
	int time0;	
	time_t tm;
	TTree* tree = new TTree("tree","tree");
				
	tree->Branch("t0",&t0,"t0/L");
	tree->Branch("extT0",&extT0,"extT0/L");
	tree->Branch("ion_flag",&ion_flag,"ion_flag/I");
	tree->Branch("multinum",&multinum,"multinum/I");
	tree->Branch("adcSum",&adcSum,"adcSum/I");
	tree->Branch("asicNo",&asicNo0,"asicNo/I");

	tree->Branch("timestamp",timestamp,"timestamp[multinum]/L");
	tree->Branch("extTimestamp",extTimestamp,"extTimestamp[multinum]/L");
	tree->Branch("stripNo",stripNo,"stripNo[multinum]/I");
	tree->Branch("feeNo",feeNo,"feeNo[multinum]/I");
	tree->Branch("chNo",chNo,"chNo[multinum]/I");
	tree->Branch("rangeType",rangeType,"rangeType[multinum]/I");
	tree->Branch("adcData",adcData,"adcData[multinum]/I");
	tree->Branch("dssdNo",dssdNo,"dssdNo[multinum]/I");

	std::cout<<"Initialization Done"<<std::endl;


	Long64_t tscorr;
	Long64_t toffset;
	Bool_t flag_first;

	TEntryList* myl = new TEntryList();
	
	nlist=rawaida.SetADCEntryList(myl);
	
	std::cout<<"rawtree adc all list is read, nlist = "<<nlist<<std::endl;

	map<Long64_t, Long64_t> evtmap;
	map<Long64_t, Long64_t>::iterator ievtmap;
	toffset= rawaida.GetToffset();							//extTimestamp : 25MHz ,timestamp : 100MHz , toffset in global unit

	i=0;
	i0=0;
	time0=time(&tm);
	while(i<nlist && inext<nlist && i0<nlist){
		if(i0%100000==0) {std::cout<<"\rProgress "<<fixed<<setprecision(4)<<100.0*i0/nlist<<" \% done. Time spent : "<<time(&tm)-time0<<"s"; std::cout.flush();}
		rawaida.GetListEntry(myl,i);
/*		flag_first=false;
        if(rawaida.rangeType==0)
            {
         	if(((rawaida.dssdNo==0 && !(rawaida.stripNo==126 ||rawaida.stripNo==127 ||rawaida.stripNo==191 ||rawaida.stripNo==192 ||rawaida.stripNo==254 ||rawaida.stripNo==255)) ||
               (rawaida.dssdNo==1 && !(rawaida.stripNo==3 ||rawaida.stripNo==4 ||rawaida.stripNo==5 ||rawaida.stripNo==127 ||rawaida.stripNo==199 ||rawaida.stripNo==255)) ||
               (rawaida.dssdNo==2 && !(rawaida.stripNo==0 ||rawaida.stripNo==1 ||rawaida.stripNo==126 ||rawaida.stripNo==127 ||rawaida.stripNo==191 ||rawaida.stripNo==192 ||rawaida.stripNo==255)) ||
               (rawaida.dssdNo==3 && !(rawaida.stripNo==0 ||rawaida.stripNo==1 ||rawaida.stripNo==126 ||rawaida.stripNo==127 ||rawaida.stripNo==191 ||rawaida.stripNo==192 ||rawaida.stripNo==255)) ||
               (rawaida.dssdNo==4 && !(rawaida.stripNo==0 ||rawaida.stripNo==1 ||rawaida.stripNo==2 || rawaida.stripNo==125 ||rawaida.stripNo==126 ||rawaida.stripNo==127 ||rawaida.stripNo==191 ||rawaida.stripNo==192 ||rawaida.stripNo==255)) ||
               (rawaida.dssdNo==5 && !((rawaida.stripNo>-1 &&rawaida.stripNo<6) || (rawaida.stripNo>124 && rawaida.stripNo<128) || (rawaida.stripNo>189 || rawaida.stripNo<194) ||rawaida.stripNo==255)))
               )
            {
           	flag_first=true;
            }
      	}else if (rawaida.rangeType==1)
      	{
         	if((rawaida.dssdNo==1 && !((rawaida.stripNo>21 && rawaida.stripNo<26)||rawaida.stripNo==92 ||(rawaida.stripNo>110 && rawaida.stripNo<116) || (rawaida.stripNo>118 && rawaida.stripNo<122))) ||
            	(rawaida.dssdNo==2 && !(rawaida.stripNo==136)) ||
            	(rawaida.dssdNo==4 && !(rawaida.stripNo==193)) ||
            	(rawaida.dssdNo==5 && !(rawaida.stripNo==135 || rawaida.stripNo==136 || rawaida.stripNo==138 || rawaida.stripNo==140 || rawaida.stripNo==143 || rawaida.stripNo==144))
         		)
         	{
         	flag_first=true;
         	}

      	}
      	if(flag_first)
      	{*/
			t0=rawaida.timestamp;
			extT0=rawaida.extTimestamp;
			asicNo0 = (int)(rawaida.feeNo-1)*4 + (int)std::floor(rawaida.chNo/16.);
			i0=i;
			tprev=rawaida.timestamp;

			multinum=0;
			adcSum=0;
			ion_flag=0;
			inext=0;
			nextfirst=0;
			while(rawaida.timestamp < t0+max_window && i < nlist){
			
				asicNo = (int)(rawaida.feeNo-1)*4 + (int)std::floor(rawaida.chNo/16.);
				ievtmap=evtmap.find(i);
				if(asicNo==asicNo0 && rawaida.timestamp-tprev <=gap_time){
					multinum++;
					timestamp[multinum-1]=rawaida.timestamp;
					extTimestamp[multinum-1]=rawaida.extTimestamp;
					stripNo[multinum-1]=rawaida.stripNo;
					rangeType[multinum-1]=rawaida.rangeType;
					adcData[multinum-1]=rawaida.adcData;
					dssdNo[multinum-1]=rawaida.dssdNo;
					feeNo[multinum-1]=rawaida.feeNo;
					chNo[multinum-1]=rawaida.chNo;
					evtmap.insert(std::make_pair(i,1));
					if(rangeType[multinum-1]==1 || (adcData[multinum-1] > 17000 && rangeType[multinum-1]==0)){
//					if(rangeType[multinum-1]==1){
					ion_flag=1;
					}
					adcSum+=rawaida.adcData;
					tprev=rawaida.timestamp;
				}else if(nextfirst==0 && ievtmap==evtmap.end()){
					nextfirst=1;
					inext=i;
				}
				i++;
				rawaida.GetListEntry(myl,i);
			}

			tree->Fill();
			if(inext>0) i=inext;
//	}else { i++; }

	}
	std::cout<<std::endl;

//	fo->WriteTObject(tree);
	fo->WriteTObject(tree);
//	fo->WriteTObject(corrTS);
	fo->Close();
	std::cout<<std::endl;
	return 1;
}


