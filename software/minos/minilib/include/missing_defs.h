/*
 * Missing definition for minilib C library
 * GPL licensed code (S. Di Mercurio and P. Acco)
 */

#ifndef __MISSING_DEFS__
#define __MISSING_DEFS__

#if !defined(__AVR__)

/* Definition of FILE struct */
struct __FILE
{
	int _file; // only field required for our lib C */
};

#else

#undef _file
#define _file len

#endif /* !defined (__AVR__) */

#endif /* __MISSING_DEFS__ */
