/*
    Network Next. Copyright 2017 - 2025 Network Next, Inc.  
    Licensed under the Network Next Source Available License 1.0
*/

#ifndef NEXT_PLATFORM_H
#define NEXT_PLATFORM_H

#define NEXT_PLATFORM_SOCKET_NON_BLOCKING       0
#define NEXT_PLATFORM_SOCKET_BLOCKING           1

#define NEXT_MUTEX_BYTES                      256

struct next_address_t;

#include "next_platform_mac.h"
#include "next_platform_linux.h"
#include "next_platform_windows.h"
#include "next_platform_ps4.h"
#include "next_platform_ps5.h"
#include "next_platform_gdk.h"
#include "next_platform_switch.h"

typedef void (*next_platform_thread_func_t)(void*);

// ----------------------------------------------------------------

NEXT_EXPORT_FUNC int next_platform_init();

NEXT_EXPORT_FUNC void next_platform_term();

// ----------------------------------------------------------------

NEXT_EXPORT_FUNC int next_platform_id();

NEXT_EXPORT_FUNC int next_platform_connection_type();

NEXT_EXPORT_FUNC double next_platform_time();

NEXT_EXPORT_FUNC void next_platform_sleep( double time );

NEXT_EXPORT_FUNC const char * next_platform_getenv( const char * var );

NEXT_EXPORT_FUNC uint16_t next_platform_ntohs( uint16_t in );

NEXT_EXPORT_FUNC uint16_t next_platform_htons( uint16_t in );

NEXT_EXPORT_FUNC int next_platform_inet_pton4( const char * address_string, uint32_t * address_out );

NEXT_EXPORT_FUNC int next_platform_inet_pton6( const char * address_string, uint16_t * address_out );

NEXT_EXPORT_FUNC int next_platform_inet_ntop6( const uint16_t * address, char * address_string, size_t address_string_size );

NEXT_EXPORT_FUNC int next_platform_hostname_resolve( const char * hostname, const char * port, struct next_address_t * address );

NEXT_EXPORT_FUNC uint16_t next_platform_preferred_client_port();

NEXT_EXPORT_FUNC bool next_platform_client_dual_stack();

// ----------------------------------------------------------------

NEXT_EXPORT_FUNC struct next_platform_socket_t * next_platform_socket_create( void * context, struct next_address_t * address, int socket_type, float timeout_seconds, int send_buffer_size, int receive_buffer_size );

NEXT_EXPORT_FUNC void next_platform_socket_destroy( struct next_platform_socket_t * socket );

NEXT_EXPORT_FUNC void next_platform_socket_send_packet( struct next_platform_socket_t * socket, const struct next_address_t * to, const void * packet_data, int packet_bytes );

NEXT_EXPORT_FUNC int next_platform_socket_receive_packet( struct next_platform_socket_t * socket, struct next_address_t * from, void * packet_data, int max_packet_size );

// ----------------------------------------------------------------

NEXT_EXPORT_FUNC struct next_platform_thread_t * next_platform_thread_create( void * context, next_platform_thread_func_t func, void * arg );

NEXT_EXPORT_FUNC void next_platform_thread_join( struct next_platform_thread_t * thread );

NEXT_EXPORT_FUNC void next_platform_thread_destroy( struct next_platform_thread_t * thread );

NEXT_EXPORT_FUNC void next_platform_client_thread_priority( struct next_platform_thread_t * thread );

NEXT_EXPORT_FUNC void next_platform_server_thread_priority( struct next_platform_thread_t * thread );

// ----------------------------------------------------------------

NEXT_EXPORT_FUNC int next_platform_mutex_create( struct next_platform_mutex_t * mutex );

NEXT_EXPORT_FUNC void next_platform_mutex_destroy( struct next_platform_mutex_t * mutex );

NEXT_EXPORT_FUNC void next_platform_mutex_acquire( struct next_platform_mutex_t * mutex );

NEXT_EXPORT_FUNC void next_platform_mutex_release( struct next_platform_mutex_t * mutex );

#ifdef __cplusplus

struct next_platform_mutex_helper_t
{
    next_platform_mutex_t * mutex;
#if NEXT_SPIKE_TRACKING
    const char * file;
    int line;
    double start_time;
    next_platform_mutex_helper_t( next_platform_mutex_t * mutex, const char * file, int line );
#else // #if NEXT_SPIKE_TRACKING
    next_platform_mutex_helper_t( next_platform_mutex_t * mutex );
#endif // #if NEXT_SPIKE_TRACKING
    ~next_platform_mutex_helper_t();
};

#if NEXT_SPIKE_TRACKING
#define next_platform_mutex_guard( _mutex ) next_platform_mutex_helper_t __mutex_helper( _mutex, __FILE__, __LINE__ )
#else // #if NEXT_SPIKE_TRACKING
#define next_platform_mutex_guard( _mutex ) next_platform_mutex_helper_t __mutex_helper( _mutex )
#endif // #if NEXT_SPIKE_TRACKING

#if NEXT_SPIKE_TRACKING
inline next_platform_mutex_helper_t::next_platform_mutex_helper_t( next_platform_mutex_t * mutex, const char * file, int line ) : mutex(mutex), file(file), line(line), start_time(next_platform_time())
#else // #if NEXT_SPIKE_TRACKING
inline next_platform_mutex_helper_t::next_platform_mutex_helper_t( next_platform_mutex_t * mutex ) : mutex(mutex)
#endif // #if NEXT_SPIKE_TRACKING
{
    next_assert( mutex );

#if NEXT_SPIKE_TRACKING
    if ( !mutex->ok )
    {
        next_printf( NEXT_LOG_LEVEL_ERROR, "bad mutex - %s:%d", file, line );
    }
#endif // #if NEXT_SPIKE_TRACKING

    next_platform_mutex_acquire( mutex );
}

inline next_platform_mutex_helper_t::~next_platform_mutex_helper_t()
{
    next_assert( mutex );
    next_platform_mutex_release( mutex );
#if NEXT_SPIKE_TRACKING
    double finish_time = next_platform_time();
    double mutex_duration = finish_time - start_time;
    if ( mutex_duration > 0.001 )
    {
        next_printf( NEXT_LOG_LEVEL_WARN, "mutex spike %.2f milliseconds at %s:%d", mutex_duration, file, line );
    }
#endif // #if NEXT_SPIKE_TRACKING
    mutex = NULL;
}

#endif // __cplusplus

// ----------------------------------------------------------------

bool next_platform_packet_tagging_can_be_enabled();

// ----------------------------------------------------------------

#endif // #ifndef NEXT_PLATFORM_H
