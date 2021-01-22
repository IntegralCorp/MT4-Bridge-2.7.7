//+------------------------------------------------------------------+
//|                                    MetaTrader 4 Integral Gateway |
//|                     Copyright 2017-2020, MetaTech Software Corp. |
//|                                       http://www.metatech.com.tr |
//+------------------------------------------------------------------+
#pragma once

//+------------------------------------------------------------------+
//| Format strings                                                   |
//+------------------------------------------------------------------+
class SIntegralFormat
  {
  private:
    typedef struct
      {
      UINT     value;
      LPCSTR   name;
      } IntStringPair;

  public:
    //--- get name
    //--- error
    static LPCSTR    FormatError(const UINT retcode);
    static LPCSTR    FormatTransactionName(const UINT value);
    static LPCSTR    FormatCmdName(const UINT value);
    static LPCSTR    FormatAddName(const UINT aValue);
    static LPCSTR    FormatUpdateName(const UINT aValue);
    static LPCSTR    FormatStateName(const UINT aValue);
    static LPCSTR    FormatLoginType(const UINT aValue);
  };
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
inline LPCSTR SIntegralFormat::FormatError(const UINT retcode)
  {
  //--- errors
  static IntStringPair errors[]=
    {
     { CmdOK,     "CmdOK"     },
     { CmdTrade,  "CmdTrade"  },
     { CmdLogin,  "CmdLogin"  },
     { CmdWarn,   "CmdWarn"   },
     { CmdErr,    "CmdErr"    },
     { CmdAtt,    "CmdAtt"    }
    };
  //--- search description
  for(UINT i=0;i<_countof(errors);i++)
    if(errors[i].value==retcode) return(errors[i].name);
  //---
  LOG_ERROR(pluginLog,"Unknown Cmd %d",retcode);
  //--- default value
  return("Unknown Cmd");
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
inline LPCSTR SIntegralFormat::FormatTransactionName(const UINT value)
  {
  //--- errors
  static IntStringPair transactions[]=
    {
     { TT_PRICES_GET,         "TT_PRICES_GET"         },
     { TT_PRICES_REQUOTE,     "TT_PRICES_REQUOTE"     },
     { TT_ORDER_IE_OPEN,      "TT_ORDER_IE_OPEN"      },
     { TT_ORDER_REQ_OPEN,     "TT_ORDER_REQ_OPEN"     },
     { TT_ORDER_MK_OPEN,      "TT_ORDER_MK_OPEN"      },
     { TT_ORDER_PENDING_OPEN, "TT_ORDER_PENDING_OPEN" },
     { TT_ORDER_IE_CLOSE,     "TT_ORDER_IE_CLOSE"     },
     { TT_ORDER_REQ_CLOSE,    "TT_ORDER_REQ_CLOSE"    },
     { TT_ORDER_MK_CLOSE,     "TT_ORDER_MK_CLOSE"     },
     { TT_ORDER_MODIFY,       "TT_ORDER_MODIFY"       },
     { TT_ORDER_DELETE,       "TT_ORDER_DELETE"       },
     { TT_ORDER_CLOSE_BY,     "TT_ORDER_CLOSE_BY"     },
     { TT_ORDER_CLOSE_ALL,    "TT_ORDER_CLOSE_ALL"    },
     { TT_BR_ORDER_OPEN,      "TT_BR_ORDER_OPEN"      },
     { TT_BR_ORDER_CLOSE,     "TT_BR_ORDER_CLOSE"     },
     { TT_BR_ORDER_DELETE,    "TT_BR_ORDER_DELETE"    },
     { TT_BR_ORDER_CLOSE_BY,  "TT_BR_ORDER_CLOSE_BY"  },
     { TT_BR_ORDER_CLOSE_ALL, "TT_BR_ORDER_CLOSE_ALL" },
     { TT_BR_ORDER_MODIFY,    "TT_BR_ORDER_MODIFY"    },
     { TT_BR_ORDER_ACTIVATE,  "TT_BR_ORDER_ACTIVATE"  },
     { TT_BR_ORDER_COMMENT,   "TT_BR_ORDER_COMMENT"   },
     { TT_BR_BALANCE,         "TT_BR_BALANCE"         }
    };
  //--- search description
  for(UINT i=0;i<_countof(transactions);i++)
    if(transactions[i].value==value) return(transactions[i].name);
  //---
  LOG_ERROR(pluginLog,"Unknown transaction value %d",value);
  //--- default value
  return("UNKNOWN_TT");
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
inline LPCSTR SIntegralFormat::FormatCmdName(const UINT value)
  {
  //--- errors
  static IntStringPair cmd[]=
    {
     { OP_BUY,        "Buy"       },
     { OP_SELL,       "Sell"      },
     { OP_BUY_LIMIT,  "BuyLimit"  },
     { OP_SELL_LIMIT, "SellLimit" },
     { OP_BUY_STOP,   "BuyStop"   },
     { OP_SELL_STOP,  "SellStop"  },
     { OP_BALANCE,    "Balance"   },
     { OP_CREDIT,     "Credit"    }
    };
  //--- search description
  for(UINT i=0;i<_countof(cmd);i++)
    if(cmd[i].value==value) return(cmd[i].name);
  //---
  LOG_ERROR(pluginLog,"Unknown Cmd %d",value);
  //--- default value
  return("Unknown Cmd");
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
inline LPCSTR SIntegralFormat::FormatAddName(const UINT value)
  {
  //--- errors
  static IntStringPair transactions[]=
    {
     { OPEN_NEW,      "OPEN_NEW"      },
     { OPEN_CLOSE,    "OPEN_CLOSE"    },
     { OPEN_RESTORE,  "OPEN_RESTORE"  },
     { OPEN_API,      "OPEN_API"      },
     { OPEN_ROLLOVER, "OPEN_ROLLOVER" }
    };
  //--- search description
  for(UINT i=0;i<_countof(transactions);i++)
    if(transactions[i].value==value) return(transactions[i].name);
  //---
  LOG_ERROR(pluginLog,"Unknown Add Reason: %d",value);
  //--- default value
  return("Unknown Add Reason");
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
inline LPCSTR SIntegralFormat::FormatUpdateName(const UINT value)
  {
  //--- errors
  static IntStringPair transactions[]=
    {
     { UPDATE_NORMAL,      "UPDATE_NORMAL"    },
     { UPDATE_ACTIVATE,    "UPDATE_ACTIVATE"  },
     { UPDATE_CLOSE,       "UPDATE_CLOSE"     },
     { UPDATE_DELETE,      "UPDATE_DELETE"    }
    };
  //--- search description
  for(UINT i=0;i<_countof(transactions);i++)
    if(transactions[i].value==value) return(transactions[i].name);
  //---
  LOG_ERROR(pluginLog,"Unknown Update Reason: %d",value);
  //--- default value
  return("Unknown Update Reason");
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
inline LPCSTR SIntegralFormat::FormatStateName(const UINT value)
  {
  //--- errors
  static IntStringPair transactions[]=
    {
     { TS_OPEN_NORMAL,      "TS_OPEN_NORMAL"   },
     { TS_OPEN_REMAND,      "TS_OPEN_REMAND"   },
     { TS_OPEN_RESTORED,    "TS_OPEN_RESTORED" },
     { TS_CLOSED_NORMAL,    "TS_CLOSED_NORMAL" },
     { TS_CLOSED_PART,      "TS_CLOSED_PART"   },
     { TS_CLOSED_BY,        "TS_CLOSED_BY"     },
     { TS_DELETED,          "TS_DELETED"       }
    };
  //--- search description
  for(UINT i=0;i<_countof(transactions);i++)
    if(transactions[i].value==value) return(transactions[i].name);
  //---
  LOG_ERROR(pluginLog,"Unknown State: %d",value);
  //--- default value
  return("Unknown State");
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
inline LPCSTR SIntegralFormat::FormatLoginType(const UINT value)
  {
  //--- errors
  static IntStringPair transactions[]=
    {
     { LOGIN_FLAG_CLIENT,      "Client"           },
     { LOGIN_FLAG_MOBILE,      "Mobile"           },
     { LOGIN_FLAG_MULTI,       "MultiTerminal"    },
     { LOGIN_FLAG_DC,          "Datacenter"       },
     { LOGIN_FLAG_ADMIN,       "Administrator"    },
     { LOGIN_FLAG_MANAGER,     "Manager"          },
     { LOGIN_FLAG_MANAPI,      "Manager API"      },
     { LOGIN_FLAG_WD,          "WatchDog"         },
     { LOGIN_FLAG_PHONE,       "Phone"            },
     { LOGIN_FLAG_WEB,         "Web"              },
     { LOGIN_FLAG_IPHONE,      "iPhone"           },
     { LOGIN_FLAG_ANDROID,     "Android"          }
    };
  //--- search description
  for(UINT i=0;i<_countof(transactions);i++)
    if(transactions[i].value==value) return(transactions[i].name);
  //---
  LOG_ERROR(pluginLog,"Unknown LoginType: %d",value);
  //--- default value
  return("Unknown LoginType");
  }
//+------------------------------------------------------------------+