#pragma once

#if defined(__GNUC__) || defined(__llvm__)
#define GNU_EXTS 1
#endif

#ifdef GNU_EXTS
// hint to the predictor
#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)
#else
#define likely(x) (x)
#define unlikely(x) (x)
#endif