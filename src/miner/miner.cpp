// Copyright (c) 2016-2018 Duality Blockchain Solutions Developers
// Copyright (c) 2014-2018 The Dash Core Developers
// Copyright (c) 2009-2018 The Bitcoin Developers
// Copyright (c) 2009-2018 Satoshi Nakamoto
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "miner/miner.h"
#include "chain/params.h"
#include "consensus/consensus.h"
#include "consensus/validation.h"
#include "miner/internal/miners-controller.h"
#include "net/net.h"
#include "primitives/transaction.h"
#include "util/moneystr.h"
#include "chain/validation.h"
#include "chain/validationinterface.h"

static bool ProcessBlockFound(const std::shared_ptr<const CBlock>& block, const CChainParams& chainparams, CConnman* connman)
{
    LogPrintf("%s\n", block->ToString());
    LogPrintf("generated %s\n", FormatMoney(block->vtx[0]->vout[0].nValue));

    // Found a solution
    {
        LOCK(cs_main);
        if (block->hashPrevBlock != chainActive.Tip()->GetBlockHash()) {
            return error("ProcessBlockFound -- generated block is stale");
        }
    }

    // Inform about the new block
    GetMainSignals().BlockFound(block->GetHash());

    // Process this block the same as if we had received it from another node
    CValidationState state;
    if (!ProcessNewBlock(chainparams, block, true, NULL)) {
        return error("ProcessBlockFound -- ProcessNewBlock() failed, block not accepted");
    }

    return true;
}

void ShutdownMiners()
{
    g_miners->Shutdown();
};

void StartMiners()
{
    g_miners->Start();
};

void ShutdownCPUMiners()
{
    g_miners->group_cpu().Shutdown();
};

void ShutdownGPUMiners()
{
#ifdef ENABLE_GPU
    g_miners->group_gpu().Shutdown();
#endif // ENABLE_GPU
};

int64_t GetHashRate()
{
    return g_miners->GetHashRate();
};

int64_t GetCPUHashRate()
{
    return g_miners->group_cpu().GetHashRate();
};

int64_t GetGPUHashRate()
{
#ifdef ENABLE_GPU
    return g_miners->group_gpu().GetHashRate();
#else
    return 0;
#endif // ENABLE_GPU
};

void SetCPUMinerThreads(int target)
{
    g_miners->group_cpu().SetNumThreads(target);
};

void SetGPUMinerThreads(int target)
{
#ifdef ENABLE_GPU
    g_miners->group_gpu().SetNumThreads(target);
#endif // ENABLE_GPU
};

std::unique_ptr<MinersController> g_miners = {nullptr};
