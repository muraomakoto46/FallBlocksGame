#include<stdio.h>
#include<string.h>
#include<stdlib.h>// rand(),system()
#include<time.h> // clock()
#include <conio.h>
#include <ctype.h>
#define WIDTH 13
#define HEIGHT 19
#define CEILING 2
#define LEFTFRAME 2
#define STARTPOSITION_X 6
#define STARTPOSITION_Y 3

/*
これは
Tentative-Enclosing Tracks Refused Influencing System
のVersion2
である。

これは、
void moveToLRD(void);
の、4号機が未完成のうちに
回転機能と
a[][]への確定機能と
次のブロックを出現させる機能
e[][]にb[][]とa[][]を反映
e[][]を表示
まで付加したものだ。

一号機では、
最も下段にブロックが載っているときに、
右または左に移動しようとすると、跳ねてしまう、
不都合な動作が見つかった。
二号機ではこれを改善させる。
そもそもブロックが枠とぶつかってしまうまで下に移動
させてはいけないだろう。
*/
/*枠の太さ
本当に1文字分の太さで足りるのだろうか。
*/
/*LRD2号機では跳ねないように改善された。
改善されたが、新たな問題が生じた。
横5マス縦5マス 全部1で埋め尽くされたものは
大丈夫だけど、
11
11
のような  小さなものや
さまざまな形状に対応できないね。
00000
11000
01100
00000
*/
/*
LRD3号機はそこを改善するのが目的で作られた。
その結果、改善された。
a[][]の要素数をb[][]の要素数よりも
縦方向に+4多くした、横方向にも+4多くした。
しかし、下移動を指示したうえで、
右移動を指示したら、
下へ移動したのち、右斜め下方向へ移動する異常が
見つかった。
LRD4号機ではそのバグを修正するために用意された。
*/
/*
LRDTCNE.cは
l leftward transference
r rightward tranference
d downward transference
t turn the block
c commitA
n next block incoming
e erase a line
を 行えるようにすることを目的として作られた。
その結果、以下のようになった。
l 完成 問題なく動作する。
r 完成 問題なく動作する。
d これも問題なく動作する。
t 右端や左端に来た時 ループするバグがある
c 問題なく動作する
n 行列を作ることには成功している。でも表示されない。
e 問題なく動作する。

lrdtcne2では 回転を問題なく動作させることを目的として
作られる。
改善策としては、b[][]の要素数をa[][]と同じにすること。
いや、そんな単純なものじゃない。
*/
int a[HEIGHT+4][WIDTH+4]={0};
int b[HEIGHT+4][WIDTH]={0};
int c[ 5][ 5]={
	{0,0,0,0,0},
	{0,0,0,1,0},
	{0,0,1,1,0},
	{0,0,1,0,0},
	{0,0,0,0,0}
};
int d[ 5][ 5]={
	{0,0,0,0,0},
	{0,0,0,0,0},
	{0,0,0,0,0},
	{0,0,0,0,0},
	{0,0,0,0,0}
};
int swapC[ 5][ 5]={
	{0,0,0,0,0},
	{0,0,0,0,0},
	{0,0,0,0,0},
	{0,0,0,0,0},
	{0,0,0,0,0}
};
//e[][]とは プレーヤーに表示させる台紙のようなものだ。
//台紙はでっかく広くなければならない。
//Javaのswingのコンテナに似ているかもね。
//javax.swing.*して container cnt =getContentPane();
// cnt.add(pn[0]);のような。
//詳しいことはやさしいJAVA活用編でも見てください。
//実際は横には右のスペースは15マス程度しか使わないけど
//メモリの範囲オーバーが怖いから多めにとっておく。
int e[HEIGHT+4][WIDTH+46]={0};
/*
e[][]のWIDTH+4をWIDTH+8にしようとおもう。
将来大きなブロックエリアの右隣りに数値や
NEXT ブロックの画像を張り付けるためのもの
*/
//c[2][2]がb[][]ではどこにあるのかを示すためのもの
int block_core_x=STARTPOSITION_X;
int block_core_y=STARTPOSITION_Y;

unsigned int point=0;

typedef struct block{
	int array[5][5];
	char color;//ブロックの種類によっては色が違う。 
	int id;
}BLOCK;
BLOCK nextBlock[3];
//え。3手先まで表示させるつもり？作るの、しんどいなぁ。
BLOCK type[8]={
{
	{
		{0,0,0,0,0},
		{0,0,1,0,0},
		{0,1,1,1,0},
		{0,0,0,0,0},
		{0,0,0,0,0}
	},
	'M',//Maron
	0// ID 0
},


{
	{
		{0,0,1,0,0},
		{0,0,1,0,0},
		{0,0,1,0,0},
		{0,0,1,0,0},
		{0,0,0,0,0}
	},
	'P',//Peach
	1 //ID 1
},

{
	{
		{0,0,0,0,0},
		{0,1,0,0,0},
		{0,1,0,0,0},
		{0,1,1,0,0},
		{0,0,0,0,0}
	},
	'C',//Cocoa
	2 // ID 2
},

{
	{
		{0,0,0,0,0},
		{0,0,0,1,0},
		{0,0,0,1,0},
		{0,0,1,1,0},
		{0,0,0,0,0}
	},
	'S',3//Strawberry
},

{
	{
		{0,0,0,0,0},
		{0,1,0,0,0},
		{0,1,1,0,0},
		{0,0,1,0,0},
		{0,0,0,0,0}
	},
	'W',4//WhiteSuger
},

{
	{
		{0,0,0,0,0},
		{0,0,0,1,0},
		{0,0,1,1,0},
		{0,0,1,0,0},
		{0,0,0,0,0}
	},
	'G',5//GrapeFruits
},

{
	{
		{0,0,0,0,0},
		{0,1,1,0,0},
		{0,1,1,0,0},
		{0,0,0,0,0},
		{0,0,0,0,0}
	},
	'B',6//Banana
},

{
	{
		{0,0,0,0,0},
		{0,0,0,0,0},
		{0,0,0,0,0},
		{0,0,0,0,0},
		{0,0,0,0,0}
	},
	'N',7//None 一番最後はブロックなしでなければならない
}
};

void viewA(void);
void viewB(void);
void viewE(void);
void viewEwithSquare(void);
int moveToRight(void);//ブロックを右移動
int moveToLeft(void);//ブロックを左移動
int moveToDown(void);//ブロックを降下移動
int checkTheCeiling(void);
char input_char(void);
int AttemptMergeCwithB(void);//実際にB上ではどこにあるのか。
int AttemptMergeBwithA(char direction);//インターフェース
int AttemptMergeBwithADo(int anotherX,int anotherY,char direction);//実際に試みる
int refleshA(void);
int refleshB(void);
int refleshC(void);
int refleshD(void);
int refleshE(void);
void setFrameA(void);
void turn(void);
int mergeAandBtoE(void);
char isImplant(void);
int next(void);
int commitA(void);
int chooseNextBlock(void);
int chooseNextNextBlock(void);
int set_nextBlockBeforeGameStart();
int setNewBlockonC();
int resetBlockCoreXY(void);
int wait(void);
int wait2(void);
int erase(void);
int checkALine(int y);
int eraseALine(void);
int applyNextNextOnE(void);


/*関数の名前が少々長くてもコンパイラは
不満を述べない。*/
/*まさかこんなに関数やグローバル変数を沢山作ることになるとは
思っていなかった。
宣言だけで300行？ 7行のテトリスに比べると とても多いね。
*/
int main(){
	int flg=0;
	char ch=' ';
	int i=0;
	int noEchoCh=0;
	setFrameA();
	refleshB();
	set_nextBlockBeforeGameStart();
	next();
	AttemptMergeCwithB();
	//applyNextNextOnE();
	system("cls");
	printf("Tentative\nEnclosured\nTrack\nRefused\nInfluencing\nSystem\n");
	printf("制作:村尾 誠 c(^_^)v");
	ch = input_char();
	system("cls");
	do{
		mergeAandBtoE();
		viewEwithSquare();
		//printf("blockCoreX=%d\n",block_core_x);
		//printf("blockCoreY=%d\n",block_core_y);
		
		//scanf("%c",&ch);
			for(i=0;i<10;i++){
				if(kbhit()){
					noEchoCh=_getch(); //visual C++用のもの エコーなしで入力できる。 
					//ch=input_char();
					ch= noEchoCh;			
				}

				wait();
				
								





			}
		

		if(ch == 'L'||ch == 'l'||ch == 'G'||ch == 'g'||ch == ';'||ch == '*'){
			moveToLeft();
			ch = 'D';
		}
		
		if(ch == 'R'||ch == 'r'){
			moveToRight();
			ch = 'D';
		}
		
		if(ch == 'D'||ch == 'd'|| ch == ' '|| ch == '\0'){
			moveToDown();
		}
		if(ch == 'Z'||ch == 'z'){
			exit(0);
		}
		
		if(ch == 'T'||ch == 't'){
			turn();
			ch = 'D';
		}
		system("cls");
		/*
			ここで着床とNEXTブロック排出処理をする。
		
			着床したかどうかチェック
			char isImplant(void);
			↓
			int commitA(void);
			a[][]にブロックb[][]を上書きして確定
			↓
			NEXTブロック選定
			↓
			NEXT
		*/
		//枠の上まで 達していないかどうかのチェック
		//達していたらゲームオーバー
		
		if(isImplant() == 'Y'){//Yes,着床している。
			//printf("isImprant\n");
			
			next();
			
			
		}else{
			;
			//moveToDown();
		}
		
		erase();
		erase();
		erase();
		//system("cls");
		//printf("↓viewB\n");
		//viewB();
		//printf("↓viewA\n");
		//viewA();
		
		
		
		//枠の上まで 達していないかどうかのチェック
		//達していたらゲームオーバー
		if(checkTheCeiling() !=0){
			//system("cls");
			printf("GAMEOVER!!\n");
			printf("(v)(>_<)(v)kanisan\n");
			break;
		}else{
			//moveToDown();
			
		}
	}while(1);
	return 0;
}
//next
int next(){
	//確定させる
	commitA();
	chooseNextNextBlock();
	setNewBlockonC();
	resetBlockCoreXY();
	refleshE();
	return 0;
}
//↓使用しません。
int chooseNextBlock(void){
	auto int number=0;
	srand( (unsigned)time( NULL ) );
	
	number = rand();
	number %=7;
	return number;
}
//↓ゲームスタート時にchooseNextNextBlock(void)を3回呼ぶだけの関数
int set_nextBlockBeforeGameStart(){
	chooseNextNextBlock();
	chooseNextNextBlock();
	chooseNextNextBlock();
	return 0;

}
int chooseNextNextBlock(void){
	auto int number=0;
	int i=0,j=0,k=0;
	time_t t;
	srand( (unsigned)time( &t /*NULL*/ ) );
	number = rand();
	number %=7;
	//[2]が[1]に移って、[1]が[0]に移って・・・
	//構造体だからこそ 二重ループなんか使わなくたって ごっそり 複写できるのだ。
	nextBlock[0]=nextBlock[1];
	nextBlock[1]=nextBlock[2];
	nextBlock[2] = type[number]; 
	for(i=0;i<3;i++){
		for(j=0;j<5;j++){
			for(k=0;k<5;k++){
				//printf("%d",nextBlock[i].array[j][k]);
			}
			//printf("↑i=%d\n",i);
		}
		//printf("\n");
	}
	return number;
}
int setNewBlockonC(){
	auto int x=0,y=0;
	refleshB();
	refleshC();
	refleshD();
	refleshswapC();
	//配列の配列の配列だとイメージしにくいやん。
	//だから構造体も使うんだ。分類番号○番の配列の配列
	//printf("----===setNewBlockOnC===---\n");
	for(y=0;y<5;y++){
		for(x=0;x<5;x++){
				//cast演算子
			c[y][x]=(int)nextBlock[0].array[y][x];
			//printf("%d,",c[y][x]);
		}
		//printf("\n");
	}
	return 0;
}
int resetBlockCoreXY(void){
	block_core_x=STARTPOSITION_X;
	block_core_y=STARTPOSITION_Y;
	return 0;
}
//turn
void turn(void){
	auto int x=0,y=0;
	auto int result1=0,result2;//回転動作も 移動させているものとみなす。
	//回転ができても、枠と重なってしまったら、元に戻さねばならん。
	//それだけでなく、b[][]の範囲の外に書き込ませないようにしなければなりません。
	refleshB();

	//まずはd[][]を リセットしてから。
	refleshD();
	for(y=0;y<5;y++){
		for(x=0;x<5;x++){
			//printf("%d",c[x][y]);
			//swapC[][]に退避
			swapC[y][x]=c[y][x];
		}
		//printf("\n");
	}
	//printf("\n");
	//回転しながらd[][]に貼って
	for(y=0;y<5;y++){
		for(x=0;x<5;x++){
			d[x][4-y]=c[y][x];
			//printf("%d",d[x][y]);
		}
		//printf("\n");
	}
	//そのうえでd[][]をc[][]に貼る。
	for(y=0;y<5;y++){
		for(x=0;x<5;x++){
			c[y][x]=d[y][x];
			//printf("%d",d[x][y]);
		}
		//printf("\n");
	}
	result1 = AttemptMergeCwithB();
	result2 = AttemptMergeBwithA('T');
	printf("result1=%d,result2=%d\n",result1,result2);
	//result が非０だった場合は 異常だから元に戻す。
	//逆回転操作は 未確認だが 若干の非可逆性があると思う。
	//それにすぐ上の二重for文と似たようなものを
	//また3回書くのは 面倒くさい。
	if(result1 != 0 || result2 != 0){
		for(y=0;y<5;y++){
			for(x=0;x<5;x++){
				c[y][x]=swapC[y][x];
			}
		}
		refleshB();
		AttemptMergeCwithB();
	}
}

//枠を描画
void setFrameA(void){
	auto int x=0,y=0;
	for(y=0;y<HEIGHT+4;y++){
		for(x=0;x<WIDTH+4;x++){
			if(y==0 || y==1 || 
				y==HEIGHT+2 || y==HEIGHT+3 || 
				x==0 || x==1 || 
				x==WIDTH+3  || x==WIDTH+2)
			{
				a[y][x]=-1;
			}
		}
	}
	//a[7][14]=1;
}

int moveToLeft(void){
	int result=0;
	
	refleshB();
	//bにcを重ね合わせてみる、
	AttemptMergeCwithB();
	//aの枠と重なっていないかをチェック
	result = AttemptMergeBwithA('L');
	if(result ==0 ){
		//あ、重なる…より一つ手前だぞ。って場合
		block_core_x--;
	}
	//問題なく うまくいけばreturn 0
	return result;
}

int moveToRight(void){
	int result=0;
	
	refleshB();
	
	//bにcを重ね合わせてみる、
	AttemptMergeCwithB();
	//aの枠と重なっていないかをチェック
	result = AttemptMergeBwithA('R');
	if( result == 0){
		//あ、重なってないぞ。って場合
		block_core_x++;
	}
	//問題なく うまくいけばreturn 0
	return result;
}

int moveToDown(void){
	int result=0;
	
	refleshB();
	
	//bにcを重ね合わせてみる、
	AttemptMergeCwithB();
	//aの枠と重なっていないかをチェック
	//チェックするだけなら併合する必要はない。
	//printf("Are you sure to down?\n");
	
	result = AttemptMergeBwithA('D');
			/*このAttemptMergeBwithA();の引数は、
				どの方向に今から1つ分ブロックを移動させるのかを
				指示するためのもの。
				'D'ならば 一つ下にずらして・・・
				つまり照準を a[3][2] に b[0][0]をあてはめるようにして
				併合することも試みる。
				*/
	if(result == 0){
		//あ、重なっていないぞ。って場合のみ
		block_core_y++;
		//block_core_x = 3;
		//↑これはmoveToDownのときに使える技だ
		//block_core_y=3; のようにね。
	}else{
		//確定処理をここに書く…べきではない。
	}
	//問題なく うまくいけばreturn 0
	//ブロックが、底か 既にa[][]にも配置済みのブロックに
	//ぶち当たったらしいなら return 非０;
	return result;
}

int checkTheCeiling(void){
	int result=0;
	
	refleshB();
	
	//bにcを重ね合わせてみる、
	AttemptMergeCwithB();
	//aの枠と重なっていないかをチェック
	//チェックするだけなら併合する必要はない。
	//printf("Are you sure to down?\n");
	
	result = AttemptMergeBwithA('U');
			/*このAttemptMergeBwithA();の引数は、
				どの方向に今から1つ分ブロックを移動させるのかを
				指示するためのもの。
				'D'ならば 一つ下にずらして・・・
				つまり照準を a[3][2] に b[0][0]をあてはめるようにして
				併合することも試みる。
				*/
	
	//問題なく うまくいけばreturn 0
	//ブロックが、天井に
	//ぶち当たったらしいなら return 非０;
	return result;
}


int refleshA(void){
	auto int x=0,y=0;
	for(y=0;y<HEIGHT+4;y++){
		for(x=0;x<WIDTH+4;x++){
			b[y][x]=0;
		}
	}
	return 0;
}

// b[][]を 0クリアする
int refleshB(void){
	auto int x=0,y=0;
	for(y=0;y<HEIGHT;y++){
		for(x=0;x<WIDTH;x++){
			b[y][x]=0;
		}
	}
	return 0;
}

int refleshC(void){
	auto int x=0,y=0;
	for(y=0;y<5;y++){
		for(x=0;x<5;x++){
			c[y][x]=0;
		}
	}
	return 0;
}

int refleshD(void){
	auto int x=0,y=0;
	for(y=0;y<5;y++){
		for(x=0;x<5;x++){
			d[y][x]=0;
		}
	}
	return 0;
}

// e[][]を 0クリアする
int refleshE(void){
	auto int x=0,y=0;
	for(y=0;y<HEIGHT+4;y++){
		for(x=0;x<WIDTH+4;x++){
			e[y][x]=0;
		}
	}
	return 0;
}

int refleshswapC(void){
	auto int x=0,y=0;
	for(y=0;y<5;y++){
		for(x=0;x<5;x++){
			swapC[y][x]=0;
		}
	}
	return 0;
}
//c[][]を b[][]の 狙った所に貼り付けようと試みる
/*注意！！ b[][]の範囲を超える位置に
何かを書きこもうとしてはいけません。
VisualStudioではそれでも なんとか動作させようとするけど、
絶対に 狙った通りには 動かない。
*/
int AttemptMergeCwithB(void){
	auto int cx=0,cy=0;
	auto int bx=0,by=0;
	auto int YoutryedToPutOnOverThere=0;
	refleshB();
	for(cy=0,by=block_core_y-2;cy<5;cy++,by++){
		for(cx=0,bx=block_core_x-2;cx<5;cx++,bx++){
			if(bx>=WIDTH || bx <= -1 || by >HEIGHT){//要素番号がマイナス1やマイナス2なんてありえない。
				YoutryedToPutOnOverThere++;
			}else{
				b[by][bx]=c[cy][cx];
			}
		}
	}
	
	return YoutryedToPutOnOverThere;//0が帰らなければエラー
}

// a[][] と b[][] を併合するのだが、a[][]のほうが二周り大きいサイズだ。
// a[][]の(x,y)=(2,2)に b[][]の(x,y)=(0,0)を貼り合わせる。
/* インターフェース
	コンピュータは数値で指示できるが、人は言葉で指示する。
	これはコンピュータと人との違いを埋め合わせるためのものだ。
*/
int AttemptMergeBwithA(char direction)
{
	//char direction は  L R D Uの3種類がある。
	//Dだったら一つ下も調べる。いや、いきなり一つ下を調べちゃいなよ。
	//結構複雑になってきましたねぇ。昔の自分だったら途中で休戦を要求しているところだよ。
	auto int anotherX = 2;
	auto int anotherY = 2;
	auto int correspondence=0;
	if(direction == 'L'){
		correspondence=AttemptMergeBwithADo(anotherX-1,anotherY,direction);
	}else if(direction == 'R'){
		correspondence=AttemptMergeBwithADo(anotherX+1,anotherY,direction);
	}else if(direction == 'D'){
		correspondence=AttemptMergeBwithADo(anotherX,anotherY+1,direction);
	}else if(direction == 'T'){
		//Turn だけは 試みてだめだったとしてもゲームオーバーにしない。
		correspondence=AttemptMergeBwithADo(anotherX,anotherY,direction);
	}else if(direction == 'U'){//ゲームオーバー？
		correspondence=AttemptMergeBwithADo(anotherX,anotherY-1,direction);
	}else{
		;
	}
	return correspondence;
}
int AttemptMergeBwithADo(int anotherX,int anotherY,char direction){
	auto int ax=0,ay=0,bx=0,by=0;
	auto int correspondence=0;
	
	for(ay=anotherY,by=0;by<HEIGHT;ay++,by++){ //初期設定はy=1ではだめ
		for(ax=anotherX,bx=0;bx<WIDTH;ax++,bx++){//初期設定がx=1ではだめだし、x<WIDTH-1も×
			if(a[ay][ax]==1 && b[by][bx]==1 && correspondence==0 /* && (direction != 'U')*/){
				correspondence=1;//既に確定済みブロックに激突する恐れあり。
				break;
			}
			
			if(a[ay][ax]==-1 && b[by][bx]==1  && correspondence==0 ){
				correspondence=-1;//壁に激突する恐れあり。
				// -1を入れたがそのままループを続けていては
				// correspondence++ して
				// んで、結局correspondenceが0になって
				// 戻り値が0や+3になりかねない。
				break;
			}
			//printf("ax=%d,ay=%d,bx=%d,by=%d\n",ax,ay,bx,by);
		}
		if(correspondence !=0 ){
			break;
		}
	}
	
	return correspondence;
	//正常なときは0が戻される
	//異常が見つかれば、非０【ひ ゼロ】 が 戻される。
}
//↓使用一時中止
int applyNextNextOnE(void){
	auto int bkx=0,bky=4,i=0,ex=0,ey=0;
	for(i=0;i<3;i++){
		for(ey=bky;ey<bky+5;ey++){
			for(ex=18;ex<18+5;ex++){
				e[ey][ex]=nextBlock[i].array[bky][bkx];
				printf("a %d",e[ey][ex]);
			}
			printf("bbb\n");
		}
		bky+=6;
		printf("bky=%d,\n",bky);
	}
	
	return 0;
}
char isImplant(void){
	auto char result=' ';
	auto int correspondence;
	correspondence = AttemptMergeBwithA('D');
	if(correspondence!=0){
		result = 'Y';
		//Yes,The Block has been just Implanted.
	}else{
		result = 'N';
		//No,The Block can be fallen down more than here.
	}
	return result;
}
//少し大きめの負荷をかける。
int wait(void){
	auto unsigned int pressure=1000;//1000
	double aaa=0.1;
	for(pressure=1000000;pressure > 0;pressure--){
		aaa+=aaa/10;
		rand();// ただ 負荷をかけるだけのためのもの
	}
	/*このやりかただとハードウェアによって
	待ち時間がばらばらになる。
	clock()関数を使うべきだ。*/
	return 0;
}
/*
翔泳社の赤色のC言語辞典
176ページを参照してね。
*/
int wait2(void){
	clock_t start,finish;
	double duration=0.0;
	start = clock();
	do{
		finish = clock();
		duration = (double)(finish-start)/CLOCKS_PER_SEC;
	}while(duration<2.0);

	return 0;
}
//ゲーム画面を作るための準備
//
int mergeAandBtoE(void){
	/*薄い折り紙の上に もう一枚薄い折り紙で覆うところを想像してみてほしい。
	1、e[][]をリフレッシュする。
	2、a[][]をe[][]の上に重ね合わせる。
	3、そのあと、すぐに c[][]をb[][]に結合し、
	4、b[][]をe[][]に結合する
	*/
	auto int ay=0,ax=0;
	auto int bx=0,by=0;
	auto int ex=0,ey=0;


	//１、
	refleshE();
	/*
		自作関数のreturn 0;の利用例
		if(!refleshE()){
			printf("Eリフレッシュ完了!\n");
		}
		このように、バグ取りで利用できそうだ。
	*/

	//2、
	for(ay=0,ey=0;ay<HEIGHT+4;ay++,ey++){
		for(ax=0,ex=0;ax<WIDTH+4;ax++,ex++){
			e[ey][ex]=a[ay][ax];
		}
	}


	//3、
	refleshB();
	AttemptMergeCwithB();//CをBに結合

	//4、
	for(ey=2,by=0;ey<HEIGHT+4;ey++,by++){
		for(ex=2,bx=0;ex<WIDTH+4;ex++,bx++){
		/*↓のif文について
			2番の二重ループでぇ、e[][]にa[][]を書いたのに
			それらを全部無視してぇ、b[][]を上書きしちゃうとぉ、
			ブロックがあ、落下中のものしかぁ、ぬぁくなっちゃう～♪
			*/
			if(e[ey][ex]==0){
				e[ey][ex]=b[by][bx];
			}
		}
	}
	/*
		ここに NEXTブロックの絵を e[][]に貼り付けるソースコードを書くんだ。
	*/
	//printf("---===NEXTブロックの絵===- \n");
	for(ey=6,by=0;by<5;ey++,by++){
		for(ex=18,bx=0;bx<5;ex++,bx++){
			e[ey][ex]=nextBlock[1].array[by][bx];
			//printf("%d,",e[ey][ex]);
		}
		//printf("1ari\n");
	}
	
	for(ey=12,by=0;by<5;ey++,by++){
		for(ex=20,bx=0;bx<5;ex++,bx++){
			e[ey][ex]=nextBlock[2].array[by][bx];
			//printf("%d,",e[ey][ex]);
		}
		//printf("1ari\n");
	}
	return 0;
}


void viewA(void){
	auto int x=0,y=0;
	for(y=0;y<HEIGHT+4;y++){
		for(x=0;x<WIDTH+4;x++){
			if(a[y][x]==-1){
				//枠は2重だが表示するときは太いままそのまま
				//表示する必要はありませんから。
				//if((x!=0) && (x!=WIDTH+3)){
					printf("X");
				//}
			}else{
				printf("%d",a[y][x]);
			}
		}
		printf("\n");
	}
}
void viewB(void){
	auto int x=0,y=0;
	for(y=0;y<HEIGHT;y++){
		for(x=0;x<WIDTH;x++){
			printf("%d",b[y][x]);
		}
		printf("\n");
	}
}
void viewE(void){
	auto int x=0,y=0;
	for(y=0;y<HEIGHT+4;y++){
		for(x=0;x<WIDTH+4;x++){
			printf("%d",e[y][x]);
		}
		printf("\n");
	}
}
void viewEwithSquare(void){
	auto int x=0,y=0;
	printf("←:すＲ    Ｇき:→\n");
	printf(" ↓:Ｄし    Ｔか:Turn   Zつ:終了\n");
	printf("消去LINE数:%d\n",point);
	for(y=0;y<HEIGHT+4;y++){
		for(x=0;x<WIDTH+20;x++){
			if(e[y][x]==-1){
				printf("Ｘ");
			}
			if(e[y][x]==1){
				printf("■");
			}
			if(e[y][x]==0){
				if( y==2 && x<WIDTH+LEFTFRAME  ){
					printf("─");
				}else if(y==3 && x==19){
					printf("Ｎ");
				}else if(y==3 && x==20){
					printf("Ｅ");
				}else if(y==3 && x==21){
					printf("Ｘ");
				}else if(y==3 && x==22){
					printf("Ｔ");
				}else if(y==11 && x==21){
					printf("↑");
				}else{
					printf("  ");
				}
			}
		}
		printf("\n");
	}
	

}
/*このcommitA処理は
もうひとつ下に移動できない場合のみ
呼び出されるべきだ。
ブロックが fall down したときだけでなく
横滑りで ブロックの隙間に
落下中のブロックが差し込まれたときも
呼び出されなければならない。
*/
/*commitAは着床処理の一部分*/
int commitA(void){
	auto int ax=2,ay=2,bx=0,by=0;
	auto int result=0;
	
	for(ay=2,by=0;by<HEIGHT;ay++,by++){
		for(ax=2,bx=0;bx<WIDTH;ax++,bx++){
			if(a[ay][ax]==0 && b[by][bx]==1){
				a[ay][ax]=1;//aにブロックを描く
			}else{
				result++;
			}
		}
	}
	
	return result;
	//正常なときは0が戻される
	//異常が見つかれば、非０【ひ ゼロ】 が 戻される。
}
//枠は消してはいけない。
int erase(void){
	eraseALine();// 2012年5月10日 eraseALineはうまくいっている
	refleshE();
	mergeAandBtoE();
	return 0;
}

//a[][]において、とある一行が ブロックで埋まっているかどうか。
//yには どの行を調べるのかを指定する。具体的に何行目を？
int checkALine(int y){
	auto int x=0,total=0;
	
	for(x=0;x<WIDTH+LEFTFRAME;x++){
		if(a[y][x]==1){
			total++;
		}
	}
	return total;
}
//なんだかんだでいけるやん。
int eraseALine(void){
	auto int x=0,y=0,dx=0,dy=0,result=0,count=0;
	for(y=HEIGHT+CEILING;y>CEILING;y--){
		result=checkALine(y);
		//printf("result=%d\n",result);
		if(result == WIDTH){
//resultがWIDTHと同じならば1行がブロックで埋まっているといえる。
			for(dy=y;dy>CEILING;dy--){
				for(dx=0;dx<WIDTH+4;dx++){
					a[dy][dx]=a[dy-1][dx];
				}
			}
			count++;
			point++;
			
		}
	}


	
	return count;//消した数
}

char input_char(void){
	auto char str[255];
	auto char mozi=0;
	auto int hantei=0;
	auto int n=255;
	fgets(str,n,stdin);
	if(str[strlen(str)-1]=='\n'){
		str[strlen(str)-1]='\0';
	}
	hantei=sscanf(str,"%c",&mozi);
	//printf("入力ミスです。\n");
	//printf("入力データ：");
	return mozi;
}
/*
編集後記
かつてにちゃんねるで、
どこかの天才が、
Javascriptを使って
たった7行で テトリスをコーディングした
ソースファイルを公開した。
そのソースファイルはtabで間隔空けないし、字下げもしないで、
7回改行しただけのものだった。
それに比べたらこのソースファイルは随分と長い。
もしかしたら無駄な記述が多く含んでいるのかもしれない。
だけど、GUIを一から作るのに応用できるような技が
含まれているのも事実でありましょう。
でっかい二次元のビット配列に、
小さい二次元のビット配列を、
OR演算するかのような 手法で 重ね合わせて、
一つの画像を作ることができることを証明した。
*/