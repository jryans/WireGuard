/*
 * A small wrapper around KLEE intrinsics so they can be left in non-KLEE builds
 * with no effect. In addition, we copy definitions from the normal KLEE headers
 * instead of including them to avoid type conflicts between kernel headers and
 * the stdlib headers that klee.h includes.
 */

#ifndef _KLEE_H
#define _KLEE_H

#ifdef KLEE_VERIFICATION

void klee_make_symbolic(void *addr, size_t nbytes, const char *name);
int klee_range(int begin, int end, const char *name);
#define klee_assert(expr)                                                      \
	((expr) ? (void)(0) :                                                  \
		  __assert_fail(#expr, __FILE__, __LINE__,                     \
				__PRETTY_FUNCTION__))
#define KLEE_TRACE_PARAM_PROTO(suffix, type)                                   \
	void klee_trace_param##suffix(type param, const char *name)
KLEE_TRACE_PARAM_PROTO(f, float);
KLEE_TRACE_PARAM_PROTO(d, double);
KLEE_TRACE_PARAM_PROTO(l, long);
KLEE_TRACE_PARAM_PROTO(ll, long long);
KLEE_TRACE_PARAM_PROTO(_u16, uint16_t);
KLEE_TRACE_PARAM_PROTO(_i32, int32_t);
KLEE_TRACE_PARAM_PROTO(_u32, uint32_t);
KLEE_TRACE_PARAM_PROTO(_i64, int64_t);
KLEE_TRACE_PARAM_PROTO(_u64, int64_t);
#undef KLEE_TRACE_PARAM_PROTO
void klee_trace_param_ptr(void *ptr, int width, const char *name);
typedef enum TracingDirection {
	TD_NONE = 0,
	TD_IN = 1,
	TD_OUT = 2,
	TD_BOTH = 3
} TracingDirection;
void klee_trace_param_ptr_directed(void *ptr, int width, const char *name,
				   TracingDirection td);
void klee_trace_param_tagged_ptr(void *ptr, int width, const char *name,
				 const char *type, TracingDirection td);
void klee_trace_param_just_ptr(void *ptr, int width, const char *name);
void klee_trace_param_fptr(void *ptr, const char *name);
void klee_trace_ret();
void klee_trace_ret_ptr(int width);
void klee_trace_ret_just_ptr(int width);

void klee_trace_param_ptr_field(void *ptr, int offset, int width,
				const char *name);
void klee_trace_param_ptr_field_directed(void *ptr, int offset, int width,
					 const char *name, TracingDirection td);
void klee_trace_param_ptr_field_just_ptr(void *ptr, int offset, int width,
					 const char *name);
void klee_trace_ret_ptr_field(int offset, int width, const char *name);
void klee_trace_ret_ptr_field_just_ptr(int offset, int width, const char *name);
void klee_trace_param_ptr_nested_field(void *ptr, int base_offset, int offset,
				       int width, const char *name);
void klee_trace_param_ptr_nested_field_directed(void *ptr, int base_offset,
						int offset, int width,
						const char *name,
						TracingDirection td);
void klee_trace_ret_ptr_nested_field(int base_offset, int offset, int width,
				     const char *name);
void klee_trace_extra_ptr(void *ptr, int width, const char *name, char *type,
			  TracingDirection td);
void klee_trace_extra_ptr_field(void *ptr, int offset, int width,
				const char *name, TracingDirection td);
void klee_trace_extra_ptr_field_just_ptr(void *ptr, int offset, int width,
					 const char *name);
void klee_trace_extra_ptr_nested_field(void *ptr, int base_offset, int offset,
				       int width, const char *name,
				       TracingDirection td);
void klee_trace_extra_ptr_nested_nested_field(void *ptr, int base_base_offset,
					      int base_offset, int offset,
					      int width, const char *name,
					      TracingDirection td);

#else // KLEE_VERIFICATION

static void klee_make_symbolic(void *addr, size_t nbytes, const char *name)
{
}
static int klee_range(int begin, int end, const char *name)
{
	return begin;
}
#define klee_assert(expr)
#define KLEE_TRACE_PARAM_PROTO(suffix, type)                                   \
	void klee_trace_param##suffix(type param, const char *name)
KLEE_TRACE_PARAM_PROTO(f, float)
{
}
KLEE_TRACE_PARAM_PROTO(d, double)
{
}
KLEE_TRACE_PARAM_PROTO(l, long)
{
}
KLEE_TRACE_PARAM_PROTO(ll, long long)
{
}
KLEE_TRACE_PARAM_PROTO(_u16, uint16_t)
{
}
KLEE_TRACE_PARAM_PROTO(_i32, int32_t)
{
}
KLEE_TRACE_PARAM_PROTO(_u32, uint32_t)
{
}
KLEE_TRACE_PARAM_PROTO(_i64, int64_t)
{
}
KLEE_TRACE_PARAM_PROTO(_u64, int64_t)
{
}
#undef KLEE_TRACE_PARAM_PROTO
void klee_trace_param_ptr(void *ptr, int width, const char *name)
{
}
typedef enum TracingDirection {
	TD_NONE = 0,
	TD_IN = 1,
	TD_OUT = 2,
	TD_BOTH = 3
} TracingDirection;
void klee_trace_param_ptr_directed(void *ptr, int width, const char *name,
				   TracingDirection td)
{
}
void klee_trace_param_tagged_ptr(void *ptr, int width, const char *name,
				 const char *type, TracingDirection td)
{
}
void klee_trace_param_just_ptr(void *ptr, int width, const char *name)
{
}
void klee_trace_param_fptr(void *ptr, const char *name)
{
}
void klee_trace_ret()
{
}
void klee_trace_ret_ptr(int width)
{
}
void klee_trace_ret_just_ptr(int width)
{
}

void klee_trace_param_ptr_field(void *ptr, int offset, int width,
				const char *name)
{
}
void klee_trace_param_ptr_field_directed(void *ptr, int offset, int width,
					 const char *name, TracingDirection td)
{
}
void klee_trace_param_ptr_field_just_ptr(void *ptr, int offset, int width,
					 const char *name)
{
}
void klee_trace_ret_ptr_field(int offset, int width, const char *name)
{
}
void klee_trace_ret_ptr_field_just_ptr(int offset, int width, const char *name)
{
}
void klee_trace_param_ptr_nested_field(void *ptr, int base_offset, int offset,
				       int width, const char *name)
{
}
void klee_trace_param_ptr_nested_field_directed(void *ptr, int base_offset,
						int offset, int width,
						const char *name,
						TracingDirection td)
{
}
void klee_trace_ret_ptr_nested_field(int base_offset, int offset, int width,
				     const char *name)
{
}
void klee_trace_extra_ptr(void *ptr, int width, const char *name, char *type,
			  TracingDirection td)
{
}
void klee_trace_extra_ptr_field(void *ptr, int offset, int width,
				const char *name, TracingDirection td)
{
}
void klee_trace_extra_ptr_field_just_ptr(void *ptr, int offset, int width,
					 const char *name)
{
}
void klee_trace_extra_ptr_nested_field(void *ptr, int base_offset, int offset,
				       int width, const char *name,
				       TracingDirection td)
{
}
void klee_trace_extra_ptr_nested_nested_field(void *ptr, int base_base_offset,
					      int base_offset, int offset,
					      int width, const char *name,
					      TracingDirection td)
{
}

#endif // KLEE_VERIFICATION

#endif // _KLEE_H
