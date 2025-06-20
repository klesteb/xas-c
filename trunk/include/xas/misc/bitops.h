
#ifndef _XAS_MISC_BITOPS_H_
#define _XAS_MISC_BITOPS_H_

/*
**  Bit set, clear, and test operations
**
**  public domain snippet by Bob Stout 
**  (from the snippets collection - www.snippets.org)
*/

#define LBOOL(x) (!(!(x)))

#define bit_set(arg,posn)   ((arg)      |=  (1L << (posn)))
#define bit_clear(arg,posn) ((arg)      &= ~(1L << (posn)))
#define bit_flip(arg,posn)  ((arg)      ^=  (1L << (posn)))
#define bit_test(arg,posn)  LBOOL((arg) &   (1L << (posn)))

#endif

