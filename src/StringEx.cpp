/**
 StringEx.cpp

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
#include <StringEx.h>

StringEx & StringEx::insert(unsigned int pos, const String & str, unsigned int subpos, unsigned int sublen) {
  if (_fail) {
    _fail++;
    return *this;
  }

  if (len < pos)
    pos = len;

  if (str.length() == 0)
    return *this;

  if (str.length() < subpos)
    return *this;

  if (str.length() < subpos + sublen)
    sublen = str.length() - subpos;

  if (sublen == 0)
    return *this;

  unsigned int newlen = len + sublen;
  if (!reserve(newlen)) {
    _fail = 1;
    return *this;
  }

  if (len - pos)
    memmove(buffer + pos + sublen, buffer + pos, len - pos);
  strncpy(buffer + pos, str.c_str() + subpos, sublen);
  len = newlen;
  return *this;
}

StringEx & StringEx::width(int n, char pad) {
  if (len >= static_cast<unsigned int>(abs(n)) )
    return *this;

  if (abs(n) <= len)
    return *this;

  if (n >= 0)
    insert(0, static_cast<unsigned int>(n) - len, pad);
  else
    insert(UINT_MAX, static_cast<unsigned int>(-n) - len, pad);

  return *this;
}

StringEx & StringEx::insert(unsigned int pos, unsigned int n, char c) {
  if (_fail) {
    _fail++;
    return *this;
  }

  if (len < pos)
    pos = len;

  if (n == 0)
    return *this;

  unsigned int newlen = len + n;
  // String class's reserve() method adds 16 bytes and truncates to a
  // multiple of 16. This method always leaves (1 - 16 bytes) room for a '\0',
  // thus no + 1 on newlen for terminating character.
  if (!reserve(newlen)) {
    _fail = 1;
    return *this;
  }
  if (len - pos)
    memmove(buffer + pos + n, buffer + pos, len - pos);
  memset(buffer + pos, c, n);
  len = newlen;
  return *this;
}

unsigned int StringEx::numHexDigits(unsigned int pos) {
  unsigned int i;
  if (pos >= len)
    return 0;

  for (i=pos; i<len-pos; i++)
    if (!isHexadecimalDigit(buffer[i]))
      break;

  return i;
}

#if 0
// ToDo - StringEx & StringEx::commas(int fieldWidth) {}
StringEx & StringEx::commas(int fieldWidth) {
  int adjust;

  if ('-' == buf[0])
    adjust=1;
  else
    adjust=0;

  if(len <= 3)
    return *this;

  size_t charCount = len - adjust;
  size_t commaCount = (charCount - 1) / 3;
  size_t index = charCount - commaCount * 3 + adjust; //Where the 1st comma goes

  unsigned newSize = commaCount + charCount + adjust;

  if (newSize > (unsigned)abs(fieldWidth)) {
    if (_rigidWidth) {
      if (len <= (unsigned)abs(fieldWidth)) {
        // It will fit w/o the commas. Lose the commas.
        commaCount = 0;
        index = len;
      } else {
        // Value will not fit. Fill field with stars
        len = 0;
        insert(0, (unsigned)abs(fieldWidth), '*');
        return *this;
      }
    } else {
      if (0 <= fieldWidth)
        fieldWidth = newSize;
      else
        fieldWidth = -newSize;
    }
  }

  len=0;
  reserve((unsigned)abs(fieldWidth));
  len=newSize;
  if (newSize < (unsigned)abs(fieldWidth) && 0 < fieldWidth) // Right justify on possitive
    insert(0, ((unsigned)abs(fieldWidth) - newSize), ' ');

  size_t i, j, k, offset;
  offset  = (0 <= fieldWidth) ? static_cast<size_t>(fieldWidth) - newSize : 0;
  k = (unsigned)abs(fieldWidth) - newSize;
  for (i=j=0; j <= commaCount; j++, index += 3) {
    for (; i < index; i++)
      buffer[offset + i + j] = buffer[i + k];

    if (j < commaCount)
      buffer[offset + i + j] = ',';
  }

  return *this;
}
#endif

StringEx & StringEx::commas(const String & strValue, int fieldWidth, int pos,
                            char separator, size_t groupsize) {
  int adjust;

  if ('-' == strValue.c_str()[0])
    adjust=1;
  else
    adjust=0;

  if (static_cast<unsigned int>(pos) > len)
    pos = len;

  size_t charCount = strValue.length() - adjust;
  size_t commaCount = (charCount - 1) / groupsize;
  size_t index = charCount - commaCount * groupsize + adjust; //Where the 1st comma goes

  unsigned newSize = commaCount + charCount + adjust;

  if (newSize > (unsigned)abs(fieldWidth)) {
    if (_rigidWidth) {
      if (strValue.length() <= (unsigned)abs(fieldWidth)) {
        // It will fit w/o the commas. Lose the commas.
        commaCount = 0;
        index = strValue.length();
      } else {
        // Value will not fit. Fill field with stars
        if (0 <= pos) {
          insert(pos, (unsigned)abs(fieldWidth), '*');
        } else {
          len = 0;
          insert(0, (unsigned)abs(fieldWidth), '*');
        }
        return *this;
      }
    } else {
      if (0 <= fieldWidth)
        fieldWidth = newSize;
      else
        fieldWidth = -newSize;
    }
  }

  if (0 <= pos) {
    insert(pos, abs(fieldWidth), ' ');
  } else {
    len=0;
    reserve((unsigned)abs(fieldWidth));
    len=newSize;
    if (newSize < (unsigned)abs(fieldWidth) && 0 < fieldWidth) // Right justify on possitive
      insert(0, ((unsigned)abs(fieldWidth) - newSize), ' ');
  }

  size_t i, j, offset;
  offset  = (0 <= fieldWidth) ? static_cast<size_t>(fieldWidth) - newSize : 0;
  offset += (0 <= pos)        ? static_cast<size_t>(pos)                  : 0;

  for (i=j=0; j <= commaCount; j++, index += groupsize) {
    for (; i < index; i++)
      buffer[offset + i + j] = strValue.c_str()[i];

    if (j < commaCount)
      buffer[offset + i + j] = separator;
  }

  if (0 > pos) {
    if (newSize < (unsigned)abs(fieldWidth) && 0 > fieldWidth) // Left justify on negative
      append(((unsigned)abs(fieldWidth) - newSize), ' ');
  }

  return *this;
}

unsigned long StringEx::hexToUL(unsigned int pos, unsigned int length, unsigned int *numDigitsProcessed) {
  unsigned long value;
  char tmp;
  char *pNewPos;
  char *pPos = &buffer[pos];

  if (numDigitsProcessed)
    *numDigitsProcessed = 0;

  if (length == 0 || pos >= len)
    return 0;

  if (length > len)
    length = len;

  if (length > sizeof(unsigned long)*2)
    return 0;

  tmp = buffer[pos + length];
  buffer[pos + length] = '\0';
  value = strtoul(pPos, &pNewPos, 16);
  buffer[pos + length] = tmp;

  if (numDigitsProcessed)
    *numDigitsProcessed = (unsigned int)(pNewPos - pPos);
  return value;
}

unsigned long long StringEx::hexToULL(unsigned int pos, unsigned int length, unsigned int *numDigitsProcessed) {
  if (numDigitsProcessed)
    *numDigitsProcessed = 0;

  if (length == 0 || pos >= len)
    return 0;

  if (length > len)
    length = len;

  unsigned int hexCount = numHexDigits(pos);
  if (hexCount > length)
    hexCount = length;

  if (hexCount > sizeof(unsigned long)*4)
    return 0;

  if (numDigitsProcessed)
    *numDigitsProcessed = hexCount;

  unsigned long long highULL = 0;
  size_t highCountLen = 0;
  if (hexCount > sizeof(unsigned long)*2) {
    highCountLen = hexCount - sizeof(unsigned long)*2;
    hexCount = sizeof(unsigned long)*2;
    highULL = static_cast<unsigned long long>(hexToUL(pos, highCountLen, nullptr)) << sizeof(unsigned long)*8;
  }

  return highULL | hexToUL( pos + highCountLen, hexCount, nullptr );
}

StringEx & StringEx::operator = (const StringEx &rhs) {
    if(this == &rhs)
        return *this;

    if(rhs.buffer)
        copy(rhs.buffer, rhs.len);
    else
        invalidate();

    return *this;
}

StringEx & StringEx::operator = (const String &rhs) {
    if(this == &rhs)
        return *this;

    if(rhs.length())
        copy(rhs.c_str(), rhs.length());
    else
        invalidate();

    return *this;
}
