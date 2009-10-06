// $Id: FWTriggerTableViewTableManager.cc,v 1.9 2009/08/26 22:21:45 dmytro Exp $

#include <math.h>
#include "TClass.h"
#include "TGClient.h"
#include "Fireworks/Core/interface/FWTriggerTableViewTableManager.h"
#include "Fireworks/Core/interface/FWTriggerTableViewManager.h"
#include "Fireworks/Core/interface/FWTriggerTableView.h"
#include "Fireworks/Core/interface/FWEventItem.h"
#include "Fireworks/Core/interface/FWColorManager.h"
#include "Fireworks/TableWidget/interface/FWTableWidget.h"

FWTriggerTableViewTableManager::FWTriggerTableViewTableManager (const FWTriggerTableView *view)
   : m_view(view),
     m_graphicsContext(0),
     m_renderer(0)
{
   GCValues_t gc = *(m_view->m_tableWidget->GetWhiteGC().GetAttributes());
   m_graphicsContext = gClient->GetResourcePool()->GetGCPool()->GetGC(&gc,kTRUE);
   m_highlightContext = gClient->GetResourcePool()->GetGCPool()->GetGC(&gc,kTRUE);
   m_highlightContext->SetForeground(gVirtualX->GetPixel(kBlue));
   m_highlightContext->SetBackground(gVirtualX->GetPixel(kBlue));
   m_renderer = new FWTextTableCellRenderer(m_graphicsContext,m_highlightContext);
}

FWTriggerTableViewTableManager::~FWTriggerTableViewTableManager ()
{
   delete m_renderer;
}

int
FWTriggerTableViewTableManager::numberOfRows() const
{
   // number of triggers
   if ( !m_view->m_columns.empty() )
      return m_view->m_columns.front().values.size();
   else
      return 0;
}

int
FWTriggerTableViewTableManager::numberOfColumns() const
{
   // Columns:
   // * trigger name
   // * passed/failed
   //
   // in future
   // * how often passed in the current file (%)
   // * how often passed in the current file with current selection(%)
   return m_view->m_columns.size();
}

std::vector<std::string>
FWTriggerTableViewTableManager::getTitles () const
{
   unsigned int n = numberOfColumns();
   std::vector<std::string> ret;
   ret.reserve(n);
   for (unsigned int i = 0; i < n; ++i) {
      ret.push_back(m_view->m_columns.at(i).title);
//        printf("%s\n", ret.back().c_str());
   }
   return ret;
}

int
FWTriggerTableViewTableManager::unsortedRowNumber(int iSortedRowNumber) const
{
//      printf("%d indices, returning %d (%d)\n", (int)m_sortedToUnsortedIndices.size(),
//          iSortedRowNumber,
//          iSortedRowNumber < (int)m_sortedToUnsortedIndices.size() ? m_sortedToUnsortedIndices[iSortedRowNumber] : -1);
   if (iSortedRowNumber >= (int)m_sortedToUnsortedIndices.size())
      return 0;
   return m_sortedToUnsortedIndices[iSortedRowNumber];
}

FWTableCellRendererBase
*FWTriggerTableViewTableManager::cellRenderer(int iSortedRowNumber,
                                              int iCol) const
{
   const int realRowNumber = unsortedRowNumber(iSortedRowNumber);
   if ( !m_view->m_columns.empty() &&
        int(m_view->m_columns.size())>iCol &&
        int(m_view->m_columns.front().values.size())>realRowNumber ) {
      if (m_view->m_manager->colorManager().background() == kBlack)
         m_graphicsContext->SetForeground(0xe0e0e0);
      else
         m_graphicsContext->SetForeground(0x404040);
      m_renderer->setData(m_view->m_columns.at(iCol).values.at(realRowNumber), false);
   } else {
      m_renderer->setData("invalid", false);
   }
   return m_renderer;
}

void
FWTriggerTableViewTableManager::dataChanged()
{
   m_sortedToUnsortedIndices.clear();
   for ( int i=0; i< numberOfRows(); ++i)
      m_sortedToUnsortedIndices.push_back(i);
   FWTableManagerBase::dataChanged();
}

namespace {
   template <typename TMap>
   void doSort(int col,
               const std::vector<FWTriggerTableView::Column>& iData,
               TMap& iOrdered,
               std::vector<int>& oRowToIndex)
   {
      unsigned int index=0;
      for(std::vector<std::string>::const_iterator it = iData.at(col).values.begin(),
          itEnd = iData.at(col).values.end(); it!=itEnd; ++it,++index) {
         iOrdered.insert(std::make_pair(*it,index));
      }
      unsigned int row = 0;
      for(typename TMap::iterator it = iOrdered.begin(),
          itEnd = iOrdered.end();
          it != itEnd;
          ++it,++row) {
         oRowToIndex[row]=it->second;
      }
   }
}

void
FWTriggerTableViewTableManager::implSort(int col, bool sortOrder)
{
   if(sortOrder) {
      std::multimap<std::string,int,std::greater<std::string> > ordered;
      doSort(col, m_view->m_columns, ordered, m_sortedToUnsortedIndices);
   } else {
      std::multimap<std::string,int> ordered;
      doSort(col, m_view->m_columns, ordered, m_sortedToUnsortedIndices);
   }
}

