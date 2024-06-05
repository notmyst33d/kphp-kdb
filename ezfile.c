//usr/bin/cc -x c -O2 -Wall -pthread -o .buildbin "$0" && ./.buildbin $@; exit

#include "ez.h"

char *cc_compile_flags[] = { "-g", "-std=gnu89", "-fPIC", "-fcommon", "-I.", "-Ivv", "-Ikfs", "-Inet", "-Iobjs", "-Icommon", "-Ibinlog", "-Idrinkless", "-I/usr/include/openssl-1.1", "-DCOMMIT=\"none\"", 0 };
char *cxx_compile_flags[] = { "-g", "-std=gnu++17", "-fPIC", "-fcommon", "-I.", "-Ivv", "-Ikfs", "-Inet", "-Iobjs", "-Icommon", "-Ibinlog", "-Idrinkless", "-I/usr/include/openssl-1.1", "-DCOMMIT=\"none\"", 0 };

char *shared_lib[] = { "-shared", 0 };
char *kdb_binary[] = { "-Lout", "-lm", "-lz", "-lcrypto", "-lkdb-binlog", "-lkdb-common", "-lkdb-crypto", "-lkdb-kfs", "-lkdb-net", "-lkdb-vv", 0 };
char *kphp_binary[] = { "-Lout", "-lz", "-lkdb-common", "-ldrinkless", 0 };
char *kphp_engine[] = { "-r", 0 };

#define cc(in, out) global_queue_compile(in, out, cc_compile_flags, COMPILER_CC)
#define cxx(in, out) global_queue_compile(in, out, cxx_compile_flags, COMPILER_CXX)
#define ld global_queue_link

void build_kdb_common(void) {
    cc("common/aho-kmp.c", "out/common/aho-kmp.o");
    cc("common/base64.c", "out/common/base64.o");
    cc("common/common-data.c", "out/common/common-data.o");
    cc("common/crc32.c", "out/common/crc32.o");
    cc("common/crc32c.c", "out/common/crc32c.o");
    cc("common/diff-patch.c", "out/common/diff-patch.o");
    cc("common/estimate-split.c", "out/common/estimate-split.o");
    cc("common/fast-backtrace.c", "out/common/fast-backtrace.o");
    cc("common/kdb-data-common.c", "out/common/kdb-data-common.o");
    cc("common/listcomp.c", "out/common/listcomp.o");
    cc("common/md5.c", "out/common/md5.o");
    cc("common/pid.c", "out/common/pid.o");
    cc("common/resolver.c", "out/common/resolver.o");
    cc("common/search-y-parse.c", "out/common/search-y-parse.o");
    cc("common/server-functions.c", "out/common/server-functions.o");
    cc("common/sha1.c", "out/common/sha1.o");
    cc("common/stemmer-new.c", "out/common/stemmer-new.o");
    cc("common/string-processing.c", "out/common/string-processing.o");
    cc("common/suffix-array.c", "out/common/suffix-array.o");
    cc("common/translit.c", "out/common/translit.o");
    cc("common/unicode-utils.c", "out/common/unicode-utils.o");
    cc("common/utf8_utils.c", "out/common/utf8_utils.o");
    cc("common/word-split.c", "out/common/word-split.o");
    cc("common/xz_dec.c", "out/common/xz_dec.o");
}

void link_kdb_common(void) {
    ld("out/libkdb-common.so", (char*[]){
        "out/common/aho-kmp.o",
        "out/common/base64.o",
        "out/common/common-data.o",
        "out/common/crc32.o",
        "out/common/crc32c.o",
        "out/common/diff-patch.o",
        "out/common/estimate-split.o",
        "out/common/fast-backtrace.o",
        "out/common/kdb-data-common.o",
        "out/common/listcomp.o",
        "out/common/md5.o",
        "out/common/pid.o",
        "out/common/resolver.o",
        "out/common/search-y-parse.o",
        "out/common/server-functions.o",
        "out/common/sha1.o",
        "out/common/stemmer-new.o",
        "out/common/string-processing.o",
        "out/common/suffix-array.o",
        "out/common/translit.o",
        "out/common/unicode-utils.o",
        "out/common/utf8_utils.o",
        "out/common/word-split.o",
        "out/common/xz_dec.o", 0
    }, shared_lib, COMPILER_CC);
}

void build_kdb_vv(void) {
    cc("vv/am-amortization.c", "out/vv/am-amortization.o");
    cc("vv/am-hash.c", "out/vv/am-hash.o");
    cc("vv/am-server-functions.c", "out/vv/am-server-functions.o");
    cc("vv/am-stats.c", "out/vv/am-stats.o");
    cc("vv/vv-tl-aio.c", "out/vv/vv-tl-aio.o");
    cc("vv/vv-tl-parse.c", "out/vv/vv-tl-parse.o");
}

void link_kdb_vv(void) {
    ld("out/libkdb-vv.so", (char*[]){
        "out/vv/am-amortization.o",
        "out/vv/am-hash.o",
        "out/vv/am-server-functions.o",
        "out/vv/am-stats.o",
        "out/vv/vv-tl-aio.o",
        "out/vv/vv-tl-parse.o", 0
    }, shared_lib, COMPILER_CC);
}

void build_kdb_crypto(void) {
    cc("crypto/aesni256.c", "out/crypto/aesni256.o");
}

void link_kdb_crypto(void) {
    ld("out/libkdb-crypto.so", (char*[]){ "out/crypto/aesni256.o", 0 }, shared_lib, COMPILER_CC);
}

void build_kdb_net(void) {
    cc("net/net-aio.c", "out/net/net-aio.o");
    cc("net/net-buffers.c", "out/net/net-buffers.o");
    cc("net/net-connections.c", "out/net/net-connections.o");
    cc("net/net-crypto-aes.c", "out/net/net-crypto-aes.o");
    cc("net/net-crypto-rsa.c", "out/net/net-crypto-rsa.o");
    cc("net/net-events.c", "out/net/net-events.o");
    cc("net/net-http-client.c", "out/net/net-http-client.o");
    cc("net/net-http-server.c", "out/net/net-http-server.o");
    cc("net/net-memcache-client.c", "out/net/net-memcache-client.o");
    cc("net/net-memcache-server.c", "out/net/net-memcache-server.o");
    cc("net/net-msg-buffers.c", "out/net/net-msg-buffers.o");
    cc("net/net-msg.c", "out/net/net-msg.o");
    cc("net/net-mysql-client.c", "out/net/net-mysql-client.o");
    cc("net/net-mysql-server.c", "out/net/net-mysql-server.o");
    cc("net/net-parse.c", "out/net/net-parse.o");
    cc("net/net-rpc-client.c", "out/net/net-rpc-client.o");
    cc("net/net-rpc-common.c", "out/net/net-rpc-common.o");
    cc("net/net-rpc-server.c", "out/net/net-rpc-server.o");
    cc("net/net-rpc-targets.c", "out/net/net-rpc-targets.o");
    cc("net/net-tcp-connections.c", "out/net/net-tcp-connections.o");
    cc("net/net-tcp-rpc-common.c", "out/net/net-tcp-rpc-common.o");
    cc("net/net-tcp-rpc-server.c", "out/net/net-tcp-rpc-server.o");
    cc("net/net-udp.c", "out/net/net-udp.o");
    cc("net/net-udp-targets.c", "out/net/net-udp-targets.o");
}

void link_kdb_net(void) {
    ld("out/libkdb-net.so", (char*[]){
        "out/net/net-aio.o",
        "out/net/net-buffers.o",
        "out/net/net-connections.o",
        "out/net/net-crypto-aes.o",
        "out/net/net-crypto-rsa.o",
        "out/net/net-events.o",
        "out/net/net-http-client.o",
        "out/net/net-http-server.o",
        "out/net/net-memcache-client.o",
        "out/net/net-memcache-server.o",
        "out/net/net-msg-buffers.o",
        "out/net/net-msg.o",
        "out/net/net-mysql-client.o",
        "out/net/net-mysql-server.o",
        "out/net/net-parse.o",
        "out/net/net-rpc-client.o",
        "out/net/net-rpc-common.o",
        "out/net/net-rpc-server.o",
        "out/net/net-rpc-targets.o",
        "out/net/net-tcp-connections.o",
        "out/net/net-tcp-rpc-common.o",
        "out/net/net-tcp-rpc-server.o",
        "out/net/net-udp.o",
        "out/net/net-udp-targets.o", 0
    }, shared_lib, COMPILER_CC);
}

void build_kdb_binlog(void) {
    cc("binlog/kdb-binlog-common.c", "out/binlog/kdb-binlog-common.o");
}

void link_kdb_binlog(void) {
    ld("out/libkdb-binlog.so", (char*[]){ "out/binlog/kdb-binlog-common.o", 0 }, shared_lib, COMPILER_CC);
}

void build_kdb_kfs(void) {
    cc("kfs/kfs.c", "out/kfs/kfs.o");
}

void link_kdb_kfs(void) {
    ld("out/libkdb-kfs.so", (char*[]){ "out/kfs/kfs.o", 0 }, shared_lib, COMPILER_CC);
}

void build_random_engine(void) {
    cc("random/random-data.c", "out/random/random-data.o");
    cc("random/random-engine.c", "out/random/random-engine.o");
}

void link_random_engine(void) {
    ld("out/random-engine", (char*[]){
        "out/random/random-data.o",
        "out/random/random-engine.o", 0
    }, kdb_binary, COMPILER_CC);
}

void build_dns_engine(void) {
    cc("dns/dns-data.c", "out/dns/dns-data.o");
    cc("dns/dns-engine.c", "out/dns/dns-engine.o");
}

void link_dns_engine(void) {
    ld("out/dns-engine", (char*[]){
        "out/dns/dns-data.o",
        "out/dns/dns-engine.o", 0
    }, kdb_binary, COMPILER_CC);
}

void build_lists_engine(void) {
    cc("lists/lists-data.c", "out/lists/lists-data.o");
    cc("lists/lists-engine.c", "out/lists/lists-engine.o");
}

void link_lists_engine(void) {
    ld("out/lists-engine", (char*[]){
        "out/lists/lists-data.o",
        "out/lists/lists-engine.o", 0
    }, kdb_binary, COMPILER_CC);
}

void build_kphp_compiler(void) {
    cxx("KPHP/kphp2cpp.cpp", "out/KPHP/kphp2cpp.o");
    cxx("KPHP/compiler/bicycle.cpp", "out/KPHP/compiler/bicycle.o");
    cxx("KPHP/compiler/cfg.cpp", "out/KPHP/compiler/cfg.o");
    cxx("KPHP/compiler/compiler-core.cpp", "out/KPHP/compiler/compiler-core.o");
    cxx("KPHP/compiler/compiler.cpp", "out/KPHP/compiler/compiler.o");
    cxx("KPHP/compiler/data.cpp", "out/KPHP/compiler/data.o");
    cxx("KPHP/compiler/gentree.cpp", "out/KPHP/compiler/gentree.o");
    cxx("KPHP/compiler/io.cpp", "out/KPHP/compiler/io.o");
    cxx("KPHP/compiler/lexer.cpp", "out/KPHP/compiler/lexer.o");
    cxx("KPHP/compiler/name-gen.cpp", "out/KPHP/compiler/name-gen.o");
    cxx("KPHP/compiler/operation.cpp", "out/KPHP/compiler/operation.o");
    cxx("KPHP/compiler/pass-rl.cpp", "out/KPHP/compiler/pass-rl.o");
    cxx("KPHP/compiler/pass-ub.cpp", "out/KPHP/compiler/pass-ub.o");
    cxx("KPHP/compiler/stage.cpp", "out/KPHP/compiler/stage.o");
    cxx("KPHP/compiler/token.cpp", "out/KPHP/compiler/token.o");
    cxx("KPHP/compiler/type-inferer-core.cpp", "out/KPHP/compiler/type-inferer-core.o");
    cxx("KPHP/compiler/type-inferer.cpp", "out/KPHP/compiler/type-inferer.o");
    cxx("KPHP/compiler/types.cpp", "out/KPHP/compiler/types.o");
    cxx("KPHP/compiler/vertex.cpp", "out/KPHP/compiler/vertex.o");
}

void link_kphp_compiler(void) {
    ld("out/kphp2cpp", (char*[]){
        "out/KPHP/kphp2cpp.o",
        "out/KPHP/compiler/bicycle.o",
        "out/KPHP/compiler/cfg.o",
        "out/KPHP/compiler/compiler-core.o",
        "out/KPHP/compiler/compiler.o",
        "out/KPHP/compiler/data.o",
        "out/KPHP/compiler/gentree.o",
        "out/KPHP/compiler/io.o",
        "out/KPHP/compiler/lexer.o",
        "out/KPHP/compiler/name-gen.o",
        "out/KPHP/compiler/operation.o",
        "out/KPHP/compiler/pass-rl.o",
        "out/KPHP/compiler/pass-ub.o",
        "out/KPHP/compiler/stage.o",
        "out/KPHP/compiler/token.o",
        "out/KPHP/compiler/type-inferer-core.o",
        "out/KPHP/compiler/type-inferer.o",
        "out/KPHP/compiler/types.o",
        "out/KPHP/compiler/vertex.o", 0
    }, kphp_binary, COMPILER_CXX);
}

void build_kphp_engine(void) {
    cc("KPHP/php-engine.c", "out/KPHP/php-engine.o");
    cc("KPHP/php-engine-vars.c", "out/KPHP/php-engine-vars.o");
    cxx("KPHP/php-queries.cpp", "out/KPHP/php-queries.o");
    cxx("KPHP/php-runner.cpp", "out/KPHP/php-runner.o");
    cxx("KPHP/php-master.cpp", "out/KPHP/php-master.o");
}

void link_kphp_engine(void) {
    ld("out/kphp-engine.o", (char*[]){
        "out/KPHP/php-engine.o",
        "out/KPHP/php-engine-vars.o",
        "out/KPHP/php-queries.o",
        "out/KPHP/php-runner.o",
        "out/KPHP/php-master.o", 0
    }, kphp_engine, COMPILER_CXX);
}

void build_kphp_runtime(void) {
    cxx("KPHP/php_script.cpp", "out/KPHP/php_script.o");
    cxx("KPHP/runtime/allocator.cpp", "out/KPHP/runtime/allocator.o");
    cxx("KPHP/runtime/array_functions.cpp", "out/KPHP/runtime/array_functions.o");
    cxx("KPHP/runtime/bcmath.cpp", "out/KPHP/runtime/bcmath.o");
    cxx("KPHP/runtime/datetime.cpp", "out/KPHP/runtime/datetime.o");
    cxx("KPHP/runtime/drivers.cpp", "out/KPHP/runtime/drivers.o");
    cxx("KPHP/runtime/exception.cpp", "out/KPHP/runtime/exception.o");
    cxx("KPHP/runtime/files.cpp", "out/KPHP/runtime/files.o");
    cxx("KPHP/runtime/interface.cpp", "out/KPHP/runtime/interface.o");
    cxx("KPHP/runtime/math_functions.cpp", "out/KPHP/runtime/math_functions.o");
    cxx("KPHP/runtime/mbstring.cpp", "out/KPHP/runtime/mbstring.o");
    cxx("KPHP/runtime/misc.cpp", "out/KPHP/runtime/misc.o");
    cxx("KPHP/runtime/openssl.cpp", "out/KPHP/runtime/openssl.o");
    cxx("KPHP/runtime/regexp.cpp", "out/KPHP/runtime/regexp.o");
    cxx("KPHP/runtime/rpc.cpp", "out/KPHP/runtime/rpc.o");
    cxx("KPHP/runtime/string_functions.cpp", "out/KPHP/runtime/string_functions.o");
    cxx("KPHP/runtime/url.cpp", "out/KPHP/runtime/url.o");
    cxx("KPHP/runtime/zlib.cpp", "out/KPHP/runtime/zlib.o");
}

void link_kphp_runtime(void) {
    ld("out/libkphp-runtime.so", (char*[]){
        "out/KPHP/php_script.o",
        "out/KPHP/runtime/allocator.o",
        "out/KPHP/runtime/array_functions.o",
        "out/KPHP/runtime/bcmath.o",
        "out/KPHP/runtime/datetime.o",
        "out/KPHP/runtime/drivers.o",
        "out/KPHP/runtime/exception.o",
        "out/KPHP/runtime/files.o",
        "out/KPHP/runtime/interface.o",
        "out/KPHP/runtime/math_functions.o",
        "out/KPHP/runtime/mbstring.o",
        "out/KPHP/runtime/misc.o",
        "out/KPHP/runtime/openssl.o",
        "out/KPHP/runtime/regexp.o",
        "out/KPHP/runtime/rpc.o",
        "out/KPHP/runtime/string_functions.o",
        "out/KPHP/runtime/url.o",
        "out/KPHP/runtime/zlib.o", 0
    }, shared_lib, COMPILER_CXX);
}

void build_drinkless(void) {
    cc("drinkless/dl-utils-lite.c", "out/drinkless/dl-utils-lite.o");
}

void link_drinkless(void) {
    ld("out/libdrinkless.so", (char*[]){ "out/drinkless/dl-utils-lite.o", 0 }, shared_lib, COMPILER_CC);
}

void build_component(char *component) {
    int all = strcmp(component, "all") == 0;
    if (strcmp(component, "common") == 0 || all) build_kdb_common();
    if (strcmp(component, "vv") == 0 || all) build_kdb_vv();
    if (strcmp(component, "crypto") == 0 || all) build_kdb_crypto();
    if (strcmp(component, "net") == 0 || all) build_kdb_net();
    if (strcmp(component, "binlog") == 0 || all) build_kdb_binlog();
    if (strcmp(component, "kfs") == 0 || all) build_kdb_kfs();
    if (strcmp(component, "drinkless") == 0 || all) build_drinkless();
    if (strcmp(component, "random-engine") == 0 || all) build_random_engine();
    //if (strcmp(component, "dns-engine") == 0 || all) build_dns_engine();
    //if (strcmp(component, "lists-engine") == 0 || all) build_lists_engine();
    if (strcmp(component, "kphp-compiler") == 0 || all) build_kphp_compiler();
    if (strcmp(component, "kphp-runtime") == 0 || all) build_kphp_runtime();
    if (strcmp(component, "kphp-engine") == 0 || all) build_kphp_engine();
}

void link_component_base(char *component) {
    int all = strcmp(component, "all") == 0;
    if (strcmp(component, "common") == 0 || all) link_kdb_common();
    if (strcmp(component, "vv") == 0 || all) link_kdb_vv();
    if (strcmp(component, "crypto") == 0 || all) link_kdb_crypto();
    if (strcmp(component, "net") == 0 || all) link_kdb_net();
    if (strcmp(component, "binlog") == 0 || all) link_kdb_binlog();
    if (strcmp(component, "kfs") == 0 || all) link_kdb_kfs();
    if (strcmp(component, "drinkless") == 0 || all) link_drinkless();
}

void link_component_engine(char *component) {
    int all = strcmp(component, "all") == 0;
    if (strcmp(component, "random-engine") == 0 || all) link_random_engine();
    //if (strcmp(component, "dns-engine") == 0 || all) link_dns_engine();
    //if (strcmp(component, "lists-engine") == 0 || all) link_lists_engine();
}

void link_component_kphp(char *component) {
    int all = strcmp(component, "all") == 0;
    if (strcmp(component, "kphp-compiler") == 0 || all) link_kphp_compiler();
    if (strcmp(component, "kphp-runtime") == 0 || all) link_kphp_runtime();
    if (strcmp(component, "kphp-engine") == 0 || all) link_kphp_engine();
}

#define component_argv(func) do { \
    if (argc > 1) { \
        for (int i = 1; i < argc; i++) { \
            func(argv[i]); \
        } \
    } else { \
        func("all"); \
    } \
} while (0)

int main(int argc, char **argv) {
    ez_mkdir("out");
    ez_mkdir("out/common");
    ez_mkdir("out/vv");
    ez_mkdir("out/crypto");
    ez_mkdir("out/net");
    ez_mkdir("out/binlog");
    ez_mkdir("out/kfs");
    ez_mkdir("out/random");
    ez_mkdir("out/dns");
    ez_mkdir("out/lists");
    ez_mkdir("out/KPHP");
    ez_mkdir("out/KPHP/runtime");
    ez_mkdir("out/KPHP/compiler");
    ez_mkdir("out/drinkless");

    global_queue_start();

    component_argv(build_component);
    assert(global_queue_wait());

    component_argv(link_component_base);
    assert(global_queue_wait());

    component_argv(link_component_engine);
    component_argv(link_component_kphp);
    assert(global_queue_wait());

    global_queue_shutdown();

    INFO("Done");
    return 0;
}
