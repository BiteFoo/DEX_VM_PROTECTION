//
// Created by lz on 2017/10/24.
//

#include "Interp.h"
#include "../log.h"
#include <assert.h>
#include "../DexOpcodes.h"


s4 dvmInterpHandlePackedSwitch(const u2* switchData, s4 testVal)
{
    const int kInstrLen = 3;

    /*
     * Packed switch data format:
     *  ushort ident = 0x0100   magic value
     *  ushort size             number of entries in the table
     *  int first_key           first (and lowest) switch case value
     *  int targets[size]       branch targets, relative to switch opcode
     *
     * Total size is (4+size*2) 16-bit code units.
     */
    if (*switchData++ != kPackedSwitchSignature) {
        /* should have been caught by verifier */
        //dvmThrowInternalError("bad packed switch magic");
        return kInstrLen;
    }

    u2 size = *switchData++;
    assert(size > 0);

    s4 firstKey = *switchData++;
    firstKey |= (*switchData++) << 16;

    int index = testVal - firstKey;
    if (index < 0 || index >= size) {
        MY_LOG_VERBOSE("Value %d not found in switch (%d-%d)",
                       testVal, firstKey, firstKey+size-1);
        return kInstrLen;
    }

    /* The entries are guaranteed to be aligned on a 32-bit boundary;
     * we can treat them as a native int array.
     */
    const s4* entries = (const s4*) switchData;
    assert(((u4)(size_t)entries & 0x3) == 0);

    assert(index >= 0 && index < size);
    MY_LOG_VERBOSE("Value %d found in slot %d (goto 0x%02x)",
                   testVal, index,
                   s4FromSwitchData(&entries[index]));
    return s4FromSwitchData(&entries[index]);
}

/*
 * Find the matching case.  Returns the offset to the handler instructions.
 *
 * Returns 3 if we don't find a match (it's the size of the sparse-switch
 * instruction).
 */


s4 dvmInterpHandleSparseSwitch(const u2* switchData, s4 testVal)
{
    const int kInstrLen = 3;
    u2 size;
    const s4* keys;
    const s4* entries;

    /*
     * Sparse switch data format:
     *  ushort ident = 0x0200   magic value
     *  ushort size             number of entries in the table; > 0
     *  int keys[size]          keys, sorted low-to-high; 32-bit aligned
     *  int targets[size]       branch targets, relative to switch opcode
     *
     * Total size is (2+size*4) 16-bit code units.
     */

    if (*switchData++ != kSparseSwitchSignature) {
        /* should have been caught by verifier */
        //dvmThrowInternalError("bad sparse switch magic");
        return kInstrLen;
    }

    size = *switchData++;
    assert(size > 0);

    /* The keys are guaranteed to be aligned on a 32-bit boundary;
     * we can treat them as a native int array.
     */
    keys = (const s4*) switchData;
    assert(((u4)(size_t)keys & 0x3) == 0);

    /* The entries are guaranteed to be aligned on a 32-bit boundary;
     * we can treat them as a native int array.
     */
    entries = keys + size;
    assert(((u4)(size_t)entries & 0x3) == 0);

    /*
     * Binary-search through the array of keys, which are guaranteed to
     * be sorted low-to-high.
     */
    int lo = 0;
    int hi = size - 1;
    while (lo <= hi) {
        int mid = (lo + hi) >> 1;

        s4 foundVal = s4FromSwitchData(&keys[mid]);
        if (testVal < foundVal) {
            hi = mid - 1;
        } else if (testVal > foundVal) {
            lo = mid + 1;
        } else {
            MY_LOG_VERBOSE("Value %d found in entry %d (goto 0x%02x)",
                           testVal, mid, s4FromSwitchData(&entries[mid]));
            return s4FromSwitchData(&entries[mid]);
        }
    }

    MY_LOG_VERBOSE("Value %d not found in switch", testVal);
    return kInstrLen;
}