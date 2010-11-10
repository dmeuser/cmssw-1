#ifndef Fireworks_Core_CmsShowViewPopup_h
#define Fireworks_Core_CmsShowViewPopup_h
// -*- C++ -*-
//
// Package:     Core
// Class  :     CmsShowViewPopup
//
/**\class CmsShowViewPopup CmsShowViewPopup.h Fireworks/Core/interface/CmsShowViewPopup.h

   Description: <one line class summary>

   Usage:
    <usage>

 */
//
// Original Author:
//         Created:  Wed Jun 25 15:15:12 EDT 2008
// $Id: CmsShowViewPopup.h,v 1.19 2010/09/24 18:51:18 amraktad Exp $
//

// system include files
#include <vector>
#include <boost/shared_ptr.hpp>
#include <sigc++/sigc++.h>

#include "TGFrame.h"

// user include files
#include "Fireworks/Core/interface/FWParameterSetterEditorBase.h"

// forward declarations
class TGLabel;
class TGTextButton;
class TGButton;
class TGFrame;
class TGTab;
class TGCompositeFrame;
class TEveWindow;

class FWParameterBase;
class FWParameterSetterBase;
class FWViewBase;
class FWColorManager;
class FWDialogBuilder;

/* Helper class to organise parameters in view controller */
class ViewerParameterGUI : public TGCompositeFrame, public FWParameterSetterEditorBase
{
public:
   ViewerParameterGUI(const TGFrame*);
   virtual ~ViewerParameterGUI(){}

   ViewerParameterGUI& requestTab(const char*);
   ViewerParameterGUI& addParam(const FWParameterBase*);
   ViewerParameterGUI& separator();
   void reset();
   void populateComplete();

   TGCompositeFrame* getTabContainer();
   void addFrameToContainer(TGCompositeFrame*);
   

private:
   TGTab* m_tab;
   std::string m_selectedTabName;
   std::vector<boost::shared_ptr<FWParameterSetterBase> > m_setters;
}; 

//==============================================================================

class CmsShowViewPopup : public TGTransientFrame
{
public:
   CmsShowViewPopup(const TGWindow* p = 0, UInt_t w = 200, UInt_t h = 200, FWColorManager* cm=0, FWViewBase* wb=0, TEveWindow* ew = 0);
   virtual ~CmsShowViewPopup();

   // ---------- const member functions ---------------------

   // ---------- static member functions --------------------

   // ---------- member functions ---------------------------
   virtual void CloseWindow();
   virtual void MapWindow();
   virtual void UnmapWindow();

   bool mapped() { return m_mapped; }

   void reset(FWViewBase*, TEveWindow* ew);

   void saveImage();
   void changeBackground();
   void backgroundColorWasChanged();
   TEveWindow* getEveWindow() const { return m_eveWindow; }

   sigc::signal<void> closed_;

private:
   CmsShowViewPopup(const CmsShowViewPopup&);    // stop default

   const CmsShowViewPopup& operator=(const CmsShowViewPopup&);    // stop default

   // ---------- member data --------------------------------
   bool              m_mapped;

   TGLabel*          m_viewLabel;
   ViewerParameterGUI*     m_paramGUI;
   TGButton*         m_saveImageButton;
   TGTextButton*     m_changeBackground;

   FWColorManager* m_colorManager;
   FWViewBase*     m_viewBase;
   TEveWindow*     m_eveWindow;

   ClassDef(CmsShowViewPopup, 1);
};


#endif
