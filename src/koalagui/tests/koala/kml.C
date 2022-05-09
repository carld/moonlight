// This file is part of Moonlight Creator
//   Copyright (C) 1996-1998  Stephane Rehel
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, write to the Free
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

/*
  kml.C

  koala test
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graphics/MLPicture.h"
#include "graphics/MLEventManager.h"
#include "graphics/SystemWindow.h"

#include "koala/VOptions.h"
#include "koala/VWindow.h"

#include "koala/VButton.h"
#include "koala/VLabel.h"
#include "koala/VCheckBox.h"
#include "koala/VArrow.h"
#include "koala/VVertBar.h"
#include "koala/VHorizBar.h"
#include "koala/VVertScrollBar.h"
#include "koala/VHorizScrollBar.h"
#include "koala/VCircleButton.h"
#include "koala/VRadioButton.h"
#include "koala/VList.h"
#include "koala/VTextList.h"
#include "koala/VFolderList.h"
//#include "koala/VScrollList.h"
#include "koala/VEdit.h"
#include "koala/VRangeBar.h"
#include "koala/VValueBar.h"

#include "koala/VHorizLine.h"
#include "koala/VVertLine.h"
#include "koala/VWidgetsSet.h"
#include "koala/VGroup.h"
#include "koala/VTextBox.h"
#include "koala/VPicture.h"
#include "koala/VChooser.h"

#include "koala/VCard.h"
#include "koala/VDialog.h"
#include "koala/VCartouche.h"

/*
#include "koala/VTextIO.h"
#include "koala/VCommandLine.h"

#include "koala/VDialogWindow.h"
#include "koala/VPopupWindow.h"
#include "koala/VPopupMenu.h"
#include "koala/VBarMenu.h"
#include "koala/VAppWindow.h"
#include "koala/VStatusBar.h"

#include "koala/VScrollingDialogs.h"

#include "koala/VMessageWindow.h"
*/

//void* readPPM( const MLString& filename, IVector& size );

void buildDemo( SystemWindow* systemParent )
{
//  VWindow* w= new VWindow(systemParent);
//  w->create( 10, 10, 300+150, 300+100 );
//
//  w->beginDraw();
//  w->fillWindow( w->getGray2() );
///*
//  w->setForeground(Color8(255,0,0));
//  w->drawLine(2,2,20,20);
//  w->setForeground(Color8(0,255,0));
//  w->setBackground(Color8(0,0,0));
//  w->drawImageString( 4,4+w->getFont()->getAscent()-1,"gCOUyCOUhjy" );
//*/
//  w->endDraw();

  const int down= 30;
  VCard* card1= 0;
  VCard* card2= 0;

  int width= 300+150;
  int height= 300+100+down;

  VCartouche* cartouche= new VCartouche(systemParent);
  cartouche->create( 10, 10, width, height,
                     true, true, MLString("Koala Demo") );

  VDialog* w= cartouche->getDialog();

  w->setCardsRectangle( IRectangle( IPoint(4,4),
                                    IPoint(w->getSize().x()-5,300+down) ) );

  card1= new VCard(w);
  card1->create( 0,0, 0,0, "Card&1" );
  w->addCard(card1);

  card2= new VCard(w);
  card2->create( 0,0, 0,0, "&The second Card" );
  w->addCard(card2);

  VButton* B1= new VButton(w);
  B1->create( 250, 150, "&OK" );
  w->addWidget(B1,false,card2);
  VButton* B2= new VButton(w);
  B2->create( 250, 180, "&Cancel" );
  w->addWidget(B2,false,card2);

  VFolderList* flist= new VFolderList(w);
  flist->create( 30, 30, 170, 200, true*1 );
  w->addWidget(flist,false,card2);
  flist->appendItem( 0, "groups" );
  flist->appendItem( 0, "locations" );
  flist->appendItem( 0, "localconfyg" );
  int fid1= flist->appendFolder( 0, "Folder 1" );
  assert( fid1 != 0 );
  flist->appendItem( fid1, "item 1 /F1" );
  flist->appendItem( fid1, "item 2 /F1" );
  flist->appendItem( fid1, "item 3 /F1" );
  flist->appendItem( 0, "protocols" );
  flist->appendItem( 0, "rpcs" );
  int fid2= flist->appendFolder( fid1, "Folder 2" );
  assert( fid2 != 0 );
  flist->appendItem( fid2, "item 1 /F2" );
  flist->appendItem( fid2, "item 2 /F2" );
  flist->appendItem( fid2, "item 3 /F2" );
  flist->appendItem( fid2, "item 4 /F2" );
  flist->appendItem( 0, "machines" );
  flist->appendItem( 0, "mounts" );
  flist->appendItem( 0, "networks" );
  flist->appendItem( 0, "printers" );
  flist->appendItem( 0, "services" );
  flist->appendItem( 0, "users" );
  flist->appendItem( 0, "ThisIsAHighlyVeryLongItemYouKnow" );

//  flist->appendItem( fid1, "item 4 /F1" );
  flist->appendItem( fid2, "item 5 /F2" );

  flist->openFolder(fid1);

  VButton* unactivate= new VButton(w);
  unactivate->create( 200, 250+down, "Un&activate" );
  w->addWidget(unactivate,false,card1);

  VButton* ok= new VButton(w);
  ok->create( 10, 150+down, "&OK" );
  w->addWidget(ok,false,card1);
//w->removeWidget(ok);
//delete ok; ok= 0;
  VButton* cancel= new VButton(w);
  cancel->create( 120, 150+down, "C&ancel", 10 );
  w->addWidget(cancel,false,card1);

  VButton* exitB= new VButton(w);
  exitB->create( 120, 150+cancel->getHeight()+10+down, "&OK", 10 );
  exitB->setEnterPixmap(true);
  w->addWidget(exitB,false,card1);

  VButton* changeMe= new VButton(w);
  changeMe->create( 10, 150+cancel->getHeight()+10+down, "Chan&geMe" );
  w->addWidget(changeMe,false,card1);

  VLabel* label= new VLabel(w);
  label->create( 10, 250+down, 0, 0, "La&bel Text" );
  w->addWidget(label,true,card1);

  VCheckBox* checkbox= new VCheckBox(w);
  checkbox->create( 10, 250+20+down, 0, 0, "Check box right here" );
  w->addWidget(checkbox,false,card1);
  checkbox->setLinkWidget(label);

  VBar* vb= new VVertBar(w);
  vb->create( 240, 40+down, 200,  10, 2 );
  w->addWidget(vb,true,card1);

  VScrollBar* vsb= new VVertScrollBar(w);
  vsb->create( 270, 40+down, 200, 15, 1 );
  w->addWidget(vsb,true,card1);

  VScrollBar* hsb= new VHorizScrollBar(w);
  hsb->create( 10, 10+down, 200, 5, 1 );
  w->addWidget(hsb,true,card1);

  VRadioButton* radio= new VRadioButton(w);
  radio->create( 16, 110+down, 0, 0, "Lonely &Radio Button" );
  w->addWidget(radio,false,card1);

/*
  IVector picSize;
  void* pictureData= readPPM("/home/kepler/ml/src/koala/demo/picture.ppm",picSize);
  if( pictureData != 0 )
    {
    MLPicture* p= new MLPicture;
    p->createRGB( picSize.x(), picSize.y(), pictureData );
    VPicture* pic= new VPicture(w);
    pic->create( 300, 200+down, p );
    w->addWidget(pic,true,card1);
VButton* changeMe= new VButton(w);
changeMe->create( 305,205+down, "ChangeMe" );
w->addWidget(changeMe,false,card1);
    }
*/

  VTextBox* tb= new VTextBox(w);
  tb->create( 300, 10+down, 130, "Ex&ports" );
  w->addWidget(tb,true,card1);

  VTextList* list= new VTextList(w);
  list->create( 300, 10+tb->getSize().y()+2+down, 130, 150, true*1 );
  w->addWidget(list,false,card1);
  list->setLinkWidget(tb);

  list->appendMessage( "groups" );
  list->appendMessage( "locations" );
  list->appendMessage( "localconfyg" );
  list->appendMessage( "machines" );
  list->appendMessage( "mounts" );
  list->appendMessage( "networks" );
  list->appendMessage( "printers" );
  list->appendMessage( "protocols" );
  list->appendMessage( "rpcs" );
  list->appendMessage( "services" );
  list->appendMessage( "users" );
  list->appendMessage( "ThisIsAHighlyVeryLongItemYouKnow" );

   {
   int n= 52;
   for( int i= 1; i<= n; ++i )
     list->appendMessage( MLString::printf( "Item %d", i ) );
   }

list->setBar(3);

/*
  list->setMultipleSelections();
  list->setSelectable(2,false);
  list->select(3);
  list->select(4);
  list->setItalic(5);
  list->setBold(6);
  list->setBold(7); list->setItalic(7);
  list->refresh();
*/

  VEdit* edit= new VEdit(w);
  edit->create( 10, 70+down, 100 );
  edit->changeEdit("/Projets");
  w->addWidget(edit,false,card1);

  VChooser* chooser= new VChooser(w);
  chooser->create( 120, 70+down, 100, 20 );

  chooser->appendMessage( "Choice 1" );
  chooser->appendMessage( "Times" );
  chooser->appendMessage( "Koala Widgets" );
  chooser->appendMessage( "Koala a" );
  chooser->appendMessage( "Koala z" );
  chooser->appendMessage( "Koala e" );
  chooser->appendMessage( "Koala r" );
  chooser->appendMessage( "Koala t" );
  chooser->appendMessage( "Koala y" );
  chooser->appendMessage( "Koala u" );
  chooser->appendMessage( "Koala i" );
  chooser->appendMessage( "Koala o" );
  chooser->appendMessage( "Koala p" );
  chooser->appendMessage( "Koala k" );
  chooser->changeMessage( "Koala Widgets" );

  //"Choice 1");
  w->addWidget(chooser,false,card1);

//  VRangeBar* rb= new VRangeBar(w);
//  rb->create( 10, 35, 150, 0., 1. );

  VValueBar* valb= new VValueBar(w);
  valb->create( 10, 35+down, 150, 50,
              0., 1.,
              0., 2., 2, 0.75 );
  w->addWidget(valb,false,card1);

  VLine* hl= new VHorizLine(w);
  hl->create( IPoint(0,310+down), IPoint(w->getSize().x()-1,310+down) );
  w->addWidget(hl,true,0);

  VGroup* group= new VGroup(w);
  group->create( 10, 315+down, 200, 80, "&Little group" );
  w->addWidget(group,true,0);

  IRectangle area= group->getWidgetsArea();
  VCheckBox* checkbox2= new VCheckBox(w);
  checkbox2->create( area.p1().x()+2, area.p1().y()+2, 0, 0, "CheckM&e" );
  w->addWidget(checkbox2,false,0);
  checkbox2->setLinkWidget(group);

  VLine* hl2= new VHorizLine(w);
  hl2->create( IPoint( area.p1().x(),
                       area.p1().y()+checkbox2->getSize().y()+8),
               IPoint( area.width()-1,
                       area.p1().y()+checkbox2->getSize().y()+8) );
  w->addWidget(hl2,true,0);

/*
  group->getSet()->beginDraw();
  group->getSet()->fillWindow( Color8(255,0,0));
  group->getSet()->endDraw();
*/

  w->makeIndex();

//eventManager->wait();
//eventManager->dispatch();

//  eventManager->removeRedraw(systemParent);

  cartouche->map();
  MLEventManager* eventManager= MLEventManager::getEventManager();

  eventManager->removeRedraw(cartouche);
  cartouche->postRedraw();

//  drawCartouche();
//  w->postDrawWidget();
  w->focus();


/*
  VCommandLine* cmdline= w->newCommandLine();
  cmdline->create( 0, 300, 280, 100 );
  cmdline->addWord( "_quit" );
  cmdline->addWord( "quit" );
  cmdline->addWord( "quit23" );
  cmdline->addWord( "quit_all" );
  cmdline->addWord( "Abcd" );
  cmdline->addWord( "A3bc" );
  cmdline->addWord( "A" );
  cmdline->addWord( "Ae" );
  cmdline->addWord( "Abcdef" );

  VScrollingDialogs* sd= new VScrollingDialogs(w);
  sd->create( 281, 300, 450-281-1, 100 );

  VWindow* sd_w= sd->appendDialog(70, "Dialog One");
  sd_w->newButton()->create( 40, 25, "Button" );
  sd_w->newRadioButton()->create( 40, 5, "RadioButton" );

  VWindow* sd_w2= sd->appendDialog(90+200, "Dialog Two" );
  sd_w2->newRadioButton()->create( 10, 5, "RadioButton2" );
  sd_w2->newButton()->create( 10, 25, "Button2" );
  sd_w2->newHorizScrollBar()->create( 10, 60, 80, 15, 1 );
   {
   VScrollList* list= sd_w2->newScrollList();

   list->create( 10, 90, 10, 130 );
   list->setMultipleSelections();
    {
    int n= 52;
    for( int i= 1; i<= n; ++i )
      list->append( MLString::printf( "Item %d", i ) );
    }
   list->setSelectable(2,false);
   list->select(3);
   list->select(4);
   list->setItalic(5);
   list->setBold(6);
   list->setBold(7); list->setItalic(7);
   list->refresh();
   }

  VButton* open_dialog= w->newButton();
  open_dialog->create( 310, 258, "Dialog" );

  VDialogWindow* dialog= 0;
  VButton* dialog_button= 0;

{
VPopupMenu* pm= w->newPopup();
pm->addItem( "Toto", 1 );
VPopupMenu* subpm3= pm->addSub( "TutuFoo" );
pm->addItem( "Titi", 1 );
pm->addItem( "Bonjour", 1 );
VPopupMenu* subpm= pm->addSub( "salut" );
pm->addItem( "All is", 1 );
pm->addItem( "Fine now", 1 );

subpm->addItem( "toto", 2 );
subpm->addItem( "va", 2 );
subpm->addItem( "a la plage", 2 );

VPopupMenu* subpm2= subpm->addSub( "le matin" );
subpm2->addItem( "une foret de", 2 );
subpm2->addItem( "symboles", 2 );
subpm2->addItem( "de vivants", 2 );
subpm2->addItem( "piliers", 2 );
subpm2->addSeparator();
subpm2->addItem( "se repondent", 2 );
subpm2->addItem( "la jour s'est leve", 2 );
subpm2->addItem( "sur une etrange idee", 2 );

subpm3->addItem( "valse melancolique", 2 );
subpm3->addSeparator();
subpm3->addItem( "et", 2 );
subpm3->addItem( "langoureux", 2 );
subpm3->addItem( "vertige", 2 );
}

  VPopupMenu* pm;
  VAppWindow* app;
enum { MENU_QUIT= 100, MENU_ABOUT, MENU_ERROR, MENU_WARNING };
{
app= new VAppWindow(root);
app->create( 0,0,400, 200);
VWindow* client= app->createStandardClient();
client->setWindowBackground( vd.getBlackPixel() );
app->mapRaised();

app->getStatusBar()->setMessage( "Here is a message!" );

VBarMenu* bm= app->getBarMenu();
pm= bm;

VPopupMenu* subpm4= pm->addSub( "File" );
subpm4->addItem( "New", 1 );
subpm4->addItem( "Load...", 1 );
subpm4->addItem( "Save...", 1 );
subpm4->addItem( "Save as...", 1 );
subpm4->addSeparator();
subpm4->addItem( "Quit", int(MENU_QUIT) );

VPopupMenu* subpm3= pm->addSub( "Windows" );
pm->addItem( "Quit", int(MENU_QUIT) );
pm->addItem( "Bonjour", 1 );
VPopupMenu* subpm= pm->addSub( "salut" );
pm->addItem( "All is", 1 );
pm->addItem( "Fine now", 10000 );
pm->unactivate(10000);

subpm->addItem( "toto", 2 );
subpm->addItem( "va", 2 );
subpm->addItem( "a la plage", 2 );
VPopupMenu* subpm2= subpm->addSub( "le matin" );
subpm2->addItem( "une foret de", 2 );
subpm2->addItem( "symboles", 2 );
subpm2->addItem( "de vivants", 2 );
subpm2->addItem( "piliers", 2 );
subpm2->addSeparator();
subpm2->addItem( "se repondent", 2 );
subpm2->addItem( "la jour s'est leve", 2 );
subpm2->addItem( "sur une etrange idee", 20000 );
subpm2->unactivate(20000);

subpm3->addItem( "Error Window", int(MENU_ERROR) );
subpm3->addItem( "Warning Window", int(MENU_WARNING) );
subpm3->addSeparator();
subpm3->addCheckItem( "et", 2, true );
subpm3->addCheckItem( "langoureux", 2 );
subpm3->addCheckItem( "vertige", 20001 );
subpm3->unactivate(20001);
bm->refreshMenu();
}

 w->mapRaised();

  for(;;)
    {
    if( vd.getNWindows() == 0 )
      {
      w= 0;
      break;
      }

    vd.waitForEvent();
    vd.dispatchEvents();

    if( w->isForwardClosed() || app->isForwardClosed() )
      break;

    int menu= pm->getSelection();
    if( menu != 0 )
      {
      if( menu == MENU_QUIT )
        break;

      switch( menu )
        {
        case MENU_ERROR:
          VMessageWindow::error( app,
                   "Salut la foule!!!\nStop\nC'est une boite de dialogue" );
          break;
        case MENU_WARNING:
          VMessageWindow::warning( app,
                           "C'est simplement un petit\n*** WARNING *** !" );
          break;
        default:
          break;
        }
      }

    if( cmdline->commandAvailable() )
      {
      const MLString& command= cmdline->getCommand();
      if( command == MLString("quit") )
        break;
      if( command[0] == '!' )
        {
        if( command.length() > 1 )
          {
          MLString shCommand= "/bin/sh -c \""
                            +command.getSub( 1, command.length()-1 )
                            +"\" 2>&1";
          char buffer[513];
          vd.freezeInput();
          FILE* file= popen( shCommand.get(), "rt" );
          assert( file != 0 );
          for(;;)
            {
            fgets( buffer, 512, file );
            if( feof(file) )
              break;
            *cmdline << buffer;
            }
          fclose(file);
          vd.releaseInput();
          }
        }
       else
        *cmdline << "Got command `" << command << "'\n";
      cmdline->waitForAnother();
      }

    if( unactivate->pressed() )
      {
      list->setActive( ! list->isActive() );
      ok->setActive( ! ok->isActive() );
      cancel->setActive( ! cancel->isActive() );
      checkbox->setActive( ! checkbox->isActive() );
      label->setActive( ! label->isActive() );
      vb->setActive( ! vb->isActive() );
      vsb->setActive( ! vsb->isActive() );
      hsb->setActive( ! hsb->isActive() );
      changeMe->setActive( ! changeMe->isActive() );
      edit->setActive( ! edit->isActive() );
      radio->setActive( ! radio->isActive() );
      radiobox->setActive( ! radiobox->isActive() );
      cmdline->setActive( ! cmdline->isActive() );
      }

    if( ok->pressed() )
      {
      vsb->changeTotalLength( vsb->getTotalLength() + 1 );
      printf( "bar pos = %d / %d\n", vsb->getPosition(), vsb->getTotalLength() );
      }

    if( cancel->pressed() )
      {
      vsb->changeTotalLength( vsb->getTotalLength() - 1 );
      printf( "bar pos = %d / %d\n", vsb->getPosition(), vsb->getTotalLength() );
      }

    if( exitB->pressed() )
      break;

    if( changeMe->pressed() )
      {
      if( changeMe->getMessage() == MLString("ChangeMe") )
        changeMe->changeMessage("I change");
       else
        changeMe->changeMessage("ChangeMe");
      }

    if( open_dialog->pressed() )
      {
      open_dialog->unactivate();

      assert( dialog == 0 );
//      dialog= w->newExclusiveDialogWindow();
      dialog= w->newDialogWindow();
      dialog->createAtPointer( 200, 200 );
      assert( dialog_button == 0 );
      dialog_button= dialog->newButton();
      dialog_button->create( 10,10, "Dialog ok" );

      VEdit* edit= dialog->newEdit();
      edit->create( 10, 50, 150, "coucou" );

      dialog->mapRaised();
      }

    if( dialog != 0 )
      {
      assert( dialog_button != 0 );
      if( dialog_button->pressed() )
        {
        delete dialog;
        dialog= 0;
        dialog_button= 0;
        open_dialog->activate();
        }
      }
    }

printf( "deleting w\n" );
  delete w;

printf( "closing VDisplay\n" );

  vd.close();

  printf( "bye, all is fine\n" );

*/
}

/*
#include "tools/PPMFile.h"
void* readPPM( const MLString& filename, IVector& size )
{
  PPMFile ppm(filename.get());
  if( ! ppm.open() )
    return 0;

  size= IVector(ppm.getWidth(),ppm.getHeight());

  int lineLength= 3 * size.x();
  unsigned char* data= new unsigned char [ lineLength * size.y() ];

  for( int i= 0; i < size.y(); ++i )
    {
    boolean outcome= ppm.readLine( data + lineLength*(size.y()-1-i) );
    if( ! outcome )
      {
      delete data;
      return 0;
      }
    }

  return (void*) data;
}
*/
