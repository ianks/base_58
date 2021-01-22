#include "b58.h"
#include "./../libbase58/base58.c"

#include <openssl/sha.h>
#include <stdbool.h>
#include <unistd.h>

VALUE rb_mBase58;
VALUE rb_mBase58_Error;
VALUE rb_mBase58_EncodeError;
VALUE rb_mBase58_DecodeError;

static bool my_sha256(void *md, const void *input, size_t length) {
  SHA256_CTX context;
  if (!SHA256_Init(&context))
    return false;

  if (!SHA256_Update(&context, (unsigned char *)input, length))
    return false;

  if (!SHA256_Final(md, &context))
    return false;

  return true;
}

VALUE rb_mBase58_decode_58(VALUE self, VALUE str) {
  Check_Type(str, T_STRING);

  size_t input_size;
  input_size = RSTRING_LEN(str);

  size_t output_size;
  output_size = input_size * 733 / 1000 + 1;

  char outbuf[output_size];
  char *plaintxt;

  plaintxt = RSTRING_PTR(str);

  bool rv;

  rv = b58tobin(outbuf, &output_size, plaintxt, input_size);

  if (!rv) {
    rb_raise(rb_mBase58_EncodeError, "Could not encode string in base58");
    return Qnil;
  }

  // Account for null termination
  VALUE outstring;
  outstring = rb_str_new(outbuf, output_size);
  return outstring;
}

VALUE rb_mBase58_encode_58(VALUE self, VALUE str) {
  Check_Type(str, T_STRING);

  size_t input_size;
  input_size = RSTRING_LEN(str);

  size_t output_size;
  output_size = input_size * 2;

  char outbuf[output_size];
  char *plaintxt;

  plaintxt = RSTRING_PTR(str);

  bool rv;

  rv = b58enc(outbuf, &output_size, plaintxt, input_size);

  if (!rv) {
    rb_raise(rb_mBase58_EncodeError, "Could not encode string in base58");
    return Qnil;
  }

  // Account for null termination
  return rb_str_new(outbuf, output_size - 1);
}

VALUE rb_mBase58_check_58(VALUE self, VALUE str) {
  Check_Type(str, T_STRING);

  size_t input_size;
  input_size = RSTRING_LEN(str);

  size_t output_size;
  output_size = input_size * 2;

  char outbuf[output_size];

  char *plaintxt;
  plaintxt = RSTRING_PTR(str);

  int version;
  version = plaintxt[0];

  bool rv;

  rv =
      b58check_enc(outbuf, &output_size, version, &plaintxt[1], input_size - 1);

  if (rv) {
    return Qtrue;
  } else {
    return Qfalse;
  }
}

void Init_b58(void) {
  b58_sha256_impl = my_sha256;

  rb_mBase58 = rb_define_module("Base58");
  rb_mBase58_Error =
      rb_define_class_under(rb_mBase58, "Error", rb_eStandardError);
  rb_mBase58_EncodeError =
      rb_define_class_under(rb_mBase58, "EncodeError", rb_mBase58_Error);
  rb_mBase58_DecodeError =
      rb_define_class_under(rb_mBase58, "EncodeError", rb_mBase58_Error);

  rb_define_module_function(rb_mBase58, "encode_58", rb_mBase58_encode_58, 1);
  rb_define_module_function(rb_mBase58, "decode_58", rb_mBase58_decode_58, 1);
  rb_define_module_function(rb_mBase58, "check_58", rb_mBase58_check_58, 1);
}
