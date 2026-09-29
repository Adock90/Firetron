#include "fireefi.h"

void unicode_str_to_ascii_str(const CHAR16* src, CHAR8* dst)
{
	while (*src)
		*dst++ = (CHAR8)*src++;
	
	*dst = '\0';
}

void ascii_str_to_unicode_str(const CHAR8* src, CHAR16* dst)
{
	while(*src)
		*dst++  = (CHAR16)*src++;
	
	*dst = '\0';
}