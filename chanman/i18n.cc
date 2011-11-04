/*
 * i18n.c: Internationalization
 *
 * See the README file for copyright information and how to reach the author.
 *
 * $Id: i18n.c 1.6 2006/04/22 09:30:57 kls Exp $
 */

#include "i18n.h"

#if APIVERSNUM < 10507

const tI18nPhrase Phrases[] = {
  { "Channel manager plugin",
    "Kanal Manager Plugin",
    "",// TODO
    "Plugin per gestire i canali",
    "",// TODO
    "",// TODO
    "",// TODO
    "",// TODO
    "",
    "",// TODO
    "",// TODO
    "",// TODO
    "",// TODO
    "",// TODO
    "",// TODO
    "",// TODO
    "",
    "",// TODO
    "",// TODO
    "",// TODO
    "",
  },
  { "Channel Manager",
    "Kanal Manager",
    "",// TODO
    "Gestione canali",
    "",// TODO
    "",// TODO
    "",// TODO
    "",// TODO
    "",
    "",// TODO
    "",// TODO
    "",// TODO
    "",// TODO
    "",// TODO
    "",// TODO
    "",// TODO
    "",
    "",// TODO
    "",// TODO
    "",// TODO
    "",
  },
  { "Channel manager",
    "Kanal Manager",
    "",// TODO
    "Gestione canali",
    "",// TODO
    "",// TODO
    "",// TODO
    "",// TODO
    "",
    "",// TODO
    "",// TODO
    "",// TODO
    "",// TODO
    "",// TODO
    "",// TODO
    "",// TODO
    "",
    "",// TODO
    "",// TODO
    "",// TODO
    "",
  },
  { "Radio",
    "Radio",
    "",// TODO
    "Radio",
    "",// TODO
    "",// TODO
    "",// TODO
    "",// TODO
    "",
    "",// TODO
    "",// TODO
    "",// TODO
    "",// TODO
    "",// TODO
    "",// TODO
    "",// TODO
    "",
    "",// TODO
    "",// TODO
    "",// TODO
    "",
  },
  { "Tv",
    "TV",
    "",// TODO
    "Tv",
    "",// TODO
    "",// TODO
    "",// TODO
    "",// TODO
    "",
    "",// TODO
    "",// TODO
    "",// TODO
    "",// TODO
    "",// TODO
    "",// TODO
    "",// TODO
    "",
    "",// TODO
    "",// TODO
    "",// TODO
    "",
  },
  { "Tv Crypt",
    "TV verschlüsselt",
    "",// TODO
    "Tv codif.",
    "",// TODO
    "",// TODO
    "",// TODO
    "",// TODO
    "",
    "",// TODO
    "",// TODO
    "",// TODO
    "",// TODO
    "",// TODO
    "",// TODO
    "",// TODO
    "",
    "",// TODO
    "",// TODO
    "",// TODO
    "",
  },
  { "All",
    "Alle",
    "",// TODO
    "Tutti",
    "",// TODO
    "",// TODO
    "",// TODO
    "",// TODO
    "",
    "",// TODO
    "",// TODO
    "",// TODO
    "",// TODO
    "",// TODO
    "",// TODO
    "",// TODO
    "",
    "",// TODO
    "",// TODO
    "",// TODO
    "",
  },
  { "Group",
    "Gruppe",
    "",// TODO
    "Gruppo",
    "",// TODO
    "",// TODO
    "",// TODO
    "",// TODO
    "",
    "",// TODO
    "",// TODO
    "",// TODO
    "",// TODO
    "",// TODO
    "",// TODO
    "",// TODO
    "",
    "",// TODO
    "",// TODO
    "",// TODO
    "",
  },
  { "Name",
    "Name",
    "",// TODO
    "Nome",
    "",// TODO
    "",// TODO
    "",// TODO
    "",// TODO
    "",
    "",// TODO
    "",// TODO
    "",// TODO
    "",// TODO
    "",// TODO
    "",// TODO
    "",// TODO
    "",
    "",// TODO
    "",// TODO
    "",// TODO
    "",
  },
  { "Channels",
    "Kanäle",
    "",// TODO
    "Canali",
    "",// TODO
    "",// TODO
    "",// TODO
    "",// TODO
    "",
    "",// TODO
    "",// TODO
    "",// TODO
    "",// TODO
    "",// TODO
    "",// TODO
    "",// TODO
    "",
    "",// TODO
    "",// TODO
    "",// TODO
    "",
  },
  { "Src+Name",
    "Quelle+Name",
    "",// TODO
    "Srg+Nome",
    "",// TODO
    "",// TODO
    "",// TODO
    "",// TODO
    "",
    "",// TODO
    "",// TODO
    "",// TODO
    "",// TODO
    "",// TODO
    "",// TODO
    "",// TODO
    "",
    "",// TODO
    "",// TODO
    "",// TODO
    "",
  },
  { "Provider",
    "Provider",
    "",// TODO
    "Provider",
    "",// TODO
    "",// TODO
    "",// TODO
    "",// TODO
    "",
    "",// TODO
    "",// TODO
    "",// TODO
    "",// TODO
    "",// TODO
    "",// TODO
    "",// TODO
    "",
    "",// TODO
    "",// TODO
    "",// TODO
    "",
  },
  { "Src+Provider",
    "Quelle+Provider",
    "",// TODO
    "Srg+Prov.",
    "",// TODO
    "",// TODO
    "",// TODO
    "",// TODO
    "",
    "",// TODO
    "",// TODO
    "",// TODO
    "",// TODO
    "",// TODO
    "",// TODO
    "",// TODO
    "",
    "",// TODO
    "",// TODO
    "",// TODO
    "",
  },
  { "Src+Frequency",
    "Quelle+Frequenz",
    "",// TODO
    "Srg+Freq.",
    "",// TODO
    "",// TODO
    "",// TODO
    "",// TODO
    "",
    "",// TODO
    "",// TODO
    "",// TODO
    "",// TODO
    "",// TODO
    "",// TODO
    "",// TODO
    "",
    "",// TODO
    "",// TODO
    "",// TODO
    "",
  },
  { "Max items for alphabetic order",
    "Max Elemente für alphabetische Sortierung",
    "",// TODO
    "Voci max in ordine Alfabetico",
    "",// TODO
    "",// TODO
    "",// TODO
    "",// TODO
    "",
    "",// TODO
    "",// TODO
    "",// TODO
    "",// TODO
    "",// TODO
    "",// TODO
    "",// TODO
    "",
    "",// TODO
    "",// TODO
    "",// TODO
    "",
  },
  { "Show : 1) Prov.; 2) ChID; 3) Freq.",
    "Zeige : 1) Prov.; 2) ID; 3) Freq.",
    "",// TODO
    "Mostra: 1) Prov 2) ID Can 3) Freq",
    "",// TODO
    "",// TODO
    "",// TODO
    "",// TODO
    "",
    "",// TODO
    "",// TODO
    "",// TODO
    "",// TODO
    "",// TODO
    "",// TODO
    "",// TODO
    "",
    "",// TODO
    "",// TODO
    "",// TODO
    "",
  },
  { "Jump : 1) Alphabetic; 2) Numeric",
    "Springe : 1) Alphabetisch; 2) Numerisch",
    "",// TODO
    "Vai a: 1) Alfabetico 2) Num.",
    "",// TODO
    "",// TODO
    "",// TODO
    "",// TODO
    "",
    "",// TODO
    "",// TODO
    "",// TODO
    "",// TODO
    "",// TODO
    "",// TODO
    "",// TODO
    "",
    "",// TODO
    "",// TODO
    "",// TODO
    "",
  },
  { "Button$Switch",
    "Umschalten",
    "",// TODO
    "Cambia",
    "",// TODO
    "",// TODO
    "",// TODO
    "",// TODO
    "",
    "",// TODO
    "",// TODO
    "",// TODO
    "",// TODO
    "",// TODO
    "",// TODO
    "",// TODO
    "",
    "",// TODO
    "",// TODO
    "",// TODO
    "",
  },
  { NULL }
  };
#endif
