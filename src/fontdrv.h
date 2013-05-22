#ifndef _FONTDRV_H_
#define _FONTDRV_H_

/*
  漢字フォントのヘッダー構造体
*/
typedef struct {
    char Identifier[6];			// "FONTX2"という文字列が入る
    char FontName[8];			// Font名(8文字)
    unsigned char XSize;		// フォントの横幅
    unsigned char YSize;		// フォントの高さ
    unsigned char CodeType;		// 0x01
    unsigned char Tnum;			// 文字コードテーブルのエントリ数 0x58
    struct {
		unsigned short Start;	// 領域の始まりの文字コード
		unsigned short End;		// 領域の終わりの文字コード
    } Block[];					// Tnum個続く
} FontX_Kanji;

/*
  アスキーフォントのヘッダー構造体
*/
typedef struct {
    char Identifier[6];			// "FONTX2"という文字列が入る
    char FontName[8];			// Font名(8文字)
    unsigned char XSize;		// フォントの横幅
    unsigned char YSize;		// フォントの高さ
    unsigned char CodeType;		// 0x01
} FontX_Ascii;

#define	UNKNOWN_CHAR_REPLACE_KANJI	0x81a0	// 全角の対応する文字がない場合の置き換え文字(□)
#define	UNKNOWN_CHAR_REPLACE_ASCII	'_'		// 半角の対応する文字がない場合の置き換え文字(_)

extern unsigned char* GetFontPtr_Kanji(unsigned short );
extern unsigned char* GetFontPtr_Ascii(unsigned char );
extern void init_fontdrv(void);


#endif /* _FONTDRV_H_ */
