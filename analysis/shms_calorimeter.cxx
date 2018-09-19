#include <iostream>
#include <cmath>
#include "ROOT/RDataFrame.hxx"
#include "ROOT/RVec.hxx"
#include "TCanvas.h"
//#include "ROOT/TCanvas.hxx"
#include "Math/Vector3D.h"
#include "Math/Vector4D.h"
#include "Math/VectorUtil.h"
R__LOAD_LIBRARY(libMathMore.so)
R__LOAD_LIBRARY(libGenVector.so)

#include "THStack.h"
#include "TBufferJSON.h"

#include "nlohmann/json.hpp"
#include "THcParmList.h"
R__LOAD_LIBRARY(libHallA.so)
R__LOAD_LIBRARY(libdc.so)
R__LOAD_LIBRARY(libHallC.so)

// fmt - string formatting library
#include "fmt/core.h"
#include "fmt/ostream.h"
R__LOAD_LIBRARY(libfmt.so)

using Pvec3D   = ROOT::Math::XYZVector;
using Pvec4D   = ROOT::Math::PxPyPzMVector;
using inters = ROOT::VecOps::RVec<int>;
using doublers = ROOT::VecOps::RVec<double>;
using floaters = ROOT::VecOps::RVec<float>;
using shorters = ROOT::VecOps::RVec<short>;
using FVec     = std::vector<float>;

void shms_calorimeter(int RunNumber = 3115, const char* codatype = "COIN", int nevents = 50000) {

  std::string coda_type = codatype;
  //std::string coda_type = "SHMS";
  //std::string coda_type = "COIN";

  std::string hallc_replay_dir = "/home/ANL/warmstrong/work/sidis/";

  std::string rootfile    = std::string("ROOTfiles/coin_replay_production_");
  if(coda_type == "SHMS") {
    rootfile         = std::string("ROOTfiles/shms_replay_production_all_");// + std::to_string(RunNumber) + "_50000.root";
  }

  rootfile  += std::to_string(RunNumber) + "_" + std::to_string(nevents) +".root";

  std::string run_list_json  = "DBASE/run_list.json";

  // //std::string(std::getenv("DB_DIR"))
  //if (setenv("DB_DIR", hallc_replay_dir.c_str(), 1)) {
  //  std::cout << "Failed to set env var DB_DIR\n";
  //  std::exit(EXIT_FAILURE);
  //}

  std::string db_filename = hallc_replay_dir+"DBASE/"+coda_type+"/standard.database";

  THcParmList* hc_parms = new THcParmList();
  hc_parms->Define("gen_run_number", "Run Number", RunNumber);
  hc_parms->AddString("g_ctp_database_filename", db_filename.c_str() );
  hc_parms->Load(hc_parms->GetString("g_ctp_database_filename"), RunNumber);
  hc_parms->Load(hc_parms->GetString("g_ctp_parm_filename"));
  hc_parms->Load(hc_parms->GetString("g_ctp_kinematics_filename"), RunNumber);

  //auto j = nlohmann::json::parse(hc_parms->PrintJSON(RunNumber));
  //auto htheta_lab = hc_parms->Find("htheta_lab");
  //if(htheta_lab){
  //  htheta_lab->Print();
  //}
  auto ptheta_lab = hc_parms->Find("ptheta_lab");
  ptheta_lab->Print();
  //auto hpcentral = hc_parms->Find("hpcentral");
  //hpcentral->Print();
  auto ppcentral = hc_parms->Find("ppcentral");
  ppcentral->Print();
  //std::cout << j.dump()  << "\n";

  double P0_SHMS = hcana::json::FindVarValueOr(hc_parms,"ppcentral",0.0) ;
  std::cout <<  "SHMS P0 = " << P0_SHMS << "\n";

  double THETA_SHMS = hcana::json::FindVarValueOr(hc_parms,"ptheta_lab",0.0) ;

  //nlohmann::json j2;
  //{
  //  ifstream  in_run_file(run_list_json);
  //  in_run_file >> j2;
  //}
  //if( j2.find(std::to_string(RunNumber)) == j2.end() ) {
  //}
  //j2[std::to_string(RunNumber)] = j[std::to_string(RunNumber)];

  //{
  //  //std::cout << j.dump(2) << "\n";
  //  // write prettified JSON to another file
  //  std::ofstream o(run_list_json);
  //  o << std::setw(4) << j2 << std::endl;
  //}

  //std::cout << j2.dump() << "\n";

  ROOT::EnableImplicitMT(30);

  Pvec4D  Pbeam(0,0,10.6,0.000511);

  ROOT::RDataFrame d("T",rootfile);

  auto d0 = d
  .Define("shms_e_EoverP",
          [](doublers& EOverP) {
            return EOverP[EOverP > 0.8 && EOverP < 1.8];
          }, {"P.cal.etottracknorm"})
  .Define("shms_e_EoverP_nGood",
          [](doublers& EOverP) {
            return (int)EOverP.size();
          }, {"shms_e_EoverP"})
  .Define("hgc_pmt0",[](doublers& data) { return data[0]; },{"P.hgcer.npe"})
  .Define("hgc_pmt1",[](doublers& data) { return data[1]; },{"P.hgcer.npe"})
  .Define("hgc_pmt2",[](doublers& data) { return data[2]; },{"P.hgcer.npe"})
  .Define("hgc_pmt3",[](doublers& data) { return data[3]; },{"P.hgcer.npe"})
  .Define("ngc_pmt0",[](doublers& data) { return data[0]; },{"P.ngcer.npe"})
  .Define("ngc_pmt1",[](doublers& data) { return data[1]; },{"P.ngcer.npe"})
  .Define("ngc_pmt2",[](doublers& data) { return data[2]; },{"P.ngcer.npe"})
  .Define("ngc_pmt3",[](doublers& data) { return data[3]; },{"P.ngcer.npe"})
  .Define("hgc_xpos",[](doublers& tr_y, doublers& tr_ph) { return tr_y[0] + tr_ph[0]*156.7; },{"P.tr.y","P.tr.ph"})
  .Define("hgc_ypos",[](doublers& tr_x, doublers& tr_th) { return tr_x[0] + tr_th[0]*156.7; },{"P.tr.x","P.tr.th"})
  ;
    
  auto d2 = d0.Filter(
    [](int n_good) {
        if( n_good >0 ) {
          return true;
        }
      return  false;
    },{"shms_e_EoverP_nGood"});

  auto d3 = d2.Filter(
    [](double npe) {
      if( npe >10 ) { return true; }
      return  false;
    },{"P.ngcer.npeSum"})
  ;

  auto d33 = d3.Filter( [](double ypos) {
    if( std::abs(ypos-8) < 14.0 ) { return true; }
    return  true;//false;
  },{"hgc_ypos"});

  // Mirror cuts so cone is contined in one mirror
  auto d3_mirror1 = d3.Filter( [](double xpos,double ypos) {
    if( (xpos>7.0)&&(ypos>10.0)) { return true; }
    return  false;
  },{"hgc_xpos","hgc_ypos"});
  auto d3_mirror2 = d3.Filter( [](double xpos,double ypos) {
    if( (xpos<-7.0)&&(ypos>10.0)) { return true; }
    return  false;
  },{"hgc_xpos","hgc_ypos"});
  auto d3_mirror3 = d3.Filter( [](double xpos,double ypos) {
    if( (xpos>7.0)&&(ypos<-10.0)) { return true; }
    return  false;
  },{"hgc_xpos","hgc_ypos"});
  auto d3_mirror4 = d3.Filter( [](double xpos,double ypos) {
    if( (xpos<-7.0)&&(ypos<-10.0)) { return true; }
    return  false;
  },{"hgc_xpos","hgc_ypos"});

  auto d4 = d2.Filter(
    [](double npe) {
      if( npe >8 ) { return true; }
      return  false;
    },{"P.hgcer.npeSum"});

  auto d_pmt0 = d33.Filter(
    [](double npe) {
      if( npe >6 ) { return true; }
      return  false;
    },{"hgc_pmt0"});
  auto d_pmt1 = d33.Filter(
    [](double npe) {
      if( npe >6 ) { return true; }
      return  false;
    },{"hgc_pmt1"});
  auto d_pmt2 = d33.Filter(
    [](double npe) {
      if( npe >6 ) { return true; }
      return  false;
    },{"hgc_pmt2"});
  auto d_pmt3 = d33.Filter(
    [](double npe) {
      if( npe >6 ) { return true; }
      return  false;
    },{"hgc_pmt3"});


  auto d_hgc_pmt0 = d3.Filter(
    [](double npe0, double npe1, double npe2, double npe3) {
      if( (npe0>6)&&(npe1>6) ) { return true; }
      if( (npe1>6)&&(npe3>6) ) { return true; }
      if( (npe3>6)&&(npe2>6) ) { return true; }
      if( (npe2>6)&&(npe0>6) ) { return true; }
      return  false;
    },{"hgc_pmt0","hgc_pmt1","hgc_pmt2","hgc_pmt3"})
  ;

  //auto d5 = d3.Filter(
  //  [](double npe) {
  //    if( npe >8 ) { return true; }
  //    return  false;
  //  },{"P.hgcer.npeSum"});
  //  
  //auto d_hgc_adcs = d5.Filter(
  //  [](doublers& tdc, doublers& counter ) {
  //    return  false;
  //  },{"P.hgcer.adcPulseTime","P.hgcer.adcPulseTime","P.hgcer.adcCounter"});

  gStyle->SetOptStat(1111);

  auto h_EOverP_0       = d0.Histo1D<doublers>({"hms_e_EoverP_0","all; HMS E/P",    100,0.05,1.8},"P.cal.etottracknorm");
  auto h_EOverP_1       = d0.Histo1D<doublers>({"hms_e_EoverP_1","E/P cut; HMS E/P",100,0.05,1.8},"shms_e_EoverP");
  auto h_EOverP_nGood_0 = d0.Histo1D({"h_EOverP_nGood_0","h_EOverP_nGood_0",10,0,10},"P.cal.ntracks");
  auto h_EOverP_nGood_1 = d0.Histo1D({"h_EOverP_nGood_1","h_EOverP_nGood_0",10,0,10},"shms_e_EoverP_nGood");

  //auto h_cer_0          = d0.Histo1D({"hms_cer_0","all; HMS npe sum",100,0,20},"H.cer.npeSum");
  //auto h_cer_2          = d2.Histo1D({"hms_cer_0","with E/P cut; HMS npe sum",100,0,20},"H.cer.npeSum");

  auto s_aero_npe_0          = d0.Histo1D({"s_aero_npe_0","all; SHMS aero npe sum",100,0,20},"P.aero.npeSum");
  auto s_aero_npe_2          = d2.Histo1D({"s_aero_npe_2","with E/P cut; SHMS aero npe sum",100,0,20},"P.aero.npeSum");

  auto s_hgc_npe_0          = d0.Histo1D({"s_hgc_npe_0","all; SHMS HGC npe sum",100,0,50},"P.hgcer.npeSum");
  auto s_hgc_npe_2          = d2.Histo1D({"s_hgc_npe_2","with HMS E/P cut; SHMS HGC npe sum",100,0,50},"P.hgcer.npeSum");
  auto s_hgc_npe_3          = d3.Histo1D({"s_hgc_npe_3","with HMS E/P and cer cut; SHMS HGC npe sum",100,0,50},"P.hgcer.npeSum");

  auto s_hgc_npe_pmt0          = d33.Histo1D({"s_hgc_npe_pmt0","with HMS E/P and cer cut; SHMS HGC npe sum",100,0,50},"hgc_pmt0");
  auto s_hgc_npe_pmt1          = d33.Histo1D({"s_hgc_npe_pmt1","with HMS E/P and cer cut; SHMS HGC npe sum",100,0,50},"hgc_pmt1");
  auto s_hgc_npe_pmt2          = d33.Histo1D({"s_hgc_npe_pmt2","with HMS E/P and cer cut; SHMS HGC npe sum",100,0,50},"hgc_pmt2");
  auto s_hgc_npe_pmt3          = d33.Histo1D({"s_hgc_npe_pmt3","with HMS E/P and cer cut; SHMS HGC npe sum",100,0,50},"hgc_pmt3");

  auto s_hgc_npe_mirror1          = d3_mirror1.Histo1D({"s_hgc_npe_mirror1","with HMS E/P and cer cut; SHMS HGC npe sum",100,0,50},"hgc_pmt0");
  auto s_hgc_npe_mirror2          = d3_mirror2.Histo1D({"s_hgc_npe_mirror2","with HMS E/P and cer cut; SHMS HGC npe sum",100,0,50},"hgc_pmt1");
  auto s_hgc_npe_mirror3          = d3_mirror3.Histo1D({"s_hgc_npe_mirror3","with HMS E/P and cer cut; SHMS HGC npe sum",100,0,50},"hgc_pmt2");
  auto s_hgc_npe_mirror4          = d3_mirror4.Histo1D({"s_hgc_npe_mirror4","with HMS E/P and cer cut; SHMS HGC npe sum",100,0,50},"hgc_pmt3");
  
  auto s_dc_x_fp           = d33.Histo1D({"s_dc_x_fp ","xy fp; x",100,-50,50}, "P.dc.x_fp");
  auto s_dc_y_fp           = d33.Histo1D({"s_dc_y_fp ","xy fp; y",100,-50,50}, "P.dc.y_fp");
  auto s_dc_xp_fp          = d33.Histo1D({"s_dc_xp_fp","xy fp; xp",100,-50,50},"P.dc.xp_fp");
  auto s_dc_yp_fp          = d33.Histo1D({"s_dc_yp_fp","xy fp; xp",100,-50,50},"P.dc.yp_fp");

  auto s_hgc_npe_tdc0          = d.Histo1D({"s_hgc_npe_tdc0","HGC; tdc channel",200,0,200},"T.shms.pHGCER_tdcTime");
  auto s_ngc_npe_tdc0          = d.Histo1D({"s_ngc_npe_tdc0","NGC; tdc channel",200,0,200},"T.shms.pNGCER_tdcTime");

  auto s_hgc_npe_tdc          = d3.Histo1D({"s_hgc_npe_tdc","HGC ; tdc channel",200,0,200},"T.shms.pHGCER_tdcTime");
  auto s_ngc_npe_tdc          = d3.Histo1D({"s_ngc_npe_tdc","NGC ; tdc channel",200,0,200},"T.shms.pNGCER_tdcTime");

  auto s_ngc_npe_0          = d0.Histo1D({"s_ngc_npe_0","all; SHMS ngc npe sum",100,0,50},"P.ngcer.npeSum");
  auto s_ngc_npe_2          = d2.Histo1D({"s_ngc_npe_2","with E/P cut; SHMS ngc npe sum",100,0,50},"P.ngcer.npeSum");
  auto s_ngc_npe_3          = d4.Histo1D({"s_ngc_npe_3","with HMS E/P  and hgc cut; SHMS ngc npe sum",100,0,50},"P.ngcer.npeSum");

  auto s_ngc_npe_pmt0          = d2.Histo1D({"s_ngc_npe_pmt0","pmt1; SHMS NGC npe",100,0,50},"ngc_pmt0");
  auto s_ngc_npe_pmt1          = d2.Histo1D({"s_ngc_npe_pmt1","pmt2; SHMS NGC npe",100,0,50},"ngc_pmt1");
  auto s_ngc_npe_pmt2          = d2.Histo1D({"s_ngc_npe_pmt2","pmt3; SHMS NGC npe",100,0,50},"ngc_pmt2");
  auto s_ngc_npe_pmt3          = d2.Histo1D({"s_ngc_npe_pmt3","pmt4; SHMS NGC npe",100,0,50},"ngc_pmt3");

  auto s_EOverP_0       = d0.Histo1D<doublers>({"hms_e_EoverP_0","all; SHMS E/P",100,0.05,1.8},"P.cal.etottracknorm");
  auto s_EOverP_2       = d2.Histo1D<doublers>({"hms_e_EoverP_2","with E/P cut ; SHMS E/P",100,0.05,1.8},"P.cal.etottracknorm");
  auto s_EOverP_3       = d3.Histo1D<doublers>({"hms_e_EoverP_3","with NGC cut ; SHMS E/P",100,0.05,1.8},"P.cal.etottracknorm");
  auto s_EOverP_4       = d4.Histo1D<doublers>({"hms_e_EoverP_2","with HGC cut ; SHMS E/P",100,0.05,1.8},"P.cal.etottracknorm");

  auto s_EtrackOverP_0       = d0.Histo1D<doublers>({"hms_e_EoverP_0","all; SHMS E/P",          100,0.05,1.8},"P.cal.eprtracknorm");
  auto s_EtrackOverP_2       = d2.Histo1D<doublers>({"hms_e_EoverP_2","with E/P cut ; SHMS E/P",100,0.05,1.8},"P.cal.eprtracknorm");
  auto s_EtrackOverP_3       = d3.Histo1D<doublers>({"hms_e_EoverP_3","with NGC cut ; SHMS E/P",100,0.05,1.8},"P.cal.eprtracknorm");
  auto s_EtrackOverP_4       = d4.Histo1D<doublers>({"hms_e_EoverP_2","with HGC cut ; SHMS E/P",100,0.05,1.8},"P.cal.eprtracknorm");

  auto s_Eprtrack_0       = d0.Histo1D<doublers>({"s_Eprtrack_0","all; SHMS E/P",          100,0.05,1.8},"P.cal.eprtrack");
  auto s_Eprtrack_2       = d2.Histo1D<doublers>({"s_Eprtrack_2","with E/P cut ; SHMS E/P",100,0.05,1.8},"P.cal.eprtrack");
  auto s_Eprtrack_3       = d3.Histo1D<doublers>({"s_Eprtrack_3","with NGC cut ; SHMS E/P",100,0.05,1.8},"P.cal.eprtrack");
  auto s_Eprtrack_4       = d4.Histo1D<doublers>({"s_Eprtrack_4","with HGC cut ; SHMS E/P",100,0.05,1.8},"P.cal.eprtrack");

  auto s_HGC_xy_pos      = d3.Fill<double,double>(TH2D("s_HGC_xy_pos ","xy fp; x;y",120,-60,60,120,-60,60), {"hgc_xpos","hgc_ypos"});
  auto s_HGC_xy_pos_pmt0 = d_pmt0.Fill<double,double>(TH2D("s_HGC_xy_pos ","xy fp; x;y",120,-60,60,120,-60,60), {"hgc_xpos","hgc_ypos"});
  auto s_HGC_xy_pos_pmt1 = d_pmt1.Fill<double,double>(TH2D("s_HGC_xy_pos ","xy fp; x;y",120,-60,60,120,-60,60), {"hgc_xpos","hgc_ypos"});
  auto s_HGC_xy_pos_pmt2 = d_pmt2.Fill<double,double>(TH2D("s_HGC_xy_pos ","xy fp; x;y",120,-60,60,120,-60,60), {"hgc_xpos","hgc_ypos"});
  auto s_HGC_xy_pos_pmt3 = d_pmt3.Fill<double,double>(TH2D("s_HGC_xy_pos ","xy fp; x;y",120,-60,60,120,-60,60), {"hgc_xpos","hgc_ypos"});

  auto s_HGC_xy_pos_edges = d_hgc_pmt0.Fill<double,double>(TH2D("s_HGC_xy_pos_edges ","xy fp; x;y",120,-60,60,120,-60,60), {"hgc_xpos","hgc_ypos"});
  
  // -----------------------------------------------------------
  //
  TCanvas* c = nullptr;
  int b1 = 0;
  int b2 = 0;
  double hmax = 0.0;
  TLatex latex;

  // ---------------------------------------------------------
  //
  c = new TCanvas();
  c->Divide(2,2);
  c->cd(1);
  b1 = s_aero_npe_0->FindBin(4.0);
  b2 = s_aero_npe_0->FindBin(14.0);
  hmax = s_aero_npe_0->Integral(b1,b2)/10.0;
  s_aero_npe_0->SetMaximum(hmax);
  s_aero_npe_0->DrawCopy();
  s_aero_npe_2->SetLineColor(2);
  s_aero_npe_2->DrawCopy("same");

  std::string momentum_label = fmt::format("SHMS P0 = {0:.1f} GeV/c",P0_SHMS);
  latex.DrawLatexNDC(0.4,0.8, momentum_label.c_str());

  c->cd(2);
  b1 = s_hgc_npe_3->FindBin(10.0);
  b2 = s_hgc_npe_3->FindBin(20.0);
  hmax = 3.0*s_hgc_npe_3->Integral(b1,b2)/10.0;
  s_hgc_npe_0->SetMaximum(hmax);
  s_hgc_npe_0->DrawCopy();
  s_hgc_npe_2->SetLineColor(2);
  s_hgc_npe_2->DrawCopy("same");
  s_hgc_npe_3->SetLineColor(4);
  s_hgc_npe_3->DrawCopy("same");

  c->cd(3);
  b1 = s_hgc_npe_3->FindBin(5.0);
  b2 = s_hgc_npe_3->FindBin(18.0);
  hmax = 2.0*s_hgc_npe_3->Integral(b1,b2)/(std::abs(b2-b1));
  s_ngc_npe_0->SetMaximum(hmax);
  s_ngc_npe_0->DrawCopy();
  s_ngc_npe_2->SetLineColor(2);
  s_ngc_npe_2->DrawCopy("same");
  s_ngc_npe_3->SetLineColor(4);
  s_ngc_npe_3->DrawCopy("same");

  c->cd(4);
  s_EOverP_0->DrawCopy();
  s_EOverP_2->SetLineColor(2);
  s_EOverP_2->DrawCopy("same");
  s_EOverP_3->SetLineColor(4);
  s_EOverP_3->DrawCopy("same");
  s_EOverP_4->SetLineColor(8);
  s_EOverP_4->DrawCopy("same");
  gPad->BuildLegend();

  c->SaveAs((std::string("results/shms_calorimeter_all_")+std::to_string(RunNumber)+".pdf").c_str());
  c->SaveAs((std::string("results/shms_calorimeter_all_")+std::to_string(RunNumber)+".png").c_str());
  
  // ---------------------------------------------------------
  //
  c = new TCanvas();
  //c->Divide(2,2);
  //c->cd(1);
  s_EOverP_0->DrawCopy();
  s_EOverP_2->SetLineColor(2);
  s_EOverP_2->DrawCopy("same");
  s_EOverP_3->SetLineColor(4);
  s_EOverP_3->DrawCopy("same");
  s_EOverP_4->SetLineColor(8);
  s_EOverP_4->DrawCopy("same");
  gPad->BuildLegend();

  c->SaveAs((std::string("results/shms_calorimeter_EoverP_")+std::to_string(RunNumber)+".pdf").c_str());
  c->SaveAs((std::string("results/shms_calorimeter_EoverP_")+std::to_string(RunNumber)+".png").c_str());

  // ---------------------------------------------------------
  //
  c = new TCanvas();
  //c->Divide(2,2);
  //c->cd(1);
  s_EtrackOverP_0->DrawCopy();
  s_EtrackOverP_2->SetLineColor(2);
  s_EtrackOverP_2->DrawCopy("same");
  s_EtrackOverP_3->SetLineColor(4);
  s_EtrackOverP_3->DrawCopy("same");
  s_EtrackOverP_4->SetLineColor(8);
  s_EtrackOverP_4->DrawCopy("same");
  gPad->BuildLegend();

  c->SaveAs((std::string("results/shms_calorimeter_EtrackoverP_")+std::to_string(RunNumber)+".pdf").c_str());
  c->SaveAs((std::string("results/shms_calorimeter_EtrackoverP_")+std::to_string(RunNumber)+".png").c_str());

  // ---------------------------------------------------------
  //
  c = new TCanvas();
  //c->Divide(2,2);
  //c->cd(1);
  s_Eprtrack_0->DrawCopy();
  s_Eprtrack_2->SetLineColor(2);
  s_Eprtrack_2->DrawCopy("same");
  s_Eprtrack_3->SetLineColor(4);
  s_Eprtrack_3->DrawCopy("same");
  s_Eprtrack_4->SetLineColor(8);
  s_Eprtrack_4->DrawCopy("same");
  gPad->BuildLegend();

  c->SaveAs((std::string("results/shms_calorimeter_Eprtrack_")+std::to_string(RunNumber)+".pdf").c_str());
  c->SaveAs((std::string("results/shms_calorimeter_Eprtrack_")+std::to_string(RunNumber)+".png").c_str());

  //// ---------------------------------------------------------
  //// JSON output
  //nlohmann::json j2;
  //{
  //  ifstream  in_run_file("results/shms_cherenkov_hgc_npe.json");
  //  if(in_run_file.good()){
  //  in_run_file >> j2;
  //  }
  //}
  //j2[std::to_string(RunNumber)] = {
  //  {"ptheta_lab", THETA_SHMS},
  //  {"ppcentral", P0_SHMS},
  //  {"nevents",nevents},
  //  {"mu",    hgc_res->Value(1)},
  //  {"sigma", hgc_res->Value(2)},
  //  {"ngc_mu", ngc_res->Value(1)},
  //  {"ngc_sigma", ngc_res->Value(2)},
  //  {"hgc", {
  //            {"sum_mean",hgc_res->Value(1)},
  //            {"sum_sigma",hgc_res->Value(2)},
  //            {"mean",mirror_npe_means},
  //            {"sigma",mirror_npe_sigmas}
  //          }
  //  }
  //};
  //{
  //  std::ofstream res_file("results/shms_cherenkov_hgc_npe.json");//, std::ios_base::app);
  //  res_file << j2.dump(2) << std::endl;
  //}
  //std::cout << j2.dump(2) << std::endl;

  //std::string h_json = TBufferJSON::ToJSON(save_hist).Data();
  ////j2 << h_json;
  ////std::cout << h_json << std::endl; 
  ////std::cout << j.dump(2) << "\n";
  //// write prettified JSON to another file
  //std::ofstream o((std::string("results/shms_cherenkov_")+std::to_string(RunNumber)+".json").c_str());
  //o << std::setw(2) << h_json << std::endl;
  ////{
  ////  ifstream  in_run_file(run_list_json);
  ////  in_run_file >> j2;
  ////}
}
