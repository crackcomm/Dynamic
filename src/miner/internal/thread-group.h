// Copyright (c) 2018 Duality Blockchain Solutions Developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DYNAMIC_INTERNAL_THREAD_GROUP_H
#define DYNAMIC_INTERNAL_THREAD_GROUP_H

#include <boost/thread/locks.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <thread>

/**
 * Miner threads controller.
 * Separate object for CPU and GPU.
 */
template <class T, class Context>
class ThreadGroup
{
public:
    ThreadGroup(Context ctx);

    // Starts all miner threads
    void Start() { SyncGroupTarget(); };

    // Shuts down all miner threads
    void Shutdown()
    {
        SetNumThreads(0);
        SyncGroupTarget();
    };

    // Sets amount of threads
    void SetNumThreads(int target)
    {
        boost::shared_lock<boost::shared_mutex> guard(m);
        _target_threads = target;
    };

    // Starts or shutdowns threads to meet the target
    void SyncGroupTarget();

protected:
    Context _ctx;
    size_t _devices;
    size_t _target_threads = 0;
    std::vector<std::shared_ptr<std::thread>> _threads;
    mutable boost::shared_mutex m;
};

/** Miners device group class constructor */
template <class T, class Context>
ThreadGroup<T, Context>::ThreadGroup(Context ctx)
    : _ctx(ctx), _devices(T::TotalDevices()){};

template <class T, class Context>
void ThreadGroup<T, Context>::SyncGroupTarget()
{
    boost::shared_lock<boost::shared_mutex> guard(m);

    size_t current;
    size_t real_target = _target_threads * _devices;
    while ((current = _threads.size()) != real_target) {
        for (size_t device_index = 0; device_index < _devices; device_index++) {
            if (current < real_target) {
                _threads.push_back(std::make_shared<std::thread>(T(_ctx->MakeChild(), device_index)));
            } else {
                std::shared_ptr<std::thread> thread = _threads.back();
                _threads.pop_back();
            }
        }
    }
};

#endif // DYNAMIC_INTERNAL_THREAD_GROUP_H
