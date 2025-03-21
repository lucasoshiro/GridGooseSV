// Minimal version of mms_value

#include "ns3/mms_value.h"
#include "ns3/mms_value_internal.h"
#include "ns3/lib_memory.h"
#include "ns3/string_utilities.h"
#include "ns3/conversions.h"
#include <cstdio>

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

MmsValue*
MmsValue_newInteger(int size /*integer size in bits*/)
{
    MmsValue* self = (MmsValue*) GLOBAL_CALLOC(1, sizeof(MmsValue));

    if (self) {
        self->type = MMS_INTEGER;

        if (size <= 32)
            self->value.integer = BerInteger_createInt32();
        else
            self->value.integer = BerInteger_createInt64();

        if (self->value.integer == NULL) {
            GLOBAL_FREEMEM(self);
            self = NULL;
        }
    }

    return self;
}

void
MmsValue_setDouble(MmsValue* self, double newDoubleValue)
{
    if (self->type == MMS_FLOAT) {
        if (self->value.floatingPoint.formatWidth == 32) {
            float newFloatValue = (float) newDoubleValue;

            memcpy(self->value.floatingPoint.buf, &newFloatValue, sizeof(float));
        }
        else if (self->value.floatingPoint.formatWidth == 64) {
            memcpy(self->value.floatingPoint.buf, &newDoubleValue, sizeof(double));
        }
    }
}

uint64_t
MmsValue_getUtcTimeInMs(const MmsValue* self)
{
    uint32_t timeval32;
    const uint8_t* valueArray = self->value.utcTime;

#if (ORDER_LITTLE_ENDIAN == 1)
    memcpyReverseByteOrder((uint8_t*) &timeval32, valueArray, 4);
#else
    memcpy((uint8_t*) &timeval32, valueArray, 4);
#endif

    uint32_t fractionOfSecond = 0;

    fractionOfSecond = (valueArray[4] << 16);
    fractionOfSecond += (valueArray[5] << 8);
    fractionOfSecond += (valueArray[6]);

    uint32_t remainder = fractionOfSecond / 16777;

    uint64_t msVal = (timeval32 * 1000LL) + remainder;

    return msVal;
}

MmsValue*
MmsValue_newDouble(double value)
{
    MmsValue* self = (MmsValue*) GLOBAL_CALLOC(1, sizeof(MmsValue));

    if (self) {
        self->type = MMS_FLOAT;
        self->value.floatingPoint.formatWidth = 64;
        self->value.floatingPoint.exponentWidth = 11;

        memcpy(self->value.floatingPoint.buf, &value, sizeof(double));
    }

    return self;
}

MmsValue*
MmsValue_createEmptyArray(int size)
{
    MmsValue* self = (MmsValue*) GLOBAL_CALLOC(1, sizeof(MmsValue));

    if (self == NULL)
        goto exit_function;

    self->type = MMS_ARRAY;
    self->value.structure.size = size;
    self->value.structure.components = (MmsValue**) GLOBAL_CALLOC(size, sizeof(MmsValue*));

    if (self->value.structure.components == NULL) {
        GLOBAL_FREEMEM(self);
        self = NULL;
        goto exit_function;
    }

    int i;
    for (i = 0; i < size; i++) {
        self->value.structure.components[i] = NULL;
    }

    exit_function:
        return self;
}

void
MmsValue_setFloat(MmsValue* self, float newFloatValue)
{
    if (self->type == MMS_FLOAT) {
        if (self->value.floatingPoint.formatWidth == 32) {
            memcpy(self->value.floatingPoint.buf, &newFloatValue, sizeof(float));
        }
        else if (self->value.floatingPoint.formatWidth == 64) {
            double newDoubleValue = (double) newFloatValue;

            memcpy(self->value.floatingPoint.buf, &newDoubleValue, sizeof(double));
        }
    }
}

void
MmsValue_setUtcTimeByBuffer(MmsValue* self, const uint8_t* buffer)
{
    if (buffer) {
        uint8_t* valueArray = self->value.utcTime;

        int i;
        for (i = 0; i < 8; i++) {
            valueArray[i] = buffer[i];
        }
    }
}

void
MmsValue_setBoolean(MmsValue* self, bool boolValue)
{
    self->value.boolean = boolValue;
}

MmsValue*
MmsValue_newUnsigned(int size /*integer size in bits*/)
{
    MmsValue* self = (MmsValue*) GLOBAL_CALLOC(1, sizeof(MmsValue));

    if (self) {
        self->type = MMS_UNSIGNED;

        if (size <= 32)
            self->value.integer = BerInteger_createInt32();
        else
            self->value.integer = BerInteger_createInt64();

        if (self->value.integer == NULL) {
            GLOBAL_FREEMEM(self);
            self = NULL;
        }
    }

    return self;
}

MmsValue*
MmsValue_createEmptyStructure(int size)
{
    MmsValue* self = MmsValue_createEmptyArray(size);

    if (self == NULL)
        return NULL;

    self->type = MMS_STRUCTURE;

    return self;
}

MmsValue*
MmsValue_newOctetString(int size, int maxSize)
{
    MmsValue* self = (MmsValue*) GLOBAL_CALLOC(1, sizeof(MmsValue));

    if (self) {
        self->type = MMS_OCTET_STRING;
        self->value.octetString.size = size;
        self->value.octetString.maxSize = maxSize;
        self->value.octetString.buf = (uint8_t*) GLOBAL_CALLOC(1, abs(maxSize));

        if (self->value.octetString.buf == NULL) {
            GLOBAL_FREEMEM(self);
            self = NULL;
        }
    }

    return self;
}

void
MmsValue_setElement(MmsValue* complexValue, int index, MmsValue* elementValue)
{
    if ((complexValue->type != MMS_ARRAY) && (complexValue->type != MMS_STRUCTURE))
        return;

    if ((index < 0) || (index >= complexValue->value.structure.size))
        return;

    complexValue->value.structure.components[index] = elementValue;
}

MmsValue*
MmsValue_setUtcTime(MmsValue* self, uint32_t timeval)
{
    uint8_t* timeArray = (uint8_t*) &timeval;
    uint8_t* valueArray = self->value.utcTime;

#if (ORDER_LITTLE_ENDIAN == 1)
    memcpyReverseByteOrder(valueArray, timeArray, 4);
#else
    memcpy(valueArray, timeArray, 4);
#endif

    return self;
}

MmsValue*
MmsValue_newFloat(float value)
{
    MmsValue* self = (MmsValue*) GLOBAL_MALLOC(sizeof(MmsValue));

    if (self) {
        self->type = MMS_FLOAT;
        self->value.floatingPoint.formatWidth = 32;
        self->value.floatingPoint.exponentWidth = 8;

        memcpy(self->value.floatingPoint.buf, &value, sizeof(float));
    }

    return self;
}

static MmsValue*
MmsValue_newStringFromByteArray(const uint8_t* byteArray, int size, MmsType type)
{
    MmsValue* self = (MmsValue*) GLOBAL_CALLOC(1, sizeof(MmsValue));

    if (self == NULL)
        goto exit_function;

    self->type = type;

    self->value.visibleString.size = size;

    self->value.visibleString.buf = StringUtils_createStringFromBuffer(byteArray, size);

    if (self->value.visibleString.buf == NULL) {
        GLOBAL_FREEMEM(self);
        self = NULL;
    }

    exit_function:
        return self;
}

MmsValue*
MmsValue_newVisibleStringFromByteArray(const uint8_t* byteArray, int size)
{
    return MmsValue_newStringFromByteArray(byteArray, size, MMS_VISIBLE_STRING);
}

const char*
MmsValue_printToBuffer(const MmsValue* self, char* buffer, int bufferSize)
{
    if (self == NULL) {
        StringUtils_copyStringMax(buffer, bufferSize, "(null)");

        return buffer;
    }

    if (bufferSize)
        buffer[0] = 0;

    switch (MmsValue_getType(self))
    {
    case MMS_STRUCTURE:
    case MMS_ARRAY:
        {
            if (bufferSize == 0)
                break;
            buffer[0] = '{';

            int bufPos = 1;

            int arraySize = MmsValue_getArraySize(self);
            int i;
            for (i = 0; i < arraySize; i++) {

                const char* currentStr = MmsValue_printToBuffer((const MmsValue*) MmsValue_getElement(self, i), buffer + bufPos, bufferSize - bufPos);

                bufPos += strlen(currentStr);

                if (bufPos >= bufferSize)
                    break;

                if (i != (arraySize - 1)) {
                    buffer[bufPos++] = ',';
                }
            }

            if (bufPos < (bufferSize - 1)) {
                buffer[bufPos++] = '}';
                buffer[bufPos] = 0;
            }
            else
                buffer[bufferSize - 1] = 0;

        }
        break;

    case MMS_BINARY_TIME:
        {
            uint8_t tempBuf[20];
            Conversions_msTimeToGeneralizedTime(MmsValue_getBinaryTimeAsUtcMs(self), tempBuf);
            snprintf(buffer, bufferSize, "%s", tempBuf);
        }
        break;

    case MMS_BIT_STRING:
        {
            int bufPos = 0;

            int size = MmsValue_getBitStringSize(self);

            /* fill buffer with zeros */
            if (size > bufferSize) {
                memset(buffer, 0, bufferSize);
                break;
            }

            int i;
            for (i = 0; i < size; i++) {
                if (MmsValue_getBitStringBit(self, i))
                    buffer[bufPos++] = '1';
                else
                    buffer[bufPos++] = '0';
            }
            buffer[bufPos] = 0;
        }
        break;

    case MMS_BOOLEAN:
        if (MmsValue_getBoolean(self))
            StringUtils_copyStringMax(buffer, bufferSize, "true");
        else
            StringUtils_copyStringMax(buffer, bufferSize, "false");

        break;

    case MMS_DATA_ACCESS_ERROR:
        snprintf(buffer, bufferSize, "error %i", self->value.dataAccessError);
        break;

    case MMS_FLOAT:
        snprintf(buffer, bufferSize, "%f", MmsValue_toFloat(self));
        break;

    case MMS_GENERALIZED_TIME: /* type not supported */
        StringUtils_copyStringMax(buffer, bufferSize, "generalized time");

        break;

    case MMS_INTEGER:
        snprintf(buffer, bufferSize, "%lld", (long long) MmsValue_toInt64(self));
        break;

    case MMS_OCTET_STRING:
        {
            int size = MmsValue_getOctetStringSize(self);
            int bufPos = 0;
            int i;
            for (i = 0; i < size; i++) {
                snprintf(buffer + bufPos, bufferSize - bufPos, "%02x", self->value.octetString.buf[i]);
                bufPos += 2;

                if (bufPos >= bufferSize)
                    break;
            }
        }
        break;

    case MMS_UNSIGNED:
        snprintf(buffer, bufferSize, "%u", MmsValue_toUint32(self));
        break;

    case MMS_UTC_TIME:
        {
            uint8_t tempBuf[20];
            Conversions_msTimeToGeneralizedTime(MmsValue_getUtcTimeInMs(self), tempBuf);
            snprintf(buffer, bufferSize, "%s", tempBuf);
        }
        break;

    case MMS_STRING:
    case MMS_VISIBLE_STRING:
        StringUtils_copyStringMax(buffer, bufferSize, MmsValue_toString((MmsValue*) self));

        break;

    default:
        StringUtils_copyStringMax(buffer, bufferSize, "unknown type");

        break;
    }

    return buffer;
}

bool
MmsValue_getBoolean(const MmsValue* self)
{
    return self->value.boolean;
}

float
MmsValue_toFloat(const MmsValue* self)
{
    if (self->type == MMS_FLOAT) {
        if (self->value.floatingPoint.formatWidth == 32) {
            float val;

            memcpy(&val, self->value.floatingPoint.buf, sizeof(float));

            return val;
        }
        else if (self->value.floatingPoint.formatWidth == 64) {
            double val;

            memcpy(&val, self->value.floatingPoint.buf, sizeof(double));

            return (float) val;
        }
    }

    return 0.f;
}

int64_t
MmsValue_toInt64(const MmsValue* self)
{
    int64_t integerValue = 0;

    if ((self->type == MMS_INTEGER) || (self->type == MMS_UNSIGNED))
        BerInteger_toInt64(self->value.integer, &integerValue);

    return integerValue;
}

uint16_t
MmsValue_getOctetStringSize(const MmsValue* self)
{
    return self->value.octetString.size;
}

const char*
MmsValue_toString(MmsValue* self)
{
    if ((self->type == MMS_VISIBLE_STRING) || (self->type == MMS_STRING))
        return self->value.visibleString.buf;

    return NULL;
}

uint32_t
MmsValue_toUint32(const MmsValue* self)
{
    uint32_t integerValue = 0;

    if ((self->type == MMS_INTEGER) || (self->type == MMS_UNSIGNED))
        BerInteger_toUint32(self->value.integer, &integerValue);

    return integerValue;
}

uint64_t
MmsValue_getBinaryTimeAsUtcMs(const MmsValue* self)
{
    uint64_t timestamp = 0;

    const uint8_t* binaryTimeBuf = self->value.binaryTime.buf;

    if (self->value.binaryTime.size == 6) {

        uint16_t daysDiff;

        daysDiff = binaryTimeBuf[4] * 256;
        daysDiff += binaryTimeBuf[5];

        uint64_t mmsTime;

        mmsTime = daysDiff * (86400000LL);

        timestamp = mmsTime + (441763200000LL);
    }

    uint32_t msSinceMidnight = 0;

    msSinceMidnight = binaryTimeBuf[0] << 24;
    msSinceMidnight += binaryTimeBuf[1] << 16;
    msSinceMidnight += binaryTimeBuf[2] << 8;
    msSinceMidnight += binaryTimeBuf[3];

    timestamp += msSinceMidnight;

    return timestamp;
}
