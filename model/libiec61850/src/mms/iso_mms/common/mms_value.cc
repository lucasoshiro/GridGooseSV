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

static void
setUtcTimeMs(MmsValue* self, uint64_t timeval, uint8_t timeQuality)
{
    uint32_t timeval32 = (timeval / 1000LL);

    uint8_t* timeArray = (uint8_t*) &timeval32;
    uint8_t* valueArray = self->value.utcTime;

#if (ORDER_LITTLE_ENDIAN == 1)
    memcpyReverseByteOrder(valueArray, timeArray, 4);
#else
    memcpy(valueArray, timeArray, 4);
#endif

    uint32_t remainder = (timeval % 1000LL);
    uint32_t fractionOfSecond = (remainder) * 16777 + ((remainder * 216) / 1000);

    /* encode fraction of second */
    valueArray[4] = ((fractionOfSecond >> 16) & 0xff);
    valueArray[5] = ((fractionOfSecond >> 8) & 0xff);
    valueArray[6] = (fractionOfSecond & 0xff);

    /* encode time quality */
    valueArray[7] = timeQuality;
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

uint32_t
MmsValue_getArraySize(const MmsValue* self)
{
    return self->value.structure.size;
}

void
MmsValue_delete(MmsValue* self)
{
    if (self == NULL)
        return;

    switch (self->type)
    {
    case MMS_INTEGER:
    case MMS_UNSIGNED:
        Asn1PrimitiveValue_destroy(self->value.integer);
        break;
    case MMS_BIT_STRING:
        if (self->value.bitString.buf != NULL)
            GLOBAL_FREEMEM(self->value.bitString.buf);
        break;
    case MMS_OCTET_STRING:
        GLOBAL_FREEMEM(self->value.octetString.buf);
        break;
    case MMS_VISIBLE_STRING:
    case MMS_STRING:
        if (self->value.visibleString.buf != NULL)
            GLOBAL_FREEMEM(self->value.visibleString.buf);
        break;
    case MMS_ARRAY:
    case MMS_STRUCTURE:
    {
        int componentCount = self->value.structure.size;
        int i;

        for (i = 0; i < componentCount; i++) {
            if (self->value.structure.components[i] != NULL)
                MmsValue_delete(self->value.structure.components[i]);
        }
    }
        GLOBAL_FREEMEM(self->value.structure.components);
        break;
    default:
        break;
    }

    GLOBAL_FREEMEM(self);
}

MmsValue*
MmsValue_getElement(const MmsValue* complexValue, int index)
{
    if ((complexValue->type != MMS_ARRAY) && (complexValue->type != MMS_STRUCTURE))
        return NULL;

    if ((index < 0) || (index >= complexValue->value.structure.size))
        return NULL;

    return complexValue->value.structure.components[index];
}

MmsValue*
MmsValue_setUtcTimeMs(MmsValue* self, uint64_t timeval)
{
    setUtcTimeMs(self, timeval, 0x0a); /*  set quality as 10 bit sub-second time accuracy */

    return self;
}

MmsValue*
MmsValue_newUtcTimeByMsTime(uint64_t timeval)
{
    MmsValue* self = (MmsValue*) GLOBAL_CALLOC(1, sizeof(MmsValue));

    if (self == NULL)
        return NULL;

    self->type = MMS_UTC_TIME;
    MmsValue_setUtcTimeMs(self, timeval);

    return self;
}

MmsValue*
MmsValue_newIntegerFromInt32(int32_t integer)
{
    MmsValue* self = (MmsValue*) GLOBAL_CALLOC(1, sizeof(MmsValue));

    if (self) {
        self->type = MMS_INTEGER;
        self->value.integer = BerInteger_createFromInt32(integer);

        if (self->value.integer == NULL) {
            GLOBAL_FREEMEM(self);
            self = NULL;
        }
    }

    return self;
}

MmsValue*
MmsValue_newBinaryTime(bool timeOfDay)
{
    MmsValue* self = (MmsValue*) GLOBAL_CALLOC(1, sizeof(MmsValue));

    if (self) {
        self->type = MMS_BINARY_TIME;

        if (timeOfDay == true)
            self->value.binaryTime.size = 4;
        else
            self->value.binaryTime.size = 6;
    }

    return self;
}

MmsValue*
MmsValue_newBoolean(bool boolean)
{
    MmsValue* self = (MmsValue*) GLOBAL_CALLOC(1, sizeof(MmsValue));

    if (self) {
        self->type = MMS_BOOLEAN;

        if (boolean)
            self->value.boolean = 1;
        else
            self->value.boolean = 0;
    }

    return self;
}
