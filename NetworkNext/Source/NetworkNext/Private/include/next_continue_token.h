/*
    Network Next. Copyright © 2017 - 2024 Network Next, Inc.

    Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following 
    conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

    2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions 
       and the following disclaimer in the documentation and/or other materials provided with the distribution.

    3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote 
       products derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
    INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
    IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
    CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; 
    OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
    NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef NEXT_CONTINUE_TOKEN_H
#define NEXT_CONTINUE_TOKEN_H

#include "next.h"
#include "next_crypto.h"

struct next_continue_token_t
{
    uint64_t expire_timestamp;
    uint64_t session_id;
    uint8_t session_version;
};

inline void next_write_continue_token( next_continue_token_t * token, uint8_t * buffer, int buffer_length )
{
    (void) buffer_length;

    next_assert( token );
    next_assert( buffer );
    next_assert( buffer_length >= NEXT_CONTINUE_TOKEN_BYTES );

    uint8_t * start = buffer;

    (void) start;

    next_write_uint64( &buffer, token->expire_timestamp );
    next_write_uint64( &buffer, token->session_id );
    next_write_uint8( &buffer, token->session_version );

    next_assert( buffer - start == NEXT_CONTINUE_TOKEN_BYTES );
}

inline void next_read_continue_token( next_continue_token_t * token, const uint8_t * buffer )
{
    next_assert( token );
    next_assert( buffer );

    const uint8_t * start = buffer;

    (void) start;

    token->expire_timestamp = next_read_uint64( &buffer );
    token->session_id = next_read_uint64( &buffer );
    token->session_version = next_read_uint8( &buffer );

    next_assert( buffer - start == NEXT_CONTINUE_TOKEN_BYTES );
}

inline int next_encrypt_continue_token( uint8_t * sender_private_key, uint8_t * receiver_public_key, uint8_t * nonce, uint8_t * buffer, int buffer_length )
{
    next_assert( sender_private_key );
    next_assert( receiver_public_key );
    next_assert( buffer );
    next_assert( buffer_length >= (int) ( NEXT_CONTINUE_TOKEN_BYTES + NEXT_CRYPTO_BOX_MACBYTES ) );

    (void) buffer_length;

    if ( next_crypto_box_easy( buffer, buffer, NEXT_CONTINUE_TOKEN_BYTES, nonce, receiver_public_key, sender_private_key ) != 0 )
    {
        return NEXT_ERROR;
    }

    return NEXT_OK;
}

inline int next_decrypt_continue_token( const uint8_t * sender_public_key, const uint8_t * receiver_private_key, const uint8_t * nonce, uint8_t * buffer )
{
    next_assert( sender_public_key );
    next_assert( receiver_private_key );
    next_assert( buffer );

    if ( next_crypto_box_open_easy( buffer, buffer, NEXT_CONTINUE_TOKEN_BYTES + NEXT_CRYPTO_BOX_MACBYTES, nonce, sender_public_key, receiver_private_key ) != 0 )
    {
        return NEXT_ERROR;
    }

    return NEXT_OK;
}

inline int next_write_encrypted_continue_token( uint8_t ** buffer, next_continue_token_t * token, uint8_t * sender_private_key, uint8_t * receiver_public_key )
{
    next_assert( buffer );
    next_assert( token );
    next_assert( sender_private_key );
    next_assert( receiver_public_key );

    unsigned char nonce[NEXT_CRYPTO_BOX_NONCEBYTES];
    next_crypto_random_bytes( nonce, NEXT_CRYPTO_BOX_NONCEBYTES );

    uint8_t * start = *buffer;

    next_write_bytes( buffer, nonce, NEXT_CRYPTO_BOX_NONCEBYTES );

    next_write_continue_token( token, *buffer, NEXT_CONTINUE_TOKEN_BYTES );

    if ( next_encrypt_continue_token( sender_private_key, receiver_public_key, nonce, *buffer, NEXT_CONTINUE_TOKEN_BYTES + NEXT_CRYPTO_BOX_NONCEBYTES ) != NEXT_OK )
        return NEXT_ERROR;

    *buffer += NEXT_CONTINUE_TOKEN_BYTES + NEXT_CRYPTO_BOX_MACBYTES;

    (void) start;

    next_assert( ( *buffer - start ) == NEXT_ENCRYPTED_CONTINUE_TOKEN_BYTES );

    return NEXT_OK;
}

inline int next_read_encrypted_continue_token( uint8_t ** buffer, next_continue_token_t * token, const uint8_t * sender_public_key, const uint8_t * receiver_private_key )
{
    next_assert( buffer );
    next_assert( token );
    next_assert( sender_public_key );
    next_assert( receiver_private_key );

    const uint8_t * nonce = *buffer;

    *buffer += NEXT_CRYPTO_BOX_NONCEBYTES;

    if ( next_decrypt_continue_token( sender_public_key, receiver_private_key, nonce, *buffer ) != NEXT_OK )
    {
        return NEXT_ERROR;
    }

    next_read_continue_token( token, *buffer );

    *buffer += NEXT_CONTINUE_TOKEN_BYTES + NEXT_CRYPTO_BOX_MACBYTES;

    return NEXT_OK;
}

#endif // #ifndef NEXT_CONTINUE_TOKEN_H