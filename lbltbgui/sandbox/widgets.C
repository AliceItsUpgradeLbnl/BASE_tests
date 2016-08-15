// Mainframe macro generated from application: /Users/ploskon/devel/hepsoft/root/v5-34-34/bin/root.exe
// By ROOT version 5.34/34 on 2016-03-25 10:40:35

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
#ifndef ROOT_TGSlider
#include "TGSlider.h"
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

void widgets()
{

   // main frame
   TGMainFrame *fMainFrame954 = new TGMainFrame(gClient->GetRoot(),10,10,kMainFrame | kVerticalFrame);
   fMainFrame954->SetName("fMainFrame954");
   fMainFrame954->SetLayoutBroken(kTRUE);

   // tab widget
   TGTab *fTab612 = new TGTab(fMainFrame954,728,232);

   // container of "Tab1"
   TGCompositeFrame *fCompositeFrame615;
   fCompositeFrame615 = fTab612->AddTab("Tab1");
   fCompositeFrame615->SetLayoutManager(new TGVerticalLayout(fCompositeFrame615));


   // container of "Tab2"
   TGCompositeFrame *fCompositeFrame617;
   fCompositeFrame617 = fTab612->AddTab("Tab2");
   fCompositeFrame617->SetLayoutManager(new TGVerticalLayout(fCompositeFrame617));

   // canvas widget
   TGCanvas *fCanvas644 = new TGCanvas(fCompositeFrame617,128,88,kSunkenFrame);

   // canvas viewport
   TGViewPort *fViewPort645 = fCanvas644->GetViewPort();

   // list tree
   TGListTree *fListTree654 = new TGListTree(fCanvas644,kHorizontalFrame);

   const TGPicture *popen;       //used for list tree items
   const TGPicture *pclose;      //used for list tree items

   TGListTreeItem *item0 = fListTree654->AddItem(NULL,"Entry 1");
   popen = gClient->GetPicture("ofolder_t.xpm");
   pclose = gClient->GetPicture("folder_t.xpm");
   item0->SetPictures(popen, pclose);
   fListTree654->CloseItem(item0);
   TGListTreeItem *item1 = fListTree654->AddItem(NULL,"Entry 2");
   item1->SetPictures(popen, pclose);
   fListTree654->CloseItem(item1);
   TGListTreeItem *item2 = fListTree654->AddItem(NULL,"Entry 3");
   item2->SetPictures(popen, pclose);
   fListTree654->CloseItem(item2);
   TGListTreeItem *item3 = fListTree654->AddItem(NULL,"Entry 4");
   item3->SetPictures(popen, pclose);
   fListTree654->CloseItem(item3);
   TGListTreeItem *item4 = fListTree654->AddItem(NULL,"Entry 5");
   item4->SetPictures(popen, pclose);
   fListTree654->CloseItem(item4);

   fViewPort645->AddFrame(fListTree654);
   fListTree654->SetLayoutManager(new TGHorizontalLayout(fListTree654));
   fListTree654->MapSubwindows();
   fCanvas644->SetContainer(fListTree654);
   fCanvas644->MapSubwindows();
   fCanvas644->SetVsbPosition(0);
   fCompositeFrame617->AddFrame(fCanvas644, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));

   // embedded canvas
   TRootEmbeddedCanvas *fRootEmbeddedCanvas659 = new TRootEmbeddedCanvas(0,fCompositeFrame617,504,102,kSunkenFrame);
   Int_t wfRootEmbeddedCanvas659 = fRootEmbeddedCanvas659->GetCanvasWindowId();
   TCanvas *c123 = new TCanvas("c123", 10, 10, wfRootEmbeddedCanvas659);
   fRootEmbeddedCanvas659->AdoptCanvas(c123);
   fCompositeFrame617->AddFrame(fRootEmbeddedCanvas659, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   TGNumberEntry *fNumberEntry675 = new TGNumberEntry(fCompositeFrame617, (Double_t) 0,6,-1,(TGNumberFormat::EStyle) 5);
   fCompositeFrame617->AddFrame(fNumberEntry675, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   TGNumberEntry *fNumberEntry687 = new TGNumberEntry(fCompositeFrame617, (Double_t) 0,9,-1,(TGNumberFormat::EStyle) 5);
   fCompositeFrame617->AddFrame(fNumberEntry687, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));

   ULong_t ucolor;        // will reflect user color changes
   gClient->GetColorByName("#ffffff",ucolor);

   // combo box
   TGComboBox *fComboBox697 = new TGComboBox(fCompositeFrame617,-1,kHorizontalFrame | kSunkenFrame | kOwnBackground);
   fComboBox697->AddEntry("Entry 1 ",0);
   fComboBox697->AddEntry("Entry 2 ",1);
   fComboBox697->AddEntry("Entry 3 ",2);
   fComboBox697->AddEntry("Entry 4 ",3);
   fComboBox697->AddEntry("Entry 5 ",4);
   fComboBox697->AddEntry("Entry 6 ",5);
   fComboBox697->AddEntry("Entry 7 ",6);
   fComboBox697->Resize(102,21);
   fComboBox697->Select(-1);
   fCompositeFrame617->AddFrame(fComboBox697, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   TGVSlider *fVSlider721 = new TGVSlider(fCompositeFrame617,134,kSlider1 | kScaleBoth,-1,kVerticalFrame);
   fVSlider721->SetRange(0,40);
   fVSlider721->SetPosition(20);
   fCompositeFrame617->AddFrame(fVSlider721, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));

   // status bar
   TGStatusBar *fStatusBar728 = new TGStatusBar(fCompositeFrame617,724,32);
   fCompositeFrame617->AddFrame(fStatusBar728, new TGLayoutHints(kLHintsBottom | kLHintsExpandX));
   TGTextButton *fTextButton736 = new TGTextButton(fCompositeFrame617,"fTextButton736",-1,TGTextButton::GetDefaultGC()(),TGTextButton::GetDefaultFontStruct(),kRaisedFrame);
   fTextButton736->SetTextJustify(36);
   fTextButton736->SetMargins(0,0,0,0);
   fTextButton736->SetWrapLength(-1);
   fTextButton736->Resize(96,21);
   fCompositeFrame617->AddFrame(fTextButton736, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   TGTextButton *fTextButton743 = new TGTextButton(fCompositeFrame617,"fTextButton743",-1,TGTextButton::GetDefaultGC()(),TGTextButton::GetDefaultFontStruct(),kRaisedFrame);
   fTextButton743->SetTextJustify(36);
   fTextButton743->SetMargins(0,0,0,0);
   fTextButton743->SetWrapLength(-1);
   fTextButton743->Resize(96,21);
   fCompositeFrame617->AddFrame(fTextButton743, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   TGRadioButton *fTextButton750 = new TGRadioButton(fCompositeFrame617,"fTextButton750");
   fTextButton750->SetTextJustify(36);
   fTextButton750->SetMargins(0,0,0,0);
   fTextButton750->SetWrapLength(-1);
   fCompositeFrame617->AddFrame(fTextButton750, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   TGCheckButton *fTextButton757 = new TGCheckButton(fCompositeFrame617,"fTextButton757");
   fTextButton757->SetTextJustify(36);
   fTextButton757->SetMargins(0,0,0,0);
   fTextButton757->SetWrapLength(-1);
   fCompositeFrame617->AddFrame(fTextButton757, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));


   fTab612->SetTab(1);

   fTab612->Resize(fTab612->GetDefaultSize());
   fMainFrame954->AddFrame(fTab612, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fTab612->MoveResize(32,72,728,232);

   fMainFrame954->SetMWMHints(kMWMDecorAll,
                        kMWMFuncAll,
                        kMWMInputModeless);
   fMainFrame954->MapSubwindows();

   fMainFrame954->Resize(fMainFrame954->GetDefaultSize());
   fMainFrame954->MapWindow();
   fMainFrame954->Resize(839,534);
}  
