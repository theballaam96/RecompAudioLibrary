typedef struct CustomResampler {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    void* unk14;
    f32 unk18;
    s32 unk1C;
    f32 unk20;
    s32 unk24;
    void* unk28;
    s16 unk2C;
    s16 unk2E;
    s16 unk30;
    s16 unk32;
} CustomResampler;

typedef struct CustomPVoice_unk70 {
    u8 pad0[0xB8];
} CustomPVoice_unk70;

typedef struct CustomPVoice_unk7C_unkC {
    u8 pad0[0x88];
    s32 unk88;
} CustomPVoice_unk7C_unkC;

typedef struct CustomPVoice_unk7C CustomPVoice_unk7C;
struct CustomPVoice_unk7C {
    CustomPVoice_unk7C *unk0;
    s32 unk4;
    s16 unk8;
    s16 unkA;
    union {
        struct {
            s16 unkC;
            s16 unkE;
        };
        CustomPVoice_unk7C_unkC *unkC_ptr;
        f32 unkC_f32;
        s32 unkC_s32;
    };
    union {
        struct {
            s16 unk10;
            u8 unk12;
            u8 unk13;
        };
        s32 unk10_s32;
    };
    u8 unk14;
    u8 unk15;
    u8 pad16[0x18 - 0x16];
    f32 unk18;
    s32 unk1C;
    void *unk20;
};

typedef struct CustomPVoice_unk20_unkC {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
} CustomPVoice_unk20_unkC;

typedef struct CustomPVoice_unk20_unk10 {
    s32 unk0;
    s32 unk4;
} CustomPVoice_unk20_unk10;

typedef struct CustomPVoice_unk20 {
    s32 unk0;
    s32 unk4;
    u8 unk8;
    u8 pad9[0xC - 0x9];
    CustomPVoice_unk20_unkC *unkC;
    union {
        CustomPVoice_unk20_unk10 *unk10;
        s32 unk10_s32;
    };
} CustomPVoice_unk20;

typedef s32   (*CustomPVoice_unk28)(s32, s32, CustomResampler *);

typedef struct CustomPVoice {
    ALLink node;
    ALVoice *vvoice;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    CustomPVoice_unk20 *unk20;
    s32 unk24;
    CustomPVoice_unk28 unk28;
    CustomResampler resampler;
    s16 unk60;
    s16 unk62;
    u16 unk64;
    s16 unk66;
    s16 unk68;
    u16 unk6A;
    s16 unk6C;
    s16 unk6E;
    union {
        CustomPVoice_unk70 *unk70;
        s32 unk70_s32;
    };
    s32 unk74;
    s32 unk78;
    CustomPVoice_unk7C *unk7C;
    s32 *unk80;
    s32 unk84;
    s32 unk88;
    u8 unk8C;
    u8 pad8D[0x90 - 0x8D];
    s16 unk90;
    s16 unk92;
    u8 pad94[0x98 - 0x94];
    s32 unk98;
    u8 pad9C[0xB8 - 0x9C];
    s32 unkB8;
    s32 unkBC;
} CustomPVoice;