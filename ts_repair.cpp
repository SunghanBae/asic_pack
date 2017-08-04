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
	}else {std::cout<<"USAGE: ./asicpacker AIDARUN# AIDASUBRUN# or ./asicpacker AIDARUN#"<<endl; return -1;}

	//reading raw file
	AIDAraw rawaida(fi);

	int nl = fin.Length();
	fin.Remove(nl-5);
	fin+="_tr.root";
	//if(TFile::Open(fin)==1)
	std::cout<<"File output : "<<fin<<std::endl;

	TFile* fo = new TFile(fin,"RECREATE");

	int i,j;
	//int start_entry;
	int time0;	
	time_t tm;
        Long64_t        evt;
        Long64_t        timestamp;
        Long64_t        extTimestamp;
        Int_t           feeNo;
        Int_t           chNo;
        Int_t           dssdNo;
        Int_t           stripNo;
        Short_t         infoCode;
        Short_t         rangeType;
        Int_t           adcData;
	Long64_t 	tdel;
	Long64_t 	offset;
	Long64_t 	raw_timestamp;

	TTree* tree = new TTree("tree","tree");
	tree->Branch("evt", &evt,"evt/L");
	tree->Branch("tdel", &tdel,"tdel/L");
	tree->Branch("timestamp", &timestamp,"timestamp/L");
	tree->Branch("extTimestamp", &extTimestamp,"extTimestamp/L");
	tree->Branch("feeNo", &feeNo,"feeNo/I");
	tree->Branch("chNo", &chNo, "chNo/I");
	tree->Branch("dssdNo", &dssdNo, "dssdNo/I");
	tree->Branch("stripNo", &stripNo, "stripNo/I");
	tree->Branch("infoCode", &infoCode,"infoCode/S");
	tree->Branch("rangeType", &rangeType,"rangeType/S");
	tree->Branch("adcData", &adcData,"adcData/I");

	std::cout<<"Initialization Done"<<std::endl;

	int nlist =rawaida.GetEntriesFast();

	time0=time(&tm);
	offset=0;
	for(i=0;i<nlist;i++){

		rawaida.GetEntry(i);
		if(i==0){ tdel=0;}
		else {tdel=rawaida.timestamp-raw_timestamp;}

		evt=rawaida.evt;
		feeNo=rawaida.feeNo;
		chNo=rawaida.chNo;
		dssdNo=rawaida.dssdNo;
		stripNo=rawaida.stripNo;
		infoCode=rawaida.infoCode;
		rangeType=rawaida.rangeType;
		adcData=rawaida.adcData;
		raw_timestamp=rawaida.timestamp;

		if(tdel>300000000){
			offset+=300000000;
		}
		timestamp=rawaida.timestamp - offset;
		extTimestamp=rawaida.extTimestamp - offset/4;

		if(i%1000000==0) {std::cout<<"\r"<<i<<"'s entry reconstructed with offset "<<1.0*offset/100000000<<"s. Time spent "<<time(&tm)-time0<<"s"; std::cout.flush();}

		tree->Fill();
	}
	std::cout<<std::endl<<"job finished"<<std::endl;
	fo->WriteTObject(tree);
	fo->Close();
}




