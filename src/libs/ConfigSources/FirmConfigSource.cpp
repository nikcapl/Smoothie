/*
      This file is part of Smoothie (http://smoothieware.org/). The motion control part is heavily based on Grbl (https://github.com/simen/grbl).
      Smoothie is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
      Smoothie is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
      You should have received a copy of the GNU General Public License along with Smoothie. If not, see <http://www.gnu.org/licenses/>.
*/

#include "libs/Kernel.h"
#include "ConfigValue.h"
#include "FirmConfigSource.h"
#include "ConfigCache.h"
#include <malloc.h>


using namespace std;
#include <string>

// we use objdump in the Makefile to import your config.default file into the compiled code
// Since the two symbols below are derived from the filename, we need to change them if the filename changes
extern char _binary_config_default_start;
extern char _binary_config_default_end;


FirmConfigSource::FirmConfigSource(uint16_t name_checksum){
    this->name_checksum = name_checksum;
}

// Transfer all values found in the file to the passed cache
void FirmConfigSource::transfer_values_to_cache( ConfigCache* cache ){

    char* p = &_binary_config_default_start;
    // For each line
    while( p != &_binary_config_default_end ){
        process_char_from_ascii_config(*p++, cache);
    }
}

// Return true if the check_sums match
bool FirmConfigSource::is_named( uint16_t check_sum ){
    return check_sum == this->name_checksum;
}

// Write a config setting to the file *** FirmConfigSource is read only ***
void FirmConfigSource::write( string setting, string value ){
    //this->kernel->streams->printf("ERROR: FirmConfigSource is read only\r\n");
}

// Return the value for a specific checksum
string FirmConfigSource::read( uint16_t check_sums[3] ){

    string value = "";

    char* p = &_binary_config_default_start;
    // For each line
    while( p != &_binary_config_default_end ){
        value = process_char_from_ascii_config(*p++, check_sums);
        if (value.length())
            return value;
    }

    return value;
}

