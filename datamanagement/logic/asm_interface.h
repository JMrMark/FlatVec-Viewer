#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void compress_bytes(const char* input, int length, char* output);
void decompress_bytes(const char* input, int length, char* output);
void encode_bytes(const char* input, int length, char* output);
void decode_bytes(const char* input, int length, char* output);

#ifdef __cplusplus
}
#endif
