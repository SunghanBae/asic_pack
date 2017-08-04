//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Apr  7 11:21:00 2017 by ROOT version 6.04/10
// from TTree tree/tree
// found on file: R1110_0.root
//////////////////////////////////////////////////////////

#ifndef AIDAraw_h
#define AIDAraw_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TEntryList.h>
#include <TEntryList.h>

// Header file for the classes stored in the TTree if any.

class AIDAraw {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
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

   // List of branches
   TBranch        *b_evt;   //!
   TBranch        *b_timestamp;   //!
   TBranch        *b_extTimestamp;   //!
   TBranch        *b_feeNo;   //!
   TBranch        *b_chNo;   //!
   TBranch        *b_dssdNo;   //!
   TBranch        *b_stripNo;   //!
   TBranch        *b_infoCode;   //!
   TBranch        *b_rangeType;   //!
   TBranch        *b_adcData;   //!

//   TEntryList* li = new TEntryList();

   AIDAraw(char* filename, TTree *tree=0);
   virtual ~AIDAraw();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   //virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
   virtual Long64_t GetToffset();
   virtual Int_t    GetListEntry(TEntryList* l, Long64_t entry);
<<<<<<< HEAD
   virtual Int_t    GetEntriesFast();
=======
>>>>>>> 079cd85d2b7ebdc830da287210fd6d8a5f1c7e9e
   virtual Long64_t    SetADCEntryList(TEntryList* l);
};

#endif

//#ifdef AIDAraw_cxx
AIDAraw::AIDAraw(char* filename, TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject(filename);
      if (!f || !f->IsOpen()) {
         f = new TFile(filename);
      }
      f->GetObject("tree",tree);

   }
   Init(tree);
}

AIDAraw::~AIDAraw()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Long64_t AIDAraw::GetToffset()
{
   if(!fChain) return -1;
   fChain->GetEntry(0);
   return extTimestamp*4-timestamp;

}

Long64_t AIDAraw::SetADCEntryList(TEntryList* l)
{
   Long64_t maxn = fChain->GetEntriesFast();   
   for(Long64_t i=0; i<maxn; i++){
	fChain->GetEntry(i);
        if(infoCode==0)          //Only ADC data is taken
        {       
	        l->Enter(i);
   	  }
   if(i%10000000==0) {std::cout<<"\r"<<i<<"'th entry of raw data sorted to list";std::cout.flush();}
   }
//   l = (TEntryList*)gDirectory->Get("li");
   std::cout<<"ADC list is set as EntryList"<<std::endl;
   return l->GetN();
}

<<<<<<< HEAD
Int_t AIDAraw::GetEntriesFast()
{

	if(!fChain) return 0;

	return fChain->GetEntriesFast();

}


=======
>>>>>>> 079cd85d2b7ebdc830da287210fd6d8a5f1c7e9e
Int_t AIDAraw::GetListEntry(TEntryList*l, Long64_t entry)
{
   if(!fChain) return 0;

   return fChain->GetEntry(l->GetEntry(entry));

}


Int_t AIDAraw::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t AIDAraw::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void AIDAraw::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("evt", &evt, &b_evt);
   fChain->SetBranchAddress("timestamp", &timestamp, &b_timestamp);
   fChain->SetBranchAddress("extTimestamp", &extTimestamp, &b_extTimestamp);
   fChain->SetBranchAddress("feeNo", &feeNo, &b_feeNo);
   fChain->SetBranchAddress("chNo", &chNo, &b_chNo);
   fChain->SetBranchAddress("dssdNo", &dssdNo, &b_dssdNo);
   fChain->SetBranchAddress("stripNo", &stripNo, &b_stripNo);
   fChain->SetBranchAddress("infoCode", &infoCode, &b_infoCode);
   fChain->SetBranchAddress("rangeType", &rangeType, &b_rangeType);
   fChain->SetBranchAddress("adcData", &adcData, &b_adcData);
   Notify();
}

Bool_t AIDAraw::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void AIDAraw::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t AIDAraw::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
//#endif // #ifdef AIDAraw_cxx
