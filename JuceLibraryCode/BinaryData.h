/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#ifndef BINARYDATA_H_41088439_INCLUDED
#define BINARYDATA_H_41088439_INCLUDED

namespace BinaryData
{
    extern const char*   avent_mtl;
    const int            avent_mtlSize = 3092;

    extern const char*   avent_obj;
    const int            avent_objSize = 1744913;

    extern const char*   avent_lowpoly_mtl;
    const int            avent_lowpoly_mtlSize = 4121;

    extern const char*   avent_lowpoly_obj;
    const int            avent_lowpoly_objSize = 957874;

    extern const char*   avent2_mtl;
    const int            avent2_mtlSize = 4534;

    extern const char*   aventWheel_mtl;
    const int            aventWheel_mtlSize = 1016;

    extern const char*   aventWheel_obj;
    const int            aventWheel_objSize = 284704;

    extern const char*   aventWheel_lowpoly_mtl;
    const int            aventWheel_lowpoly_mtlSize = 964;

    extern const char*   aventWheel_lowpoly_obj;
    const int            aventWheel_lowpoly_objSize = 196345;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Number of elements in the namedResourceList array.
    const int namedResourceListSize = 9;

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes) throw();
}

#endif
