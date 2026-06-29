  
  #pragma once

#include "settings.h"
#include "secrets.h"



// ---------- Preferences ----------

inline const char* const PREF_SCENES[NUM_KEYS] =
{
    "scene1",
    "scene2",
    "scene3",
    "scene4",
    "scene5",
    "scene6"
};

inline const char* const PREF_SOURCES[NUM_KEYS] =
{
    "sourceKey1",
    "sourceKey2",
    "sourceKey3",
    "sourceKey4",
    "sourceKey5",
    "sourceKey6"
};

// ---------- Default ----------
inline const char* const DEFAULT_SCENES[NUM_KEYS] =
{
    "mix1",
    "Scena 2",
    "Scena 3",
    "arduino",
    "zoom2",
    "telescopio"
};

inline const char* const DEFAULT_SOURCES[NUM_KEYS] =
{
    "Scena 3",
    "Scena 2",
    "schermo",
    "ardu",
    "telescopio",
    "zoom2"
};
inline constexpr char DEFAULT_AUDIO[] = "micv8s";
inline constexpr char DEFAULT_SOURCE_SCENE[] = "mix1";