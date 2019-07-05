/**
 StringEx.h

 Copyright (c) 2017 Michael Hightower. All rights reserved.

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

*/
/*
  Expands on the arduino String class found in WString.h.
  The Arduino String class found in WString.h is used as a base class.

  Allows for the use of cascaded methods separated by "."
  To test for success finish the cascade sequence with .success()
  This will return a bool value like the arduino String class would have
  with a single call to concat(), etc.

  Added an insert() and append() set of methods similar to c++ string class.
  No exceptions are thrown. Use ".success()" to detect errors.
  Once an error has occured, a call to ".success()" is necessary for
  subsequent calls to StringEx methods to succeed on an object.
  Calling ".success()" clears the error state.

  ".errorLevel()" returns the number of StringEx methods that
  have been called since the error occured. This can help you find the
  errored call in a calling cascade. A value of one would refer to the
  call just before the call to ".success()".

  The value returned by ".errorLevel()" is only valid after a call to
  ".success()" and remains until the next call to ".success()".

  The meaning of return values from ".errorLevel()" will be ambigious with
  the use of "+" and other operators to concat. You only know that an error
  occurred for some operation, not which one.

*/

#ifndef STRING_EX_H_
#define STRING_EX_H_

#include <Arduino.h>
#include <limits.h>   // This has the ..._MAX defines
#include <WString.h>

class StringEx : public String //public StringSumHelper
{
  protected:
//  unsigned char concat(const char *cstr, unsigned int length) { return String::concat(cstr, length); }
    using String::concat; // This seems to work for the protected version of concat

  private:
    unsigned char _fail;
    unsigned char _level;
    unsigned char _rigidWidth;

  public:
    StringEx(const String &s) : String::String(s), _fail(0), _level(0), _rigidWidth(0) {}
    StringEx(const StringEx &s) : String::String(s), _fail(0), _level(0), _rigidWidth(0) {}
//D    StringEx(const char *cstr=nullptr) : String::String(cstr), _fail(0), _level(0), _rigidWidth(0) {}
    StringEx(const char *cstr) : String::String(cstr), _fail(0), _level(0), _rigidWidth(0) {}
    StringEx(nullptr_t np=nullptr) : String::String((const char *)0), _fail(0), _level(0), _rigidWidth(0) {(void) np;}
    StringEx(const __FlashStringHelper *pstr) : String::String(pstr), _fail(0), _level(0), _rigidWidth(0) {}
#if 1
    StringEx(char c) : String::String(c), _fail(0), _level(0), _rigidWidth(0) {}
    StringEx(unsigned char uc, unsigned char base = 10) : String::String(uc, base), _fail(0), _level(0), _rigidWidth(0) {}
    StringEx(int i, unsigned char base = 10) : String::String(i, base), _fail(0), _level(0), _rigidWidth(0) {}
    StringEx(unsigned int ui, unsigned char base = 10) : String::String(ui, base), _fail(0), _level(0), _rigidWidth(0) {}
    StringEx(long l, unsigned char base = 10) : String::String(l, base), _fail(0), _level(0), _rigidWidth(0) {}
    StringEx(unsigned long ul, unsigned char base = 10) : String::String(ul, base), _fail(0), _level(0), _rigidWidth(0) {}
    StringEx(float f, unsigned char decimalPlaces = 2) : String::String(f, decimalPlaces), _fail(0), _level(0), _rigidWidth(0) {}
    StringEx(double d, unsigned char decimalPlaces = 2) : String::String(d, decimalPlaces), _fail(0), _level(0), _rigidWidth(0) {}
#endif


    StringEx & operator = (const String &rhs);
    StringEx & operator = (const StringEx &rhs);
    StringEx & release() { String::invalidate(); return *this; }
//    using StringSumHelper::StringSumHelper;

    // Some useful additions
    unsigned int numHexDigits(unsigned int pos=0);
    unsigned long hexToUL(unsigned int pos, unsigned int length, unsigned int *numDigitsProcessed=nullptr);
    unsigned long long hexToULL(unsigned int pos, unsigned int length, unsigned int *numDigitsProcessed=nullptr);
    StringEx & commas(const String & strValue, int fieldWidth=0, int pos=INT_MAX, char separator=',', size_t groupsize=3, int dec=-1, const char decChar='.');
    StringEx & commasDec(const String & strValue, int dec, int fieldWidth=0, int pos=INT_MAX, char separator=',', size_t groupsize=3, const char decChar='.') {
      commas(strValue, fieldWidth, pos, separator, groupsize, dec, decChar);
      return *this;
    }
    StringEx & width(int n, char pad=' ');
    StringEx & rigidWidth(unsigned int b=true) { _rigidWidth=b; return *this;}

    // These calls are modeled after C++ string class
    StringEx & insert(unsigned int pos, const String & str, unsigned int subpos=0, unsigned int sublen=UINT_MAX);
    StringEx & insert(unsigned int pos, const char *cstr, unsigned int clen) {return insert(pos, cstr, 0, clen); }
    StringEx & insert(unsigned int pos, const __FlashStringHelper *pstr, unsigned int clen) {return insert(pos, pstr, 0, clen); }
    StringEx & insert(unsigned int pos, const char *cstr)  {return insert(pos, cstr, 0, UINT_MAX); }
    StringEx & insert(unsigned int pos, const __FlashStringHelper *pstr)  {return insert(pos, pstr, 0, UINT_MAX); }
    StringEx & insert(unsigned int pos, unsigned int n, char c);
    StringEx & insertDec(const unsigned int pos, const char sep='.');
    StringEx & append(const String & str, unsigned int subpos=0, unsigned int sublen=UINT_MAX) {return insert(UINT_MAX, str, subpos, sublen); }
    StringEx & append(const char *cstr, unsigned int clen) {return insert(UINT_MAX, cstr, 0, clen); }
    StringEx & append(const __FlashStringHelper *pstr, unsigned int clen) {return insert(UINT_MAX, pstr, 0, clen); }
    StringEx & append(const char *cstr)  {return insert(UINT_MAX, cstr, 0, UINT_MAX); }
    StringEx & append(const __FlashStringHelper *pstr)  {return insert(UINT_MAX, pstr, 0, UINT_MAX); }
    StringEx & append(unsigned int n, char c) {return insert(UINT_MAX, n, c); }

    // These are modeled after the arduino String class methods with tweaked returns
    // The arduino String class would return void for these calls
    StringEx & replace(char find, char replace) { if(_fail){_fail++;}else{String::replace(find, replace);} return *this; }
    StringEx & replace(const String& find, const String& replace) { if(_fail){_fail++;}else{String::replace(find, replace);} return *this; }
    StringEx & remove(unsigned int index) { if(_fail){_fail++;}else{String::remove(index);} return *this; }
    StringEx & remove(unsigned int index, unsigned int count) { if(_fail){_fail++;}else{String::remove(index, count);} return *this; }
    StringEx & toLowerCase(void) { if(_fail){_fail++;}else{String::toLowerCase();} return *this; };
    StringEx & toUpperCase(void) { if(_fail){_fail++;}else{String::toUpperCase();} return *this; };;
    StringEx & trim(void) { if(_fail){_fail++;}else{String::trim();} return *this; };;
    StringEx & setCharAt(unsigned int index, char c) { if(_fail){_fail++;}else{String::setCharAt(index, c);} return *this; }

    // The arduino String class would return true on success for these calls.
    // This StringEx class returns StringEx. If you want to test for true on success,
    // you end the series of calls with ".correct()". exp. concat("fun").success()
    StringEx & concat(const String &str) { if(_fail){_fail++;}else{_fail=String::concat(str) ? 0:1;} return *this; }
    StringEx & concat(const char *cstr) { if(_fail){_fail++;}else{_fail=String::concat(cstr) ? 0:1;} return *this; }
    StringEx & concat(char c) { if(_fail){_fail++;}else{_fail=String::concat(c) ? 0:1;} return *this; }
    StringEx & concat(unsigned char c) { if(_fail){_fail++;}else{_fail=String::concat(c) ? 0:1;} return *this; }
    StringEx & concat(int num) { if(_fail){_fail++;}else{_fail=String::concat(num) ? 0:1;} return *this; }
    StringEx & concat(unsigned int num) { if(_fail){_fail++;}else{_fail=String::concat(num) ? 0:1;} return *this; };
    StringEx & concat(long num) { if(_fail){_fail++;}else{_fail=String::concat(num) ? 0:1;} return *this; }
    StringEx & concat(unsigned long num) { if(_fail){_fail++;}else{_fail=String::concat(num) ? 0:1;} return *this; }
    StringEx & concat(float num) { if(_fail){_fail++;}else{_fail=String::concat(num) ? 0:1;} return *this; }
    StringEx & concat(double num) { if(_fail){_fail++;}else{_fail=String::concat(num) ? 0:1;} return *this; }
    StringEx & concat(const __FlashStringHelper * str) { if(_fail){_fail++;}else{_fail=String::concat(str) ? 0:1;} return *this; }

    // A failing success() return can only be seen on the 1st call to ".success()", after the event.
    // _fail is always cleared after a call.
    unsigned char success(void) {_level = _fail; _fail=0; return _level == 0; }
    unsigned char errorLevel(void) {return _level;}
    void setFail(unsigned char fail) {_fail=fail; return;} // For testing class
};
#endif // STRING_EX_H_
