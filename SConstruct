import os

commit = os.popen('git log -1 --pretty=format:"%H"').read()
env = Environment(
    CPPPATH = ['TL', 'binlog', 'common', 'drinkless', 'kfs', 'net', 'vv', '.'],
    CPPDEFINES = { 'COMMIT': f'\\"{commit}\\"' },
    CFLAGS = ['-std=gnu89', '-fcommon'],
    CXXFLAGS = ['-std=gnu++17', '-fcommon'],
)

common_sources = Glob('common/*.c', exclude = 'common/stemmer.c')
common = env.Library('kdb-common', common_sources)
common_oldstemmer = env.Library('kdb-common-oldstemmer', Glob('common/*.c', exclude = 'common/stemmer-new.c'))

tlc_env = env.Clone()
tlc_env.Append(LIBS = ['kdb-common', 'm'], LIBPATH = '.')
tlc_env.Program('tlc-new', ['TL/tlc-new.c', 'TL/tl-parser-new.c'])

env.Command('combined.tl', Glob('*/*.tl'), "cat */*.tl | sed 's/[[:space:]]*$//' > combined.tl || ( rm combined.tl && false )")
env.Command('combined2.tl', ['combined.tl', 'tlc-new'], "./tlc-new -w 2 -E combined.tl 2> combined2.tl || ( cat combined2.tl && rm combined2.tl && false )")
env.Command('TL/constants.h', ['combined2.tl', 'TL/gen_constants_h.awk'], "awk -f TL/gen_constants_h.awk <combined2.tl >TL/constants.h || ( rm TL/constants.h && false )")

vv_sources = Glob('vv/*.c')
vv = env.Library('kdb-vv', vv_sources)

crypto_sources = Glob('crypto/*.c')
crypto = env.Library('kdb-crypto', crypto_sources)

net_sources = Glob('net/*.c')
net = env.Library('kdb-net', net_sources)

binlog_sources = ['binlog/kdb-binlog-common.c']
binlog = env.Library('kdb-binlog', binlog_sources)

kfs_sources = Glob('kfs/*.c')
kfs = env.Library('kdb-kfs', kfs_sources)

drinkless_sources = ['drinkless/dl-utils-lite.c', 'drinkless/dl-utils.c', 'drinkless/dl-perm.c']
drinkless = env.Library('drinkless', drinkless_sources)

engine_env = env.Clone()
engine_env.Append(LIBS = [vv, net, crypto, binlog, kfs, common, drinkless, 'm'], LIBPATH = '.')
engine_env.ParseConfig('pkg-config zlib --cflags --libs')
engine_env.ParseConfig('pkg-config libcrypto --cflags --libs')
engine_oldstemmer_env = env.Clone()
engine_oldstemmer_env.Append(LIBS = [vv, net, crypto, binlog, kfs, common_oldstemmer, drinkless, 'm'], LIBPATH = '.')
engine_oldstemmer_env.ParseConfig('pkg-config zlib --cflags --libs')
engine_oldstemmer_env.ParseConfig('pkg-config libcrypto --cflags --libs')
engine_image_env = env.Clone()
engine_image_env.Append(LIBS = [vv, net, crypto, binlog, kfs, common, drinkless, 'm'], LIBPATH = '.')
engine_image_env.ParseConfig('pkg-config zlib --cflags --libs')
engine_image_env.ParseConfig('pkg-config libcrypto --cflags --libs')
engine_image_env.ParseConfig('pkg-config GraphicsMagick --cflags --libs')
engine_image_env.ParseConfig('pkg-config libwebp --cflags --libs')

engine_env.Program('random-engine', ['random/random-engine.c', 'random/random-data.c'])
engine_env.Program('dns-engine', ['dns/dns-engine.c', 'dns/dns-data.c'])
engine_env.Program('lists-engine', ['lists/lists-engine.c', 'lists/lists-data.c'])
engine_env.Program('memcached-engine', ['memcached/memcached-engine.c', 'memcached/memcached-data.c'])
engine_env.Program('pmemcached-ram-engine', ['pmemcached/pmemcached-engine.c', 'pmemcached/pmemcached-data.c', 'pmemcached/pmemcached-index-ram.c'])
engine_env.Program('pmemcached-disk-engine', ['pmemcached/pmemcached-engine.c', 'pmemcached/pmemcached-data.c', 'pmemcached/pmemcached-index-disk.c'])
engine_env.Program('storage-engine', ['storage/storage-engine.c', 'storage/storage-data.c', 'storage/storage-rpc.c', 'storage/storage-content.c'])
engine_env.Program('photo-engine', ['photo/photo-engine.c', 'photo/photo-data.c', 'photo/utils.c', 'photo/dl.c'])

engine_oldstemmer_env.Program('text-engine', ['text/text-engine.c', 'text/text-data.c'])

engine_image_env.Program('image-engine', ['image/image-engine.c', 'image/image-data.c'])

kphp_runtime_sources = Glob('KPHP/runtime/*.cpp', exclude = ['KPHP/runtime/array.cpp', 'KPHP/runtime/class.cpp', 'KPHP/runtime/string.cpp', 'KPHP/runtime/string_buffer.cpp', 'KPHP/runtime/variable.cpp'])
env.Library('kphp-runtime', [
    common_sources,
    drinkless_sources,
    kphp_runtime_sources,
    'KPHP/php_script.cpp',
])
env.Library('kphp-engine', [
    binlog_sources,
    common_sources,
    vv_sources,
    crypto_sources,
    net_sources,
    kfs_sources,
    drinkless_sources,
    kphp_runtime_sources,
    'KPHP/php-master.cpp',
    'KPHP/php-queries.cpp',
    'KPHP/php-runner.cpp',
    'KPHP/php_script.cpp',
    'KPHP/php-engine.c',
    'KPHP/php-engine-vars.c',
])
env.Program('kphp2cpp', [Glob('KPHP/compiler/*.cpp'), 'KPHP/kphp2cpp.cpp'], LIBS = [common, drinkless], LIBPATH = '.')
