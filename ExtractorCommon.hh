/**
 * MaNGOS is a full featured server for World of Warcraft, supporting
 * the following clients: 1.12.x, 2.4.3, 3.3.5a, 4.3.4a and 5.4.8
 *
 * Copyright (C) 2005-2021 MaNGOS <https://getmangos.eu>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * World of Warcraft, and all World of Warcraft or Warcraft art, images,
 * and lore are copyrighted by Blizzard Entertainment, Inc.
 */

#include <vector>
#include <string>
#include <cstdint>


using namespace std;

class ExtractorCommon
{
public:
    ExtractorCommon(){};
    ~ExtractorCommon(){};

    enum class CoreNumber
    {
        CLIENT_CLASSIC = 0,
        CLIENT_TBC = 1,
        CLIENT_WOTLK= 2,
        CLIENT_CATA= 3,
        CLIENT_MOP= 4,
        CLIENT_WOD= 5,
        CLIENT_LEGION = 6,
        CLIENT_UNKNOWN = 255,
    };


protected:
    /** where to open **/
    uint32_t getClientIdentiy(const char *path);
    CoreNumber getCoreNumberByClientIdentiy(uint32_t client_identity);
    void showBanner(const std::string& sTitle, int iCoreNumber);

    const string kTitles [7] { "Classic", "The Burning Bruscade", "Warth of the Lich King", "Cataclym", "Mist of Pandaria","Warlords of Draenor", "Legion"};
                                           
    static constexpr uint32_t kCientIdenties[10] {5875, 6005, 6141, 8606, 12340, 15595, 18274, 18414, 20740, 21355};
    /*                                             c     c     c     tbc   woltk  cata   mop    mop    wod    leg */  

    const std::vector<std::string> kGameBinaries{
        "wow.exe",
        "Wow.exe",
        "WoW.exe",
        "World of Warcraft.exe",
        "World of Warcraft.app/Contents/MacOS/World of Warcraft",
        "World of Warcraft-64.app/Contents/MacOS/World of Warcraft-64"};

    const vector<string> kGameLocales{"deDE", "enCN", "enGB", "enTW", "enUS", "esES", "esMX", "frFR", "itIT", "koKR", "ruRU", "zhCN", "zhTW"};

    const vector<string> MPQList[7] = {{
        /* vanilla */
        "common.MPQ",
        "expansion.MPQ",
        "patch.MPQ",
        "patch-2.MPQ",
        "%s/expansion-locale-%s.MPQ",
        "%s/expansion-speech-%s.MPQ",
        "%s/locale-%s.MPQ",
        "%s/patch-%s-2.MPQ",
        "%s/patch-%s.MPQ",
        "%s/speech-%s.MPQ"        
        },
        /* the burning crusade */
        {"common.MPQ",
        "common-2.MPQ",
        "expansion.MPQ",
        "lichking.MPQ",
        "patch.MPQ",
        "patch-2.MPQ",
        "patch-3.MPQ",
        "%s/expansion-locale-%s.MPQ",
        "%s/expansion-speech-%s.MPQ",
        "%s/lichking-locale-%s.MPQ",
        "%s/lichking-speech-%s.MPQ",
        "%s/locale-%s.MPQ",
        "%s/patch-%s.MPQ",
        "%s/patch-%s-2.MPQ",
        "%s/patch-%s-3.MPQ",
        "%s/speech-%s.MPQ"},
        /*warth of the lich king */
        { 
        "common.MPQ",
        "common-2.MPQ",
        "expansion.MPQ",
        "lichking.MPQ",
        "patch.MPQ",
        "patch-2.MPQ",
        "patch-3.MPQ",
        "%s/expansion-locale-%s.MPQ",
        "%s/expansion-speech-%s.MPQ",
        "%s/lichking-locale-%s.MPQ",
        "%s/lichking-speech-%s.MPQ",
        "%s/locale-%s.MPQ",
        "%s/patch-%s.MPQ",
        "%s/patch-%s-2.MPQ",
        "%s/patch-%s-3.MPQ",
        "%s/speech-%s.MPQ"},
        /* cataclysm */
        { 
        "alternate.MPQ",
        "art.MPQ",
        "expansion1.MPQ",
        "expansion2.MPQ",
        "expansion3.MPQ",
        "world.MPQ",
        "world2.MPQ",
        "%s/expansion1-locale-%s.MPQ",
        "%s/expansion1-speech-%s.MPQ",
        "%s/expansion2-locale-%s.MPQ",
        "%s/expansion2-speech-%s.MPQ",
        "%s/expansion3-locale-%s.MPQ",
        "%s/expansion3-speech-%s.MPQ",
        "%s/locale-%s.MPQ",
        "%s/speech-%s.MPQ" 
         },
         /* mist of pandaria */
         { 
        "alternate.MPQ",
        "expansion1.MPQ",
        "expansion2.MPQ",
        "expansion3.MPQ",
        "expansion4.MPQ",
        "world.MPQ",
        "%s/expansion1-speech-%s.MPQ",
        "%s/expansion2-speech-%s.MPQ",
        "%s/expansion3-speech-%s.MPQ",
        "%s/expansion4-speech-%s.MPQ",
        "%s/locale-%s.MPQ",
        "%s/speech-%s.MPQ"
         },
         { /* warlords of draenor - place holder */
           
         },
         { /* legion - place holder */

         }
        };

};
