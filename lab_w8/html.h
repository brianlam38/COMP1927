// FILE: html.h
//
// This is some utilites for CS23 search engine project ( or general case)
//
// HTML parser utility implementation
// see html.h for detail usage

#ifndef _HTML__H_
#define _HTML__H_

//! This is a html parser to retrieve URL from a html page.
//!
//! \param html the html data to be parsed,
//! \param urlofthispage  the current absolute URL path of the HTML page passed in by \a html, since many websites using relative path
//!                       in there webpages, and we need the absolute URL as output, so we require the current url.
//! \param result the result URL will be written in this buffer. Users should allocate it and set it to zero before calling this function.
//! \param pos from which pos in the HTML do we start parse?
//! \return 1 + the pos of the new founded URL in HTML, -1 if end of doc is reached.
//! \warning sometimes return an empty \a result which means a URL path we cannot understand like "../../a.html".

//! \a Usage Example  (retrieve all URL in a page)
//! int pos = 0;
//! char result[1000];
//! BZERO(result, 1000);
//! while ((pos = GetNextURL(html, urlofthispage, result, pos)) > 0) {
//!     /* DO SOMETHING WITH THE RESULT URL */
//!     BZERO(result, 1000);
//! }
//! Here you retrieve all the URL from the documents. One URL in each loop.
//! \warning Make sure that every time you call this, you've BZEROed the \a result.
//! \warning Make sure result is big enough to hold any URL.
int GetNextURL(char* html, char* urlofthispage, char* result, int pos);

//! \brief Make all letters in word be in lower cases.
void NormalizeWord(char* word);

//! \brief normalize URL 
//! 
//! \param URL to url to be normalized
//! \return 1 if this url is pure text format (html/php/jsp), 0 if it is of other type (pdf/jpg........)
int NormalizeURL(char* URL);

//! \brief removes all white space char's in a large string
void removeWhiteSpace(char* html);

#define IS_ALPHA(c) ((('a'<=(c))&&((c)<='z'))||(('A'<=(c))&&((c)<='Z'))) 

#endif
