// brute_state6.c
// Windows: cl /O2 brute_state6.c
// Linux/Mac: gcc -O3 -o brute_state6 brute_state6.c
#include <stdio.h>
#include <stdint.h>
#include <string.h>

static inline uint32_t ror32(uint32_t val, int n) {
    return (val >> n) | (val << (32 - n));
}

static void round_function(const uint32_t *state, uint32_t *rr) {
    uint32_t r12=state[0], r15=state[1], r13=state[2], r11=state[3];
    uint32_t r2=state[4], r3=state[5], r14=state[6], r16=state[7];
    uint32_t r5=state[8], r17=state[9], r4=state[10], r7=state[11];
    uint32_t r6=state[12], r19=state[13], r21=state[14], r20=state[15];
    uint32_t t, tmp4, tmp12, tmp5, tmp15, tmp2;

    for (int i = 0; i < 4; i++) {
        t=r12+r11; r13^=ror32(t,25);
        t=r16+r14; r3^=ror32(t,25);
        t=r4+r17;  r5^=ror32(t,25);
        t=r19+r6;  r20^=ror32(t,25);
        t=r13+r11; r15^=ror32(t,23);
        t=r3+r14;  r2^=ror32(t,23);
        t=r5+r17;  r7^=ror32(t,23);
        t=r20+r6;  r21^=ror32(t,23);
        t=r15+r13; r12^=ror32(t,19);
        t=r2+r3;   r16^=ror32(t,19);
        t=r7+r5;   r4^=ror32(t,19);
        t=r21+r20; r19^=ror32(t,19);
        t=r12+r15; r11^=ror32(t,14);
        t=r16+r2;  r14^=ror32(t,14);
        t=r4+r7;   r17^=ror32(t,14);
        t=r19+r21; r6^=ror32(t,14);
        t=r20+r11; r16^=ror32(t,25);
        t=r14+r13; r4^=ror32(t,25);
        t=r17+r3;  r19^=ror32(t,25);
        t=r6+r5;   r12^=ror32(t,25);
        t=r16+r11; r7^=ror32(t,23);
        t=r4+r14;  r21^=ror32(t,23);
        t=r19+r17; r15^=ror32(t,23);
        t=r12+r6;  r2^=ror32(t,23);
        t=r7+r16;  r20^=ror32(t,19);
        tmp4=r21+r4; r13^=ror32(tmp4,19);
        tmp4=r15+r19; tmp12=r2+r12;
        r3^=ror32(tmp4,19); r5^=ror32(tmp12,19);
        tmp4=r20+r7; tmp5=r13+r21;
        tmp15=r3+r15; tmp2=r5+r2;
        r11^=ror32(tmp4,14); r14^=ror32(tmp5,14);
        r17^=ror32(tmp15,14); r6^=ror32(tmp2,14);
    }

    rr[0]=r12; rr[1]=r15; rr[2]=r13; rr[3]=r11; rr[4]=r2;
}

int main() {
    // Sample 0
    uint32_t state_base[16] = {
        0xc6096c57, 0xc0d4865f, 0x77c0d486, 0x865f1701, 0xd4865f17,
        0x37a6c743, 0x00000000, 0x6ca444ef, 0x00000001, 0x00000000,
        0x694ce16a, 0x5f1701d9, 0x77c0d486, 0xc0d4865f, 0xd4865f17, 0xaf511ca1
    };
    uint32_t target[5] = {0xbdd90000, 0xbebbc2b0, 0x88dc5c1b, 0x12615b3a, 0x37b89db9};

    uint32_t state[16];
    uint32_t rr[5];

    printf("Brute-forcing state[6] with state[7]=0x6CA444EF, state[8]=1...\n");
    fflush(stdout);

    for (uint64_t s6 = 0; s6 <= 0xFFFFFFFF; s6++) {
        memcpy(state, state_base, sizeof(state));
        state[6] = (uint32_t)s6;

        round_function(state, rr);

        uint32_t o0 = state[0] + rr[0];
        if (o0 != target[0]) continue;

        uint32_t o1 = state[1] + rr[1];
        if (o1 != target[1]) continue;

        uint32_t o2 = state[2] + rr[2];
        uint32_t o3 = state[3] + rr[3];
        uint32_t o4 = state[4] + rr[4];
        if (o2 == target[2] && o3 == target[3] && o4 == target[4]) {
            printf("FOUND! state[6] = 0x%08x\n", (uint32_t)s6);
            fflush(stdout);
        }

        if ((s6 & 0x0FFFFFFF) == 0) {
            printf("Progress: %lu / 4294967296\n", (unsigned long)s6);
            fflush(stdout);
        }
    }

    printf("Search complete.\n");
    return 0;
}
