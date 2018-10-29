DIST_SUBDIRS = third_party/secp256k1 third_party/univalue

AM_LDFLAGS = $(PTHREAD_CFLAGS) $(LIBTOOL_LDFLAGS) $(HARDENED_LDFLAGS)
AM_CXXFLAGS = $(HARDENED_CXXFLAGS)
AM_CPPFLAGS = $(HARDENED_CPPFLAGS)
EXTRA_LIBRARIES =

DYNAMIC_CONFIG_INCLUDES=-I$(builddir)
DYNAMIC_INCLUDES = -I$(builddir) -I$(builddir)/third_party/obj $(BOOST_CPPFLAGS) $(LEVELDB_CPPFLAGS) $(CRYPTO_CFLAGS) $(SSL_CFLAGS)

DYNAMIC_INCLUDES += -I$(srcdir)/third_party/math
DYNAMIC_INCLUDES += -I$(srcdir)/third_party/secp256k1/include
DYNAMIC_INCLUDES += -I$(srcdir)/third_party/univalue/include

LIBDYNAMIC_SERVER=libdynamic_server.a
LIBDYNAMIC_WALLET=libdynamic_wallet.a
LIBDYNAMIC_COMMON=libdynamic_common.a
LIBDYNAMIC_CLI=libdynamic_cli.a
LIBDYNAMIC_UTIL=libdynamic_util.a
LIBDYNAMIC_CRYPTO=crypto/libdynamic_crypto.a
LIBDYNAMICQT=qt/libdynamicqt.a
LIBSECP256K1=third_party/secp256k1/libsecp256k1.la
LIBUNIVALUE=third_party/univalue/libunivalue.la

$(LIBSECP256K1): $(wildcard third_party/secp256k1/src/*) $(wildcard third_party/secp256k1/include/*)
	$(AM_V_at)$(MAKE) $(AM_MAKEFLAGS) -C $(@D) $(@F)

$(LIBUNIVALUE): $(wildcard third_party/univalue/lib/*) $(wildcard third_party/univalue/include/*)
	$(AM_V_at)$(MAKE) $(AM_MAKEFLAGS) -C $(@D) $(@F)

# Make is not made aware of per-object dependencies to avoid limiting building parallelization
# But to build the less dependent modules first, we manually select their order here:
EXTRA_LIBRARIES += \
  crypto/libdynamic_crypto.a \
  libdynamic_util.a \
  libdynamic_common.a \
  libdynamic_server.a \
  libdynamic_cli.a
if ENABLE_WALLET
DYNAMIC_INCLUDES += $(BDB_CPPFLAGS)
EXTRA_LIBRARIES += libdynamic_wallet.a
endif
if ENABLE_ZMQ
EXTRA_LIBRARIES += libdynamic_zmq.a
endif

if BUILD_DYNAMIC_LIBS
lib_LTLIBRARIES = libdynamicconsensus.la
LIBDYNAMIC_CONSENSUS=libdynamicconsensus.la
else
LIBDYNAMIC_CONSENSUS=
endif

bin_PROGRAMS =
TESTS =
BENCHMARKS =

if BUILD_DYNAMICD
  bin_PROGRAMS += dynamicd
endif

if BUILD_DYNAMIC_UTILS
  bin_PROGRAMS += dynamic-cli dynamic-tx
endif

.PHONY: FORCE check-symbols check-security
# dynamic #
DYNAMIC_CORE_H = \
  bdap/auditdata.h \
  bdap/bdap.h \
  bdap/domainentry.h \
  bdap/domainentrydb.h \
  bdap/entrycertificate.h \
  bdap/entrychannel.h \
  bdap/entrycheckpoints.h \
  bdap/entrylink.h \
  bdap/identity.h \
  chain/dynode/active.h \
  chain/amount.h \
  chain/blockencodings.h \
  chain/chain.h \
  chain/checkpoints.h \
  chain/checkqueue.h \
  chain/coins.h \
  chain/instantsend.h \
  chain/merkleblock.h \
  chain/params.h \
  chain/paramsbase.h \
  chain/paramsseeds.h \
  chain/pow.h \
  chain/spentindex.h \
  chain/spork.h \
  chain/undo.h \
  chain/validation.h \
  chain/validationinterface.h \
  clientversion.h \
  compat/byteswap.h \
  compat/compat.h \
  compat/endian.h \
  compat/sanity.h \
  compat/span.h \
  consensus/consensus.h \
  consensus/merkle.h \
  consensus/params.h \
  consensus/validation.h \
  db/addrdb.h \
  db/flat-database.h \
  db/txdb.h \
  db/txmempool.h \
  db/wrapper.h \
  dynode/payments.h \
  dynode/sync.h \
  dynode/dynode.h \
  dynode/config.h \
  dynode/manager.h \
  fluid/fluid.h \
  fluid/fluiddb.h \
  fluid/dynode.h \
  fluid/mining.h \
  fluid/mint.h \
  fluid/sovereign.h \
  governance/classes.h \
  governance/exceptions.h \
  governance/governance.h \
  governance/object.h \
  governance/validators.h \
  governance/vote.h \
  governance/votedb.h \
  http/rpc.h \
  http/server.h \
  init.h \
  keys/bip39.h \
  keys/bip39_english.h \
  keys/hdchain.h \
  keys/key.h \
  keys/key_io.h \
  keys/keystore.h \
  keys/pubkey.h \
  third_party/math/arith_uint256.h \
  third_party/math/uint256.h \
  miner/impl/miner-cpu.h \
  miner/impl/miner-gpu.h \
  miner/internal/hash-rate-counter.h \
  miner/internal/miner-base.h \
  miner/internal/miner-context.h \
  miner/internal/miners-controller.h \
  miner/internal/miners-group.h \
  miner/internal/thread-group.h \
  miner/miner-util.h \
  miner/miner.h \
  net/address.h \
  net/base.h \
  net/fulfilledman.h \
  net/messagemaker.h \
  net/net.h \
  net/processing.h \
  net/protocol.h \
  net/torcontrol.h \
  policy/fees.h \
  policy/policy.h \
  policy/rbf.h \
  primitives/block.h \
  primitives/keys.h \
  primitives/transaction.h \
  privatesend/client.h \
  privatesend/privatesend.h \
  privatesend/server.h \
  privatesend/util.h \
  rpc/client.h \
  rpc/keepass.h \
  rpc/protocol.h \
  rpc/register.h \
  rpc/server.h \
  script/interpreter.h \
  script/script.h \
  script/script_error.h \
  script/sigcache.h \
  script/sign.h \
  script/standard.h \
  support/core_io.h \
  chain/memusage.h \
  support/fs.h \
  support/scheduler.h \
  support/sync.h \
  support/allocators/secure.h \
  support/allocators/zeroafterfree.h \
  support/cleanse.h \
  support/events.h \
  support/lockedpool.h \
  util/addrman.h \
  util/alert.h \
  util/base58.h \
  util/bloom.h \
  support/cachemap.h \
  support/cachemultimap.h \
  util/compressor.h \
  util/cuckoocache.h \
  util/hash.h \
  support/support/indirectmap.h \
  support/limitedmap.h \
  util/logging.h \
  util/util/memusage.h \
  util/messagesigner.h \
  util/moneystr.h \
  util/noui.h \
  util/ntp.h \
  util/operations.h \
  support/prevector.h \
  util/psnotificationinterface.h \
  util/random.h \
  support/reverselock.h \
  util/serialize.h \
  util/streams.h \
  util/strencodings.h \
  support/threadinterrupt.h \
  support/threadsafety.h \
  util/time.h \
  util/timedata.h \
  util/tinyformat.h \
  util/ui_interface.h \
  util/util.h \
  util/warnings.h \
  version.h \
  versionbits.h \
  wallet/coincontrol.h \
  wallet/crypter.h \
  wallet/db.h \
  wallet/rpcwallet.h \
  wallet/wallet.h \
  wallet/wallet_ismine.h \
  wallet/walletdb.h \
  zmq/zmqabstractnotifier.h \
  zmq/zmqconfig.h\
  zmq/zmqnotificationinterface.h \
  zmq/zmqpublishnotifier.h

third_party/obj/build.h: FORCE
	@$(MKDIR_P) $(builddir)/third_party/obj
	@$(top_srcdir)/share/genbuild.sh $(abs_top_builddir)/src/third_party/obj/build.h \
	  $(abs_top_srcdir)
libdynamic_util_a-clientversion.$(OBJEXT): third_party/obj/build.h

# server: shared between dynamicd and dynamic-qt
libdynamic_server_a_CPPFLAGS = $(AM_CPPFLAGS) $(DYNAMIC_INCLUDES) $(MINIUPNPC_CPPFLAGS) $(EVENT_CFLAGS) $(EVENT_PTHREADS_CFLAGS)
libdynamic_server_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libdynamic_server_a_SOURCES = \
  bdap/auditdata.cpp \
  bdap/domainentry.cpp \
  bdap/domainentrydb.cpp \
  bdap/entrycertificate.cpp \
  bdap/entrychannel.cpp \
  bdap/entrycheckpoints.cpp \
  bdap/entrylink.cpp \
  bdap/identity.cpp \
  bdap/rpcdomainentry.cpp \
  chain/dynode/active.cpp \
  chain/blockencodings.cpp \
  chain/chain.cpp \
  chain/checkpoints.cpp \
  chain/instantsend.cpp \
  chain/merkleblock.cpp \
  chain/pow.cpp \
  chain/spork.cpp \
  chain/validation.cpp \
  chain/validationinterface.cpp \
  db/addrdb.cpp \
  db/txdb.cpp \
  db/txmempool.cpp \
  db/wrapper.cpp \
  dynode/payments.cpp\
  dynode/sync.cpp \
  dynode.cpp \
  dynode/config.cpp \
  dynode/manager.cpp \
  fluid/fluid.cpp \
  fluid/db.cpp \
  fluid/dynode.cpp \
  fluid/mining.cpp \
  fluid/mint.cpp \
  fluid/sovereign.cpp \
  fluid/rpc.cpp \
  governance/classes.cpp \
  governance/governance.cpp \
  governance/object.cpp \
  governance/validators.cpp \
  governance/vote.cpp \
  governance/votedb.cpp \
  http/rest.cpp \
  http/rpc.cpp \
  http/server.cpp \
  init.cpp \
  miner/impl/miner-cpu.cpp \
  miner/impl/miner-gpu.cpp \
  miner/internal/hash-rate-counter.cpp \
  miner/internal/miner-base.cpp \
  miner/internal/miners-controller.cpp \
  miner/miner-util.cpp \
  miner/miner.cpp \
  net/addrman.cpp \
  net/fulfilledman.cpp \
  net/net.cpp \
  net/processing.cpp \
  net/torcontrol.cpp \
  policy/fees.cpp \
  policy/policy.cpp \
  privatesend/privatesend.cpp \
  privatesend/server.cpp \
  rpc/blockchain.cpp \
  rpc/dynode.cpp \
  rpc/governance.cpp \
  rpc/mining.cpp \
  rpc/misc.cpp \
  rpc/net.cpp \
  rpc/rawtxn.cpp \
  rpc/server.cpp \
  script/sigcache.cpp \
  support/fs.cpp \
  util/alert.cpp \
  util/bloom.cpp \
  util/logging.cpp \
  util/messagesigner.cpp \
  util/noui.cpp \
  util/ntp.cpp \
  util/operations.cpp \
  util/psnotificationinterface.cpp \
  util/sendalert.cpp \
  util/timedata.cpp \
  util/ui_interface.cpp \
  util/warnings.cpp \
  versionbits.cpp \
  $(DYNAMIC_CORE_H)

if ENABLE_ZMQ
LIBDYNAMIC_ZMQ=libdynamic_zmq.a

libdynamic_zmq_a_CPPFLAGS = $(DYNAMIC_INCLUDES) $(ZMQ_CFLAGS)
libdynamic_zmq_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libdynamic_zmq_a_SOURCES = \
  zmq/zmqabstractnotifier.cpp \
  zmq/zmqnotificationinterface.cpp \
  zmq/zmqpublishnotifier.cpp
endif

# wallet: shared between dynamicd and dynamic-qt, but only linked
# when wallet enabled
libdynamic_wallet_a_CPPFLAGS = $(AM_CPPFLAGS) $(DYNAMIC_INCLUDES)
libdynamic_wallet_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libdynamic_wallet_a_SOURCES = \
  policy/rbf.cpp \
  privatesend/client.cpp \
  privatesend/util.cpp \
  rpc/keepass.cpp \
  wallet/crypter.cpp \
  wallet/db.cpp \
  wallet/rpcdump.cpp \
  wallet/rpcwallet.cpp \
  wallet/wallet.cpp \
  wallet/wallet_ismine.cpp \
  wallet/walletdb.cpp \
  $(DYNAMIC_CORE_H)

# crypto primitives library
crypto_libdynamic_crypto_a_CPPFLAGS = $(AM_CPPFLAGS) $(DYNAMIC_CONFIG_INCLUDES) $(PIC_FLAGS)
crypto_libdynamic_crypto_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS) $(PIC_FLAGS)
crypto_libdynamic_crypto_a_SOURCES = \
  crypto/argon2d/argon2.c \
  crypto/argon2d/argon2.h \
  crypto/argon2d/core.c \
  crypto/argon2d/core.h \
  crypto/argon2d/encoding.c \
  crypto/argon2d/encoding.h \
  crypto/argon2d/opt.c \
  crypto/argon2d/thread.c \
  crypto/argon2d/thread.h \
  crypto/blake2/blake2-impl.h \
  crypto/blake2/blake2.h \
  crypto/blake2/blake2b.c \
  crypto/blake2/blamka-round-opt.h \
  crypto/blake2/blamka-round-ref.h \
  crypto/chacha20.cpp \
  crypto/chacha20.h \
  crypto/common.h \
  crypto/hmac_sha256.cpp \
  crypto/hmac_sha256.h \
  crypto/hmac_sha512.cpp \
  crypto/hmac_sha512.h \
  crypto/ripemd160.cpp \
  crypto/ripemd160.h \
  crypto/sha1.cpp \
  crypto/sha1.h \
  crypto/sha256.cpp \
  crypto/sha256.h \
  crypto/sha256_avx2.cpp \
  crypto/sha256_shani.cpp \
  crypto/sha256_sse4.cpp \
  crypto/sha256_sse41.cpp \
  crypto/sha512.cpp \
  crypto/sha512.h

# common: shared between dynamicd, and dynamic-qt and non-server tools
libdynamic_common_a_CPPFLAGS = $(AM_CPPFLAGS) $(DYNAMIC_INCLUDES)
libdynamic_common_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libdynamic_common_a_SOURCES = \
  chain/amount.cpp \
  chain/coins.cpp \
  chain/params.cpp \
  consensus/merkle.cpp \
  support/core_read.cpp \
  support/core_write.cpp \
  support/scheduler.cpp \
  keys/bip39.cpp \
  keys/hdchain.cpp \
  keys/key.cpp \
  keys/key_io.cpp \
  keys/keystore.cpp \
  keys/pubkey.cpp \
  third_party/math/arith_uint256.cpp \
  net/address.cpp \
  net/base.cpp \
  net/protocol.cpp \
  primitives/block.cpp \
  primitives/keys.cpp \
  primitives/transaction.cpp \
  script/interpreter.cpp \
  script/script.cpp \
  script/script_error.cpp \
  script/sign.cpp \
  script/standard.cpp \
  util/base58.cpp \
  util/compressor.cpp \
  util/hash.cpp \
  $(DYNAMIC_CORE_H)

# util: shared between all executables.
# This library *must* be included to make sure that the glibc
# backward-compatibility objects and their sanity checks are linked.
libdynamic_util_a_CPPFLAGS = $(AM_CPPFLAGS) $(DYNAMIC_INCLUDES)
libdynamic_util_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libdynamic_util_a_SOURCES = \
  chain/paramsbase.cpp \
  clientversion.cpp \
  compat/glibc_sanity.cpp \
  compat/glibcxx_sanity.cpp \
  compat/strnlen.cpp \
  support/sync.cpp \
  third_party/math/uint256.cpp \
  rpc/protocol.cpp \
  support/cleanse.cpp \
  support/lockedpool.cpp \
  util/moneystr.cpp \
  util/random.cpp \
  util/strencodings.cpp \
  support/threadinterrupt.cpp \
  util/time.cpp \
  util/util.cpp \
  $(DYNAMIC_CORE_H)

if GLIBC_BACK_COMPAT
libdynamic_util_a_SOURCES += compat/glibc_compat.cpp
endif

# cli: shared between dynamic-cli and dynamic-qt
libdynamic_cli_a_CPPFLAGS = $(AM_CPPFLAGS) $(DYNAMIC_INCLUDES)
libdynamic_cli_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libdynamic_cli_a_SOURCES = \
  rpc/client.cpp \
  $(DYNAMIC_CORE_H)

nodist_libdynamic_util_a_SOURCES = $(srcdir)/obj/build.h
#

# dynamicd binary #
dynamicd_SOURCES = dynamicd.cpp
dynamicd_CPPFLAGS = $(AM_CPPFLAGS) $(DYNAMIC_INCLUDES)
dynamicd_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
dynamicd_LDFLAGS = $(RELDFLAGS) $(AM_LDFLAGS) $(LIBTOOL_APP_LDFLAGS)

if TARGET_WINDOWS
dynamicd_SOURCES += dynamicd-res.rc
endif

dynamicd_LDADD = \
  $(LIBDYNAMIC_SERVER) \
  $(LIBDYNAMIC_COMMON) \
  $(LIBUNIVALUE) \
  $(LIBDYNAMIC_UTIL) \
  $(LIBDYNAMIC_CRYPTO) \
  $(LIBLEVELDB) \
  $(LIBLEVELDB_SSE42) \
  $(LIBMEMENV) \
  $(LIBSECP256K1)

if ENABLE_ZMQ
dynamicd_LDADD += $(LIBDYNAMIC_ZMQ) $(ZMQ_LIBS)
endif

if ENABLE_WALLET
dynamicd_LDADD += libdynamic_wallet.a
endif

dynamicd_LDADD += $(BOOST_LIBS) $(BDB_LIBS) $(SSL_LIBS) $(CRYPTO_LIBS) $(MINIUPNPC_LIBS) $(EVENT_PTHREADS_LIBS) $(EVENT_LIBS)

# dynamic-cli binary #
dynamic_cli_SOURCES = dynamic-cli.cpp
dynamic_cli_CPPFLAGS = $(AM_CPPFLAGS) $(DYNAMIC_INCLUDES) $(EVENT_CFLAGS)
dynamic_cli_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
dynamic_cli_LDFLAGS = $(RELDFLAGS) $(AM_LDFLAGS) $(LIBTOOL_APP_LDFLAGS)

if TARGET_WINDOWS
dynamic_cli_SOURCES += dynamic-cli-res.rc
endif

dynamic_cli_LDADD = \
  $(LIBDYNAMIC_CLI) \
  $(LIBUNIVALUE) \
  $(LIBDYNAMIC_UTIL)
dynamic_cli_LDADD += $(BOOST_LIBS) $(SSL_LIBS) $(CRYPTO_LIBS) $(EVENT_LIBS)
#

# dynamic-tx binary #
dynamic_tx_SOURCES = dynamic-tx.cpp
dynamic_tx_CPPFLAGS = $(AM_CPPFLAGS) $(DYNAMIC_INCLUDES)
dynamic_tx_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
dynamic_tx_LDFLAGS = $(RELDFLAGS) $(AM_LDFLAGS) $(LIBTOOL_APP_LDFLAGS)

if TARGET_WINDOWS
dynamic_tx_SOURCES += dynamic-tx-res.rc
endif

dynamic_tx_LDADD = \
  $(LIBUNIVALUE) \
  $(LIBDYNAMIC_COMMON) \
  $(LIBDYNAMIC_UTIL) \
  $(LIBDYNAMIC_CRYPTO) \
  $(LIBSECP256K1)

dynamic_tx_LDADD += $(BOOST_LIBS) $(CRYPTO_LIBS)
#

# dynamicconsensus library #
if BUILD_DYNAMIC_LIBS
include_HEADERS = script/dynamicconsensus.h
libdynamicconsensus_la_SOURCES = \
  crypto/hmac_sha512.cpp \
  crypto/ripemd160.cpp \
  crypto/sha1.cpp \
  crypto/sha256.cpp \
  crypto/sha512.cpp \
  util/hash.cpp \
  primitives/transaction.cpp \
  keys/pubkey.cpp \
  script/dynamicconsensus.cpp \
  script/interpreter.cpp \
  script/script.cpp \
  third_party/math/uint256.cpp \
  util/strencodings.cpp

if GLIBC_BACK_COMPAT
  libdynamicconsensus_la_SOURCES += compat/glibc_compat.cpp
endif

libdynamicconsensus_la_LDFLAGS = $(AM_LDFLAGS) -no-undefined $(RELDFLAGS)
libdynamicconsensus_la_LIBADD = $(LIBSECP256K1)
libdynamicconsensus_la_CPPFLAGS = $(AM_CPPFLAGS) -I$(builddir)/third_party/math -I$(builddir)/third_party/obj -I$(srcdir)/third_party/secp256k1/include -DBUILD_DYNAMIC_INTERNAL
libdynamicconsensus_la_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)

endif
#

CLEANFILES = third_party/leveldb/libleveldb.a third_party/leveldb/libmemenv.a
CLEANFILES += *.gcda *.gcno
CLEANFILES += bdap/*.gcda bdap/*.gcno
CLEANFILES += test/bench/*.gcda test/bench/*.gcno
CLEANFILES += chain/*.gcda chain/*.gcno
CLEANFILES += compat/*.gcda compat/*.gcno
CLEANFILES += consensus/*.gcda consensus/*.gcno
CLEANFILES += crypto/*.gcda crypto/*.gcno
CLEANFILES += db/*.gcda db/*.gcno
CLEANFILES += fluid/*.gcda fluid/*.gcno
CLEANFILES += governance/*.gcda governance/*.gcno
CLEANFILES += http/*.gcda http/*.gcno
CLEANFILES += keys/*.gcda keys/*.gcno
CLEANFILES += third_party/math/*.gcda third_party/math/*.gcno
CLEANFILES += miner/*.gcda miner/*.gcno
CLEANFILES += net/*.gcda net/*.gcno
CLEANFILES += policy/*.gcda policy/*.gcno
CLEANFILES += primitives/*.gcda primitives/*.gcno
CLEANFILES += privatesend/*.gcda privatesend/*.gcno
CLEANFILES += script/*.gcda script/*.gcno
CLEANFILES += support/*.gcda support/*.gcno
CLEANFILES += rpc/*.gcda rpc/*.gcno
CLEANFILES += third_party/univalue/*.gcda third_party/univalue/*.gcno
CLEANFILES += util/*.gcda util/*.gcno
CLEANFILES += wallet/*.gcda wallet/*.gcno
CLEANFILES += wallet/test/*.gcda wallet/test/*.gcno
CLEANFILES += zmq/*.gcda zmq/*.gcno

DISTCLEANFILES = third_party/obj/build.h

EXTRA_DIST = third_party/leveldb

clean-local:
	-$(MAKE) -C third_party/leveldb clean
	-$(MAKE) -C third_party/secp256k1 clean
	-$(MAKE) -C third_party/univalue clean
	-rm -f third_party/leveldb/*/*.gcda third_party/leveldb/*/*.gcno third_party/leveldb/helpers/memenv/*.gcda third_party/leveldb/helpers/memenv/*.gcno
	-rm -f dynamic-config.* stamp-h1

.rc.o:
	@test -f $(WINDRES)
	$(AM_V_GEN) $(WINDRES) -DWINDRES_PREPROC -i $< -o $@

.mm.o:
	$(AM_V_CXX) $(OBJCXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) \
	  $(CPPFLAGS) $(AM_CXXFLAGS) $(QT_INCLUDES) $(AM_CXXFLAGS) $(PIE_FLAGS) $(CXXFLAGS) -c -o $@ $<

check-symbols: $(bin_PROGRAMS)
if GLIBC_BACK_COMPAT
	@echo "Checking glibc back compat..."
	$(AM_V_at) READELF=$(READELF) CPPFILT=$(CPPFILT) $(top_srcdir)/contrib/devtools/symbol-check.py < $(bin_PROGRAMS)
endif

check-security: $(bin_PROGRAMS)
if HARDEN
	@echo "Checking binary security..."
	$(AM_V_at) READELF=$(READELF) OBJDUMP=$(OBJDUMP) $(top_srcdir)/contrib/devtools/security-check.py < $(bin_PROGRAMS)
endif

%.pb.cc %.pb.h: %.proto
	@test -f $(PROTOC)
	$(AM_V_GEN) $(PROTOC) --cpp_out=$(@D) --proto_path=$(abspath $(<D) $<)

if EMBEDDED_LEVELDB
include Makefile.leveldb.include
endif

if ENABLE_GPU
include Makefile.gpu.include
endif

if ENABLE_TESTS
include Makefile.test.include
endif

if ENABLE_BENCH
include Makefile.bench.include
endif

if ENABLE_QT
include Makefile.qt.include
endif

if ENABLE_QT_TESTS
include Makefile.qttest.include
endif