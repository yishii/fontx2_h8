/**********************************************************************************
  Module       :
  Coded        : Yasuhiro ISHII
  Note         : FONTX2 ドライバ
  Version      :
  Last updated :
**********************************************************************************/

#include <h8/300h/3069S.H>
#include <t_services.h>
#include <common.h>
#include <kernel_id.h>
#include "fontdata.h"
#include "fontdrv.h"

/*
  漢字フォントに関する定義
 */
// フォントサイズ(X)
#define	FONT_SIZE_X_KANJI			(((FontX_Kanji*)font_table_kanji)->XSize)
// フォントサイズ(Y)
#define	FONT_SIZE_Y_KANJI			(((FontX_Kanji*)font_table_kanji)->YSize)
// 1フォントのサイズ(Bytes)
#define FONT_SIZE_KANJI				((FONT_SIZE_X_KANJI >> 3) * FONT_SIZE_Y_KANJI)
// 文字コードタイプ
#define	FONT_CODE_TYPE_KANJI		(((FontX_Kanji*)font_table_kanji)->CodeType)
// 文字参照テーブル数
#define	FONT_CODE_TNUM_KANJI		(((FontX_Kanji*)font_table_kanji)->Tnum)
#define FONT_BLOCK_S_KANJI(n)		(SWITCH_ENDIAN_US((((FontX_Kanji*)font_table_kanji)->Block[n]).Start))
#define FONT_BLOCK_E_KANJI(n)		(SWITCH_ENDIAN_US((((FontX_Kanji*)font_table_kanji)->Block[n]).End))

/*
  ASCIIフォントに関する定義
 */
// フォントサイズ(X)
#define	FONT_SIZE_X_ASCII			(((FontX_Ascii*)font_table_ascii)->XSize)
// フォントサイズ(Y)
#define	FONT_SIZE_Y_ASCII			(((FontX_Ascii*)font_table_ascii)->YSize)
// 1フォントのサイズ(Bytes)
#define FONT_SIZE_ASCII				((FONT_SIZE_X_ASCII >> 3) * FONT_SIZE_Y_ASCII)
// 文字コードタイプ
#define	FONT_CODE_TYPE_ASCII		(((FontX_Ascii*)font_table_ascii)->CodeType)
// 文字データスタートアドレス
#define	FONT_DATA_START_ADDR		(((unsigned char*)font_table_ascii) + 17)

static unsigned char* ucpFontDataStart_Kanji;
static unsigned short usFontSearchTable_Kanji[256];

void init_fontdrv(void);
unsigned char* GetFontPtr_Ascii(unsigned char );
unsigned char* GetFontPtr_Kanji(unsigned short );
static int SearchFontAreaNo_Kanji(unsigned short );


/*
  FONTX2 フォントドライバの初期化処理

  内容:
		フォント実データへのポインタ生成
		フォントテーブルデータを参照し、フォントサーチ用のテーブルを生成
*/
void init_fontdrv(void){
	int i;
	
	ucpFontDataStart_Kanji = (unsigned char*)(font_table_kanji + FONT_CODE_TNUM_KANJI * 4 + 18);

	usFontSearchTable_Kanji[0] = 0;

	for (i=1;i<FONT_CODE_TNUM_KANJI;i++){
		usFontSearchTable_Kanji[i] = (FONT_BLOCK_E_KANJI(i-1) - FONT_BLOCK_S_KANJI(i-1) + 1) +
			usFontSearchTable_Kanji[i-1];
	}
}

/*
  指定したSHIFT-JIS文字コードから、FONTX2文字コードのテーブル
  より文字データの存在するエリアの番号を得る
*/
static int SearchFontAreaNo_Kanji(unsigned short code){
	int i;
	int ret;

	for (i=0;i<FONT_CODE_TNUM_KANJI;i++){
		if((FONT_BLOCK_S_KANJI(i) <= code) && (FONT_BLOCK_E_KANJI(i) >= code)){
			return(i);
		}
	}
	return (-1);
}

/*
  指定したAsciiコードから、該当するフォントデータのビットマップエリア
  へのポインタを得る
*/
unsigned char* GetFontPtr_Ascii(unsigned char code){
	if (code > 0x7f){
		code = UNKNOWN_CHAR_REPLACE_ASCII;
	}
	return (FONT_DATA_START_ADDR + code * FONT_SIZE_ASCII);
}

/*
  指定したSHIFT-JIS文字コードから、該当するフォントデータのビットマップ
  エリアへのポインタを得る
*/
unsigned char* GetFontPtr_Kanji(unsigned short code){
	int FontArea;
	unsigned short CodeOffset;
	unsigned long FontStartAddr;

	// 指定文字コードの属するフォントのエリアを算出
	FontArea = SearchFontAreaNo_Kanji(code);
//	printf("FontArea = %d\n",FontArea);

	// 文字コードに該当するフォントが見つからない場合
	if (FontArea == -1){
//		printf("Unknown code!");
		// 不明なので置き換え用のコードに挿げ替え
		code = UNKNOWN_CHAR_REPLACE_KANJI;
		FontArea = SearchFontAreaNo_Kanji(code);
	}

	// フォントのブロック先頭からのオフセット量算出
	CodeOffset = code - FONT_BLOCK_S_KANJI(FontArea);

	// フォントエリア先頭からの指定フォントの開始オフセット量算出
	FontStartAddr = (usFontSearchTable_Kanji[FontArea] + CodeOffset) * FONT_SIZE_KANJI;
	
//	printf("code=%04x,Area=%d,Table[Area]=%04x,CodeOffset=%04x\n",
//		   code,FontArea,usFontSearchTable_Kanji[FontArea],CodeOffset);

	// フォントスタート実アドレス + オフセット量の和を返す
	return((unsigned char*)(ucpFontDataStart_Kanji + FontStartAddr));	
}


