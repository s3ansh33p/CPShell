/**
 * @file rtc.hpp
 * @author SnailMath (from https://github.com/SnailMath/CPclock/blob/main/rtc.hpp)
 * @author Modified by Sean McGinty
 * @brief Real Time Clock for the Classpad
 * @version 1.0
 * @date 2022-06-27
 */

#pragma once

class RTC {
    public:
        uint8_t getSeconds() {
            int sd = regRead(RSECCNT);
            // convert from binary coded decimal
            return (sd&0xf) + (((sd&0xf0)>>4)*10);
        }
        uint8_t getMinutes() {
            int md = regRead(RMINCNT);
            // convert from binary coded decimal
            return (md&0xf) + (((md&0xf0)>>4)*10);
        }
        uint8_t getHours() {
            int hd = regRead(RHRCNT);
            // convert from binary coded decimal
            return (hd&0xf) + (((hd&0xf0)>>4)*10);
        }
        uint8_t getDay() {
            return regRead(RDAYCNT);
        }
        uint8_t getMonth() {
            return regRead(RMONCNT);
        }
        uint16_t getYear() {
            return regRead(RYRCNT);
        }

        void setSeconds(uint8_t seconds) {
            setAny(RSECCNT, seconds);
        }
        void setMinutes(uint8_t minutes) {
            setAny(RMINCNT, minutes);
        }
        void setHours(uint8_t hours) {
            setAny(RHRCNT, hours);
        }
        void setDay(uint8_t day) {
            setAny(RDAYCNT, day);
        }
        void setMonth(uint8_t month) {
            setAny(RMONCNT, month);
        }
        void setYear(uint16_t year) {
            regClearBit(RTC_START);
            regWrite(RYRCNT, year);
            regSetBit(RTC_START);
        }

        // up to 6 arguments are supported
        void setDate(uint8_t seconds){setDate(seconds, getMinutes(), getHours(), getDay(), getMonth(), getYear());}
        void setDate(uint8_t seconds, uint8_t minutes){setDate(seconds, minutes, getHours(), getDay(), getMonth(), getYear());}
        void setDate(uint8_t seconds, uint8_t minutes, uint8_t hours){setDate(seconds, minutes, hours, getDay(), getMonth(), getYear());}
        void setDate(uint8_t seconds, uint8_t minutes, uint8_t hours, uint8_t days){setDate(seconds, minutes, hours, days, getMonth(), getYear());}
        void setDate(uint8_t seconds, uint8_t minutes, uint8_t hours, uint8_t days, uint8_t months){setDate(seconds, minutes, hours, days, months, getYear());}

        void setDate(uint8_t seconds, uint8_t minutes, uint8_t hours, uint8_t days, uint8_t months, uint16_t year) {
            regClearBit(RTC_START);
			regWrite(RSECCNT, seconds);
			regWrite(RMINCNT, minutes);
			regWrite(RHRCNT,  hours);
			regWrite(RDAYCNT, days);
			regWrite(RMONCNT, months);
			regWrite(RYRCNT,  year);
			regSetBit(RTC_START);
        }
    private:
        static const unsigned int baseAddress = 0xA413FEC0;
        enum Registers16 {
            RYRCNT	= 0x0E, //Year counter
        };
        enum Registers8 {
            R64CNT	= 0x00, //64Hz counter
            RSECCNT	= 0x02, //Second counter
            RMINCNT	= 0x04, //Minute counter
            RHRCNT	= 0x06, //Hour   counter
            RWKCNT	= 0x08, //Day of week counter
            RDAYCNT	= 0x0A, //Date counter
            RMONCNT	= 0x0C, //Month counter
            RCR2	= 0x1E, //RTC Control Register 2

        };

        inline volatile uint8_t  *regAddress (Registers8 reg) {
            return reinterpret_cast<volatile uint8_t* >(baseAddress +  reg);
        }
        inline volatile uint16_t *regAddress (Registers16 reg) {
            return reinterpret_cast<volatile uint16_t*>(baseAddress +  reg);
        }

        inline uint8_t  regRead(Registers8  reg){
            return *regAddress(reg);
        }
        inline uint16_t regRead(Registers16 reg){
            return *regAddress(reg);
        }

        inline void regWrite(Registers8  reg, uint8_t  value){
            *regAddress(reg) = value;
        }
        inline void regWrite(Registers16 reg, uint16_t value){
            *regAddress(reg) = value;
        }

        void setAny(Registers8 reg, uint8_t value) {
            regClearBit(RTC_START);
            regWrite(reg, value);
            regSetBit(RTC_START);
        }

        enum RCR2_BITS{
            RTC_START	= 0b00000001, //enable to start, stop to change time
            RTC_RESET	= 0b00000010, //reset divider circuit
            RTC_ROUND	= 0b00000100, //30second adjust
            RTC_HALT	= 0b00001000, //halt crystal oscillator
            RTC_INT0	= 0b00010000, //Timer Interrupt settings
            RTC_INT1	= 0b00100000, //0=disabled, 1=1/256s, 2=1/64s, 3=1/16s
            RTC_INT2	= 0b01000000, //4=1/4s, 5=1/2s, 6=1s, 7=2s
            RTC_INTERRUPT	= 0b10000000  //reset to clear interrupt
        };
        inline bool regTestBit(	RCR2_BITS bit){
            return regRead(	RCR2) & bit;}
        inline void regSetBit(	RCR2_BITS bit){
            regWrite(	RCR2,regRead(
                    RCR2)|bit);}
        inline void regClearBit(RCR2_BITS bit){
            regWrite(	RCR2,regRead(
                    RCR2)&(~bit));}
};

// create instance
RTC rtc;
