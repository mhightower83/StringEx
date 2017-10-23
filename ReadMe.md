
# CAUTION - This Library is not finished.

# StringEx - Extensions to the Arduino String Class
This Library builds on the Arduino String Class found in `WString.h`.
The Arduino String Class is used as a base class.
This Library allows for the use of cascaded methods separated by "."
To test for success finish the cascade sequence with `.success()`
This will return a bool value like the arduino String class would have
with a single call to concat(), etc.

- [Some Useful Additions](#some-useful-additions)
- [My interpretation of the insert and append methods](#my-interpretation-of-the-insert-and-append-methods)
- [Void Return Arduino String class methods](#void-return-arduino-string-class-methods)
- [The Arduino String Class concat Methods](#the-arduino-string-class-concat-methods)
- [How to test for success](#how-to-test-for-success)
- [Notes](#notes)

## Some Useful Additions
* `unsigned int numHexDigits(unsigned int pos=0)`
  - Starts at _pos_ counting HEX digits.
  - When it encounters a non-HEX digit returns count.
* `unsigned long hexToUL(unsigned int _pos_, unsigned int length, unsigned int *numDigitsProcessed=nullptr)`
  - Converts a string with HEX digits back into a binary value.
  - If the start of the string is not a HEX digit or the number of HEX digits exceeds 8 or the length specified exceeds 16, 0 is returned.
  - On success it returns the converted HEX digits as a 32 bit value
  - _length_ specifies how many digits to process.
  - (optinal) _*numDigitsProcessed_ points to a variable to hold the result of how many valid HEX digits were processed.
* `unsigned long long hexToULL(unsigned int pos, unsigned int length, unsigned int *numDigitsProcessed=nullptr)`
  - Same as hexToUL() except the maximum number of HEX digits is 16 and the  returned value is 64 bits.
* `StringEx & commas(const String & strValue, int fieldWidth=0, int pos=INT_MAX)` --
  - For _strValue_ an ASCII string containing a number is expected. Commas will be added.
  - If _width_ is positive, the new StringEx will be right justified in a field of the _width_ specified.
  - If _width_ is negative, the new string will be left justified in a field of the _width_ specified.
  - If _width_ is zero, it is implied that _width_ is not specified. A simple insert is performed.
  - If _pos_ is specified and positive, the result will be inserted at that index.
  - If _pos_ is specified and negative and this call is tacked onto an existing StringEx variable, the variable is cleared 1st.
* `StringEx & width(int n)`
  - Adds spaces to the string to make its total length the value of _n_.
  - If _n_ is positive, the original string is right justified. Spaces inserted to the left of the string.
  - If _n_ is negative, the original string is left justified. Spaces appended to the string.
* `StringEx & rigidWidth(unsigned int b=true)`
  - This affects `.commas(... )` results.
  - When a value exceeds the width specified and it fits with out the commas,
 commas will not be added.
  - If the value will not fit the field with or with out commas, the field is filled with stars, '\*'.
  - Default behavior, `rigidWidth()` has not been called (_b=false_), is to allow the field to stretch to accommodate the long value.

## My interpretation of the insert and append methods
Added an insert() and append() set of methods similar to C++ string class.
However, no exceptions are thrown on failure.
Use ".success()" to detect errors.
Once an error has occurred, a call to ".success()" is necessary for
subsequent calls to StringEx methods to succeed on an object.
Calling ".success()" clears the error state.
* `StringEx & insert(unsigned int pos, const String & str, unsigned int subpos=0, unsigned int sublen=UINT_MAX)`
* `StringEx & insert(unsigned int pos, const char *cstr, unsigned int clen)`
* `StringEx & insert(unsigned int pos, const __FlashStringHelper *pstr, unsigned int clen)`
* `StringEx & insert(unsigned int pos, const char *cstr)`
* `StringEx & insert(unsigned int pos, const __FlashStringHelper *pstr)`
* `StringEx & insert(unsigned int pos, unsigned int n, char c)`
* `StringEx & append(const String & str, unsigned int subpos=0, unsigned int sublen=UINT_MAX)`
* `StringEx & append(const char *cstr, unsigned int clen)`
* `StringEx & append(const __FlashStringHelper *pstr, unsigned int clen)`
* `StringEx & append(const char *cstr)`
* `StringEx & append(const __FlashStringHelper *pstr)`
* `StringEx & append(unsigned int n, char c)`

Example:
```
Serial.println( StringEx("The quick jumped.").insert(10, String("dog fox cat bird"), 4, 4) );
```
Would print:
```
The quick fox jumped.
```
## Void Return Arduino String class methods
This group of Arduino String Class methods return void.
The StringEx Class wraps each of these calls and returns StringEx.
Thus you can cascade these calls.
* `StringEx & replace(char find, char replace)`
* `StringEx & replace(const String& find, const String& replace)`
* `StringEx & remove(unsigned int index)`
* `StringEx & remove(unsigned int index, unsigned int count)`
* `StringEx & toLowerCase(void)`
* `StringEx & toUpperCase(void)`
* `StringEx & trim(void)`
* `StringEx & setCharAt(unsigned int index, char c)`

Example:
```
Serial.println(StringEx("This is not fun?").remove(7,4).replace('?','!'));
Serial.println(StringEx("    Fun?   ").trim().toUpperCase().setCharAt(3,'!'));
```
Would print:
```
This is fun!
FUN!
```

## The Arduino String Class concat Methods
This group of Arduino String Class methods return true on success.
The StringEx Class places a wrapper around each of these calls so that it can returns StringEx.
It also tracks a failure. So that at the end a series of calls you can test for success.
For the bool result you end the series of calls with `.success()`. For more details see "[How to test for success](#how-to-test-for-success)" below.
* `StringEx & concat(const String &str)`
* `StringEx & concat(const char *cstr)`
* `StringEx & concat(char c)`
* `StringEx & concat(unsigned char c)`
* `StringEx & concat(int num)`
* `StringEx & concat(unsigned int ## How to test for success.num)`
* `StringEx & concat(long num)`
* `StringEx & concat(unsigned long num)`
* `StringEx & concat(float num)`
* `StringEx & concat(double num)`
* `StringEx & concat(const __FlashStringHelper * str)`

Example:
```
if (StringEx("Remember to have some").concat(" fun").success())
  Serial.println("all is good");
```

## How to test for success
A failing `.success()` return can only be seen on the 1st call to `.success()`.
The error is cleared after the call; however,
you can use `.errorLevel()` to get the value of how many calls back the error occured.
The value returned by `.errorLevel()` is only valid after a call to
`.success()` and remains until the next call to `.success()`.
Thus, if you call `.success().success()` then do `.errorlevel()` it will always be zero.
* `unsigned char success(void)`
  - Returns true if all effected StringEx calls have been successful since the last call.
  - Returns false if an error has occured.
* `unsigned char errorLevel(void)`
  - Returns how many calls back the error occured.
* `void setFail(unsigned char fail)`
  - For testing class.
  - Causes `.success()` to report fail.
  - Subsequent calls to `.errorLevel()` will report the _fail_ value.

## Notes
- The practice here of using `unsigned int` for `bool` was copied from the original String Class Library.
- Negative position values implies left, positive implies right.
- The return values from ".errorLevel()" will be ambigious with
the use of "+" and other operators to concat.
You only know that an error has occurred for some operation, not which one.
