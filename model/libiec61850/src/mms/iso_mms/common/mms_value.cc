// Minimal version of mms_value

#include "ns3/mms_value.h"
#include "ns3/mms_value_internal.h"
#include "ns3/lib_memory.h"

static int
bitStringByteSize(const MmsValue* value)
{
    int bitSize = value->value.bitString.size;
    return (bitSize / 8) + ((bitSize % 8) > 0);
}

void
MmsValue_setBitStringFromInteger(MmsValue* self, uint32_t intValue)
{
    int bitPos;

    for (bitPos = 0; bitPos < self->value.bitString.size; bitPos++) {
        if ((intValue & 1) == 1)
            MmsValue_setBitStringBit(self, bitPos, true);
        else
            MmsValue_setBitStringBit(self, bitPos, false);

        intValue = intValue >> 1;
    }
}

void
MmsValue_setBitStringBit(MmsValue* self, int bitPos, bool value)
{
    if (bitPos < self->value.bitString.size) {
        int bytePos = bitPos / 8;
        int bitPosInByte = 7 - (bitPos % 8);

        int bitMask = (1 << bitPosInByte);

        if (value)
            self->value.bitString.buf[bytePos] |= bitMask;
        else
            self->value.bitString.buf[bytePos] &= (~bitMask);
    }
}

bool
MmsValue_getBitStringBit(const MmsValue* self, int bitPos)
{
    if (bitPos < self->value.bitString.size) {
        int bytePos = bitPos / 8;

        int bitPosInByte = 7 - (bitPos % 8);

        int bitMask = (1 << bitPosInByte);

        if ((self->value.bitString.buf[bytePos] & bitMask) > 0)
            return true;
        else
            return false;

    }
    else
        return false; /* out of range bits are always zero */
}

uint32_t
MmsValue_getBitStringAsInteger(const MmsValue* self)
{
    uint32_t value = 0;

    int bitPos;

    for (bitPos = 0; bitPos < self->value.bitString.size; bitPos++) {
        if (MmsValue_getBitStringBit(self, bitPos)) {
            value += (1 << bitPos);
        }
    }

    return value;
}

MmsValue*
MmsValue_newUtcTime(uint32_t timeval)
{
    MmsValue* self = (MmsValue*) GLOBAL_CALLOC(1, sizeof(MmsValue));

    uint8_t* timeArray;
    uint8_t* valueArray;

    if (self == NULL)
        goto exit_function;

    self->type = MMS_UTC_TIME;

    timeArray = (uint8_t*) &timeval;
    valueArray = self->value.utcTime;

#if (ORDER_LITTLE_ENDIAN == 1)
    valueArray[0] = timeArray[3];
    valueArray[1] = timeArray[2];
    valueArray[2] = timeArray[1];
    valueArray[3] = timeArray[0];
#else
    valueArray[0] = timeArray[0];
    valueArray[1] = timeArray[1];
    valueArray[2] = timeArray[2];
    valueArray[3] = timeArray[3];
#endif

    exit_function:
        return self;
}

MmsType
MmsValue_getType(const MmsValue* self)
{
    return self->type;
}

MmsValue*
MmsValue_newBitString(int bitSize)
{
    MmsValue* self = (MmsValue*) GLOBAL_CALLOC(1, sizeof(MmsValue));

    if (self == NULL)
        return NULL;

    self->type = MMS_BIT_STRING;
    self->value.bitString.size = abs(bitSize);
    self->value.bitString.buf = (uint8_t*) GLOBAL_CALLOC(bitStringByteSize(self), 1);

    if (self->value.bitString.buf == NULL) {
        GLOBAL_FREEMEM(self);
        self = NULL;
    }

    return self;
}

int
MmsValue_getBitStringSize(const MmsValue* self)
{
    return self->value.bitString.size;
}

uint32_t
MmsValue_getBitStringAsIntegerBigEndian(const MmsValue* self)
{
    uint32_t value = 0;

    int bitPos;
    int i = 0;

    for (bitPos = (self->value.bitString.size - 1); bitPos >= 0; bitPos--) {

        if (MmsValue_getBitStringBit(self, bitPos)) {
            value += (1 << i);
        }

        i++;
    }

    return value;
}

void
MmsValue_setBitStringFromIntegerBigEndian(MmsValue* self, uint32_t intValue)
{
    int bitPos;

    for (bitPos = (self->value.bitString.size - 1); bitPos >= 0; bitPos--) {
        if ((intValue & 1) == 1)
            MmsValue_setBitStringBit(self, bitPos, true);
        else
            MmsValue_setBitStringBit(self, bitPos, false);

        intValue = intValue >> 1;
    }
}