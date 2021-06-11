/**
 * Static class consisting of functions for parsing a Pronto Hex string (like 0000 006C 0022 0002 015B 00AD ...)  into an IrSignal,
 * and vice versa.
 * <a href="http://harctoolbox.org/Glossary.html#ProntoSemantics">Reference</a>.
 *
 * Note: Unless you have "infinitely much" memory, it is a very bad idea to put Pronto Hex in your source files.
 * Better is to use, for example IrScruitinizer to convert the signals offline,
 * and put the converted version in your source files instead.
 */

// Maintainer note:
// Make sure that this class does not use floating point arithmetics.

#pragma once

#include "InfraredTypes.h"
#include "IrSignal.h"
#include "Board.h"

class Pronto {
public:
    static constexpr unsigned int digitsInProntoNumber      = 4U;
    static constexpr unsigned int numbersInPreamble         = 4U;

private:
    static constexpr unsigned int bitsInHexadecimal         = 4U;
    static constexpr unsigned int hexMask                   = 0xFU;
    static constexpr uint16_t learnedToken                  = 0x0000U;
    static constexpr uint16_t learnedNonModulatedToken      = 0x0100U;
    static constexpr uint32_t referenceFrequency            = 4145146UL;
    static constexpr uint16_t fallbackFrequencyCode         = 0x0040U; // To use with frequency = 0;
    static constexpr frequency_t fallbackFrequency          = 64767U; // To use with frequency = 0;
    static constexpr uint32_t microsecondsInSeconds         = 1000000UL;

    Pronto() = delete;

    static microseconds_t* mkSequence(const uint16_t *data, size_t pairs, microseconds_t timebase);

    static frequency_t toFrequency(uint16_t code);

    static uint16_t toFrequencyCode(frequency_t frequency);

    static frequency_t effectiveFrequency(frequency_t frequency);

    static microseconds_t toTimebase(frequency_t frequency_t);

    static size_t lengthHexString(size_t introLength, size_t repeatLength);

    static char* prelude(frequency_t frequency, size_t introLength, size_t repeatLength);

    static char hexDigit(unsigned int x);

    static void appendChar(char *result, unsigned int& index, char ch);

    static void appendDuration(char *result, unsigned int& index, uint16_t duration, microseconds_t timebase);

    static void appendDigit(char *result, unsigned int& index, unsigned int number);

    static void appendNumber(char *result, unsigned int& index, uint16_t number);

    static void appendSequence(char *result, unsigned int& index, const microseconds_t *data, size_t length, microseconds_t timebase);

    static void appendSequence(char *result, unsigned int& index, const IrSequence& irSequence, microseconds_t timebase);

    static void dumpSequence(Stream& stream, const microseconds_t *data, size_t length, microseconds_t timebase);

    static void dumpDuration(Stream& stream, microseconds_t duration, microseconds_t timebase);

    static void dumpNumber(Stream& stream, uint16_t number);

    static void dumpDigit(Stream& stream, unsigned int number);

public:
    /**
     * Function for parsing its input data into an IrSignal. The ending sequence will always be empty.
     * @param data Numerical data, the number in the Pronto form.
     * @param size Number of data points.
     * @return IrSignal
     */
    static IrSignal *parse(const uint16_t *data, size_t size);

    /**
     * Function for parsing its input data into an IrSignal. The ending sequence will always be empty.
     * @param str Text string containing a Pronto form signal.
     * @return IrSignal
     */
    static IrSignal *parse(const char *str);

#if HAS_FLASH_READ || defined(DOXYGEN)
    /**
     * Function for parsing its input data into an IrSignal. The ending sequence will always be empty.
     * @param str Text string containing a Pronto form signal.
     * @return IrSignal
     */
    static IrSignal *parse_PF(const uint_farptr_t str);

    static IrSignal *parse_PF(const char * ptr);

    /**
     * Function for parsing its input data into an IrSignal. The ending sequence will always be empty.
     * @param str Text string containing a Pronto form signal.
     * This form handles the F(...) form.
     * Available only on platforms implementing the str*_PF functions.
     * @return IrSignal
     */
    static IrSignal *parse(const __FlashStringHelper *str);
#endif

    /**
     * Function for generating a Pronto Hex string from the argument.
     * @param irSignal
     * @return Zero terminated string. Has been generated by new[], and must be manually delete[]-d by the user.
     */
    static char* toProntoHex(const IrSignal& irSignal) {
        return toProntoHex(irSignal.getIntro(), irSignal.getRepeat(), irSignal.getFrequency());
    }

    /**
     * Function for generating a Pronto Hex string from the arguments.
     *
     * @param introSequence
     * @param repeatSequence
     * @param frequency
     * @return Zero terminated string. Has been generated by new[], and must be manually delete[]-d by the user.
     */
    static char* toProntoHex(const IrSequence& introSequence, const IrSequence& repeatSequence = IrSequence::emptyInstance, frequency_t frequency = IrSignal::defaultFrequency) {
        return toProntoHex(introSequence.getDurations(), introSequence.getLength(), repeatSequence.getDurations(), repeatSequence.getLength(), frequency);
    }

    /**
     * Function for generating a Pronto Hex string from the arguments.
     *
     * @param introData
     * @param introLength
     * @param frequency
     * @return Zero terminated string. Has been generated by new[], and must be manually delete[]-d by the user.
     */
    static char* toProntoHex(const microseconds_t* introData, size_t introLength, frequency_t frequency = IrSignal::defaultFrequency) {
        return toProntoHex(introData, introLength, nullptr, 0, frequency);
    }

    /**
     * Function for generating a Pronto Hex string from the arguments.
     *
     * @param introData
     * @param introLength
     * @param repeatData
     * @param repeatLength
     * @param frequency
     * @return Zero terminated string. Has been generated by new[], and must be manually delete[]-d by the user.
     */
    static char* toProntoHex(const microseconds_t* introData, size_t introLength, const microseconds_t* repeatData = nullptr, size_t repeatLength = 0, frequency_t frequency = IrSignal::defaultFrequency);

    /**
     * Function for printing data as Pronto Hex string on the stream given as argument.
     * @param stream Stream on which to write
     * @param irSignal
     */
    static void dump(Stream& stream, const IrSignal& irSignal) {
        return dump(stream, irSignal.getIntro(), irSignal.getRepeat(), irSignal.getFrequency());
    }

    /**
     * Function for printing data as Pronto Hex string on the stream given as argument.
     *
     * @param stream Stream on which to write
     * @param introSequence
     * @param repeatSequence
     * @param frequency
     */
    static void dump(Stream& stream, const IrSequence& introSequence, const IrSequence& repeatSequence = IrSequence::emptyInstance, frequency_t frequency = IrSignal::defaultFrequency) {
        return dump(stream, introSequence.getDurations(), introSequence.getLength(), repeatSequence.getDurations(), repeatSequence.getLength(), frequency);
    }

    /**
     * Function for printing data as Pronto Hex string on the stream given as argument.
     *
     * @param stream
     * @param introData
     * @param introLength
     * @param repeatData
     * @param repeatLength
     * @param frequency
     */
    static void dump(Stream& stream, const microseconds_t* introData, size_t introLength, const microseconds_t* repeatData = nullptr, size_t repeatLength = 0, frequency_t frequency = IrSignal::defaultFrequency);
};