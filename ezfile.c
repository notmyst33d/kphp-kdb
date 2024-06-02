//usr/bin/cc -x c -O2 -Wall -lpthread -o .buildbin "$0" && ./.buildbin; exit

#include "ez.h"

void cc(char *in, char *out) {
    Command *cmd = cmd_new();
    char message[512];

    sprintf(message, "cc %s", in);
    cmd_message(cmd, message);

    cmd_push(cmd, "cc");
    cmd_push(cmd, "-std=gnu89");
    cmd_push(cmd, "-fPIC");
    cmd_push(cmd, "-fcommon");
    cmd_push(cmd, "-I.");
    cmd_push(cmd, "-Ivv");
    cmd_push(cmd, "-Ikfs");
    cmd_push(cmd, "-Inet");
    cmd_push(cmd, "-Iobjs");
    cmd_push(cmd, "-Icommon");
    cmd_push(cmd, "-Ibinlog");
    cmd_push(cmd, "-I/usr/include/openssl-1.1");
    cmd_push(cmd, "-DCOMMIT=\"none\"");
    cmd_push(cmd, "-c");
    cmd_push(cmd, "-o");
    cmd_push(cmd, out);
    cmd_push(cmd, in);

    queue_push(&global_queue, cmd);
}

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

int link_kdb_common(void) {
    Command *cmd = cmd_new();
    cmd_push(cmd, "cc");
    cmd_push(cmd, "-shared");
    cmd_push(cmd, "-o");
    cmd_push(cmd, "out/libkdb-common.so");
    cmd_push(cmd, "out/common/aho-kmp.o");
    cmd_push(cmd, "out/common/base64.o");
    cmd_push(cmd, "out/common/common-data.o");
    cmd_push(cmd, "out/common/crc32.o");
    cmd_push(cmd, "out/common/crc32c.o");
    cmd_push(cmd, "out/common/diff-patch.o");
    cmd_push(cmd, "out/common/estimate-split.o");
    cmd_push(cmd, "out/common/fast-backtrace.o");
    cmd_push(cmd, "out/common/kdb-data-common.o");
    cmd_push(cmd, "out/common/listcomp.o");
    cmd_push(cmd, "out/common/md5.o");
    cmd_push(cmd, "out/common/pid.o");
    cmd_push(cmd, "out/common/resolver.o");
    cmd_push(cmd, "out/common/search-y-parse.o");
    cmd_push(cmd, "out/common/server-functions.o");
    cmd_push(cmd, "out/common/sha1.o");
    cmd_push(cmd, "out/common/stemmer-new.o");
    cmd_push(cmd, "out/common/string-processing.o");
    cmd_push(cmd, "out/common/suffix-array.o");
    cmd_push(cmd, "out/common/translit.o");
    cmd_push(cmd, "out/common/unicode-utils.o");
    cmd_push(cmd, "out/common/utf8_utils.o");
    cmd_push(cmd, "out/common/word-split.o");
    cmd_push(cmd, "out/common/xz_dec.o");
    assert(cmd_execute(cmd));
    cmd_free(cmd);
    return 0;
}

void build_kdb_vv(void) {
    cc("vv/am-amortization.c", "out/vv/am-amortization.o");
    cc("vv/am-hash.c", "out/vv/am-hash.o");
    cc("vv/am-server-functions.c", "out/vv/am-server-functions.o");
    cc("vv/am-stats.c", "out/vv/am-stats.o");
    cc("vv/vv-tl-aio.c", "out/vv/vv-tl-aio.o");
    cc("vv/vv-tl-parse.c", "out/vv/vv-tl-parse.o");
}

int link_kdb_vv(void) {
    Command *cmd = cmd_new();
    cmd_push(cmd, "cc");
    cmd_push(cmd, "-shared");
    cmd_push(cmd, "-o");
    cmd_push(cmd, "out/libkdb-vv.so");
    cmd_push(cmd, "out/vv/am-amortization.o");
    cmd_push(cmd, "out/vv/am-hash.o");
    cmd_push(cmd, "out/vv/am-server-functions.o");
    cmd_push(cmd, "out/vv/am-stats.o");
    cmd_push(cmd, "out/vv/vv-tl-aio.o");
    cmd_push(cmd, "out/vv/vv-tl-parse.o");
    assert(cmd_execute(cmd));
    cmd_free(cmd);
    return 0;
}

void build_kdb_crypto(void) {
    cc("crypto/aesni256.c", "out/crypto/aesni256.o");
}

int link_kdb_crypto(void) {
    Command *cmd = cmd_new();
    cmd_push(cmd, "cc");
    cmd_push(cmd, "-shared");
    cmd_push(cmd, "-o");
    cmd_push(cmd, "out/libkdb-crypto.so");
    cmd_push(cmd, "out/crypto/aesni256.o");
    assert(cmd_execute(cmd));
    cmd_free(cmd);
    return 0;
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

int link_kdb_net(void) {
    Command *cmd = cmd_new();
    cmd_push(cmd, "cc");
    cmd_push(cmd, "-shared");
    cmd_push(cmd, "-o");
    cmd_push(cmd, "out/libkdb-net.so");
    cmd_push(cmd, "out/net/net-buffers.o");
    cmd_push(cmd, "out/net/net-connections.o");
    cmd_push(cmd, "out/net/net-crypto-aes.o");
    cmd_push(cmd, "out/net/net-crypto-rsa.o");
    cmd_push(cmd, "out/net/net-events.o");
    cmd_push(cmd, "out/net/net-memcache-server.o");
    cmd_push(cmd, "out/net/net-msg-buffers.o");
    cmd_push(cmd, "out/net/net-msg.o");
    cmd_push(cmd, "out/net/net-parse.o");
    cmd_push(cmd, "out/net/net-rpc-common.o");
    cmd_push(cmd, "out/net/net-rpc-server.o");
    cmd_push(cmd, "out/net/net-rpc-targets.o");
    cmd_push(cmd, "out/net/net-tcp-connections.o");
    cmd_push(cmd, "out/net/net-tcp-rpc-common.o");
    cmd_push(cmd, "out/net/net-tcp-rpc-server.o");
    cmd_push(cmd, "out/net/net-udp.o");
    cmd_push(cmd, "out/net/net-udp-targets.o");
    assert(cmd_execute(cmd));
    cmd_free(cmd);
    return 0;
}

void build_kdb_binlog(void) {
    cc("binlog/kdb-binlog-common.c", "out/binlog/kdb-binlog-common.o");
}

int link_kdb_binlog(void) {
    Command *cmd = cmd_new();
    cmd_push(cmd, "cc");
    cmd_push(cmd, "-shared");
    cmd_push(cmd, "-o");
    cmd_push(cmd, "out/libkdb-binlog.so");
    cmd_push(cmd, "out/binlog/kdb-binlog-common.o");
    assert(cmd_execute(cmd));
    cmd_free(cmd);
    return 0;
}

void build_kdb_kfs(void) {
    cc("kfs/kfs.c", "out/kfs/kfs.o");
}

int link_kdb_kfs(void) {
    Command *cmd = cmd_new();
    cmd_push(cmd, "cc");
    cmd_push(cmd, "-shared");
    cmd_push(cmd, "-o");
    cmd_push(cmd, "out/libkdb-kfs.so");
    cmd_push(cmd, "out/kfs/kfs.o");
    assert(cmd_execute(cmd));
    cmd_free(cmd);
    return 0;
}

void build_random_engine(void) {
    cc("random/random-data.c", "out/random/random-data.o");
    cc("random/random-engine.c", "out/random/random-engine.o");
}

int link_random_engine(void) {
    Command *cmd = cmd_new();
    cmd_push(cmd, "cc");
    cmd_push(cmd, "-Lout");
    cmd_push(cmd, "-lm");
    cmd_push(cmd, "-lz");
    cmd_push(cmd, "-lcrypto");
    cmd_push(cmd, "-lkdb-binlog");
    cmd_push(cmd, "-lkdb-common");
    cmd_push(cmd, "-lkdb-crypto");
    cmd_push(cmd, "-lkdb-kfs");
    cmd_push(cmd, "-lkdb-net");
    cmd_push(cmd, "-lkdb-vv");
    cmd_push(cmd, "-o");
    cmd_push(cmd, "out/random-engine");
    cmd_push(cmd, "out/random/random-data.o");
    cmd_push(cmd, "out/random/random-engine.o");
    assert(cmd_execute(cmd));
    cmd_free(cmd);
    return 0;
}

void build_dns_engine(void) {
    cc("dns/dns-data.c", "out/dns/dns-data.o");
    cc("dns/dns-engine.c", "out/dns/dns-engine.o");
}

int link_dns_engine(void) {
    Command *cmd = cmd_new();
    cmd_push(cmd, "cc");
    cmd_push(cmd, "-Lout");
    cmd_push(cmd, "-lm");
    cmd_push(cmd, "-lz");
    cmd_push(cmd, "-lcrypto");
    cmd_push(cmd, "-lkdb-binlog");
    cmd_push(cmd, "-lkdb-common");
    cmd_push(cmd, "-lkdb-crypto");
    cmd_push(cmd, "-lkdb-kfs");
    cmd_push(cmd, "-lkdb-net");
    cmd_push(cmd, "-lkdb-vv");
    cmd_push(cmd, "-o");
    cmd_push(cmd, "out/dns-engine");
    cmd_push(cmd, "out/dns/dns-data.o");
    cmd_push(cmd, "out/dns/dns-engine.o");
    assert(cmd_execute(cmd));
    cmd_free(cmd);
    return 0;
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

    for (int i = 0; i < MAX_THREADS; i++) {
        pthread_create(&thread_pool[i], 0, queue_worker, 0);
    }

    build_kdb_common();
    build_kdb_vv();
    build_kdb_crypto();
    build_kdb_net();
    build_kdb_binlog();
    build_kdb_kfs();
    build_random_engine();
    build_dns_engine();

    while (global_queue.size > 0 && !fail) {
        usleep(1000);
    }
    if (fail) {
        ERROR("Compilation aborted");
        queue_shutdown();
        return -1;
    }
    queue_shutdown();

    assert(link_kdb_common());
    assert(link_kdb_vv());
    assert(link_kdb_crypto());
    assert(link_kdb_net());
    assert(link_kdb_binlog());
    assert(link_kdb_kfs());
    assert(link_random_engine());
    assert(link_dns_engine());

    INFO("Done");
    return 0;
}
