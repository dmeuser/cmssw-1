// $Id: EEMDisplayPlugins.cc,v 1.18 2007/11/24 17:45:55 dellaric Exp $

/*!
  \file EEMDisplayPlugins
  \brief Display Plugin for Quality Histograms (2D)
  \author B. Gobbo 
  \version $Revision: 1.18 $
  \date $Date: 2007/11/24 17:45:55 $
*/

#include <TH1F.h>
#include <TH2F.h>
#include <TH3F.h>
#include <TProfile.h>
#include <TProfile2D.h>

#include <TStyle.h>
#include <TCanvas.h>
#include <TColor.h>

#include "DQMServices/Core/interface/MonitorElement.h"

#include "DQM/EcalCommon/interface/ColorPalette.h"
#include <DQM/EcalCommon/interface/Numbers.h>
#include "DQM/EcalEndcapMonitorDisplayPlugins/interface/EEMDisplayPlugins.h"

// Temporary: this class should be instantiated once, but due to bugs 
// it happens that's instantiated more times...
static bool  first = true;
static TH2C* t1 = new TH2C( "eem_text1", "text1", 85, 0, 85, 20, 0, 20 );
static TH2C* t2 = new TH2C( "eem_text2", "text2", 17, 0, 17,  4, 0,  4 );
static TH2C* t3 = new TH2C( "eem_text3", "text3", 10, 0, 10,  5, 0,  5 );
static TH2C* t4 = new TH2C( "eem_text4", "text4",  2, 0,  2,  1, 0,  1 );
static TH2C* t5 = new TH2C( "eem_text5", "text5", 86, 1, 87,  1, 0,  1 );
static TH2C* t6 = new TH2C( "eem_text6", "text6", 18, 0, 360,  2, -85, 85 );
//

template<class T> void EEMDisplayPlugins::adjustRange( T obj ) {
  if( obj->GetEntries() == 0 ) { return; }
  int first_bin = -1, last_bin=-1;
  for( int bin=1; bin < obj->GetNbinsX()+1; bin++ ) {
    if( obj->GetBinContent(bin) > 0 ) {
      if( first_bin == -1 ) { first_bin = bin; }
      last_bin = bin;
    }
  }
  
  if( first_bin < 1 || last_bin < 1 ) { return; }
  if( first_bin > 1 ) { first_bin -= 1; }
  if( last_bin < obj->GetNbinsX() ){ last_bin += 1; }
  
  obj->GetXaxis()->SetRange( first_bin, last_bin );
}

EEMDisplayPlugins::EEMDisplayPlugins( IgState *state ) : VisDQMDisplayPlugin( state ) {

  nbx = 0;
  nby = 0;
  name = "";

  for( int i=0; i<6; i++ ) {
    TColor* color;
    if( ! gROOT->GetColor( 301+i )) {
      color = new TColor( 301+i, ecdqm::rgb[i][0], ecdqm::rgb[i][1], ecdqm::rgb[i][2], "" );
    }
    else {
      color = gROOT->GetColor( 301+i );
      color->SetRGB( ecdqm::rgb[i][0], ecdqm::rgb[i][1], ecdqm::rgb[i][2] );
    }
  }
  for( short i=0; i<6; i++ ) pCol3[i]  = i+301;

  for( int i=0; i<10; i++ ) {
    TColor* color;
    if( ! gROOT->GetColor( 401+i )) {
      color = new TColor( 401+i, ecdqm::rgb2[i][0], ecdqm::rgb2[i][1], ecdqm::rgb2[i][2], "" );
    }
    else {
      color = gROOT->GetColor( 401+i );
      color->SetRGB( ecdqm::rgb2[i][0], ecdqm::rgb2[i][1], ecdqm::rgb2[i][2] );
    }
  }
  for( short i=0; i<10; i++ ) pCol4[i] = i+401;

  text1 = t1;
  text2 = t2;
  text3 = t3;
  text4 = t4;
  text5 = t5;
  text6 = t6;
  //text1 = new TH2C( "text1", "text1", 85, 0, 85, 20, 0, 20 );
  //text2 = new TH2C( "text2", "text2", 17, 0, 17,  4, 0,  4 );
  //text3 = new TH2C( "text3", "text3", 10, 0, 10,  5, 0,  5 );
  //text4 = new TH2C( "text4", "text4",  2, 0,  2,  1, 0,  1 );
  //text5 = new TH2C( "text5", "text5", 86, 1, 87,  1, 0,  1 );
  //text6 = new TH2C( "text6", "text6", 18, 0, 360,  2, -85, 85 );

  text1->SetMinimum( 0.1 );
  text2->SetMinimum( 0.1 );
  text3->SetMinimum( 0.1 );
  text4->SetMinimum( 0.1 );
  text5->SetMinimum( 0.1 );
  text6->SetMinimum( -18.01 );

  // The use of "first" will be removed when multiple instantiation problem will be fixed
  if( first ) {
    first = false;
    for( short i=0; i<68; i++ ) {
      text1->Fill( 2+(i/4)*5, 2+(i%4)*5, i+1 );
      text2->Fill( i/4, i%4, i+1 );
    }
    for( short i=0; i<2; i++ ) {
      text3->Fill( 2+i*5, 2, i+1+68 );
      text4->Fill( i, 0., i+1+68 );
    }
    for ( short i=0; i<18; i++ ) {
      int x = 1 + i%9;
      int y = 1 + i/9;
      text6->SetBinContent(x, y, Numbers::iEE(i+1));
    }
  }

  text1->SetMarkerSize( 2 );
  text2->SetMarkerSize( 2 );
  text3->SetMarkerSize( 2 );
  text4->SetMarkerSize( 2 );
  text5->SetMarkerSize( 2 );
  text6->SetMarkerSize( 2 );

}

bool EEMDisplayPlugins::applies( DisplayData *data ) {
 
  name = (data->object)->GetName();

  // Ecal Endcap DQM related MEs start with "EE"
  if( name.find( "EE" ) == 0 ) {
    return true;
  }
  return false;
}

std::string EEMDisplayPlugins::preDraw( DisplayData *data ) {

  MonitorElement* me = data->me;
  if( me  ) {
    data->pad->SetFrameFillColor( 10 );
    if (me->hasWarning()) data->pad->SetFillColor( 5 );
    if (me->hasError()) data->pad->SetFillColor( 2 );
  }

  if( dynamic_cast<TProfile2D*>( data->object ) ) {
    return preDrawTProfile2D( data );
  }
  else if( dynamic_cast<TProfile*>( data->object ) ) {
    return preDrawTProfile( data );
  }
  else if( dynamic_cast<TH2F*>( data->object ) ) {
    return preDrawTH2F( data );
  }
  else if( dynamic_cast<TH1F*>( data->object ) ) {
    return preDrawTH1F( data );
  }
  
  return "";
  
}

std::string EEMDisplayPlugins::preDrawTProfile2D( DisplayData *data ) {

  TProfile2D* obj = dynamic_cast<TProfile2D*>( data->object );

  name = (data->object)->GetName();

  if( obj ) {
    gStyle->SetOptStat( 0 );
    obj->SetStats( kFALSE );
    nbx = obj->GetNbinsX();
    nby = obj->GetNbinsY();
    obj->GetXaxis()->SetNdivisions( 17 );
    obj->GetYaxis()->SetNdivisions( 4 );
    (data->pad)->SetGridx();
    (data->pad)->SetGridy();
    gStyle->SetPalette( 10, pCol4 );
    obj->SetOption( "colz" );
  }

  return "";

}

std::string EEMDisplayPlugins::preDrawTProfile( DisplayData *data ) {

  TProfile* obj = dynamic_cast<TProfile*>( data->object );

  name = (data->object)->GetName();

  if( obj ) {

    if( name.find( "EEBCT crystal in beam vs event" ) < name.size() ) {
      gStyle->SetOptStat( 0 );
      obj->SetStats( kFALSE );
      adjustRange( obj );
      return "";
    }

    if( name.find( "EEBCT average rec energy" ) < name.size() ) {
      gStyle->SetOptStat( "e" );
      obj->SetStats( kTRUE );
      adjustRange( obj );
      return "";
    }

    if( name.find( "EEBCT" ) < name.size() ) {
      gStyle->SetOptStat( "e" );
      obj->SetStats( kTRUE );
      return "";
    }

    gStyle->SetOptStat( "euomr" );
    obj->SetStats( kTRUE );
    return "";

  }

  return "";

}

std::string EEMDisplayPlugins::preDrawTH2F( DisplayData *data ) {

  TH2F* obj = dynamic_cast<TH2F*>( data->object );

  name = (data->object)->GetName();

  if( obj ) {

    // This applies to all
    gStyle->SetCanvasBorderMode( 0 );
    gStyle->SetPadBorderMode( 0 );
    gStyle->SetPadBorderSize( 0 );
    (data->pad)->SetLogy( 0 );
    gStyle->SetOptStat( 0 );
    obj->SetStats( kFALSE );
    nbx = obj->GetNbinsX();
    nby = obj->GetNbinsY();
    
    if( nbx == 85 && nby == 20 ) {
      obj->GetXaxis()->SetNdivisions( 17 );
      obj->GetYaxis()->SetNdivisions( 4 );
    }
    if( nbx == 17 && nby == 4 ) {
      obj->GetXaxis()->SetNdivisions( 17 );
      obj->GetYaxis()->SetNdivisions( 4 );
    }
    else if( nbx == 10 && nby == 5 ) {
      obj->GetXaxis()->SetNdivisions( 10 );
      obj->GetYaxis()->SetNdivisions( 1 );
    }
    else if( nbx == 2 && nby == 1 ) {
      obj->GetXaxis()->SetNdivisions( 2 );
      obj->GetYaxis()->SetNdivisions( 1 );
    }
    else if( nbx == 360 && nby == 170 ) {
      obj->GetXaxis()->SetNdivisions( 18 );
      obj->GetYaxis()->SetNdivisions( 2 );
    }
    (data->pad)->SetGridx();
    (data->pad)->SetGridy();

    // Occupancy-like (10 x grays) plots
    if( name.find( "EEIT" ) < name.size() &&
	name.find( "quality" ) >= name.size() ) {
      obj->SetMinimum( 0. );
      gStyle->SetPalette(10, pCol4 );
      obj->SetOption( "colz" );
      return "";
    }    

    // Occupancy-like (10 x grays) plots
    if( name.find( "EEIT" ) < name.size() ) {
      obj->SetMinimum( 0. );
      gStyle->SetPalette(10, pCol4 );
      obj->SetOption( "colz" );
      return "";
    }

    if( name.find( "summary" ) < name.size() ) {
      gStyle->SetOptStat(" ");
      obj->SetOption( "col" );
      gStyle->SetPalette(6, pCol3);
      obj->GetXaxis()->SetNdivisions(18, kFALSE);
      obj->GetYaxis()->SetNdivisions(2);
      (data->pad)->SetGridx();
      (data->pad)->SetGridy();
      obj->SetMinimum(-0.00000001);
      obj->SetMaximum(6.0);
      gStyle->SetPaintTextFormat("+g");
      return "";
    }

    // Atypical plot...
    if( name.find( "EEMM event" ) < name.size() ) {
      gStyle->SetPalette( 10, pCol4 );
      obj->SetOption( "colz" );
      return "";
    }

    // Atypical plot...
    if( name.find( "EECLT" ) < name.size() ) {
      gStyle->SetPalette( 10, pCol4 );
      obj->SetOption( "colz" );
      return "";
    }

    // Quality-like (green, yellow, red) plots
    if( name.find( "entries or read crystals errors" ) < name.size() ) {
      obj->GetXaxis()->SetNdivisions( 86 );
      obj->GetYaxis()->SetNdivisions(  0 );
      (data->pad)->SetGridx( 1 );
      (data->pad)->SetGridy( 0 );
      obj->SetMinimum( -0.00000001 );
      obj->SetMaximum( 5.0 );
      gStyle->SetPalette( 6, pCol3 );
      obj->SetOption( "col" );
      return "";
    }

    // Atypical plot...
    if( name.find( "read crystal errors" ) < name.size() ) {
      obj->GetXaxis()->SetNdivisions( 0 );
      obj->GetYaxis()->SetNdivisions( 0 );
      obj->SetMinimum( -0.00000001 );
      obj->SetMaximum( 5.0 );
      gStyle->SetPalette( 6, pCol3 );
      obj->SetOption( "col" );
      return "";
    }    

    // Atypical plot...
    if( name.find( "EEBCT crystal on beam" ) < name.size() ||
	  name.find( "EEBCT crystal with maximum rec energy" ) < name.size() ) {
      obj->GetXaxis()->SetNdivisions( 17 );
      obj->GetYaxis()->SetNdivisions(  4 );
      obj->SetMinimum( 0.00000001 );
      gStyle->SetPalette( 1 );
      obj->SetOption( "colz" );
      return "";
    }    

    // Atypical plot...
    if( name.find( "EEBCT energy deposition in the 3x3" ) < name.size() ) {
      obj->SetLineColor( kRed );
      obj->SetFillColor( kRed );
      obj->SetOption( "box" );
      return "";
    }    
    
    // Atypical plot...
    if( name.find( "EEBHT" ) < name.size() ) {
      (data->pad)->SetGridx( 0 );
      (data->pad)->SetGridy( 0 );
      return "";
    }

    // Quality-like (green, yellow, red) plots
    if( name.find( "quality" ) < name.size() ) {
      obj->SetMinimum( -0.00000001 );
      obj->SetMaximum( 6.0 );
      gStyle->SetPalette( 6, pCol3 );
      obj->SetOption( "col" );
      return "";
    }

  }

  return "";    

}

std::string EEMDisplayPlugins::preDrawTH1F( DisplayData *data ) {

  TH1F* obj = dynamic_cast<TH1F*>( data->object );

  name = (data->object)->GetName();

  if( obj ) {

    // Apply to following...
    obj->SetStats(kTRUE);
    if ( obj->GetMaximum(1.e15) > 0. ) {
      gPad->SetLogy(1);
    } else {
      gPad->SetLogy(0);
    }

    if( name.find( "EEBCT readout crystals number" ) < name.size() ||
	name.find( "EEBCT rec energy cry" ) < name.size() ||
	name.find( "EEBCT found gains cry" ) < name.size() ||
	name.find( "EEBCT rec Ene sum" ) < name.size() ) {
      adjustRange( obj ); 
      gStyle->SetOptStat(1110);
      return "";
    }

    if( name.find( "EEBCT crystals done" ) < name.size() ) {
      text5->Reset();
      for( int i=1; i<1701; i++ ) {
	int step = int( obj->GetBinContent( i ) );
	if( step > 0 ) text5->SetBinContent( step+1, 1, i );
      }
      adjustRange( obj ); 
      gStyle->SetOptStat( "e" );
      return "";      
    }						    

    if( name.find( "EEBCT readout crystals errors" ) < name.size() ||
	name.find( "EEBCT Desynchronization" ) < name.size() ||
	name.find( "EEBCT E1 in the max cry" ) < name.size() ||
	name.find( "EEBCT number of entries" ) < name.size() ) {
      adjustRange( obj ); 
      gStyle->SetOptStat( "e" );
      return "";
    }

    if( name.find( "EEBCT table is moving" ) < name.size() ) {
      gStyle->SetOptStat( "e" );
      return "";
    }
     
    if( name.find( "EEBCT average rec energy in the single" ) < name.size() ||
	name.find( "EEBCT average rec energy in the 3x3" ) < name.size() ) {
      obj->SetOption( "e" );
      return "";
    }

    gStyle->SetOptStat("euomr");

  }

  return "";

}

void EEMDisplayPlugins::postDraw( DisplayData *data ) {

  if( dynamic_cast<TProfile2D*>( data->object ) ) {
    postDrawTProfile2D( data );
  }
  else if( dynamic_cast<TH2F*>( data->object ) ) {
    postDrawTH2F( data );
  }

}

void EEMDisplayPlugins::postDrawTProfile2D( DisplayData *data ) {

  text1->Draw( "text,same" );

}


void EEMDisplayPlugins::postDrawTH2F( DisplayData *data ) {

  TH2F* obj = dynamic_cast<TH2F*>( data->object );

  name = (data->object)->GetName();

  if( obj ) {
    nbx = obj->GetNbinsX();
    nby = obj->GetNbinsY();
    if( nbx == 85 && nby == 20 ) {
      text1->Draw( "text,same" );
    }
    else if( nbx == 17 && nby == 4 ) {
      text2->Draw( "text,same" );
    }
    else if( nbx == 10 && nby == 5 ) {
      text3->Draw( "text,same" );
    }
    else if( nbx == 2 && nby == 1 ) {
      text4->Draw( "text,same" );
    }
    else if( nbx == 86 && nby == 1 ) {
      text5->Draw( "text90,same" );
    }
    else if( nbx == 360 && nby == 170 ) {
      text6->Draw( "text,same" );
      gStyle->SetPaintTextFormat();
    }
  }

}

