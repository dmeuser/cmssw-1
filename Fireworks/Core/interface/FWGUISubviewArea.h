#ifndef Fireworks_Core_FWGUISubviewArea_h
#define Fireworks_Core_FWGUISubviewArea_h
// -*- C++ -*-
//
// Package:     Core
// Class  :     FWGUISubviewArea
// 
/**\class FWGUISubviewArea FWGUISubviewArea.h Fireworks/Core/interface/FWGUISubviewArea.h

 Description: Manages the GUI area where a sub Subview is displayed

 Usage:
    <usage>

*/
//
// Original Author:  Chris Jones
//         Created:  Fri Feb 15 14:13:29 EST 2008
// $Id: FWGUISubviewArea.h,v 1.5 2008/07/07 00:17:32 chrjones Exp $
//

// system include files
#include "TGFrame.h"
#include <sigc++/signal.h>
// user include files

// forward declarations
class TGSplitFrame;
class TGButton;

class FWGUISubviewArea : public TGVerticalFrame
{

   public:
      FWGUISubviewArea(unsigned int iIndex, const TGSplitFrame *iParent,TGSplitFrame*);
      virtual ~FWGUISubviewArea();

      // ---------- const member functions ---------------------
      //index says which sub area this occupies [used for configuration saving]
      unsigned int index() const {
         return m_index;
      }
   
      bool isDocked() const {
         return m_docked;
      }
      // ---------- static member functions --------------------
      static const TGPicture * swapIcon();
      static const TGPicture * undockIcon();
      static const TGPicture * closeIcon();
   
      // ---------- member functions ---------------------------
      void swapToBigView();
      void destroy();
      void undock();
   
      void beingDocked(TGFrame*);
   
      void enableDestructionButton(bool);
      void setIndex(unsigned int iIndex);
      void enableSwapButton(bool);
      sigc::signal<void,unsigned int> swappedToBigView_;
      sigc::signal<void,unsigned int> goingToBeDestroyed_;
   
      sigc::signal<void> bigViewUndocked_;
      sigc::signal<void> bigViewDocked_;
   private:
      FWGUISubviewArea(const FWGUISubviewArea&); // stop default

      const FWGUISubviewArea& operator=(const FWGUISubviewArea&); // stop default

      // ---------- member data --------------------------------
      TGSplitFrame* m_mainSplit;
      unsigned int m_index;
      TGButton* m_swapButton;
      TGButton* m_undockButton;
      TGButton* m_closeButton;
      TGCompositeFrame* m_buttons;
   
      bool m_undockedSwappableView;
      bool m_undockedDestructabledView;
      bool m_docked;
};


#endif
