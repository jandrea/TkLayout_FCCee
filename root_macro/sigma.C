// Created in 2021 by N. Lorber

#include "TH1I.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TProfile.h"
#include <iostream>
#include "TRandom.h"
#include <TStyle.h>
#include <TLegend.h>

/*
path1 and path2 are the paths to the result folders of the two runs you want to compare.
outputpath is the path to the folder where you want to save the plots being generated.
Adapt the paths below according to your own local directory.
*/

TString path1 = "../tkLayout_results/squareVXD_results_14.06_22:05/" ;
TString path2 = "../tkLayout_results/ref_results_14.06_18:13/" ;
TString outputpath = "../tkLayout_results/3000tracksSquare/" ;


void sigma(TString filename1, TString filename2, TString canvas_name, TString profile_name){
   TFile * file1 = new TFile( filename1.Data() );
   TFile * file2 = new TFile( filename2.Data() );
   
   TCanvas * c1 = (TCanvas*)file1->Get( canvas_name.Data() );
   TCanvas * c2 = (TCanvas*)file2->Get( canvas_name.Data() );
   
   // The three values below have to be changed to the set of momentum values you chose when installing tkLayout
   TString profile_name2 = profile_name + "1" ;
   TString profile_name3 = profile_name + "10" ;
   TString profile_name4 = profile_name + "100" ;

   TProfile * profileA = (TProfile*)c1->GetPrimitive( profile_name2.Data() );
   TProfile * profileB = (TProfile*)c2->GetPrimitive( profile_name2.Data() );
   TProfile * profileC = (TProfile*)c1->GetPrimitive( profile_name3.Data() );
   TProfile * profileD = (TProfile*)c2->GetPrimitive( profile_name3.Data() );
   TProfile * profileE = (TProfile*)c1->GetPrimitive( profile_name4.Data() );
   TProfile * profileF = (TProfile*)c2->GetPrimitive( profile_name4.Data() );

   c1->Clear();
   c2->Clear();
      
   auto c0= new TCanvas("c0","Two runs comparison",200,10,700,500);
   gStyle->SetOptStat(0);

   map<string, double> myMap;

   for (int iBin=1; iBin<=profileA->GetNbinsX(); ++iBin) {
      double sigmaA = profileA->GetBinError(iBin) ;
      double sigmaB = profileB->GetBinError(iBin) ;
      double pAiB = profileA->GetBinContent(iBin) ;
      double pBiB = profileB->GetBinContent(iBin) ;
      double totalsigmaAB = sqrt(pow(sigmaA/pBiB,2) + pow(pAiB*sigmaB/(pBiB*pBiB),2)) ;
      myMap["sAB"+std::to_string(iBin)] = totalsigmaAB ;
   }

   for (int iBin=1; iBin<=profileC->GetNbinsX(); ++iBin) {
      double sigmaC = profileC->GetBinError(iBin) ;
      double sigmaD = profileD->GetBinError(iBin) ;
      double pCiB = profileC->GetBinContent(iBin) ;
      double pDiB = profileD->GetBinContent(iBin) ;
      double totalsigmaCD = sqrt(pow(sigmaC/pDiB,2) + pow(pCiB*sigmaD/(pDiB*pDiB),2)) ;
      myMap["sCD"+std::to_string(iBin)] = totalsigmaCD ;
   }

   for (int iBin=1; iBin<=profileE->GetNbinsX(); ++iBin) {
      double sigmaE = profileE->GetBinError(iBin) ;
      double sigmaF = profileF->GetBinError(iBin) ;
      double pEiB = profileE->GetBinContent(iBin) ;
      double pFiB = profileF->GetBinContent(iBin) ;
      double totalsigmaEF = sqrt(pow(sigmaE/pFiB,2) + pow(pEiB*sigmaF/(pFiB*pFiB),2)) ;
      myMap["sEF"+std::to_string(iBin)] = totalsigmaEF ;
   }



   profileA->Divide(profileB);
   profileC->Divide(profileD);
   profileE->Divide(profileF);

   int nbinXA =profileA->GetNbinsX();
   float binminA = profileA->GetXaxis()->GetXmin();
   float binmaxA = profileA->GetXaxis()->GetXmax();

   int nbinXC =profileC->GetNbinsX();
   float binminC = profileC->GetXaxis()->GetXmin();
   float binmaxC = profileC->GetXaxis()->GetXmax();

   int nbinXE =profileE->GetNbinsX();
   float binminE = profileE->GetXaxis()->GetXmin();
   float binmaxE = profileE->GetXaxis()->GetXmax();

   TH1F * ratioPlotAB = new TH1F("ratioPlotAB", canvas_name, nbinXA, binminA, binmaxA);
   TH1F * ratioPlotCD = new TH1F("ratioPlotCD", canvas_name, nbinXC, binminC, binmaxC);
   TH1F * ratioPlotEF = new TH1F("ratioPlotEF", canvas_name, nbinXE, binminE, binmaxE);




   for (int iBin=4; iBin<=profileA->GetNbinsX(); ++iBin) {
      string sABcurrent = "sAB" + std::to_string(iBin);
      string sCDcurrent = "sCD" + std::to_string(iBin);
      string sEFcurrent = "sEF" + std::to_string(iBin);
      ratioPlotAB->SetBinContent(iBin, profileA->GetBinContent(iBin));
      ratioPlotAB->SetBinError(iBin, myMap[sABcurrent] );
      ratioPlotCD->SetBinContent(iBin, profileC->GetBinContent(iBin));
      ratioPlotCD->SetBinError(iBin, myMap[sCDcurrent] );
      ratioPlotEF->SetBinContent(iBin, profileE->GetBinContent(iBin));
      ratioPlotEF->SetBinError(iBin, myMap[sEFcurrent] );
   }

   ratioPlotAB->SetMarkerStyle(24);
   ratioPlotAB->SetMarkerColor(1);
   ratioPlotAB->SetFillColor(1);
   ratioPlotAB->SetLineColor(1);
   ratioPlotAB->SetLineWidth(1);

   ratioPlotCD->SetMarkerStyle(25);
   ratioPlotCD->SetMarkerColor(2);
   ratioPlotCD->SetFillColor(2);
   ratioPlotCD->SetLineColor(2);

   ratioPlotEF->SetMarkerStyle(26);
   ratioPlotEF->SetMarkerColor(4);
   ratioPlotEF->SetFillColor(4);
   ratioPlotEF->SetLineColor(4);

   ratioPlotEF->Draw();
   ratioPlotCD->Draw("same");
   ratioPlotAB->Draw("same");

   // You'll probably want to chang the three values of p below for the ones you chose when installing tkLayout
   auto legend = new TLegend(0.65,0.1,0.9,0.28);
   legend->AddEntry(ratioPlotAB,"p = 1 GeV","lep");
   legend->AddEntry(ratioPlotCD,"p = 10 GeV","lep");
   legend->AddEntry(ratioPlotEF,"p = 100 GeV","lep");
   legend->Draw();

   c0->SaveAs(outputpath + canvas_name + ".root");
   c0->SaveAs(outputpath + canvas_name + ".png");
   
   c0->Clear();
   c0->Close();
}


void sigma(){

   TString canvas = "init0" ;
   TString profile = "init1" ;

   for (int i = 1; i<9; i++) {

      for (int j = 1; j<4; j++) {

         for (int k = 1; k<3; k++) {

            for (int l = 1; l<3; l++) {

               TString canvas = "reset0" ;
               TString profile = "reset1" ;

               if (i==1) {
               canvas = "cotgThres_";
               profile = "cotgTh_vs_eta";
               }

               if (i==2) {
               canvas = "ctauRes_";
               profile = "ctau_vs_eta";
               }

               if (i==3) {
               canvas = "d0res_";
               profile = "d0_vs_eta";
               }

               if (i==4) {
               canvas = "linptres_";
               profile = "pT_vs_eta";
               }

               if (i==5) {
               canvas = "logptres_";
               profile = "pT_vs_eta";
               }

               if (i==6) {
               canvas = "phi0res_";
               profile = "phi0_vs_eta";
               }

               if (i==7) {
               canvas = "pres_";
               profile = "p_vs_eta";
               }

               if (i==8) {
               canvas = "z0res_";
               profile = "z0_vs_eta";
               }

               if (j==1) {
               canvas.Append("Tracker_") ;
               }

               if (j==2) {
               canvas.Append("VXD_") ;
               }
               
               if (j==3) {
               canvas.Append("IT+OT_") ;
               }

               if (k==1) {
               canvas.Append("noMS_") ;
               }

               if (k==2) {
               canvas.Append("withMS_") ;
               }

               if (l==1) {
               canvas.Append("P000") ;
               }

               if (l==2) {
               canvas.Append("Pt000") ;
               }

               sigma(
                  path1 + canvas + ".root",
                  path2 + canvas + ".root",
                  canvas,
                  profile
               ) ;
            }
         }
      }
   }
}