//usr/bin/cc -x c -O2 -Wall -lpthread -o .buildbin "$0" && ./.buildbin; exit

#include "ez.h"

char *compile_flags[] = { "-std=gnu89", "-fPIC", "-fcommon", "-I.", "-Ivv", "-Ikfs", "-Inet", "-Iobjs", "-Icommon", "-Ibinlog", "-I/usr/include/openssl-1.1", "-DCOMMIT=\"none\"", 0 };
char *shared_lib[] = { "-shared", 0 };
char *binary[] = { "-Lout", "-lm", "-lz", "-lcrypto", "-lkdb-binlog", "-lkdb-common", "-lkdb-crypto", "-lkdb-kfs", "-lkdb-net", "-lkdb-vv", 0 };

#define cc(in, out) global_queue_cc(in, out, compile_flags)
#define ld global_queue_ld

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
    }, shared_lib);
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
    }, shared_lib);
}

void build_kdb_crypto(void) {
    cc("crypto/aesni256.c", "out/crypto/aesni256.o");
}

void link_kdb_crypto(void) {
    ld("out/libkdb-crypto.so", (char*[]){ "out/crypto/aesni256.o", 0 }, shared_lib);
}

void build_kdb_net(void) {
    cc("net/net-aio.c", "out/net/net-aio.o");
    cc("net/net-buffers.c", "out/net/net-buffers.o");
    cc("net/net-connections.c", "out/net/net-connections.o");
    cc("net/net-crypto-aes.c", "out/net/net-crypto-aes.o");
    cc("net/net-crypto-rsa.c", "out/net/net-crypto-rsa.o");
    cc("net/net-events.c", "out/net/net-events.o");
    cc("net/net-memcache-server.c", "out/net/net-memcache-server.o");
    cc("net/net-msg-buffers.c", "out/net/net-msg-buffers.o");
    cc("net/net-msg.c", "out/net/net-msg.o");
    cc("net/net-parse.c", "out/net/net-parse.o");
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
        "out/net/net-memcache-server.o",
        "out/net/net-msg-buffers.o",
        "out/net/net-msg.o",
        "out/net/net-parse.o",
        "out/net/net-rpc-common.o",
        "out/net/net-rpc-server.o",
        "out/net/net-rpc-targets.o",
        "out/net/net-tcp-connections.o",
        "out/net/net-tcp-rpc-common.o",
        "out/net/net-tcp-rpc-server.o",
        "out/net/net-udp.o",
        "out/net/net-udp-targets.o", 0
    }, shared_lib);
}

void build_kdb_binlog(void) {
    cc("binlog/kdb-binlog-common.c", "out/binlog/kdb-binlog-common.o");
}

void link_kdb_binlog(void) {
    ld("out/libkdb-binlog.so", (char*[]){ "out/binlog/kdb-binlog-common.o", 0 }, shared_lib);
}

void build_kdb_kfs(void) {
    cc("kfs/kfs.c", "out/kfs/kfs.o");
}

void link_kdb_kfs(void) {
    ld("out/libkdb-kfs.so", (char*[]){ "out/kfs/kfs.o", 0 }, shared_lib);
}

void build_random_engine(void) {
    cc("random/random-data.c", "out/random/random-data.o");
    cc("random/random-engine.c", "out/random/random-engine.o");
}

void link_random_engine(void) {
    ld("out/random-engine", (char*[]){
        "out/random/random-data.o",
        "out/random/random-engine.o", 0
    }, binary);
}

void build_dns_engine(void) {
    cc("dns/dns-data.c", "out/dns/dns-data.o");
    cc("dns/dns-engine.c", "out/dns/dns-engine.o");
}

void link_dns_engine(void) {
    ld("out/dns-engine", (char*[]){
        "out/dns/dns-data.o",
        "out/dns/dns-engine.o", 0
    }, binary);
}

void build_lists_engine(void) {
    cc("lists/lists-data.c", "out/lists/lists-data.o");
    cc("lists/lists-engine.c", "out/lists/lists-engine.o");
}

void link_lists_engine(void) {
    ld("out/lists-engine", (char*[]){
        "out/lists/lists-data.o",
        "out/lists/lists-engine.o", 0
    }, binary);
}

int main(void) {
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

    global_queue_start();

    build_kdb_common();
    build_kdb_vv();
    build_kdb_crypto();
    build_kdb_net();
    build_kdb_binlog();
    build_kdb_kfs();
    build_random_engine();
    build_dns_engine();
    build_lists_engine();
    assert(global_queue_wait());

    link_kdb_common();
    link_kdb_vv();
    link_kdb_crypto();
    link_kdb_net();
    link_kdb_binlog();
    link_kdb_kfs();
    assert(global_queue_wait());

    link_random_engine();
    link_dns_engine();
    link_lists_engine();
    assert(global_queue_wait());

    global_queue_shutdown();

    INFO("Done");
    return 0;
}
