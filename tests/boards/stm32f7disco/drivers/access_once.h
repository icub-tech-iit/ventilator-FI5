#ifndef __ACCESS_ONCE_H__
#define __ACCESS_ONCE_H__

#if defined(__ARMCOMPILER_VERSION)
#define ACCESS_ONCE(x) (x)
#else
#define ACCESS_ONCE(x) (*((volatile typeof(x) *)&(x)))
#endif

#endif
