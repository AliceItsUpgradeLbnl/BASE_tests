// Mainframe macro generated from application: /Users/ploskon/devel/hepsoft/root/v5-34-34/bin/root.exe
// By ROOT version 5.34/34 on 2016-03-28 02:54:13

#ifndef ROOT_TGDockableFrame
#include "TGDockableFrame.h"
#endif
#ifndef ROOT_TGMenu
#include "TGMenu.h"
#endif
#ifndef ROOT_TGMdiDecorFrame
#include "TGMdiDecorFrame.h"
#endif
#ifndef ROOT_TG3DLine
#include "TG3DLine.h"
#endif
#ifndef ROOT_TGMdiFrame
#include "TGMdiFrame.h"
#endif
#ifndef ROOT_TGMdiMainFrame
#include "TGMdiMainFrame.h"
#endif
#ifndef ROOT_TGMdiMenu
#include "TGMdiMenu.h"
#endif
#ifndef ROOT_TGListBox
#include "TGListBox.h"
#endif
#ifndef ROOT_TGNumberEntry
#include "TGNumberEntry.h"
#endif
#ifndef ROOT_TGScrollBar
#include "TGScrollBar.h"
#endif
#ifndef ROOT_TGComboBox
#include "TGComboBox.h"
#endif
#ifndef ROOT_TGuiBldHintsEditor
#include "TGuiBldHintsEditor.h"
#endif
#ifndef ROOT_TGuiBldNameFrame
#include "TGuiBldNameFrame.h"
#endif
#ifndef ROOT_TGFrame
#include "TGFrame.h"
#endif
#ifndef ROOT_TGFileDialog
#include "TGFileDialog.h"
#endif
#ifndef ROOT_TGShutter
#include "TGShutter.h"
#endif
#ifndef ROOT_TGButtonGroup
#include "TGButtonGroup.h"
#endif
#ifndef ROOT_TGCanvas
#include "TGCanvas.h"
#endif
#ifndef ROOT_TGFSContainer
#include "TGFSContainer.h"
#endif
#ifndef ROOT_TGuiBldEditor
#include "TGuiBldEditor.h"
#endif
#ifndef ROOT_TGColorSelect
#include "TGColorSelect.h"
#endif
#ifndef ROOT_TGButton
#include "TGButton.h"
#endif
#ifndef ROOT_TGFSComboBox
#include "TGFSComboBox.h"
#endif
#ifndef ROOT_TGLabel
#include "TGLabel.h"
#endif
#ifndef ROOT_TRootGuiBuilder
#include "TRootGuiBuilder.h"
#endif
#ifndef ROOT_TGTab
#include "TGTab.h"
#endif
#ifndef ROOT_TGListView
#include "TGListView.h"
#endif
#ifndef ROOT_TGStatusBar
#include "TGStatusBar.h"
#endif
#ifndef ROOT_TGListTree
#include "TGListTree.h"
#endif
#ifndef ROOT_TGuiBldGeometryFrame
#include "TGuiBldGeometryFrame.h"
#endif
#ifndef ROOT_TGToolBar
#include "TGToolBar.h"
#endif
#ifndef ROOT_TGuiBldDragManager
#include "TGuiBldDragManager.h"
#endif
#ifndef ROOT_TGObject
#include "TGObject.h"
#endif

#include "Riostream.h"

void buttons()
{

   // main frame
   TGMainFrame *fMainFrame683 = new TGMainFrame(gClient->GetRoot(),10,10,kMainFrame | kVerticalFrame);
   fMainFrame683->SetName("fMainFrame683");
   fMainFrame683->SetLayoutBroken(kTRUE);
   TGRadioButton *fTextButton558 = new TGRadioButton(fMainFrame683,"fTextButton558");
   fTextButton558->SetTextJustify(36);
   fTextButton558->SetMargins(0,0,0,0);
   fTextButton558->SetWrapLength(-1);
   fMainFrame683->AddFrame(fTextButton558, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fTextButton558->MoveResize(64,144,112,18);
   TGCheckButton *fTextButton563 = new TGCheckButton(fMainFrame683,"fTextButton563");
   fTextButton563->SetTextJustify(36);
   fTextButton563->SetMargins(0,0,0,0);
   fTextButton563->SetWrapLength(-1);
   fMainFrame683->AddFrame(fTextButton563, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fTextButton563->MoveResize(72,216,115,18);

   fMainFrame683->SetMWMHints(kMWMDecorAll,
                        kMWMFuncAll,
                        kMWMInputModeless);
   fMainFrame683->MapSubwindows();

   fMainFrame683->Resize(fMainFrame683->GetDefaultSize());
   fMainFrame683->MapWindow();
   fMainFrame683->Resize(490,372);
}  
