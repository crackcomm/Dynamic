// Copyright (c) 2016-2018 Duality Blockchain Solutions Developers
// Copyright (c) 2014-2018 The Dash Core Developers
// Copyright (c) 2009-2018 The Bitcoin Developers
// Copyright (c) 2009-2018 Satoshi Nakamoto
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "miner/internal/miner-base.h"
#include "chain/params.h"
#include "miner/miner-util.h"
#include "primitives/block.h"
#include "util/util.h"
#include "chain/validation.h"
#include "chain/validationinterface.h"


MinerBase::MinerBase(MinerContextRef ctx, std::size_t device_index)
    : _ctx(ctx),
      _device_index(device_index){};

void MinerBase::Loop()
{
    LogPrintf("DynamicMiner%s -- started\n", DeviceName());
    SetThreadPriority(THREAD_PRIORITY_LOWEST);
    RenameThread(tfm::format("dynamic-%s-miner", DeviceName()).data());
    // TODO(crackcomm): one for all should be fine
    // GetMainSignals().ScriptForMining(_coinbase_script);


    CBlock block;
    CBlockIndex* last_tip = nullptr;

    try {
        while (true) {
            // Get blockchain tip
            CBlockIndex* tip = _ctx->tip();
            // Update block and tip if changed
            if (last_tip != tip) {
                last_tip = tip;
                block = *_ctx->block();
            } else if (_ctx->block()->nBits != block.nBits && _ctx->block()->nTime != block.nTime) {
                block = *_ctx->block();
            }
            // Increment nonce
            IncrementExtraNonce(&block, tip, _extra_nonce);
            // TODO:
            LogPrintf("DynamicMiner%s -- Running miner with %u transactions in block (%u bytes)\n", DeviceName(), block.vtx.size(),
                GetSerializeSize(block, SER_NETWORK, PROTOCOL_VERSION));
            // set loop start for counter
            _hash_target = arith_uint256().SetCompact(block.nBits);
            // start mining the block
            while (true) {
                // try mining the block
                int64_t hashes = TryMineBlock(&block);
                // increment hash statistics
                _ctx->counter->Increment(hashes);
                // Check for stop or if block needs to be rebuilt
                boost::this_thread::interruption_point();
                if (block.nNonce >= 0xffff0000)
                    // CRITICAL TODO: recreate block
                    break;
                // Update block time
                if (UpdateTime(&block, _ctx->chainparams.GetConsensus(), tip) < 0)
                    // CRITICAL TODO: recreate block
                    break; // Recreate the block if the clock has run backwards,
                           // so that we can use the correct time.
                if (_ctx->chainparams.GetConsensus().fPowAllowMinDifficultyBlocks) {
                    // Changing block.nTime can change work required on testnet:
                    _hash_target.SetCompact(block.nBits);
                }
            }
        }
    } catch (const boost::thread_interrupted&) {
        LogPrintf("DynamicMiner%s -- terminated\n", DeviceName());
        throw;
    } catch (const std::runtime_error& e) {
        LogPrintf("DynamicMiner%s -- runtime error: %s\n", DeviceName(), e.what());
        return;
    }
}

void MinerBase::ProcessFoundSolution(CBlock* block, const uint256& hash)
{
    // Found a solution
    SetThreadPriority(THREAD_PRIORITY_NORMAL);
    LogPrintf("DynamicMiner%s:\n proof-of-work found  \n  hash: %s  \ntarget: %s\n", DeviceName(), hash.GetHex(), _hash_target.GetHex());
    // TODO:
    // ProcessBlockFound(block, _ctx->chainparams, &_connman);
    SetThreadPriority(THREAD_PRIORITY_LOWEST);
    // TODO:
    // _coinbase_script->KeepScript();

    // TODO:
    // // In regression test mode, stop mining after a block is found.
    // if (_ctx->chainparams.MineBlocksOnDemand()) {
    //     throw boost::thread_interrupted();
    // }
}

// provides implementation for miner-context.h
void block_index_delete_noop(CBlockIndex*){};
