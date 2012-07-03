static const char *DESCRIPTION    = trNOOP( "Channel manager plugin" );
static const char *MAINMENUENTRY  = trNOOP( "Channel manager" );
static const char *VERSION        = "0.0.9";

enum eChannelSortMode { csmNumber, csmName, csmProvider, csmSrcProvider, csmSrcName , csmSrcFrequency};

enum eChannelFilterMode { cfmAll, cfmRadio, cfmTv, cfmTvCrypted };



class cPluginChanman : public cPlugin {
private:
  // Add any member variables or functions you may need here.
public:
  cPluginChanman(void);
  virtual ~cPluginChanman();
  virtual const char *Version(void) { return VERSION; }
  virtual const char *Description(void) { return tr(DESCRIPTION); }
  virtual const char *CommandLineHelp(void);
  virtual bool ProcessArgs(int argc, char *argv[]);
  virtual bool Initialize(void);
  virtual bool Start(void);
  virtual void Stop(void);
  virtual void Housekeeping(void);
  virtual void MainThreadHook(void);
  virtual cString Active(void);
  virtual const char *MainMenuEntry(void) { return tr(MAINMENUENTRY); }
  virtual cOsdObject *MainMenuAction(void);
  virtual cMenuSetupPage *SetupMenu(void);
  virtual bool SetupParse(const char *Name, const char *Value);
  virtual bool Service(const char *Id, void *Data = NULL);
  virtual const char **SVDRPHelpPages(void);
  virtual cString SVDRPCommand(const char *Command, const char *Option, int &ReplyCode);
};



class cMyChannel : public cListObject {
 private:
  static eChannelSortMode sortMode;
  static eChannelFilterMode filterMode;
  cChannel *channel;
  int pos;
  bool isIn;
  bool toCut;
 public:
  cMyChannel(cChannel *Channel, int Pos) { channel = Channel; isIn=true;pos=Pos;toCut=false;}
  cChannel *GetCh(void){return channel;}
  void SetCh(cChannel *Channel){ channel = Channel; isIn=true; }
  static void SetSortMode(eChannelSortMode SortMode){ sortMode=SortMode;}
  static void SetFilterMode(eChannelFilterMode FilterMode){ filterMode=FilterMode;}
  static void IncFilterMode(void) { filterMode = eChannelFilterMode((filterMode == cfmTvCrypted) ? cfmAll : filterMode + 1); }
  bool IsIn(void){return isIn;}
  void In(bool IsIn){isIn=IsIn;}
  bool ToCut(){return toCut;}
  void ToCut(bool ToCut){toCut=ToCut;}
  static eChannelSortMode SortMode(void){ return sortMode;}
  static eChannelFilterMode FilterMode(void){ return filterMode;}
  virtual int Compare(const cListObject &ListObject) const;
};


class cMyChannels : public cList<cMyChannel>{
 private:
  bool modified;
 public:
  void Set(void);
  //~cMyChannels(void);
  //void Reset(void);
  void FilterChan(void);
  bool Filtered(cMyChannel *lchannel);
  void SetModified(void){modified=true;}
  bool Modified(void);
};


class cToCutChannel : public cListObject {
 private:
  cMyChannel *mychannel;
 public:
  cToCutChannel(cMyChannel *MyChannel){mychannel=MyChannel;}
  cMyChannel *Get(void){return mychannel;}
};


class cMenuSetupChanMan : public cMenuSetupPage {
private:
  int newNrRow;
  int newWhatShow;
  int newWhatJump;
protected:
  virtual void Store(void);
public:
  cMenuSetupChanMan(void);
  };



class cItemChoice1 : public cOsdItem {
 private:
  int positem;
  static bool increasing;
  cMyChannel *firstlch;
  int numch;
  int src;
 public:
  cItemChoice1(int positem, cMyChannel *first, int numch);
  cMyChannel *GetFirst(void){return firstlch;}
  int GetNum(void){return numch;}
  int Number(void){return positem;}
  int Src(void){return src;}
  virtual int Compare(const cListObject &ListObject) const;
  static void SetIncreasing(bool Inc){increasing=Inc;}
  static bool Increasing(void){return increasing;}
};



class cFirstMenu : public cOsdMenu {
 private:
  int number;
  cTimeMs numberTimer;
  void Setup(void);
 public:
  cFirstMenu(void);
  virtual ~cFirstMenu(void);
  bool IsFirst(cMyChannel *lchannel, int nch);
  bool IsLast(cMyChannel *lchannel, int nch);
  bool SkipIt(cMyChannel *lchannel);
  bool Filtered(cMyChannel *lchannel);
  void JumpNext(void);
  void JumpNextLetter(int num);
  const char * NameProv(cItemChoice1 *cx);
  void SetFirstHelp(void);
  eOSState ProcessKey(eKeys k);
  eOSState Number(eKeys k);
};


class cSecondMenu : public cOsdMenu {
 private:
  int number;
  cTimeMs numberTimer;
  void Propagate(void);
  cChannel *GetChannel(int Index);
  cMyChannel *GetMyChannel(int Index);

 public:
  cSecondMenu(cMyChannel *firstlch, int numch);
  virtual ~cSecondMenu(void);
  eOSState ProcessKey(eKeys k);
  eOSState Switch(void);
  eOSState Number(eKeys k);
  void Move(int From, int To);
  void Cut();
  void Paste();
};

class cItemChoice2 : public cOsdItem {
 private:
  int positem;
  static bool increasing;
  cChannel *channel;
  cMyChannel *mychannel;
 public:
  cItemChoice2(int positem,cChannel *channel,cMyChannel *mychannel);
  cChannel *GetChan(void){return channel;}
  cMyChannel *GetMyChan(void){return mychannel;}
  int Number(void){return positem;}
  void SetPos(int Positem){positem=Positem;}
/* #if APIVERSNUM >= 10507 */
/*   const char *Icon(void); */
/* #else */
/*   int Icon(void); */
/* #endif */
  int Icon(void); 
  virtual int Compare(const cListObject &ListObject) const;
  virtual void Set(void);
  static void IncFlag(void){increasing=!increasing;}
};
