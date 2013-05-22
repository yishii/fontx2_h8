#ifndef _FONTDRV_H_
#define _FONTDRV_H_

/*
  �����t�H���g�̃w�b�_�[�\����
*/
typedef struct {
    char Identifier[6];			// "FONTX2"�Ƃ��������񂪓���
    char FontName[8];			// Font��(8����)
    unsigned char XSize;		// �t�H���g�̉���
    unsigned char YSize;		// �t�H���g�̍���
    unsigned char CodeType;		// 0x01
    unsigned char Tnum;			// �����R�[�h�e�[�u���̃G���g���� 0x58
    struct {
		unsigned short Start;	// �̈�̎n�܂�̕����R�[�h
		unsigned short End;		// �̈�̏I���̕����R�[�h
    } Block[];					// Tnum����
} FontX_Kanji;

/*
  �A�X�L�[�t�H���g�̃w�b�_�[�\����
*/
typedef struct {
    char Identifier[6];			// "FONTX2"�Ƃ��������񂪓���
    char FontName[8];			// Font��(8����)
    unsigned char XSize;		// �t�H���g�̉���
    unsigned char YSize;		// �t�H���g�̍���
    unsigned char CodeType;		// 0x01
} FontX_Ascii;

#define	UNKNOWN_CHAR_REPLACE_KANJI	0x81a0	// �S�p�̑Ή����镶�����Ȃ��ꍇ�̒u����������(��)
#define	UNKNOWN_CHAR_REPLACE_ASCII	'_'		// ���p�̑Ή����镶�����Ȃ��ꍇ�̒u����������(_)

extern unsigned char* GetFontPtr_Kanji(unsigned short );
extern unsigned char* GetFontPtr_Ascii(unsigned char );
extern void init_fontdrv(void);


#endif /* _FONTDRV_H_ */
