// Copyright (c) 2018 Duality Blockchain Solutions Developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "miner/internal/miners-controller.h"
#include "chain/chain.h"
#include "miner/internal/miner-context.h"
#include "miner/miner-util.h"
#include "net/net.h"
#include "db/txmempool.h"
#include "chain/validation.h"
#include "chain/validationinterface.h"

void ConnectMinerSignals(MinersController* miners_)
{
    miners_->connman().ConnectSignalNode(boost::bind(&MinersController::NotifyNode, miners_, _1));
    GetMainSignals().UpdatedBlockTip.connect(boost::bind(&MinersController::NotifyBlock, miners_, _1, _2, _3));
    GetMainSignals().SyncTransaction.connect(boost::bind(&MinersController::NotifyTransaction, miners_, _1, _2, _3));
}

MinersController::MinersController(const CChainParams& chainparams, CConnman& connman)
    : MinersController(std::make_shared<MinerContext>(chainparams, connman)){};

MinersController::MinersController(MinerContextRef ctx)
    : _ctx(ctx),
      _group_cpu(_ctx->MakeChild()),
#ifdef ENABLE_GPU
      _group_gpu(_ctx->MakeChild()),
#endif // ENABLE_GPU
      _connected(!_ctx->chainparams.MiningRequiresPeers()),
      _downloaded(!_ctx->chainparams.MiningRequiresPeers())
{
    InitializeCoinbaseScript();
    ConnectMinerSignals(this);
};

CConnman& MinersController::connman() { return _ctx->connman; }

const CChainParams& MinersController::chainparams() const { return _ctx->chainparams; }

void MinersController::Start()
{
    _enable_start = true;
    if (can_start()) {
        _group_cpu.Start();
#ifdef ENABLE_GPU
        _group_gpu.Start();
#endif // ENABLE_GPU
    }
};

void MinersController::Shutdown()
{
    _group_cpu.Shutdown();
#ifdef ENABLE_GPU
    _group_gpu.Shutdown();
#endif // ENABLE_GPU
};

int64_t MinersController::GetHashRate() const
{
#ifdef ENABLE_GPU
    return _group_gpu.GetHashRate() + _group_cpu.GetHashRate();
#else
    return _group_cpu.GetHashRate();
#endif // ENABLE_GPU
}

void MinersController::InitializeCoinbaseScript()
{
    GetMainSignals().ScriptForMining(_coinbase_script);
    // Throw an error if no script was provided.  This can happen
    // due to some internal error but also if the keypool is empty.
    // In the latter case, already the pointer is NULL.
    if (!_coinbase_script || _coinbase_script->reserveScript.empty()) {
        throw std::runtime_error("No coinbase script available (mining requires a wallet)");
    }
}

std::unique_ptr<CBlockTemplate> MinersController::CreateBlockTemplate()
{
    _last_sync_time = GetTime();
    _last_txn_time = mempool.GetTransactionsUpdated();
    _ctx->set_tip(chainActive.Tip());
    return CreateNewBlock(_ctx->chainparams, _coinbase_script->reserveScript);
}

void MinersController::NotifyNode(const CNode* node)
{
    _connected = true;
    Start();
};

void MinersController::NotifyBlock(const CBlockIndex* index_new, const CBlockIndex* index_fork, bool initial_download)
{
    _downloaded = initial_download || _downloaded;
    // Compare with current tip (checks for unexpected behaviour or old block)
    if (index_new != chainActive.Tip())
        return;
    // Create new block template for miners
    std::unique_ptr<CBlockTemplate> block_template = CreateBlockTemplate();
    if (!block_template) {
        LogPrintf("DynamicMinersController -- Keypool ran out, please call keypoolrefill before restarting the mining thread\n");
        return;
    }
    // trigger sync
    Start();
};

void MinersController::NotifyTransaction(const CTransaction& txn, const CBlockIndex* index, int pos_in_block)
{
    const int64_t latest_txn = mempool.GetTransactionsUpdated();
    if (latest_txn == _last_txn_time) {
        return;
    }
    if (GetTime() - _last_txn_time > 60) {
        _last_txn_time = latest_txn;
        // trigger sync
    }
};