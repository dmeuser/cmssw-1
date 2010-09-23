// -*- C++ -*-
//
// Package:     Core
// Class  :     FWEveView
// 
// Implementation:
//     [Notes on implementation]
//
// Original Author:  Alja Mrak-Tadel
//         Created:  Thu Mar 16 14:11:32 CET 2010
// $Id: FWEveView.cc,v 1.30 2010/09/17 16:18:55 amraktad Exp $
//



#include <RVersion.h>
#include <boost/bind.hpp>
#include <stdexcept>


// user include files

#define private public  //!!! TODO add get/sets for camera zoom and FOV
#include "TGLOrthoCamera.h"
#include "TGLPerspectiveCamera.h"
#undef private
#include "TGLCameraGuide.h"

#include "TGLEmbeddedViewer.h"
#include "TEveViewer.h"
#include "TGLScenePad.h"
#include "TEveManager.h"
#include "TEveElement.h"
#include "TEveWindow.h"
#include "TEveScene.h"
#include "TEveCalo.h"

#include "Fireworks/Core/interface/FWEveView.h"
#include "Fireworks/Core/interface/CmsShowViewPopup.h"
#include "Fireworks/Core/interface/FWEventAnnotation.h"
#include "Fireworks/Core/interface/CmsAnnotation.h"
#include "Fireworks/Core/interface/FWGLEventHandler.h"
#include "Fireworks/Core/interface/FWViewContextMenuHandlerGL.h"
#include "Fireworks/Core/interface/FWConfiguration.h"
#include "Fireworks/Core/interface/FWColorManager.h"
#include "Fireworks/Core/interface/fwLog.h"
#include "Fireworks/Core/interface/Context.h"
#include "Fireworks/Core/interface/FWViewContext.h"
#include "Fireworks/Core/interface/FWViewEnergyScale.h"
//#include "Fireworks/Core/src/FWDialogBuilder.h"



namespace fireworks
{
class Context;
}

//
// constructors and destructor
//

FWEveView::FWEveView(TEveWindowSlot* iParent, FWViewType::EType type, unsigned int version) :
   FWViewBase(version),
   m_type(type),
   m_viewer(0),
   m_eventScene(0),
   m_ownedProducts(0),
   m_geoScene(0),
   m_overlayEventInfo(0),
   m_overlayLogo(0),
   m_energyMaxValAnnotation(0),
   m_cameraGuide(0),
   m_context(0),
   // style
#if ROOT_VERSION_CODE >= ROOT_VERSION(5,26,0)
   m_imageScale(this, "Image Scale", 1.0, 1.0, 6.0),
#endif
   m_eventInfoLevel(this, "Overlay Event Info", 0l, 0l, 2l),
   m_drawCMSLogo(this,"Show Logo",false),
   m_pointSmooth(this, "Smooth points", false),
   m_pointSize(this, "Point size", 1.0, 1.0, 10.0),
   m_lineSmooth(this, "Smooth lines", false),
   m_lineWidth(this,"Line width",1.0,1.0,10.0),
   m_lineOutlineScale(this, "Outline width scale", 1.0, 0.01, 10.0),
   m_lineWireframeScale(this, "Wireframe width scale", 1.0, 0.01, 10.0),
   m_showCameraGuide(this,"Show Camera Guide",false),
   // scales
   m_energyScaleMode(this, "ScaleMode", 1l, 1l, 2l),
   m_energyMaxAbsVal(this, "MaxAbsVal", 150.0, 0.01, 1000.0 ),
   m_energyMaxTowerHeight(this, "MaxTowerH", 100.0, 1.0, 300.0),

   m_viewContext(new FWViewContext())
{
   m_viewer = new TEveViewer(typeName().c_str());

   TGLEmbeddedViewer* embeddedViewer;
#if ROOT_VERSION_CODE >= ROOT_VERSION(5,25,4)
   embeddedViewer =  m_viewer->SpawnGLEmbeddedViewer(0);
#else
   embeddedViewer =  m_viewer->SpawnGLEmbeddedViewer();
#endif
   iParent->ReplaceWindow(m_viewer);
   gEve->GetViewers()->AddElement(m_viewer);

   m_eventScene =  gEve->SpawnNewScene(Form("EventScene %s", typeName().c_str()));
   m_ownedProducts = new TEveElementList("ViewSpecificProducts");
   m_eventScene->AddElement(m_ownedProducts);

   m_viewer->AddScene(m_eventScene);

   // spawn geo scene
   m_geoScene = gEve->SpawnNewScene(Form("GeoScene %s", typeName().c_str()));
   m_geoScene->GetGLScene()->SetSelectable(kFALSE);
   m_viewer->AddScene(m_geoScene);
   m_viewContextMenu.reset(new FWViewContextMenuHandlerGL(m_viewer));

   FWGLEventHandler* eh = new FWGLEventHandler((TGWindow*)embeddedViewer->GetGLWidget(), (TObject*)embeddedViewer);
   embeddedViewer->SetEventHandler(eh);
   eh->openSelectedModelContextMenu_.connect(openSelectedModelContextMenu_);
   eh->SetDoInternalSelection(kFALSE);
   FWViewContextMenuHandlerGL* ctxHand = new FWViewContextMenuHandlerGL(m_viewer);
   ctxHand->setPickCameraCenter(true);
   m_viewContextMenu.reset(ctxHand);
   
   // style params

   m_overlayEventInfo = new FWEventAnnotation(embeddedViewer);
   m_overlayEventInfo->setLevel(0);

   m_eventInfoLevel.addEntry(0, "Nothing");
   m_eventInfoLevel.addEntry(1, "Run / event");
   m_eventInfoLevel.addEntry(2, "Run / event / lumi");
   m_eventInfoLevel.addEntry(3, "Full");
   m_eventInfoLevel.changed_.connect(boost::bind(&FWEventAnnotation::setLevel,m_overlayEventInfo, _1));
   
   m_overlayLogo = new CmsAnnotation(embeddedViewer, 0.02, 0.98);
   m_overlayLogo->setVisible(false);
   m_drawCMSLogo.changed_.connect(boost::bind(&CmsAnnotation::setVisible,m_overlayLogo, _1));

   m_cameraGuide = new TGLCameraGuide(0.9, 0.1, 0.08);
   m_cameraGuide->SetState(TGLOverlayElement::kInvisible);
   embeddedViewer->AddOverlayElement(m_cameraGuide);
   m_showCameraGuide.changed_.connect(boost::bind(&TGLCameraGuide::SetBinaryState,m_cameraGuide, _1));

   m_pointSmooth.changed_.connect(boost::bind(&FWEveView::pointLineScalesChanged,this));
   m_pointSize.changed_.connect(boost::bind(&FWEveView::pointLineScalesChanged,this));
   m_lineSmooth.changed_.connect(boost::bind(&FWEveView::pointLineScalesChanged,this));
   m_lineWidth.changed_.connect(boost::bind(&FWEveView::pointLineScalesChanged,this));
   m_lineOutlineScale.changed_.connect(boost::bind(&FWEveView::pointLineScalesChanged,this));
   m_lineWireframeScale.changed_.connect(boost::bind(&FWEveView::pointLineScalesChanged,this));


   // scale params

   m_energyScaleMode.addEntry(kFixedScale,   "FixedScale");
   m_energyScaleMode.addEntry(kAutoScale,    "AutoScale");
   m_energyScaleMode.addEntry(kCombinedScale,"CombinedScale");
   m_energyScaleMode.changed_.connect(boost::bind(&FWEveView::updateEnergyScales,this));

   m_energyMaxAbsVal.changed_.connect(boost::bind(&FWEveView::updateEnergyScales,this));
   m_energyMaxTowerHeight.changed_.connect(boost::bind(&FWEveView::setMaxTowerHeight,this));
}

FWEveView::~FWEveView()
{
   m_geoScene->RemoveElements();
   m_ownedProducts->Destroy();
   viewerGL()->DeleteOverlayElements(TGLOverlayElement::kAll);
   m_viewer->DestroyWindowAndSlot();
}

//______________________________________________________________________________
// const member functions

const std::string& 
FWEveView::typeName() const
{
   return m_type.name();
}

FWViewContextMenuHandlerBase* 
FWEveView::contextMenuHandler() const {
   return (FWViewContextMenuHandlerBase*)m_viewContextMenu.get();
}

TGLViewer* 
FWEveView::viewerGL() const
{
   return  m_viewer->GetGLViewer();
}

void
FWEveView::saveImageTo(const std::string& iName) const
{
   bool succeeded = false;
#if ROOT_VERSION_CODE >= ROOT_VERSION(5,26,0)
   succeeded = viewerGL()->SavePictureScale(iName, m_imageScale.value());
#else
   succeeded = viewerGL()->SavePicture(iName.c_str());
#endif

   if(!succeeded) {
      throw std::runtime_error("Unable to save picture");
   }
   fwLog(fwlog::kInfo) <<  "Saved image " << iName << std::endl;
}

//-------------------------------------------------------------------------------
void
FWEveView::pointLineScalesChanged()
{
   viewerGL()->SetSmoothPoints(m_pointSmooth.value());
   viewerGL()->SetPointScale  (m_pointSize.value());
   viewerGL()->SetSmoothLines (m_lineSmooth.value());
   viewerGL()->SetLineScale   (m_lineWidth.value());
   viewerGL()->SetOLLineW     (m_lineOutlineScale.value());
   viewerGL()->SetWFLineW     (m_lineWireframeScale.value());
   viewerGL()->RequestDraw();
}

void
FWEveView::eventBegin()
{
}

void
FWEveView::eventEnd()
{
   m_overlayEventInfo->setEvent();
   updateEnergyScales();
}

void
FWEveView::setBackgroundColor(Color_t iColor)
{
   FWColorManager::setColorSetViewer(viewerGL(), iColor);
}

void
FWEveView::resetCamera()
{
   viewerGL()->ResetCurrentCamera();
}

void
FWEveView::updateEnergyScales()
{
   // TODO check here GLOBAL ...

   TEveCaloViz* calo = getEveCalo();
   if (calo)
   {
      calo->SetMaxValAbs(m_energyMaxAbsVal.value());
      if (m_energyScaleMode.value() == kFixedScale)
      {
         if (calo->GetScaleAbs() == false)
         {
            calo->SetScaleAbs(true);
         }
      }
      else if (m_energyScaleMode.value() == kAutoScale)
      {
         if (calo->GetScaleAbs()) 
         {
            calo->SetScaleAbs(false);
         }
      }
      else if (m_energyScaleMode.value() == kCombinedScale)
      {
         float dataMax = calo->GetData()->GetMaxVal(calo->GetPlotEt());
         bool abs = (m_energyMaxAbsVal.value() >= dataMax);

         if (abs != calo->GetScaleAbs())
         {
            calo->SetScaleAbs(abs);

            // draw annotation
            if (abs)
            {
               if (m_energyMaxValAnnotation) viewerGL()->RemoveOverlayElement(m_energyMaxValAnnotation);
            }
            else
            {
               m_energyMaxValAnnotation = new TGLAnnotation(viewerGL(), Form("Et %f", dataMax), 0.1, 0.9);
               m_energyMaxValAnnotation->SetRole(TGLOverlayElement::kViewer);
               m_energyMaxValAnnotation->SetUseColorSet(false);
               m_energyMaxValAnnotation->SetTextSize(0.05);
               m_energyMaxValAnnotation->SetTextColor(kMagenta);
            }
            fwLog(fwlog::kInfo) << typeName() << Form("Scale mode has changed %s  AbsVal[%f] < CaloMaxVal[%f]", abs ? "Fixed" :"Automatic",
                                                      m_energyMaxAbsVal.value(), dataMax) << std::endl;
         }
      }

      energyScalesChanged();
   }
}

/* Emit signal to proxy builders when scale have changes */
void
FWEveView::energyScalesChanged()
{
   if (getEveCalo()) 
   {
      viewContext()->getEnergyScale("Calo")->setVal(getEveCalo()->GetValToHeight());
      viewContext()->scaleChanged();
      getEveCalo()->ElementChanged();
      gEve->Redraw3D();
   }
}
void
FWEveView::setMaxTowerHeight()
{
   TEveCaloViz* calo = getEveCalo();
   if (calo && (typeId() != FWViewType::kLego && typeId() != FWViewType::kLegoHF))
   {
      calo->SetMaxTowerH(m_energyMaxTowerHeight.value());
      energyScalesChanged();
   }
}

//-------------------------------------------------------------------------------
void
FWEveView::addTo(FWConfiguration& iTo) const
{
   // take care of parameters
   FWConfigurableParameterizable::addTo(iTo);
   
   { 
      assert ( m_overlayEventInfo );
      m_overlayEventInfo->addTo(iTo);
   }
   { 
      assert ( m_overlayLogo );
      m_overlayLogo->addTo(iTo);
   }
}

void
FWEveView::setFrom(const FWConfiguration& iFrom)
{
   // Make sure you change the version ranges here
   // whenever you update the logic.
   // The rationale should be:
   // (version range supported by the next block) && (version range in the configuration file)
   //
   // This is not "forward" compatible, but I don't think
   // we care.
   if (version() >= 2 && iFrom.version() >= 1)
   {
      for(const_iterator it =begin(), itEnd = end();
          it != itEnd;
          ++it) {
         (*it)->setFrom(iFrom);      
      }  
   }
   if (iFrom.version() > 1)
   {
      assert( m_overlayEventInfo);
      m_overlayEventInfo->setFrom(iFrom);
   }
   {
      assert( m_overlayLogo);
      m_overlayLogo->setFrom(iFrom);
   }
}

//______________________________________________________________________________


void
FWEveView::addToOrthoCamera(TGLOrthoCamera* camera, FWConfiguration& iTo) const
{
   // zoom
   std::ostringstream s;
   s<<(camera->fZoom);
   std::string name("cameraZoom");
   iTo.addKeyValue(name+typeName(),FWConfiguration(s.str()));
   
   // transformation matrix
   std::string matrixName("cameraMatrix");
   for ( unsigned int i = 0; i < 16; ++i ) {
      std::ostringstream osIndex;
      osIndex << i;
      std::ostringstream osValue;
      osValue << camera->GetCamTrans()[i];
      iTo.addKeyValue(matrixName+osIndex.str()+typeName(),FWConfiguration(osValue.str()));
   }   
}

void
FWEveView::setFromOrthoCamera(TGLOrthoCamera* camera,  const FWConfiguration& iFrom)
{
   try {
      // zoom
      std::string zoomName("cameraZoom"); zoomName += typeName();
      if (iFrom.valueForKey(zoomName) == 0 )
      {
         throw std::runtime_error("can't restore parameter cameraZoom");
      }
      std::istringstream s(iFrom.valueForKey(zoomName)->value());
      s>>(camera->fZoom);
      
      // transformation matrix
      std::string matrixName("cameraMatrix");
      for ( unsigned int i = 0; i < 16; ++i ) {
         std::ostringstream os;
         os << i;
         const FWConfiguration* value = iFrom.valueForKey( matrixName + os.str() + typeName() );
         if ( value ==  0 )
         {
            throw std::runtime_error ("can't restore parameter cameraMatrix.");
         }
         std::istringstream s(value->value());
         s>> (camera->RefCamTrans()[i]);
      }
   }
   catch (const std::runtime_error& iException)
   {
      fwLog(fwlog::kInfo) << "Caught exception while restoring camera parameters in view " << typeName() << "\n.";
      viewerGL()->ResetCamerasAfterNextUpdate();      

   }
   camera->IncTimeStamp();
}
 
void
FWEveView::addToPerspectiveCamera(TGLPerspectiveCamera* cam, const std::string& name, FWConfiguration& iTo) const
{   
   // transformation matrix
   std::string matrixName("cameraMatrix");
   for ( unsigned int i = 0; i < 16; ++i ){
      std::ostringstream osIndex;
      osIndex << i;
      std::ostringstream osValue;
      osValue << (cam->GetCamTrans())[i];
      iTo.addKeyValue(matrixName+osIndex.str()+name,FWConfiguration(osValue.str()));
   }
   
   // transformation matrix base
   matrixName = "cameraMatrixBase";
   for ( unsigned int i = 0; i < 16; ++i ){
      std::ostringstream osIndex;
      osIndex << i;
      std::ostringstream osValue;
      osValue << (cam->GetCamBase())[i];
      iTo.addKeyValue(matrixName+osIndex.str()+name,FWConfiguration(osValue.str()));
   }
   {
      std::ostringstream osValue;
      osValue << cam->fFOV;
      iTo.addKeyValue(name+" FOV",FWConfiguration(osValue.str()));
   }
}

void
FWEveView::setFromPerspectiveCamera(TGLPerspectiveCamera* cam, const std::string& name, const FWConfiguration& iFrom)
{
   try {
      std::string matrixName("cameraMatrix");
      for ( unsigned int i = 0; i < 16; ++i ){
         std::ostringstream os;
         os << i;
         const FWConfiguration* value = iFrom.valueForKey( matrixName + os.str() + name );
         if ( value ==  0 )
         {
            throw std::runtime_error ("can't restore parameter cameraMatrix.");
         }
         std::istringstream s(value->value());
         s>>((cam->RefCamTrans())[i]);
      }
      
      // transformation matrix base
      matrixName = "cameraMatrixBase";
      for ( unsigned int i = 0; i < 16; ++i ){
         std::ostringstream os;
         os << i;
         const FWConfiguration* value = iFrom.valueForKey( matrixName + os.str() + name );
         if ( value ==  0 )
         {
            throw std::runtime_error ("can't restore parameter cameraMatrixBase.");
         }
     
         std::istringstream s(value->value());
         s>>((cam->RefCamBase())[i]);
      }
      
      {
         const FWConfiguration* value = iFrom.valueForKey( name + " FOV" );
         if ( value ==  0 )
         {
            throw std::runtime_error ("can't restore parameter cameraMatrixBase.");
         }
         std::istringstream s(value->value());
         s>>cam->fFOV;
      }
      
      cam->IncTimeStamp();
   }
   catch (const std::runtime_error& iException)
   {
      fwLog(fwlog::kInfo) << "Caught exception while restoring camera parameters in view " << typeName() << "\n.";
      viewerGL()->ResetCamerasAfterNextUpdate();  
      fwLog(fwlog::kDebug) << "Reset camera fo view "  << typeName() << "\n.";
   }
}


void 
FWEveView::populateController(ViewerParameterGUI& gui) const
{
   gui.requestTab("Style").
      addParam(&m_eventInfoLevel).
      addParam(&m_drawCMSLogo).
      addParam(&m_showCameraGuide).
      separator().
#if ROOT_VERSION_CODE >= ROOT_VERSION(5,26,0)
      addParam(&m_imageScale).
#endif
      addParam(&m_pointSize).
      addParam(&m_pointSmooth).
      addParam(&m_lineSmooth).
      addParam(&m_lineWidth).
      addParam(&m_lineOutlineScale).
      addParam(&m_lineWireframeScale);

   if (getEveCalo())
   {
      gui.requestTab("Scales").
         addParam(&m_energyScaleMode).
         addParam(&m_energyMaxAbsVal);

      if (typeId() != FWViewType::kLego && typeId() != FWViewType::kLegoHF )
         gui.requestTab("Scales").
            addParam(&m_energyMaxTowerHeight);
   }
}
