
/*
 * Chanman.c: A plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 * $Id$
 */

#include <cctype>
#include <vdr/tools.h>
#include <vdr/timers.h>
#include <vdr/plugin.h>
#include <vdr/interface.h>
#include <vdr/device.h>
#include "i18n.h"


#ifdef USE_WAREAGLEICON
#include <vdr/iconpatch.h>
#endif


#define CHANNELNUMBERTIMEOUT 1000 //ms



static const char *VERSION        = "0.0.8";

#include "chanman.h"


// --- cMyChannel (MyChannels) -------------------------------------------------------



eChannelSortMode cMyChannel::sortMode=csmNumber;
eChannelFilterMode cMyChannel::filterMode=cfmAll;

int cMyChannel::Compare(const cListObject &ListObject) const
{
  cMyChannel *p = (cMyChannel *)&ListObject;
  int r = -1;
  if (sortMode == csmProvider)
     r = strcoll(channel->Provider(), p->channel->Provider());
  else if (sortMode == csmName || r == 0)
     r = strcoll(channel->Name(),p->channel->Name());
  else if (sortMode == csmSrcName || sortMode == csmSrcProvider || sortMode == csmSrcFrequency ){
    r = strcoll(*cSource::ToString(channel->Source()),*cSource::ToString(p->channel->Source())); 
    if(r==0){
      if(sortMode == csmSrcName) r = strcoll(channel->Name(), p->channel->Name());
      else if(sortMode == csmSrcProvider) r = strcoll(channel->Provider(), p->channel->Provider());
      else r = channel->Frequency() - p->channel->Frequency();
    }
  } else if (sortMode == csmNumber || r == 0){
    r = pos -  p->pos;
  }
  return r;
};

void cMyChannels::Set() {
  int pos=1;
  for (cChannel *channel = Channels.First(); channel; channel = Channels.Next(channel)) {
    Add(new cMyChannel(channel,pos));
    pos++;
  }
}


/*cMyChannels::~cMyChannels() {
 fprintf(stdout, "Canali distrutti\n");
}*/


/*
void cMyChannels::Reset() { 
  cMyChannel *lchannel = First();  
  for (cChannel *channel = Channels.First(); channel; channel = Channels.Next(channel) ) { 
    lchannel->SetCh(channel); 
    lchannel = Next(lchannel);
  } 
  }*/ 

void cMyChannels::FilterChan() {
  for (cMyChannel *lchannel = First(); lchannel; lchannel = Next(lchannel)) {
    if (Filtered(lchannel)){
      lchannel->In(true);  
    } else {
      lchannel->In(false);  
    }
  }
}

bool cMyChannels::Filtered(cMyChannel *lchannel) {
  bool res=false;

  eChannelFilterMode  fm=lchannel->FilterMode();
  if (lchannel->GetCh()->GroupSep() || fm==cfmAll)
    res=true;
  else if(fm==cfmRadio)
    res=(lchannel->GetCh()->Vpid() == 1 || lchannel->GetCh()->Vpid() == 0);
  else if(fm==cfmTv)
    res=(lchannel->GetCh()->Vpid() > 1 && lchannel->GetCh()->Ca() == 0);
  else if(fm==cfmTvCrypted)
    res=(lchannel->GetCh()->Vpid() > 1 && lchannel->GetCh()->Ca() != 0);
  

  return res;
}


bool cMyChannels::Modified(void)
{
  bool Result = modified;
  modified = false;
  return Result;
}


cMyChannels MyChannels;


cList<cToCutChannel> ToCutChannels;

//----



// --- cMenuSetupChanMan -------------------------------------------------------
int NrRow = 30;
int WhatShow = 1;
int WhatJump = 1;

cMenuSetupChanMan::cMenuSetupChanMan(void)
{
  newNrRow = NrRow;
  newWhatShow = WhatShow;
  newWhatJump = WhatJump;
  Add(new cMenuEditIntItem( tr("Max items for alphabetic order"),&newNrRow));
  Add(new cMenuEditIntItem( tr("Show : 1) Prov.; 2) ChID; 3) Freq."),&newWhatShow));
  Add(new cMenuEditIntItem( tr("Jump : 1) Alphabetic; 2) Numeric"),&newWhatJump));
}

void cMenuSetupChanMan::Store(void)
{
  SetupStore("NrRow",    NrRow = newNrRow);
  SetupStore("WhatShow", WhatShow = newWhatShow);
  SetupStore("WhatJump", WhatJump = newWhatJump);
}





// Gruppi in ordine di inserimento
bool cItemChoice1::increasing=false;

//----




bool cItemChoice2::increasing=true;



// ---Setting filter menu
//class cFilterMenu : public cOsdMenu {
// private:
//  //void Setup(void);
// public:
//  cFilterMenu(void);
//  ~cFilterMenu(void);
//  //  eOSState ProcessKey(eKeys k);
//  // eOSState Number(eKeys k);
//};

//class cFilterItem : public cOsdItem {
// private:
//  int positem;
 
// public:
//  cFilterItem(int positem, const char *FilterName);
//  //  int Number(void){return positem;}
//};

//------ 


// --- cPluginChanman -------------------------------------------------------


cPluginChanman::cPluginChanman(void)
{
  NrRow=30;
  WhatShow=1;
  WhatJump=1;

  // Initialize any member variables here.
  // DON'T DO ANYTHING ELSE THAT MAY HAVE SIDE EFFECTS, REQUIRE GLOBAL
  // VDR OBJECTS TO EXIST OR PRODUCE ANY OUTPUT!
}

cPluginChanman::~cPluginChanman()
{
  // Clean up after yourself!
}

const char *cPluginChanman::CommandLineHelp(void)
{
  // Return a string that describes all known command line options.
  return NULL;
}

bool cPluginChanman::ProcessArgs(int argc, char *argv[])
{
  // Implement command line argument processing here if applicable.
  return true;
}

bool cPluginChanman::Initialize(void)
{
  // Initialize any background activities the plugin shall perform.
  return true;
}

bool cPluginChanman::Start(void)
{
  // Start any background activities the plugin shall perform.
#if APIVERSNUM >= 10507
  I18nRegister( "chanman" );
#else
  RegisterI18n( Phrases );
#endif
  return true;
}

void cPluginChanman::Stop(void)
{
  // Stop any background activities the plugin shall perform.
}

void cPluginChanman::Housekeeping(void)
{
  // Perform any cleanup or other regular tasks.
}

void cPluginChanman::MainThreadHook(void)
{
  // Perform actions in the context of the main program thread.
  // WARNING: Use with great care - see PLUGINS.html!
}

cString cPluginChanman::Active(void)
{
  // Return a message string if shutdown should be postponed
  return NULL;
}

cOsdObject *cPluginChanman::MainMenuAction(void)
{
  // Perform the action when selected from the main VDR menu.
  return new cFirstMenu;  
}

cMenuSetupPage *cPluginChanman::SetupMenu(void)
{
  // Return a setup menu in case the plugin supports one.
  return new cMenuSetupChanMan;
}

bool cPluginChanman::SetupParse(const char *Name, const char *Value)
{
  // Parse your own setup parameters and store their values.
  
  if      (!strcasecmp(Name, "NrRow"))     NrRow = atoi(Value);
  else if (!strcasecmp(Name, "WhatShow"))  WhatShow = atoi(Value);
  else if (!strcasecmp(Name, "WhatJump"))  WhatJump = atoi(Value);
  else
     return false;
  
  return true;
}

bool cPluginChanman::Service(const char *Id, void *Data)
{
  // Handle custom service requests from other plugins
  return false;
}

const char **cPluginChanman::SVDRPHelpPages(void)
{
  // Return help text for SVDRP commands this plugin implements
  return NULL;
}

cString cPluginChanman::SVDRPCommand(const char *Command, const char *Option, int &ReplyCode)
{
  // Process SVDRP commands this plugin implements
  return NULL;
}


//----------Filter Menu---------

//cFilterMenu::cFilterMenu() : cOsdMenu(tr("Filters"),4) {
  
//  Add(new cFilterItem(1,tr("Radio")));
//  Add(new cFilterItem(2,tr("Televisions")));
//  Add(new cFilterItem(3,tr("Crypted Televisions")));
//  Add(new cFilterItem(4,tr("Television and Crypted")));
//  Add(new cFilterItem(5,tr("All")));

//  Display();
//}

//cFilterMenu::~cFilterMenu(void) {
//}

//cFilterItem::cFilterItem(int Positem, const char *FilterName)
//{
//  positem=Positem;
//  char *buffer=NULL;
//  asprintf(&buffer, "%3d \t%s", positem, FilterName); 
//  SetText(buffer, false);
//}


//----------cFirstMenu---------

cFirstMenu::cFirstMenu(void) : cOsdMenu(tr("Channel Manager"),4,27,8) {

  MyChannels.Set();
  Setup();
  SetFirstHelp();
  Display();
}


// enum eChannelFilterMode { cfmAll, cfmRadio, cfmTv, cfmTvCrypted };
void cFirstMenu::SetFirstHelp() {
  const char *Blu[] = { NULL, tr("Radio"), tr("Tv"), tr("Tv Crypt"),tr("All") };

  SetHelp(cMyChannel::SortMode()==csmNumber ? tr("Src+Frequency"):tr("Group"),
	  cMyChannel::SortMode()==csmProvider ? tr("Src+Provider"):tr("Provider"),
	  cMyChannel::SortMode()==csmName ? tr("Src+Name"):tr("Name"),
	  Blu[cMyChannel::FilterMode()+1]
	  );
  
}

cFirstMenu::~cFirstMenu(void) {
  MyChannels.Clear();
  cMyChannel::SetSortMode(csmNumber);
  cMyChannel::SetFilterMode(cfmAll);

  ToCutChannels.Clear();
}


void cFirstMenu::Setup() {
  int nch=0; 
  int positem=0;
  number=0;
  bool isInGroup=false;  


  cMyChannel *first=NULL;
  cChannel *currentChannel;
  currentChannel = Channels.GetByNumber(cDevice::CurrentChannel());  
   
  cItemChoice1 *currentItem = NULL;
  
  Clear();
  for (cMyChannel *lchannel = MyChannels.First(); lchannel; lchannel = MyChannels.Next(lchannel)) {
    if(lchannel->GetCh()==currentChannel) isInGroup=true;
    if (!SkipIt(lchannel)){
      if (IsFirst(lchannel, nch)) {
	first=lchannel;
	nch=1;
      } else if(nch!=0){ // Incrementa solo se c'e' un first
	nch++;
      }
    }
  
    if (((nch>1) || (nch==1 && lchannel->SortMode()!=csmNumber) || (nch==1 && lchannel->SortMode()==csmNumber && lchannel->FilterMode()==cfmAll )) &&
												    (lchannel==MyChannels.Last() || IsLast(lchannel,nch))) {
      positem++;
      cItemChoice1 *item = new cItemChoice1(positem,first,nch);
      Add(item);
      if (isInGroup){
	// fprintf(stderr, "Sono nel gruppo %s\n",first->GetCh()->Name());
	currentItem = item;
	isInGroup=false;
      }
      nch=0;
    }
    
  }
  
  SetCurrent(currentItem);
}


bool cFirstMenu::SkipIt(cMyChannel *lchannel) {
  return (!(lchannel->IsIn()) || ((lchannel->SortMode()!=csmNumber) && (lchannel->GetCh()->GroupSep())));  
} 

bool cFirstMenu::IsFirst(cMyChannel *lchannel, int nch) {
  bool res=false;
  
  if (lchannel->SortMode()==csmNumber) res=(lchannel->GetCh()->GroupSep());  
  else if (lchannel->SortMode()==csmName) res=(nch==0); 
  else if (lchannel->SortMode()==csmProvider) res=(nch==0);
  else if (lchannel->SortMode()==csmSrcProvider) res=(nch==0);
  else if (lchannel->SortMode()==csmSrcName) res=(nch==0);
  else if (lchannel->SortMode()==csmSrcFrequency) res=(nch==0);
  return res; 
} 

bool cFirstMenu::IsLast(cMyChannel *lchannel, int nch) {
  bool res=false;
  if (lchannel->SortMode()==csmNumber) res=(MyChannels.Next(lchannel)->GetCh()->GroupSep());
  else if (lchannel->SortMode()==csmName || lchannel->SortMode()==csmSrcName ) 
    res=(strncmp(lchannel->GetCh()->Name(),MyChannels.Next(lchannel)->GetCh()->Name(),1) || nch==NrRow); 
  else if (lchannel->SortMode()==csmProvider || lchannel->SortMode()==csmSrcProvider) {
    if (!lchannel->GetCh()->GroupSep()){
      // nchannel=MyChannels.Next(lchannel);
      // fprintf(stderr, "%d, %d\n",lchannel, nchannel);
      // fprintf(stderr, "%s = %s \n",lchannel->GetCh()->Name(), nchannel->GetCh()->Name());
      // fprintf(stderr, "%s = %s \n",lchannel->GetCh()->Provider(), nchannel->GetCh()->Provider());
      // fprintf(stderr, "Prima\n");
      res=(strcoll(lchannel->GetCh()->Provider(),MyChannels.Next(lchannel)->GetCh()->Provider())!=0);
      // fprintf(stderr, "Dopo\n");
    }
  } else if (lchannel->SortMode()==csmSrcFrequency) {
    if (!lchannel->GetCh()->GroupSep()){
      res=(lchannel->GetCh()->Frequency()!=MyChannels.Next(lchannel)->GetCh()->Frequency());
    }
  }
  //  r = strcoll(channel->Provider(), p->channel->Provider());  

return res;
} 

void cFirstMenu::JumpNext()
{
  int last = Count() - 1;
  int current=Current();
  int Src;
  if (cMyChannel::SortMode()==csmProvider || cMyChannel::SortMode()==csmName || cMyChannel::SortMode()==csmNumber){
    if (current!=last) SetCurrent(Get(Count()-1));
    else SetCurrent(Get(0));
  } else {
    if (current==last) SetCurrent(Get(0));
    else {
      Src=((cItemChoice1 *)Get(current))->Src();
      for (cItemChoice1 *ci = (cItemChoice1 *)Get(current); ci && ci->Src()==Src; ci = (cItemChoice1 *)ci->Next())
      if (ci && ci!=(cItemChoice1 *)Last()) SetCurrent((cOsdItem *)ci->Next());
      else SetCurrent(Get(0));
    }
  }
}


const char * cFirstMenu::NameProv(cItemChoice1 *cx){
  cMyChannel *q=(cMyChannel *)cx->GetFirst();
  if (cMyChannel::SortMode()==csmName || cMyChannel::SortMode()==csmSrcName){
    return q->GetCh()->Name();
  } else if (cMyChannel::SortMode()==csmProvider || cMyChannel::SortMode()==csmSrcProvider){
    return q->GetCh()->Provider();
  }
  // Caso csmNumber
  cMyChannel *q1=(cMyChannel *)cx->GetFirst()->Prev();
  return q1->GetCh()->Name();
}

void cFirstMenu::JumpNextLetter(int num)
{
  //  const char *KeyAlpha[] = {tr(" "), tr("A"), tr("D"), tr("G"), tr("J"), tr("M"), tr("P"), tr("T"), tr("W")};
  const char *KeyAlpha[] = {" ", "A", "D", "G", "J", "M", "P", "T", "W"};
  //  int last = Count() - 1;
  int current=Current();

  cItemChoice1 *cic=(cItemChoice1 *)Get(current);

  cItemChoice1 *ci=(cItemChoice1 *)Get(current);


  if (strncmp(KeyAlpha[num-1], NameProv(ci) ,1)>0){
    // La lettera premuta e' successiva all'iniziale del corrente item  
    for(ci = cic; 
	ci && strncmp(KeyAlpha[num-1], NameProv(ci), 1)>0; 
	ci = (cItemChoice1 *)ci->Next());
    // fprintf(stderr, "%s = %s \n",KeyAlpha[num-1], NameProv(ci));
   
  } else if (num < 9 && strncmp(KeyAlpha[num], NameProv(ci), 1)<=0) {
    // La lettera premuta e' precedente all'iniziale del corrente item  
    for( ; ci && strncmp(KeyAlpha[num-1], NameProv(ci) ,1)<=0; ci = (cItemChoice1 *)ci->Prev());
    if (ci) ci = (cItemChoice1 *)ci->Next();
    // else ci = (cItemChoice1 *)First();
    //fprintf(stderr, "%s = %s \n",KeyAlpha[num-1], ci->GetFirst()->GetCh()->Name());
  } else {
    // La lettera premuta e' precedente all'iniziale del corrente item  e num==9
    for(ci = cic; 
	ci && strncmp(NameProv(cic), NameProv(ci), 1)==0; 
	ci = (cItemChoice1 *)ci->Next());
    // fprintf(stderr, "%s = %s \n",NameProv(cic), NameProv(ci));
    //fprintf(stderr, "%s = %s \n",cic->GetFirst()->GetCh()->Name(), ci->GetFirst()->GetCh()->Name() );
  }

  if (ci && ci!=(cItemChoice1 *)Last()) SetCurrent((cOsdItem *)ci);
  else SetCurrent(Get(0));    
}




eOSState cFirstMenu::Number(eKeys Key)
{
  if (numberTimer.TimedOut())
    number = 0;
  if(!number && Key==k0){
    if (cMyChannel::SortMode()==csmNumber){
      if (cItemChoice1::Increasing()){ 
	MyChannels.Sort();
	Setup();
	cItemChoice1::SetIncreasing(false);	
      } else {
	Sort();
	cItemChoice1::SetIncreasing(true);	
      }
    } else {
      JumpNext();
    }
    Display();
  } else if (WhatJump==1 && (cMyChannel::SortMode()!=csmNumber || cMyChannel::SortMode()==csmNumber && cItemChoice1::Increasing())) {
    JumpNextLetter(Key-k0);
    Display();
  } else {
    number = number * 10 + Key - k0;
    for (cItemChoice1 *ci = (cItemChoice1 *)First(); ci; ci = (cItemChoice1 *)ci->Next()){
      if (ci->Number() == number) {
	SetCurrent(ci);
	Display();
	break;
      }
    }
  }
  
  numberTimer.Set(CHANNELNUMBERTIMEOUT);
  return osContinue;
}

eOSState cFirstMenu::ProcessKey(eKeys Key)
{
  eOSState state = cOsdMenu::ProcessKey(Key);
  if (state == osUnknown) 
  {
    //switch (Key & ~k_Repeat)
    switch (Key)
    { 
      case k0...k9:
	return Number(Key);
	break;
      case kUp:
      case kUp|k_Repeat:
	break; 
      case kDown:
      case kDown|k_Repeat:
	break; 
      case kLeft:
      case kLeft|k_Repeat:
	break; 
      case kRight:
      case kRight|k_Repeat:
	break; 
      case kRed:
	if(cMyChannel::SortMode()==csmNumber) {
	  cMyChannel::SetSortMode(csmSrcFrequency);
	  MyChannels.Sort();
	} else {
	  cMyChannel::SetSortMode(csmNumber);
	  MyChannels.Sort();
	  cItemChoice1::SetIncreasing(false);	
	}	
	MyChannels.SetModified();
	//	Setup();
	//	SetFirstHelp();
	//	Display();
	break;
      case kGreen:
	if(cMyChannel::SortMode()==csmProvider) cMyChannel::SetSortMode(csmSrcProvider);
	else cMyChannel::SetSortMode(csmProvider);
	MyChannels.Sort();
	MyChannels.SetModified();
	//	Setup();
	//	SetFirstHelp();
	//	Display();
	break; 
      case kYellow:
	if(cMyChannel::SortMode()==csmName) cMyChannel::SetSortMode(csmSrcName);
	else cMyChannel::SetSortMode(csmName);
	MyChannels.Sort();
	MyChannels.SetModified();
	//	Setup();
	//	SetFirstHelp();
	//	Display();
	break;
      case kBlue:
	cMyChannel::IncFilterMode();
	MyChannels.FilterChan();
	MyChannels.SetModified();
	//	Setup();
	//	SetFirstHelp();
	//	Display();
	break;
      case kOk:
	return AddSubMenu(new cSecondMenu( ((cItemChoice1 *)Get(Current()))->GetFirst(), ((cItemChoice1 *)Get(Current()))->GetNum()));
        // Dove Torna una volta uscito dal sotto menu'?
	break;
      case kBack:
        return osEnd;
      default:
        return state;
    }
    state = osContinue;
  }
  if(MyChannels.Modified()){
    Setup();
    SetFirstHelp();
    Display();
  }
  return state;

}

//---- cItemChoice1----



cItemChoice1::cItemChoice1(int Positem, cMyChannel *lchannel, int nch)
{
  char *buffer=NULL;
  positem=Positem;
  if (lchannel->SortMode()==csmNumber) {
    //    fprintf(stderr, "%s \n",lchannel->GetCh()->Name());
    if(MyChannels.Next(lchannel)) firstlch=MyChannels.Next(lchannel);
    else firstlch=NULL;
    nch--;
  }  else firstlch=lchannel;


  numch=nch;
  if (lchannel->SortMode()==csmSrcProvider || lchannel->SortMode()==csmProvider) {
    src=lchannel->GetCh()->Source();
    asprintf(&buffer, "%3d\t%s \t%s \t%3d", positem, lchannel->GetCh()->Provider(), *cSource::ToString(lchannel->GetCh()->Source()), nch);
  } else if (lchannel->SortMode()==csmSrcFrequency) {
    src=lchannel->GetCh()->Source();
    asprintf(&buffer, "%3d\t%8d %s \t%s \t%3d", positem, lchannel->GetCh()->Frequency(), lchannel->GetCh()->Provider(), 
	     *cSource::ToString(lchannel->GetCh()->Source()), nch);
  } else if (lchannel->SortMode()==csmSrcName) {
    src=lchannel->GetCh()->Source();
    asprintf(&buffer, "%3d\t%s \t%s \t%3d", positem, lchannel->GetCh()->Name(), *cSource::ToString(lchannel->GetCh()->Source()), nch);
  } else asprintf(&buffer, "%3d\t%s\t\t%3d", positem, lchannel->GetCh()->Name(), nch);
  
  SetText(buffer, false);
}


int cItemChoice1::Compare(const cListObject &ListObject) const
{
  cItemChoice1 *p = (cItemChoice1 *)&ListObject;
  cMyChannel *q=(cMyChannel *)p->GetFirst()->Prev(); //Il precedente al primo e' il nome del gruppo
  cMyChannel *q0=(cMyChannel *)firstlch->Prev(); // Come sopra

  return strcoll(q0->GetCh()->Name(), q->GetCh()->Name());
}



/*
int cMenuChannelItem::Compare(const cListObject &ListObject) const
{
  cMenuChannelItem *p = (cMenuChannelItem *)&ListObject;
  int r = -1;
  if (sortMode == csmProvider)
     r = strcoll(channel->Provider(), p->channel->Provider());
  if (sortMode == csmName || r == 0)
     r = strcoll(channel->Name(), p->channel->Name());
  if (sortMode == csmNumber || r == 0)
     r = channel->Number() - p->channel->Number();
  return r;
}
*/


// --- cSecondMenu -------------------------------------------------------


cSecondMenu::cSecondMenu(cMyChannel *firstlch, int numch) : cOsdMenu(tr("Channels"),4,25) {
  cMyChannel *lchannel;
  number=0;
  lchannel=firstlch;

  for (int i=1; i<=numch; i++) {
    // Salta tutti i "canali-sezione" ed i canali filtrati
    while(lchannel->GetCh()->GroupSep() || !(lchannel->IsIn())) lchannel = MyChannels.Next(lchannel);
    
    cItemChoice2 *item = new cItemChoice2(i,lchannel->GetCh(),lchannel);
    Add(item);
    lchannel = MyChannels.Next(lchannel);
  }

  SetHelp(tr("Button$Switch"),
	  tr("Button$Cut"),
	  cMyChannel::SortMode()==csmNumber ? tr("Button$Paste"):tr(""),
	  cMyChannel::SortMode()==csmNumber ? tr("Button$Mark"):tr(""));
  Display();
}

cSecondMenu::~cSecondMenu() {
}

cChannel *cSecondMenu::GetChannel(int Index)
{
  cItemChoice2 *p = (cItemChoice2 *)Get(Index);
  return p ? (cChannel *)p->GetChan() : NULL;
}



cMyChannel *cSecondMenu::GetMyChannel(int Index)
{
  cItemChoice2 *p = (cItemChoice2 *)Get(Index);
  return p ? (cMyChannel *)p->GetMyChan() : NULL;
}


void cSecondMenu::Propagate(void)
{
  int i=1;
  Channels.ReNumber();
  for (cItemChoice2 *ci = (cItemChoice2 *)First(); ci; ci = (cItemChoice2 *)ci->Next()){
      ci->SetPos(i);
      i++;
      ci->Set();
  }
  Display();
  Channels.SetModified(true);
  MyChannels.SetModified();
}


void cSecondMenu::Move(int From, int To)
{
  int CurrentChannelNr = cDevice::CurrentChannel();
  cChannel *CurrentChannel = Channels.GetByNumber(CurrentChannelNr);
  cChannel *FromChannel = GetChannel(From);// From e' la posizione nel menu di cItemChoice2
  cChannel *ToChannel = GetChannel(To); // GetChannel Restituisce la posizione nella lista Channels.
  cMyChannel *FromMyChannel = GetMyChannel(From);
  cMyChannel *ToMyChannel = GetMyChannel(To); 
  
  if (FromChannel && ToChannel) {
     int FromNumber = FromChannel->Number();
     int ToNumber = ToChannel->Number();
     Channels.Move(FromChannel, ToChannel);
     MyChannels.Move(FromMyChannel, ToMyChannel);
     cOsdMenu::Move(From, To);
     Propagate();

     isyslog("channel %d moved to %d", FromNumber, ToNumber);
     if (CurrentChannel && CurrentChannel->Number() != CurrentChannelNr) {
        if (!cDevice::PrimaryDevice()->Replaying() || cDevice::PrimaryDevice()->Transferring())
           Channels.SwitchTo(CurrentChannel->Number());
        else
           cDevice::SetCurrentChannel(CurrentChannel);
        }
     }
}


void cSecondMenu::Paste(void){

  //  int CurrentChannelNr = cDevice::CurrentChannel();
  //  cChannel *CurrentChannel = Channels.GetByNumber(CurrentChannelNr);

  cMyChannel *FromMyChannel;

  cItemChoice2 *CurrentChoice=(cItemChoice2 *)Get(Current());

  cMyChannel *ToMyChannel=CurrentChoice->GetMyChan();

  cChannel *FromChannel;
  cChannel *ToChannel = ToMyChannel->GetCh();

  int i=1;
  for (cToCutChannel *mychancut = ToCutChannels.First(); mychancut; mychancut = ToCutChannels.Next(mychancut)){
  //  for (cToCutChannel *mychancut = ToCutChannels.Last(); mychancut; mychancut = ToCutChannels.Prev(mychancut)){
    FromMyChannel=mychancut->Get();
    FromMyChannel->ToCut(false);

    FromChannel=FromMyChannel->GetCh();
    
    Channels.Move(FromChannel, ToChannel);
    MyChannels.Move(FromMyChannel, ToMyChannel);
    
    //void cOsdMenu::Add(cOsdItem *Item, bool Current, cOsdItem *After)

    //cItemChoice2 *item = new cItemChoice2(i,lchannel->GetCh(),lchannel);
    //    Add(item);

    Add(new cItemChoice2(i,FromChannel,FromMyChannel), true, CurrentChoice);
    i++;

    //    ToChannel=Channels.Next(ToChannel);
    //    ToMyChannel=MyChannels.Next(ToMyChannel);
    
    ToChannel=FromChannel;
    ToMyChannel=FromMyChannel;
    
    isyslog("channel %d moved to %d", i, i);
  }

  ToCutChannels.Clear();
  Propagate();
}

void cSecondMenu::Cut(void){
  cItemChoice2 *ci = (cItemChoice2 *)Get(Current());
  cMyChannel *mch = ci->GetMyChan();
  if (!(mch->ToCut())){
    mch->ToCut(true);
    ToCutChannels.Ins(new cToCutChannel(mch));
  } else {
    mch->ToCut(false);

    cToCutChannel *mychancut = ToCutChannels.First();
    while(mychancut && mychancut->Get()!=mch){
      mychancut = ToCutChannels.Next(mychancut);
    }
    if(mychancut->Get()==mch) ToCutChannels.Del(mychancut,false);//non cancello l'oggetto con false ma solo il puntatore!
  }

  ci->Set();
  Display();
}


eOSState cSecondMenu::Switch(void)
{
  cChannel *ch = ((cItemChoice2 *)Get(Current()))->GetChan();
  if (ch)
     return cDevice::PrimaryDevice()->SwitchChannel(ch, true) ? osEnd : osContinue;
  return osEnd;
}




eOSState cSecondMenu::Number(eKeys Key)
{
  if (numberTimer.TimedOut())
    number = 0;
   if(!number && Key==k0){
    cItemChoice2::IncFlag();
    Sort();
    Display();
  }
  number = number * 10 + Key - k0;
  for (cItemChoice2 *ci = (cItemChoice2 *)First(); ci; ci = (cItemChoice2 *)ci->Next()){
    if (ci->Number() == number) {
      SetCurrent(ci);
      Display();
      break;
    }
  }
  numberTimer.Set(CHANNELNUMBERTIMEOUT);
  return osContinue;
}




eOSState cSecondMenu::ProcessKey(eKeys Key)
{
  eOSState state = cOsdMenu::ProcessKey(Key);
  if (state == osUnknown) 
  {
    //switch (Key & ~k_Repeat)
    switch (Key)
    { 
      case k0...k9:
	return Number(Key);
      case kUp:
      case kUp|k_Repeat:
	break; 
      case kDown:
      case kDown|k_Repeat:
	break; 
      case kLeft:
      case kLeft|k_Repeat:
	break; 
      case kRight:
      case kRight|k_Repeat:
	break; 
      case kRed:
	// Cambia canale rimanendo sul menu'
	Switch();
	break;
      case kGreen:
	Cut();
	break; 
      case kYellow:
	if (cMyChannel::SortMode()==csmNumber)	Paste();
	break;
      case kBlue:
	if (cMyChannel::SortMode()==csmNumber)	Mark();
	break;
      case kOk:   
	return Switch();
    case kBack:
        return osEnd;
      default:
        return state;
    }
    state = osContinue;
  }
  return state;

}


//----Choices in Second Menu----


/* #if APIVERSNUM >= 10507 */
/* const char *cItemChoice2::Icon() */
/* { */
/*   if (channel->Vpid() == 1 || channel->Vpid() == 0){ */
/* #ifdef USE_WAREAGLEICON */
/*     if (Setup.WarEagleIcons) { */
/*       return IsLangUtf8() ? ICON_RADIO_UTF8 : ICON_RADIO; */
/*     } else { */
/* #endif   */
/*       return "R"; */
/*     }  */
/*   } else if (channel->Ca() == 0){ */
/* #ifdef USE_WAREAGLEICON */
/*     if (Setup.WarEagleIcons) { */
/*       return IsLangUtf8() ? ICON_TV_UTF8 : ICON_TV; */
/*     } else { */
/* #endif */
/*       return "T"; */
/*     } */
/*   } else { */
/* #ifdef USE_WAREAGLEICON */
/*     if (Setup.WarEagleIcons) { */
/*       return IsLangUtf8() ? ICON_TV_CRYPTED_UTF8 : ICON_TV_CRYPTED; */
/*     }  */
/*     else */
/* #endif */
/*       return "C"; */
/*   } */
/* } */
/* #endif */

/* #if APIVERSNUM < 10507 */
/* int cItemChoice2::Icon() */
/* { */
/*   if (channel->Vpid() == 1 || channel->Vpid() == 0) */
/* #ifdef USE_WAREAGLEICON */
/*     return ICON_RADIO; */
/* #else */
/*   return 'R'; */
/* #endif */
/*   else if (channel->Ca() == 0) */
/* #ifdef USE_WAREAGLEICON */
/*     return ICON_TV; */
/* #else */
/*   return 'T'; */
/* #endif */
/*   else */
/* #ifdef USE_WAREAGLEICON */
/*     return ICON_TV_VERSCHL; */
/* #else */
/*   return 'C'; */
/* #endif */
/* } */
/* #endif */

int cItemChoice2::Icon()
{
  if (channel->Vpid() == 1 || channel->Vpid() == 0)
    return 'R';
  else if (channel->Ca() == 0)
    return 'T';
  else
    return 'C';
}


cItemChoice2::cItemChoice2(int Positem, cChannel *Channel,cMyChannel *MyChannel)
{
  positem=Positem;
  channel=Channel;
  mychannel=MyChannel;
  Set();
  /*  char *buffer=NULL;

  if (WhatShow==1)
    asprintf(&buffer, 
#if APIVERSNUM >= 10507
	     "%3d\t%s %s\t %s",
#else
	     "%3d\t%c %s\t %s",
#endif
	     positem, Icon(), channel->Name(),  channel->Provider()); 
  else if (WhatShow==2)
    asprintf(&buffer,  channel->Rid() ?
#if APIVERSNUM >= 10507 
	     "%3d\t%s %s\t %s-%d-%d-%d-%d" : "%3d\t%s %s\t %s-%d-%d-%d",
#else
	     "%3d\t%c %s\t %s-%d-%d-%d-%d" : "%3d\t%c %s\t %s-%d-%d-%d",
#endif
	     positem, Icon(), channel->Name(),*cSource::ToString(channel->Source()), 
	     channel->Nid(), channel->Tid(), channel->Sid(), channel->Rid());
  else
    asprintf(&buffer,  
#if APIVERSNUM >= 10507 
	     "%3d\t%s %s\t %d",
#else
	     "%3d\t%c %s\t %d",
#endif
	     positem, Icon(), channel->Name(), channel->Frequency());
  
	     SetText(buffer, false);*/
}



void cItemChoice2::Set(void)
{
  //  positem=Positem;
  //  channel=Channel;
  char *buffer=NULL;
  int signpos;
  signpos=mychannel->ToCut() ? -(positem) : positem;
  if (WhatShow==1)
    asprintf(&buffer, 
/* #if APIVERSNUM >= 10507 */
/* 	     "%3d\t%s %s\t %s", */
/* #else */
/* 	     "%3d\t%c %s\t %s", */
/* #endif */
 	     "%3d\t%c %s\t %s",
	     signpos, Icon(), channel->Name(),  channel->Provider()); 
  else if (WhatShow==2)
    asprintf(&buffer,  channel->Rid() ?
/* #if APIVERSNUM >= 10507  */
/* 	     "%3d\t%s %s\t %s-%d-%d-%d-%d" : "%3d\t%s %s\t %s-%d-%d-%d", */
/* #else */
/* 	     "%3d\t%c %s\t %s-%d-%d-%d-%d" : "%3d\t%c %s\t %s-%d-%d-%d", */
/* #endif */
 	     "%3d\t%c %s\t %s-%d-%d-%d-%d" : "%3d\t%c %s\t %s-%d-%d-%d", 
	     signpos, Icon(), channel->Name(),*cSource::ToString(channel->Source()), 
	     channel->Nid(), channel->Tid(), channel->Sid(), channel->Rid());
  else
    asprintf(&buffer,  
/* #if APIVERSNUM >= 10507  */
/* 	     "%3d\t%s %s\t %d", */
/* #else */
/* 	     "%3d\t%c %s\t %d", */
/* #endif */
 	     "%3d\t%c %s\t %d", 
	     signpos, Icon(), channel->Name(), channel->Frequency());
  
  SetText(buffer, false);
}


int cItemChoice2::Compare(const cListObject &ListObject) const
{
  cItemChoice2 *p = (cItemChoice2 *)&ListObject;
  int r = -1;
  if (increasing)
    r = positem - p->Number();
  else r = p->Number() - positem;

  return r;
}

//----------

VDRPLUGINCREATOR(cPluginChanman); // Don't touch this!
