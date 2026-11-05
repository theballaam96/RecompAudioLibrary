#include "modding.h"
#include "ultra64.h"
#include "recompconfig.h"
#include "recompdata.h"
#include "recompui.h"
#include "recompui_event_structs.h"
#include "rt64_extended_gbi.h"
#include "libaudio.h"
#include "donk_audio.h"
#include "enums.h"
#include "common_structs.h"

extern ALBank *D_global_asm_8076D1F8;
extern ALBank *D_global_asm_8076D1FC;
void *_malloc(s32);
void func_global_asm_8061134C(void *);

#define AUDIO_ROM_FAKE_BASE 0x40000000
u32 allocated_base = AUDIO_ROM_FAKE_BASE;

typedef struct NewAudioSample NewAudioSample;
struct NewAudioSample {
    u8 *sample;
    f32 sample_length; // In seconds
    u32 sample_size; // in bytes
    u8 bank; // 0 = OST, 1 = SFX
    u8 inst;
    u16 snd;
    u32 base_start; // DO NOT PRE-POPULATE
    u32 base_end; // DO NOT PRE-POPULATE
    NewAudioSample *prev; // DO NOT PRE-POPULATE
    NewAudioSample *next; // DO NOT PRE-POPULATE
};

u32 secToEnvelopeTime(f32 sec) {
    if (sec < 0.0f) {
        return 0;
    }
    return (u32)(sec * 1000000.0f);
}

NewAudioSample *sample_chain = NULL;

void pushToSampleChain(NewAudioSample *smp) {
    NewAudioSample *curr;
    NewAudioSample *prev;

    if (!sample_chain) {
        sample_chain = smp;
        return;
    }
    curr = sample_chain;
    while (curr) {
        if (!curr->next) {
            curr->next = smp;
            smp->prev = curr;
            return;
        }
        curr = curr->next;
    }
}

void allocateBase(NewAudioSample *smp) {
    smp->base_start = allocated_base;
    smp->base_end = smp->base_start + smp->sample_size;
    allocated_base = smp->base_end;
}

RECOMP_EXPORT void ReplaceSample(NewAudioSample *smp) {
    ALBank *bnk;
    ALSound *snd;
    ALWaveTable *wt;
    ALEnvelope *env;

    pushToSampleChain(smp);
    allocateBase(smp);
    bnk = D_global_asm_8076D1F8;
    if (smp->bank == 0) {
        bnk = D_global_asm_8076D1FC;
    }
    // Replace bank entries
    snd = bnk->instArray[smp->inst]->soundArray[smp->snd];
    wt = snd->wavetable;
    env = snd->envelope;
    wt->base = (void*)smp->base_start;
    wt->len  = smp->sample_size;
    env->decayTime = secToEnvelopeTime(smp->sample_length);
}

s32 getCustomBuffer(u32 addr) {
    u8 *buffer;
    u32 i;
    u32 start;
    u32 size;
    NewAudioSample *curr;

    if (addr < AUDIO_ROM_FAKE_BASE) {
        // Quick escape for non-custom sounds
        return NULL;
    }
    curr = sample_chain;
    while (curr) {
        if ((addr >= curr->base_start) && (addr < curr->base_end)) {
            start = addr - curr->base_start;
            size = curr->sample_size - start;
            buffer = _malloc(size);
            for (i = 0; i < size; i++) {
                buffer[i] = curr->sample[start + i];
            }
            func_global_asm_8061134C(buffer);
            return (s32)&buffer[0];
        }
        curr = curr->next;
    }
    return NULL;
}

RECOMP_PATCH Acmd *func_global_asm_80740A90(Acmd *arg0, CustomPVoice *arg1, s32 arg2, s32 arg3, s16 arg4, s16 arg5, s32 arg6) {
    s32 sp2C;
    s32 sp28;
    Acmd *sp24;
    Acmd *sp20;
    Acmd *sp1C;

    if (arg3 > 0) {
        sp28 = getCustomBuffer(arg1->resampler.unk10);
        if (sp28 == 0) {
            sp28 = arg1->unk28(arg1->resampler.unk10, arg3, (CustomResampler *)arg1->resampler.unk0);
        }
        sp2C = sp28 & 7;
        arg3 += sp2C;
        sp24 = arg0++;
        sp24->words.w0 = ((((arg3 - (arg3 & 7)) + 8) & 0xFFF) << 0xC) | 0x04000000 | (arg5 & 0xFFF);
        sp24->words.w1 = (s32) (sp28 - sp2C);
        ;
    } else {
        sp2C = 0;
    }
    if (arg6 & 2) {
        sp20 = arg0++;
        sp20->words.w0 = 0x0F000000;
        sp20->words.w1 = (s32) (arg1->unk10 & 0x1FFFFFFF);
    }
    sp1C = arg0++;
    sp1C->words.w0 = (arg1->unkC & 0x1FFFFFFF & 0xFFFFFF) | 0x01000000;
    sp1C->words.w1 = (s32) (((arg6 & 0xF) << 0x1C) | (((arg2 * 2) & 0xFFF) << 0x10) | ((sp2C & 0xF) << 0xC) | (arg4 & 0xFFF));
    arg1->resampler.unkC = 0;
    return arg0;
}