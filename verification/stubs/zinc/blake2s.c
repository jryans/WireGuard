#include <zinc/blake2s.h>

#include <linux/init.h>

#include <klee.h>

int __init blake2s_mod_init(void)
{
	return 0;
}

void blake2s_init(struct blake2s_state *state, const size_t outlen)
{
	klee_trace_param_ptr(state, sizeof(struct blake2s_state), "state");
	klee_trace_param_u64(outlen, "outlen");

	klee_assert(outlen > 0);
	klee_assert(outlen <= BLAKE2S_HASH_SIZE);
}

void blake2s_init_key(struct blake2s_state *state, const size_t outlen,
		      const void *key, const size_t keylen)
{
	klee_trace_param_ptr(state, sizeof(struct blake2s_state), "state");
	klee_trace_param_u64(outlen, "outlen");
        // Trace pointer as a simple value since it's variable length.
	klee_trace_param_u64((uint64_t)key, "key");
	klee_trace_param_u64(keylen, "keylen");

	klee_assert(outlen > 0);
	klee_assert(outlen <= BLAKE2S_HASH_SIZE);
	klee_assert(key);
	klee_assert(keylen > 0);
	klee_assert(keylen <= BLAKE2S_KEY_SIZE);
}

void blake2s_update(struct blake2s_state *state, const u8 *in, size_t inlen)
{
	klee_trace_param_ptr(state, sizeof(struct blake2s_state), "state");
        // Trace pointer as a simple value since it's variable length.
	klee_trace_param_u64((uint64_t)in, "in");
	klee_trace_param_u64(inlen, "inlen");
}

void blake2s_final(struct blake2s_state *state, u8 *out, const size_t outlen)
{
	klee_trace_param_ptr(state, sizeof(struct blake2s_state), "state");
        // Trace pointer as a simple value since it's variable length.
	klee_trace_param_u64((uint64_t)out, "out");
	klee_trace_param_u64(outlen, "outlen");

	klee_assert(out);
	klee_assert(outlen > 0);
	klee_assert(outlen <= BLAKE2S_HASH_SIZE);

	klee_make_symbolic(out, outlen, "blake2s_hash");
}

void blake2s_hmac(u8 *out, const u8 *in, const u8 *key, const size_t outlen,
		  const size_t inlen, const size_t keylen)
{
        // Trace pointer as a simple value since it's variable length.
	klee_trace_param_u64((uint64_t)out, "out");
        // Trace pointer as a simple value since it's variable length.
	klee_trace_param_u64((uint64_t)in, "in");
        // Trace pointer as a simple value since it's variable length.
	klee_trace_param_u64((uint64_t)key, "key");
	klee_trace_param_u64(outlen, "outlen");
	klee_trace_param_u64(inlen, "inlen");
	klee_trace_param_u64(keylen, "keylen");

	klee_assert(out);
	klee_assert(outlen > 0);
	klee_assert(outlen <= BLAKE2S_HASH_SIZE);

	klee_make_symbolic(out, outlen, "blake2s_hash");
}
