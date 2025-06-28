/*
    Network Next. Copyright © 2017 - 2025 Network Next, Inc.

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

#include "NetworkNextModule.h"
#include "NetworkNextNetDriver.h"
#include "NetworkNextSocketSubsystem.h"
#include "Core.h"
#include "next.h"

DEFINE_LOG_CATEGORY(LogNetworkNext);

IMPLEMENT_MODULE(FNetworkNextModule, NetworkNext)

void FNetworkNextModule::StartupModule()
{
    UE_LOG(LogNetworkNext, Display, TEXT("Network Next Plugin loaded"));

    m_initialized_sdk = false;

	next_config_t config;
	next_default_config( &config );

	const char * buyer_public_key = "pTajZoYIAqBcMcYIz97X2hh2sDb/0Oe3S6bFmxc0v6IiCnjNQZJ1sQ==";

    // IMPORTANT: If you don't want to pass "BuyerPrivateKey" via server-only INI file, you can pass it via command line below
    FString BuyerPrivateKey;
    if (FParse::Value(FCommandLine::Get(), TEXT("-buyerPrivateKey="), BuyerPrivateKey))
    {
        next_copy_string(config.buyer_private_key, TCHAR_TO_ANSI(*BuyerPrivateKey), sizeof(config.buyer_private_key));
    }

    if (next_init(NULL, &config) != NEXT_OK)
    {
        UE_LOG(LogNetworkNext, Error, TEXT("Network Next SDK failed to initalize!"));
        return;
    }

    UE_LOG(LogNetworkNext, Display, TEXT("Network Next SDK initialized"));

    m_initialized_sdk = true;

    // Perform network next allocations through the unreal allocator instead of default malloc/free
    next_allocator(&FNetworkNextModule::Malloc, &FNetworkNextModule::Free);

    // Setup logging to go to the "NetworkNext" log category
    next_log_function(&FNetworkNextModule::Log);

    CreateNetworkNextSocketSubsystem();
}

void FNetworkNextModule::ShutdownModule()
{
    UE_LOG(LogNetworkNext, Display, TEXT("Network Next Plugin shutting down"));

    DestroyNetworkNextSocketSubsystem();

    next_term();

    m_initialized_sdk = false;

    UE_LOG(LogNetworkNext, Display, TEXT("Network Next Plugin unloaded"));
}

void FNetworkNextModule::Log(int level, const char* format, ...)
{
    va_list args;
    va_start(args, format);
    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    FString Message = FString(buffer);

    switch (level)
    {
    case NEXT_LOG_LEVEL_ERROR:
        UE_LOG(LogNetworkNext, Error, TEXT("%s"), *Message);
        break;
    case NEXT_LOG_LEVEL_WARN:
        UE_LOG(LogNetworkNext, Warning, TEXT("%s"), *Message);
        break;
    case NEXT_LOG_LEVEL_INFO:
        UE_LOG(LogNetworkNext, Display, TEXT("%s"), *Message);
        break;
    default:
        UE_LOG(LogNetworkNext, Display, TEXT("%s"), *Message);
        break;
    }
}

void* FNetworkNextModule::Malloc(void* context, size_t size)
{
    return FMemory::Malloc(size);
}

void FNetworkNextModule::Free(void* context, void* src)
{
    return FMemory::Free(src);
}

void FNetworkNextModule::UpgradePlayer(APlayerController* PlayerController, const FString& UserId)
{
    if (PlayerController == nullptr)
        return;

    UWorld* World = PlayerController->GetWorld();

    if (World == nullptr)
        return;

    UNetworkNextNetDriver* NetDriver = Cast<UNetworkNextNetDriver>(World->GetNetDriver());

    if (NetDriver == nullptr)
        return;

    FNetworkNextSocketServer* ServerSocket = NetDriver->GetServerSocket();

    if (ServerSocket == nullptr)
        return;

    UNetConnection* Connection = PlayerController->GetNetConnection();

    if (Connection == nullptr)
        return;

    ServerSocket->UpgradeClient(Connection->GetRemoteAddr(), UserId);
}
