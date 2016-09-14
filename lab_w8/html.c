// FILE: html.c
//
// This is some utilites for CS23 search engine project ( or general case)
//
// HTML parser utility implementation
// see html.h for detail usage

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "html.h"

/*

*GetNextURL*
------------

Description: Given a HTML string buffer, the URL of the HTML,
and a position index, find the closest URL after the position
and copy the URL into the result buffer, which is also an input
argument. This function is the main component of the HTML parser
provided as an utility for the TinySearchEngine. This function
is designed such that it is meant to be repeatedly called to
extract URLs from the HTML one at a time, starting from the 
beginning of the HTML and terminating when the end of the HTML
is reached. The return value of this function is meant for the
repeated calls of this function; the real return value is the
third input argument, which is the result buffer, in which a
new URL will be written if one is found. This function can 
handle normal absolute and relative URLs generally  found in
the <a href=""> tags; however, more extreme cases, like this
<a href="../../../a.txt">, are not currently being extracted.

Input: html_buffer, urlofthispage, result_buffer, current_position

Return: Position of the URL found

** Pseudo Code **

(1) IF (first call) THEN
      Remove white space characters from the page
    END
(2) Find the <a> or <A> html tags by scanning through the html text
(3) Keep going until we may have found a URL
(4) IF (It actually is NOT a URL, which has multiple possibilities) THEN
      Recursively call self from the next position
    END
(5) IF (It is an absolute URL) THEN
      Set result buffer to contain this URL
      Return the current position
    ELSE (It is an relative URL) THEN
      Produce the result URL by combining the relative URL with the urlofthispage
      Set result buffer to contain this URL
      Return the current position
    END
(7) Return -1 to signal completion

*****

*/

int GetNextURL(char* html, char* urlofthispage, char* result, int pos)
{
  char c;
  int len, i, j;
  char* p1;  //!< pointer pointed to the start of a new-founded URL.
  char* p2;  //!< pointer pointed to the end of a new-founded URL.

  // NEW
  // Clean up \n chars
  if(pos == 0) {
    removeWhiteSpace(html);
  }
  // /NEW

  // Find the <a> <A> HTML tag.
  while (0 != (c = html[pos])) {
    if ((c=='<') &&
        ((html[pos+1] == 'a') || (html[pos+1] == 'A'))) {
      break;
    }
    pos++;
  }
  //! Find the URL it the HTML tag. They usually look like <a href="www.cs.dartmouth.edu">
  //! We try to find the quote mark in order to find the URL inside the quote mark.
  if (c) {
    
    // Added by Matt Mukerjee
    // check for equals first... some HTML tags don't have quotes...or use single quotes instead
    p1 = strchr(&(html[pos+1]), '=');
    
    if ((!p1) || (*(p1-1) == 'e') || ((p1 - html - pos) > 10)) {
      // keep going...
      return GetNextURL(html,urlofthispage,result,pos+1);
    }
    if (*(p1+1) == '\"' || *(p1+1) == '\'')
      p1++;
    // added by Matt Mukerjee
    p1++;    
    // added by Matt Mukerjee
    p2 = strpbrk(p1, "\'\">");
    if (!p2) {
      // Added by Matt Mukerjee
      // keep going...
      return GetNextURL(html,urlofthispage,result,pos+1);
    }
    if (*p1 == '#') { // Why bother returning anything here....recursively keep going...
      // Added by Matt Mukerjee      
      return GetNextURL(html,urlofthispage,result,pos+1);
    }
    if (!strncmp(p1, "mailto:",7))
      return GetNextURL(html, urlofthispage, result, pos+1);
    if (!strncmp(p1, "http", 4) || !strncmp(p1, "HTTP", 4)) {
      //! Nice! The URL we found is in absolute path.
      strncpy(result, p1, (p2-p1));
      return  (int)(p2 - html + 1);
    } else {
      //! We find a URL. HTML is a terrible standard. So there are many ways to present a URL.
      if (p1[0] == '.') {
        //! Some URLs are like <a href="../../../a.txt"> I cannot handle this. 
	// again...probably good to recursively keep going..
	// NEW
        
        return GetNextURL(html,urlofthispage,result,pos+1);
	// /NEW
      }
      if (p1[0] == '/') {
        //! this means the URL is the absolute path
        for (i = 7; i < strlen(urlofthispage); i++)
          if (urlofthispage[i] == '/')
            break;
        strcpy(result, urlofthispage);
        result[i] = 0;
        strncat(result, p1, (p2 - p1));
        return (int)(p2 - html + 1);        
      } else {
        //! the URL is a absolute path.
        len = strlen(urlofthispage);
        for (i = (len - 1); i >= 0; i--)
          if (urlofthispage[i] == '/')
            break;
        for (j = (len - 1); j >= 0; j--)
          if (urlofthispage[j] == '.')
              break;
        if (i == (len -1)) {
          //! urlofthis page is like http://www.cs.dartmouth.edu/
            strcpy(result, urlofthispage);
            result[i + 1] = 0;
            strncat(result, p1, p2 - p1);
            return (int)(p2 - html + 1);
        }
        if ((i <= 6)||(i > j)) {
          //! urlofthispage is like http://www.cs.dartmouth.edu/~abc
          //! or http://www.cs.dartmouth.edu
          strcpy(result, urlofthispage);
          result[len] = '/';
          strncat(result, p1, p2 - p1);
          return (int)(p2 - html + 1);
        }
        strcpy(result, urlofthispage);
        result[i + 1] = 0;
        strncat(result, p1, p2 - p1);
        return (int)(p2 - html + 1);
      }
    }
  }    
  return -1;
}

/*

*NormalizeWord*
---------------

Description: Make sure all the Roman letters in the URL are
of lower cases, for ease of carrying out string comparison in
the future when trying to decide if two URL are the same or not.
Basically a linear scan, starting from the beginning of the URL,
is performed. Whenever a capital letter character is encountered
(by checking its ASCII code value), it is replaced by the
corresponding lower case letter.

Input: input_url

** Pseudo Code **

(1) FOR (every character in the input string) DO
      IF (this character is a capital letter) DO
        Change this letter to lower case
      END
    DONE

*****

*/

void NormalizeWord(char* word)
{
  int i = 0;
  while (word[i]) {
      // NEW
    if (word[i] < 91 && word[i] > 64) // Bounded below so this funct. can run on all urls
      // /NEW
      word[i] += 32;
    i++;
  }
}

/*

*NormalizeURL*
--------------

Description: Normalize the input URL string and return its validity.
The normalization contains two major components: first, if the
URL ends with a trailing slash '/' character, this trailing slash
will be removed from the URL; and second, if the URL points to
a file (with an extension), then only certain file extensions are
accepted; currently, acceptable normal file extensions start with
.htm, .HTM, .php, or .jsp. A URL is valid if it is long enough and,
if pointing to a file, points to a file of acceptable format;
otherwise the URL is considered invalid and therefore will not
be added to the url_list for future processing.

Input: input_url

Output: validity of the input URL: 0 - invalid
                                   1 - valid

** Pseudo Code **

(1) Return error signal if input url is too short.
(2) IF (input url ends with '/') THEN
      Remove the '/'
    END
(3) Find the positions of the last occurrences of '/' and '.'
(4) IF (the '/' and '.' are positioned s.t. they indicate the url points to a file) THEN
      IF (the file extension starts with .htm or .HTM or .php or .jsp) THEN
        Do nothing...
      ELSE
        Return bad url signal
      END
    END
(5) Return good url signal

*****

*/

int NormalizeURL(char* URL)
{
  int len = strlen(URL);
  if (len <= 1 )
    return 0;
  //! Normalize all URLs.
  if (URL[len - 1] == '/') {
    URL[len - 1] = 0;
    len--;
  }
  int i, j;
  len = strlen(URL);
  //! Safe check.
  if (len < 2)
    return 0;
  //! Locate the URL's suffix.
  for (i = len - 1; i >= 0; i--)
    if (URL[i] =='.')
      break;
  for (j = len - 1; j >= 0; j--)
    if (URL[j] =='/')
      break;
  //! We ignore other file types.
  //! So if a URL link is to a file that are not in the file type of the following
  //! one of four, then we will discard this URL, and it will not be in the URL list.
  if ((j >= 7) && (i > j) && ((i + 2) < len)) {
    if ((!strncmp((URL + i), ".htm", 4))
        ||(!strncmp((URL + i), ".HTM", 4))
        ||(!strncmp((URL + i), ".php", 4))
        ||(!strncmp((URL + i), ".jsp", 4))
        ) {
      len = len; // do nothing.
    } else {
      return 0; // bad type
    }
  }
  return 1;
}

/*

*removeWhiteSpace*
------------------

Description: Removes the white space characters from the input
string buffer that contains the html content. This function
basically scans through the entire html buffer content character
by character, and abandons any white space character it encounters.
The ASCII code of the characters are used to determine whether
a character is a white space or not; Characters with ASCII code
values below 32 are considered white space characters, and are
thus removed.

Input: string_buffer

** Pseudo Code **

(1) Create a target buffer one character than the input buffer, and clear it
(2) FOR (Every character in the input buffer) DO
      IF (the current character is not a while space character) THEN
        Append it to the end of the target buffer
      END
    DONE
(3) Overwrite the input buffer with the target buffer
(4) Release targer buffer
    
*****

*/

void removeWhiteSpace(char* html)
{
  char *buffer = malloc(strlen(html)+1), *in, *out;
  memset(buffer,0,strlen(html)+1);
  out = buffer;
  for (in = html; *in != '\0'; in++) {
    if (*in <= ' ') continue;
	*out++ = *in;
  }
  strcpy(html,buffer);
  free(buffer);
}
