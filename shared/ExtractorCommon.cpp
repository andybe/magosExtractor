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

#include <fstream>
#include <iostream>
#include <sstream>

#include "ExtractorCommon.hh"

using namespace std;

constexpr uint32_t
ExtractorCommon::kCientIdenties[];

uint32_t
ExtractorCommon::getClientIdentiy(const char *path)
{
    uint32_t client_identity = 0;
    /* 
       Find the game binary by name.
    */
    ifstream *rf;
    stringstream exec_clients;
    for (int i = 0; i < kGameBinaries.size(); i++)
    {
        stringstream filename;
        filename << path << "/" << kGameBinaries[i];
        exec_clients << "'" << kGameBinaries[i] << "'" << " ";
        rf = new ifstream();
        rf->open(filename.str(), ios::out | ios::binary);
        if (rf->is_open())
        {
            break;
        }
        delete (rf);
        rf = nullptr;
    }
    if (!rf)
    {
        cout << "Game binary not found (one of)!" << endl;
        cout << "( " << exec_clients.str() << ")";
        exit(1);
    }

    /** identity byte **/
    unsigned char byteSearchBuffer[1];

    /** byte compare **/
    string mem;
    unsigned char preWOTLK[4];
    unsigned char postTBC[5];

    while (rf->good())
    {
        rf->read((char *)byteSearchBuffer, 1);

        if (byteSearchBuffer[0] == 0x35 || byteSearchBuffer[0] == 0x36 || byteSearchBuffer[0] == 0x38)
        {
            rf->read((char *)preWOTLK, 3);
            mem =  (char *)preWOTLK;

            if (mem.compare("875")==0) //  Vanilla (5)875?
            {
                client_identity = kCientIdenties[0];
            }
            else if (mem.compare("005")==0) //  Vanilla (6)005?
            {
                client_identity = kCientIdenties[1];
            }
            else if (mem.compare("141")==0) //  Vanilla (6)141?
            {
                client_identity = kCientIdenties[2];
            }
            else if (mem.compare("606")==0) //  TBC (8)606?
            {
                client_identity = kCientIdenties[3];
            }
        }
        else if (byteSearchBuffer[0] == 0x31 || byteSearchBuffer[0] == 0x32 ) 
        {

            rf->read((char *)postTBC, 4);
            mem =  (char *)postTBC;

            if ((mem.compare("2340")==0)) // WOTLK (1)2340?
            {
                client_identity = kCientIdenties[4];
            }
            else if ((mem.compare("5595")==0)) //  CATA (1)5595?
            {
                client_identity = kCientIdenties[5];
            }
            else if ((mem.compare("8274")==0)) //  MoP (1)8274?
            {
                client_identity = kCientIdenties[6];
            }
            else if ((mem.compare("8414")==0)) // MoP (1)8414?
            {
                client_identity = kCientIdenties[7];
            }
            else if ((mem.compare("0740")==0)) //  WOD (2)0740?
            {
                client_identity = kCientIdenties[8];
            }
            else if ((mem.compare("1355")==0)) //  LEG (2)1355?
            {
                client_identity = kCientIdenties[9];
            }
        }

        if (client_identity != 0)
        {
            break;
        }
    }

    rf->close();
    delete (rf);

    if (client_identity == 0)
    {
        cout << "Fatal Error: failed to identify build version!" << endl << endl;
        cout << "Supported build versions:" << endl;
        cout << "0 - Vanilla: 5875, 6005, 6141" << endl;
        cout << "1 - TBC:      8606" << endl;
        cout << "2 - WOTLK:    12340" << endl;
        cout << "3 - CATA:     15595" << endl;
        cout << "4 - MOP:      18274/18414 " << endl;
        cout << "5 - WOD:      20740" << endl;
        cout << "6 - LEG:      21355" << endl;
        cout << "Exiting program!!" << endl;
        exit(1);
    }
    int core = static_cast<int>(getCoreNumberByClientIdentiy(client_identity));
    cout << "Client: World of Warcraft - " << kTitles[core] << " (" << client_identity  << ")" <<  endl;

    return client_identity;
}

ExtractorCommon::CoreNumber 
ExtractorCommon::getCoreNumberByClientIdentiy(uint32_t ci)
{
    switch (ci)
    {
    case kCientIdenties[0]:
    case kCientIdenties[1]:
    case kCientIdenties[2]:
        return CoreNumber::CLIENT_CLASSIC;
        break;
    case kCientIdenties[3]:
        return CoreNumber::CLIENT_TBC;
        break;
    case kCientIdenties[4]:
        return CoreNumber::CLIENT_WOTLK;
        break;
    case kCientIdenties[5]:
        return CoreNumber::CLIENT_CATA;
        break;
    case kCientIdenties[6]:
    case kCientIdenties[7]:
        return CoreNumber::CLIENT_MOP;
        break;
    case kCientIdenties[8]:
        return CoreNumber::CLIENT_WOD;
        break;
    case kCientIdenties[9]:
        return CoreNumber::CLIENT_LEGION;
        break;
    }

    return CoreNumber::CLIENT_UNKNOWN;  
}


/**
*  This function displays the standard mangos banner to the console
*
*  @PARAM sTitle is the Title text (directly under the MaNGOS logo)
*  @PARAM iCoreNumber is the Core Number
*/
void
 ExtractorCommon::showBanner(const string& sTitle, int iCoreNumber)
{
    cout << \
        "        __  __      _  _  ___  ___  ___      " << endl << \
        "       |  \\/  |__ _| \\| |/ __|/ _ \\/ __|  " << endl << \
        "       | |\\/| / _` | .` | (_ | (_) \\__ \\  " << endl << \
        "       |_|  |_\\__,_|_|\\_|\\___|\\___/|___/ " << endl << \
        endl << \
        "       " << sTitle; 

    if (iCoreNumber == 255) {
        cout << endl << endl;
        return;
    }

    cout  << " for ";

    switch (iCoreNumber)
    {
    case static_cast<int>(CoreNumber::CLIENT_CLASSIC):
        cout << "MaNGOSZero" << endl;
        break;
    case static_cast<int>(CoreNumber::CLIENT_TBC):
        cout << "MaNGOSOne" << endl;
        break;
    case static_cast<int>(CoreNumber::CLIENT_WOTLK):
        cout << "MaNGOSTwo" << endl;
        break;
    case static_cast<int>(CoreNumber::CLIENT_CATA):
        cout << "MaNGOSThree" << endl;
        break;
    case static_cast<int>(CoreNumber::CLIENT_MOP):
        cout << "MaNGOSFour" << endl;
        break;
    case static_cast<int>(CoreNumber::CLIENT_WOD):
        cout << "MaNGOSFive" << endl;
        break;
    case static_cast<int>(CoreNumber::CLIENT_LEGION):
        cout << "MaNGOSSix" << endl;
        break;
    default:
        cout << "Unknown Version" << endl;
        break;
    }
    cout << "  ________________________________________________" << endl;

}
