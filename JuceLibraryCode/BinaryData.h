/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   author_png;
    const int            author_pngSize = 931;

    extern const char*   fbinvertedbutton_png;
    const int            fbinvertedbutton_pngSize = 1189;

    extern const char*   fbinvertedbuttondown_png;
    const int            fbinvertedbuttondown_pngSize = 1199;

    extern const char*   fbinvertedbuttonon_png;
    const int            fbinvertedbuttonon_pngSize = 1194;

    extern const char*   stereobutton_png;
    const int            stereobutton_pngSize = 1416;

    extern const char*   stereobuttondown_png;
    const int            stereobuttondown_pngSize = 1452;

    extern const char*   stereobuttonon_png;
    const int            stereobuttonon_pngSize = 1429;

    extern const char*   timeislockedbutton_png;
    const int            timeislockedbutton_pngSize = 710;

    extern const char*   timeislockedbuttondown_png;
    const int            timeislockedbuttondown_pngSize = 746;

    extern const char*   timeislockedbuttonon_png;
    const int            timeislockedbuttonon_pngSize = 726;

    extern const char*   logo_png;
    const int            logo_pngSize = 15998;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 11;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Points to the start of a list of resource filenames.
    extern const char* originalFilenames[];

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes);

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding original, non-mangled filename (or a null pointer if the name isn't found).
    const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
}
