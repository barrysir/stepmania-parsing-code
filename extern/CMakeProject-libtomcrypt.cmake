# This is taken from src/Makefile.am in the Stepmania repo
set(LIBTOMCRYPT_SRC
libtomcrypt/src/ciphers/aes/aes.c 
libtomcrypt/src/hashes/md5.c libtomcrypt/src/hashes/sha1.c libtomcrypt/src/hashes/helper/hash_memory.c 
libtomcrypt/src/math/fp/ltc_ecc_fp_mulmod.c libtomcrypt/src/math/ltm_desc.c libtomcrypt/src/math/multi.c 
libtomcrypt/src/math/rand_prime.c libtomcrypt/src/misc/base64/base64_decode.c 
libtomcrypt/src/misc/base64/base64_encode.c libtomcrypt/src/misc/burn_stack.c libtomcrypt/src/misc/crypt/crypt.c 
libtomcrypt/src/misc/crypt/crypt_argchk.c libtomcrypt/src/misc/crypt/crypt_cipher_descriptor.c 
libtomcrypt/src/misc/crypt/crypt_cipher_is_valid.c libtomcrypt/src/misc/crypt/crypt_find_cipher.c 
libtomcrypt/src/misc/crypt/crypt_find_cipher_any.c libtomcrypt/src/misc/crypt/crypt_find_cipher_id.c 
libtomcrypt/src/misc/crypt/crypt_find_hash.c libtomcrypt/src/misc/crypt/crypt_find_hash_any.c 
libtomcrypt/src/misc/crypt/crypt_find_hash_id.c libtomcrypt/src/misc/crypt/crypt_find_hash_oid.c 
libtomcrypt/src/misc/crypt/crypt_find_prng.c libtomcrypt/src/misc/crypt/crypt_fsa.c libtomcrypt/src/misc/crypt/crypt_hash_descriptor.c 
libtomcrypt/src/misc/crypt/crypt_hash_is_valid.c libtomcrypt/src/misc/crypt/crypt_ltc_mp_descriptor.c 
libtomcrypt/src/misc/crypt/crypt_prng_descriptor.c libtomcrypt/src/misc/crypt/crypt_prng_is_valid.c 
libtomcrypt/src/misc/crypt/crypt_register_cipher.c libtomcrypt/src/misc/crypt/crypt_register_hash.c 
libtomcrypt/src/misc/crypt/crypt_register_prng.c libtomcrypt/src/misc/crypt/crypt_unregister_cipher.c 
libtomcrypt/src/misc/crypt/crypt_unregister_hash.c libtomcrypt/src/misc/crypt/crypt_unregister_prng.c 
libtomcrypt/src/misc/error_to_string.c libtomcrypt/src/misc/pkcs5/pkcs_5_1.c libtomcrypt/src/misc/pkcs5/pkcs_5_2.c libtomcrypt/src/misc/zeromem.c 
libtomcrypt/src/modes/cbc/cbc_decrypt.c libtomcrypt/src/modes/cbc/cbc_done.c libtomcrypt/src/modes/cbc/cbc_encrypt.c 
libtomcrypt/src/modes/cbc/cbc_getiv.c libtomcrypt/src/modes/cbc/cbc_setiv.c libtomcrypt/src/modes/cbc/cbc_start.c 
libtomcrypt/src/modes/cfb/cfb_decrypt.c libtomcrypt/src/modes/cfb/cfb_done.c libtomcrypt/src/modes/cfb/cfb_encrypt.c 
libtomcrypt/src/modes/cfb/cfb_getiv.c libtomcrypt/src/modes/cfb/cfb_setiv.c libtomcrypt/src/modes/cfb/cfb_start.c 
libtomcrypt/src/modes/ctr/ctr_decrypt.c libtomcrypt/src/modes/ctr/ctr_done.c libtomcrypt/src/modes/ctr/ctr_encrypt.c 
libtomcrypt/src/modes/ctr/ctr_getiv.c libtomcrypt/src/modes/ctr/ctr_setiv.c libtomcrypt/src/modes/ctr/ctr_start.c libtomcrypt/src/modes/ctr/ctr_test.c 
libtomcrypt/src/modes/ecb/ecb_decrypt.c libtomcrypt/src/modes/ecb/ecb_done.c libtomcrypt/src/modes/ecb/ecb_encrypt.c 
libtomcrypt/src/modes/ecb/ecb_start.c 
libtomcrypt/src/modes/ofb/ofb_decrypt.c libtomcrypt/src/modes/ofb/ofb_done.c 
libtomcrypt/src/pk/asn1/der/bit/der_decode_bit_string.c 
libtomcrypt/src/pk/asn1/der/bit/der_encode_bit_string.c libtomcrypt/src/pk/asn1/der/bit/der_length_bit_string.c 
libtomcrypt/src/pk/asn1/der/boolean/der_decode_boolean.c libtomcrypt/src/pk/asn1/der/boolean/der_encode_boolean.c 
libtomcrypt/src/pk/asn1/der/boolean/der_length_boolean.c libtomcrypt/src/pk/asn1/der/choice/der_decode_choice.c 
libtomcrypt/src/pk/asn1/der/ia5/der_decode_ia5_string.c libtomcrypt/src/pk/asn1/der/ia5/der_encode_ia5_string.c 
libtomcrypt/src/pk/asn1/der/ia5/der_length_ia5_string.c libtomcrypt/src/pk/asn1/der/integer/der_decode_integer.c 
libtomcrypt/src/pk/asn1/der/integer/der_encode_integer.c libtomcrypt/src/pk/asn1/der/integer/der_length_integer.c 
libtomcrypt/src/pk/asn1/der/object_identifier/der_decode_object_identifier.c 
libtomcrypt/src/pk/asn1/der/object_identifier/der_encode_object_identifier.c 
libtomcrypt/src/pk/asn1/der/object_identifier/der_length_object_identifier.c 
libtomcrypt/src/pk/asn1/der/octet/der_decode_octet_string.c libtomcrypt/src/pk/asn1/der/octet/der_encode_octet_string.c 
libtomcrypt/src/pk/asn1/der/octet/der_length_octet_string.c 
libtomcrypt/src/pk/asn1/der/printable_string/der_decode_printable_string.c 
libtomcrypt/src/pk/asn1/der/printable_string/der_encode_printable_string.c 
libtomcrypt/src/pk/asn1/der/printable_string/der_length_printable_string.c 
libtomcrypt/src/pk/asn1/der/sequence/der_decode_sequence_ex.c 
libtomcrypt/src/pk/asn1/der/sequence/der_decode_sequence_flexi.c 
libtomcrypt/src/pk/asn1/der/sequence/der_decode_sequence_multi.c 
libtomcrypt/src/pk/asn1/der/sequence/der_encode_sequence_ex.c 
libtomcrypt/src/pk/asn1/der/sequence/der_encode_sequence_multi.c libtomcrypt/src/pk/asn1/der/sequence/der_length_sequence.c 
libtomcrypt/src/pk/asn1/der/sequence/der_sequence_free.c libtomcrypt/src/pk/asn1/der/set/der_encode_set.c 
libtomcrypt/src/pk/asn1/der/set/der_encode_setof.c libtomcrypt/src/pk/asn1/der/short_integer/der_decode_short_integer.c 
libtomcrypt/src/pk/asn1/der/short_integer/der_encode_short_integer.c 
libtomcrypt/src/pk/asn1/der/short_integer/der_length_short_integer.c libtomcrypt/src/pk/asn1/der/utctime/der_decode_utctime.c 
libtomcrypt/src/pk/asn1/der/utctime/der_encode_utctime.c libtomcrypt/src/pk/asn1/der/utctime/der_length_utctime.c 
libtomcrypt/src/pk/asn1/der/utf8/der_decode_utf8_string.c libtomcrypt/src/pk/asn1/der/utf8/der_encode_utf8_string.c 
libtomcrypt/src/pk/asn1/der/utf8/der_length_utf8_string.c libtomcrypt/src/pk/dsa/dsa_decrypt_key.c 
libtomcrypt/src/pk/dsa/dsa_encrypt_key.c libtomcrypt/src/pk/dsa/dsa_export.c libtomcrypt/src/pk/dsa/dsa_free.c libtomcrypt/src/pk/dsa/dsa_import.c 
libtomcrypt/src/pk/dsa/dsa_make_key.c libtomcrypt/src/pk/dsa/dsa_shared_secret.c libtomcrypt/src/pk/dsa/dsa_sign_hash.c 
libtomcrypt/src/pk/dsa/dsa_verify_hash.c libtomcrypt/src/pk/dsa/dsa_verify_key.c 
libtomcrypt/src/pk/pkcs1/pkcs_1_i2osp.c libtomcrypt/src/pk/pkcs1/pkcs_1_mgf1.c 
libtomcrypt/src/pk/pkcs1/pkcs_1_oaep_decode.c libtomcrypt/src/pk/pkcs1/pkcs_1_oaep_encode.c libtomcrypt/src/pk/pkcs1/pkcs_1_os2ip.c 
libtomcrypt/src/pk/pkcs1/pkcs_1_pss_decode.c libtomcrypt/src/pk/pkcs1/pkcs_1_pss_encode.c libtomcrypt/src/pk/pkcs1/pkcs_1_v1_5_decode.c 
libtomcrypt/src/pk/pkcs1/pkcs_1_v1_5_encode.c libtomcrypt/src/pk/rsa/rsa_decrypt_key.c libtomcrypt/src/pk/rsa/rsa_encrypt_key.c 
libtomcrypt/src/pk/rsa/rsa_export.c libtomcrypt/src/pk/rsa/rsa_exptmod.c libtomcrypt/src/pk/rsa/rsa_free.c libtomcrypt/src/pk/rsa/rsa_import.c 
libtomcrypt/src/pk/rsa/rsa_make_key.c libtomcrypt/src/pk/rsa/rsa_sign_hash.c libtomcrypt/src/pk/rsa/rsa_verify_hash.c libtomcrypt/src/prngs/fortuna.c 
libtomcrypt/src/prngs/rng_get_bytes.c libtomcrypt/src/prngs/rng_make_prng.c 
libtomcrypt/src/prngs/sprng.c libtomcrypt/src/prngs/yarrow.c 
)

set(LIBTOMCRYPT_HPP
libtomcrypt/src/headers/tomcrypt_argchk.h libtomcrypt/src/headers/tomcrypt_cfg.h libtomcrypt/src/headers/tomcrypt_cipher.h 
libtomcrypt/src/headers/tomcrypt_custom.h libtomcrypt/src/headers/tomcrypt.h libtomcrypt/src/headers/tomcrypt_hash.h 
libtomcrypt/src/headers/tomcrypt_mac.h libtomcrypt/src/headers/tomcrypt_macros.h libtomcrypt/src/headers/tomcrypt_math.h 
libtomcrypt/src/headers/tomcrypt_misc.h libtomcrypt/src/headers/tomcrypt_pkcs.h libtomcrypt/src/headers/tomcrypt_pk.h 
libtomcrypt/src/headers/tomcrypt_prng.h
)

source_group("" FILES ${LIBTOMCRYPT_SRC})
source_group("" FILES ${LIBTOMCRYPT_HPP})

# message(WARNING $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/headers>)

add_library("libtomcrypt" STATIC ${LIBTOMCRYPT_SRC} ${LIBTOMCRYPT_HPP})
target_include_directories("libtomcrypt" PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/libtomcrypt/src/headers>)

set_property(TARGET "libtomcrypt" PROPERTY FOLDER "External Libraries")