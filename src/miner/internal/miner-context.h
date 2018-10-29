// Copyright (c) 2018 Duality Blockchain Solutions Developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DYNAMIC_INTERNAL_MINER_CONTEXT_H
#define DYNAMIC_INTERNAL_MINER_CONTEXT_H

#include "miner/internal/hash-rate-counter.h"

class CBlock;
class CChainParams;
class CConnman;
class CBlockIndex;

class MinerBase;
class MinerContext;
class MinersController;

/** Miner context shared_ptr */
using MinerContextRef = std::shared_ptr<MinerContext>;
// used so we don't delete CBlockIndex
void block_index_delete_noop(CBlockIndex*);

/**
 * Miner context.
 */
class MinerContext
{
public:
    const CChainParams& chainparams;
    CConnman& connman;
    HashRateCounterRef counter;

    MinerContext(const CChainParams& chainparams_, CConnman& connman_)
        : chainparams(chainparams_), connman(connman_), counter(){};

    MinerContext(const CChainParams& chainparams_, CConnman& connman_, HashRateCounterRef counter_)
        : chainparams(chainparams_), connman(connman_), counter(counter_){};

    /* Constructs child context */
    explicit MinerContext(const MinerContext* ctx_)
        : MinerContext(ctx_->chainparams, ctx_->connman, ctx_->counter->MakeChild()){};

    /** Creates child context for group or miner */
    MinerContextRef MakeChild() const { return std::make_shared<MinerContext>(this); }

private:
    // shared block for miners
    // they have to randomize
    std::shared_ptr<CBlock> _block{nullptr};
    // curent blockchain tip
    // should be set with `set_tip` to avoid deletion
    std::shared_ptr<CBlockIndex> _tip{nullptr};

protected:
    friend class MinerBase;
    friend class MinersController;

    CBlock* block() { return _block.get(); }
    CBlockIndex* tip() { return _tip.get(); }

    void set_tip(CBlockIndex* new_tip_) { _tip.reset(new_tip_, block_index_delete_noop); };
    void set_block(CBlock* new_block_) { _block.reset(new_block_); };
};

#endif // DYNAMIC_INTERNAL_MINER_CONTEXT_H
