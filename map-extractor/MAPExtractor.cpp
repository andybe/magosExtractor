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
#include <iostream>
#include <sstream>
#include <fstream>
#include <regex>

#include "ExtractorCommon.hh"

#include "loadlib.h"

using namespace std;


class MapExtractor : ExtractorCommon {
    public:
        MapExtractor(int , char **);
        ~MapExtractor();

        bool hasGameIdenty();

    private:
        uint32_t client_identy;
        // more data 
        bool map_float_to_int = false;
        // all dbc & maps
        int  extract_parts = 3;
        bool extract_dbc = true;
        bool extract_maps = true;
        // game data
        string *input_path = new string(".");
        // output directory
        string *output_path = new string("."); 

        void Usage(const char *);

        void LoadCommonMPQFiles(CoreNumber);

};

/**
 * @brief
 *
 * @param argc
 * @param argv
 */
MapExtractor::MapExtractor(int argc, char **argv)
{
	showWebsiteBanner();

    try
    {
        for (int i = 1; i < argc; ++i)
        {
            string *arg = new string(argv[i]);
            if (arg->compare("-i") == 0 || arg->compare("--input") == 0)
            {
                i++;
                input_path = new string(argv[i]);
            }
            else if (arg->compare("-o") == 0 || arg->compare("--output") == 0)
            {
                i++;
                output_path = new string(argv[i]);
            }
            else if (arg->compare("-f") == 0 || arg->compare("--flat") == 0)
            {
                i++;
                if (atoi(argv[i]) != 0)
                {
                    map_float_to_int = true;
                }
            }
            else if (arg->compare("-e") == 0 || arg->compare("--extract") == 0)
            {
                i++;
                extract_parts = atoi(argv[i]);
                
                if (extract_parts < 1 || extract_parts > 3)
                {
                    throw invalid_argument("extract parameter must be between 1-3");
                }

                if (extract_parts == 3 || extract_parts == 1 ) extract_maps = true;
                if (extract_parts == 3 || extract_parts == 2 ) extract_dbc = true;
            }
            else if (arg->compare("-h") == 0 || arg->compare("--help") == 0)
            {
                Usage(argv[0]);
                // this isn't a mistake
                exit(0);
            }
        }
    }
    catch (...)
    {
        Usage(argv[0]);
        exit(1);
    }

    // choice
    cout << "Selected Options:" << endl << endl;
    cout << " Input Path: " << input_path->c_str() << endl;
    cout << " Output Path: "<<  output_path->c_str() << endl;
    cout << " Flat export: " << (map_float_to_int ? "true" : "false") << endl;
    cout << " Extract dbc: " << (extract_dbc  ? "true" : "false") << endl;
    cout << " Extract maps: " << (extract_maps ? "true" : "false") << endl;

    if (!extract_dbc && !extract_maps) {
        cout << "Please make a choice of extract!" << endl;
        exit(1);
    }
}

MapExtractor::~MapExtractor()
{
    CloseArchives();
};

/**
 * @brief
 *
 * @param exec
 */
void
MapExtractor::Usage(const char *exec)
{
    cout << " Usage: " << exec << endl;
    cout << " Extract client database files and generate map files." << endl;
    cout << "   -h, --help            show the usage" << endl;
    cout << "   -i, --input <path>    search path for game client archives" << endl;
    cout << "   -o, --output <path>   target path for generated files" << endl;
    cout << "   -f, --flat #          store height information as integers reducing map" << endl;
    cout << "                         size, but also accuracy" << endl;
    cout << "   -e, --extract #       extract specified client data. 1 = maps, 2 = DBCs," << endl;
    cout << "                         3 = both. Defaults to extracting both." << endl << endl;
    cout << " Example:" << endl;
    cout << " - use input path and do not flatten maps:" << endl;
    cout << "   " << exec << " -f 0 -i c:\\games\\world of warcraft\" -o  c:\\games\\world of warcraft\\extract" << endl;
}

bool 
MapExtractor::hasGameIdenty()
{
    cout << endl;
    client_identy = getClientIdentiy(input_path->c_str());
    LoadCommonMPQFiles(getCoreNumberByClientIdentiy(static_cast<int>(client_identy)));
    showBanner("DBC EXtractor & Map Generator");
    return (client_identy ? true : false);
}

void
MapExtractor::LoadCommonMPQFiles(CoreNumber core)
{
    std::stringstream filename;
    ifstream *rf = nullptr;
    string locale = "";

    vector<string> kList = MPQList[static_cast<int>(core)];

    for(int i=0; i < kGameLocals.size();i++) {
        filename.str("");
        filename << input_path->c_str() << "/Data/" << kGameLocals[i] << "/locale-" << kGameLocals[i] << ".MPQ";

        rf = fileExist(filename.str());
        if (rf)
        {
            locale = kGameLocals[i];
            cout << " Found locale: " << locale << endl;
            break;
        }
    }
    if (!rf) {
        cout << " No locals for this client" <<  endl << endl;
        exit(1);
    }
    //close the fileExist
    rf->close();
    delete(rf);
    // Loading MPQ in reverse-order.
    for (int i = (kList.size() - 1); i >= 0; i--)
    {
        filename.str("");
        filename << input_path->c_str() << "/Data/" << kList[i];
        // replace %s elements with real language
        string mpqfile = regex_replace(filename.str(), regex("\\%s"), locale);

        if ((rf = fileExist(mpqfile)))
        {
            rf->close();
            delete(rf);
            HANDLE fileHandle;
            // push the MPQ file handler on the stack
            if (!OpenArchive(mpqfile.c_str(), &fileHandle))
            {
                // files incomplete
                cout << " Error missing client file: "<< mpqfile << endl;
                CloseArchives();
                exit(1);
            }
        }
    }

}

int main(int argc, char **argv)
{
    MapExtractor extractor(argc, argv);

    if (!extractor.hasGameIdenty()) {
        exit(1);
    }

}
