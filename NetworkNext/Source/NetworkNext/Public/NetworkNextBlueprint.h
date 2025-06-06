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

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "NetworkNextBlueprint.generated.h"

UCLASS()
class NETWORKNEXT_API UNetworkNextBlueprint : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:

    /**
     * Upgrades a player session on Network Next
     */
    UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Network Next", meta = (WorldContext = "WorldContextObject", DisplayName = "Upgrade Player Session"))
    static void UpgradePlayer(UObject* WorldContextObject, APlayerController* PlayerController, const FString& UserId);

    /**
     * Get latency
     */
    UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Network Next", meta = (WorldContext = "WorldContextObject", DisplayName = "Get Latency"))
    static float GetLatency(UObject* WorldContextObject);

    /**
     * Get jitter
     */
    UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Network Next", meta = (WorldContext = "WorldContextObject", DisplayName = "Get Jitter"))
    static float GetJitter(UObject* WorldContextObject);

    /**
     * Get packet loss
     */
    UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Network Next", meta = (WorldContext = "WorldContextObject", DisplayName = "Get Packet Loss"))
    static float GetPacketLoss(UObject* WorldContextObject);

	/**
	 * Is the server ready for clients to connect?
	 * Please wait until this is true before reporting to your matchmaker that this server is ready for clients to connect to it.
	 * It will time out and become ready in ~15 seconds no matter what, so the server will never get stuck in the waiting to be ready.
	 */

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Network Next", meta = (WorldContext = "WorldContextObject", DisplayName = "Is Server Ready?"))
    static bool IsServerReady(UObject* WorldContextObject);

    /**
     * Does the platform support enabling DSCP packet tagging?
	 * This is a new feature that primarily helps reduce jitter and packet loss over Wi-Fi connections.
	 * Please provide a user interface within you game to allow users to enable packet tagging when this returns true. 
	 * The only platform this returns false on is XBox, and this is because the XBox platform already provides a way for
	 * the player to enable/disable packet tagging in their system dialogs. On other platforms, you can have packet tagging
	 * too, but you need to let your user enable it manually. It is important to leave it off by default (as it is) because
	 * it can cause problems for players with older routers (eg. all packets get dropped).
     */
    UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Network Next", meta = (WorldContext = "WorldContextObject", DisplayName = "Can Packet Tagging Be Enabled?"))
    static bool CanPacketTaggingBeEnabled(UObject* WorldContextObject);

    /**
     * Enable DSCP packet tagging (it is disabled by default)
	 * Takes effect on the next client or server create. Has no effect on any client or server socket already created.
     */
    UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Network Next", meta = (WorldContext = "WorldContextObject", DisplayName = "Enable Packet Tagging"))
    static void EnablePacketTagging(UObject* WorldContextObject);

    /**
     * Disable DSCP packet tagging
	 * Takes effect on the next client or server create. Has no effect on any client or server socket already created.
     */
    UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Network Next", meta = (WorldContext = "WorldContextObject", DisplayName = "Disable Packet Tagging"))
    static void DisablePacketTagging(UObject* WorldContextObject);
};
