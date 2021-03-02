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
�����
Tentative-Enclosing Tracks Refused Influencing System
��Version2
�ł���B

����́A
void moveToLRD(void);
�́A4���@���������̂�����
��]�@�\��
a[][]�ւ̊m��@�\��
���̃u���b�N���o��������@�\
e[][]��b[][]��a[][]�𔽉f
e[][]��\��
�܂ŕt���������̂��B

�ꍆ�@�ł́A
�ł����i�Ƀu���b�N���ڂ��Ă���Ƃ��ɁA
�E�܂��͍��Ɉړ����悤�Ƃ���ƁA���˂Ă��܂��A
�s�s���ȓ��삪���������B
�񍆋@�ł͂�������P������B
���������u���b�N���g�ƂԂ����Ă��܂��܂ŉ��Ɉړ�
�����Ă͂����Ȃ����낤�B
*/
/*�g�̑���
�{����1�������̑����ő����̂��낤���B
*/
/*LRD2���@�ł͒��˂Ȃ��悤�ɉ��P���ꂽ�B
���P���ꂽ���A�V���Ȗ�肪�������B
��5�}�X�c5�}�X �S��1�Ŗ��ߐs�����ꂽ���̂�
���v�����ǁA
11
11
�̂悤��  �����Ȃ��̂�
���܂��܂Ȍ`��ɑΉ��ł��Ȃ��ˁB
00000
11000
01100
00000
*/
/*
LRD3���@�͂��������P����̂��ړI�ō��ꂽ�B
���̌��ʁA���P���ꂽ�B
a[][]�̗v�f����b[][]�̗v�f������
�c������+4���������A�������ɂ�+4���������B
�������A���ړ����w�����������ŁA
�E�ړ����w��������A
���ֈړ������̂��A�E�΂߉������ֈړ�����ُ킪
���������B
LRD4���@�ł͂��̃o�O���C�����邽�߂ɗp�ӂ��ꂽ�B
*/
/*
LRDTCNE.c��
l leftward transference
r rightward tranference
d downward transference
t turn the block
c commitA
n next block incoming
e erase a line
�� �s����悤�ɂ��邱�Ƃ�ړI�Ƃ��č��ꂽ�B
���̌��ʁA�ȉ��̂悤�ɂȂ����B
l ���� ���Ȃ����삷��B
r ���� ���Ȃ����삷��B
d ��������Ȃ����삷��B
t �E�[�⍶�[�ɗ����� ���[�v����o�O������
c ���Ȃ����삷��
n �s�����邱�Ƃɂ͐������Ă���B�ł��\������Ȃ��B
e ���Ȃ����삷��B

lrdtcne2�ł� ��]����Ȃ����삳���邱�Ƃ�ړI�Ƃ���
�����B
���P��Ƃ��ẮAb[][]�̗v�f����a[][]�Ɠ����ɂ��邱�ƁB
����A����ȒP���Ȃ��̂���Ȃ��B
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
//e[][]�Ƃ� �v���[���[�ɕ\��������䎆�̂悤�Ȃ��̂��B
//�䎆�͂ł������L���Ȃ���΂Ȃ�Ȃ��B
//Java��swing�̃R���e�i�Ɏ��Ă��邩���ˁB
//javax.swing.*���� container cnt =getContentPane();
// cnt.add(pn[0]);�̂悤�ȁB
//�ڂ������Ƃ͂₳����JAVA���p�҂ł����Ă��������B
//���ۂ͉��ɂ͉E�̃X�y�[�X��15�}�X���x�����g��Ȃ�����
//�������͈̔̓I�[�o�[���|�����瑽�߂ɂƂ��Ă����B
int e[HEIGHT+4][WIDTH+46]={0};
/*
e[][]��WIDTH+4��WIDTH+8�ɂ��悤�Ƃ������B
�����傫�ȃu���b�N�G���A�̉E�ׂ�ɐ��l��
NEXT �u���b�N�̉摜�𒣂�t���邽�߂̂���
*/
//c[2][2]��b[][]�ł͂ǂ��ɂ���̂����������߂̂���
int block_core_x=STARTPOSITION_X;
int block_core_y=STARTPOSITION_Y;

unsigned int point=0;

typedef struct block{
	int array[5][5];
	char color;//�u���b�N�̎�ނɂ���Ă͐F���Ⴄ�B 
	int id;
}BLOCK;
BLOCK nextBlock[3];
//���B3���܂ŕ\�����������H���́A����ǂ��Ȃ��B
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
	'N',7//None ��ԍŌ�̓u���b�N�Ȃ��łȂ���΂Ȃ�Ȃ�
}
};

void viewA(void);
void viewB(void);
void viewE(void);
void viewEwithSquare(void);
int moveToRight(void);//�u���b�N���E�ړ�
int moveToLeft(void);//�u���b�N�����ړ�
int moveToDown(void);//�u���b�N���~���ړ�
int checkTheCeiling(void);
char input_char(void);
int AttemptMergeCwithB(void);//���ۂ�B��ł͂ǂ��ɂ���̂��B
int AttemptMergeBwithA(char direction);//�C���^�[�t�F�[�X
int AttemptMergeBwithADo(int anotherX,int anotherY,char direction);//���ۂɎ��݂�
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


/*�֐��̖��O�����X�����Ă��R���p�C����
�s�����q�ׂȂ��B*/
/*�܂�������ȂɊ֐���O���[�o���ϐ����R��邱�ƂɂȂ�Ƃ�
�v���Ă��Ȃ������B
�錾������300�s�H 7�s�̃e�g���X�ɔ�ׂ�� �ƂĂ������ˁB
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
	printf("����:���� �� c(^_^)v");
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
					noEchoCh=_getch(); //visual C++�p�̂��� �G�R�[�Ȃ��œ��͂ł���B 
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
			�����Œ�����NEXT�u���b�N�r�o����������B
		
			�����������ǂ����`�F�b�N
			char isImplant(void);
			��
			int commitA(void);
			a[][]�Ƀu���b�Nb[][]���㏑�����Ċm��
			��
			NEXT�u���b�N�I��
			��
			NEXT
		*/
		//�g�̏�܂� �B���Ă��Ȃ����ǂ����̃`�F�b�N
		//�B���Ă�����Q�[���I�[�o�[
		
		if(isImplant() == 'Y'){//Yes,�������Ă���B
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
		//printf("��viewB\n");
		//viewB();
		//printf("��viewA\n");
		//viewA();
		
		
		
		//�g�̏�܂� �B���Ă��Ȃ����ǂ����̃`�F�b�N
		//�B���Ă�����Q�[���I�[�o�[
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
	//�m�肳����
	commitA();
	chooseNextNextBlock();
	setNewBlockonC();
	resetBlockCoreXY();
	refleshE();
	return 0;
}
//���g�p���܂���B
int chooseNextBlock(void){
	auto int number=0;
	srand( (unsigned)time( NULL ) );
	
	number = rand();
	number %=7;
	return number;
}
//���Q�[���X�^�[�g����chooseNextNextBlock(void)��3��ĂԂ����̊֐�
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
	//[2]��[1]�Ɉڂ��āA[1]��[0]�Ɉڂ��āE�E�E
	//�\���̂����炱�� ��d���[�v�Ȃ񂩎g��Ȃ������� �������� ���ʂł���̂��B
	nextBlock[0]=nextBlock[1];
	nextBlock[1]=nextBlock[2];
	nextBlock[2] = type[number]; 
	for(i=0;i<3;i++){
		for(j=0;j<5;j++){
			for(k=0;k<5;k++){
				//printf("%d",nextBlock[i].array[j][k]);
			}
			//printf("��i=%d\n",i);
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
	//�z��̔z��̔z�񂾂ƃC���[�W���ɂ������B
	//������\���̂��g���񂾁B���ޔԍ����Ԃ̔z��̔z��
	//printf("----===setNewBlockOnC===---\n");
	for(y=0;y<5;y++){
		for(x=0;x<5;x++){
				//cast���Z�q
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
	auto int result1=0,result2;//��]����� �ړ������Ă�����̂Ƃ݂Ȃ��B
	//��]���ł��Ă��A�g�Əd�Ȃ��Ă��܂�����A���ɖ߂��˂΂Ȃ��B
	//���ꂾ���łȂ��Ab[][]�͈̔͂̊O�ɏ������܂��Ȃ��悤�ɂ��Ȃ���΂Ȃ�܂���B
	refleshB();

	//�܂���d[][]�� ���Z�b�g���Ă���B
	refleshD();
	for(y=0;y<5;y++){
		for(x=0;x<5;x++){
			//printf("%d",c[x][y]);
			//swapC[][]�ɑޔ�
			swapC[y][x]=c[y][x];
		}
		//printf("\n");
	}
	//printf("\n");
	//��]���Ȃ���d[][]�ɓ\����
	for(y=0;y<5;y++){
		for(x=0;x<5;x++){
			d[x][4-y]=c[y][x];
			//printf("%d",d[x][y]);
		}
		//printf("\n");
	}
	//���̂�����d[][]��c[][]�ɓ\��B
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
	//result ����O�������ꍇ�� �ُ킾���猳�ɖ߂��B
	//�t��]����� ���m�F���� �኱�̔�t��������Ǝv���B
	//����ɂ�����̓�dfor���Ǝ����悤�Ȃ��̂�
	//�܂�3�񏑂��̂� �ʓ|�������B
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

//�g��`��
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
	//b��c���d�ˍ��킹�Ă݂�A
	AttemptMergeCwithB();
	//a�̘g�Əd�Ȃ��Ă��Ȃ������`�F�b�N
	result = AttemptMergeBwithA('L');
	if(result ==0 ){
		//���A�d�Ȃ�c�����O�����B���ďꍇ
		block_core_x--;
	}
	//���Ȃ� ���܂�������return 0
	return result;
}

int moveToRight(void){
	int result=0;
	
	refleshB();
	
	//b��c���d�ˍ��킹�Ă݂�A
	AttemptMergeCwithB();
	//a�̘g�Əd�Ȃ��Ă��Ȃ������`�F�b�N
	result = AttemptMergeBwithA('R');
	if( result == 0){
		//���A�d�Ȃ��ĂȂ����B���ďꍇ
		block_core_x++;
	}
	//���Ȃ� ���܂�������return 0
	return result;
}

int moveToDown(void){
	int result=0;
	
	refleshB();
	
	//b��c���d�ˍ��킹�Ă݂�A
	AttemptMergeCwithB();
	//a�̘g�Əd�Ȃ��Ă��Ȃ������`�F�b�N
	//�`�F�b�N���邾���Ȃ畹������K�v�͂Ȃ��B
	//printf("Are you sure to down?\n");
	
	result = AttemptMergeBwithA('D');
			/*����AttemptMergeBwithA();�̈����́A
				�ǂ̕����ɍ�����1���u���b�N���ړ�������̂���
				�w�����邽�߂̂��́B
				'D'�Ȃ�� ����ɂ��炵�āE�E�E
				�܂�Ə��� a[3][2] �� b[0][0]�����Ă͂߂�悤�ɂ���
				�������邱�Ƃ����݂�B
				*/
	if(result == 0){
		//���A�d�Ȃ��Ă��Ȃ����B���ďꍇ�̂�
		block_core_y++;
		//block_core_x = 3;
		//�������moveToDown�̂Ƃ��Ɏg����Z��
		//block_core_y=3; �̂悤�ɂˁB
	}else{
		//�m�菈���������ɏ����c�ׂ��ł͂Ȃ��B
	}
	//���Ȃ� ���܂�������return 0
	//�u���b�N���A�ꂩ ����a[][]�ɂ��z�u�ς݂̃u���b�N��
	//�Ԃ����������炵���Ȃ� return ��O;
	return result;
}

int checkTheCeiling(void){
	int result=0;
	
	refleshB();
	
	//b��c���d�ˍ��킹�Ă݂�A
	AttemptMergeCwithB();
	//a�̘g�Əd�Ȃ��Ă��Ȃ������`�F�b�N
	//�`�F�b�N���邾���Ȃ畹������K�v�͂Ȃ��B
	//printf("Are you sure to down?\n");
	
	result = AttemptMergeBwithA('U');
			/*����AttemptMergeBwithA();�̈����́A
				�ǂ̕����ɍ�����1���u���b�N���ړ�������̂���
				�w�����邽�߂̂��́B
				'D'�Ȃ�� ����ɂ��炵�āE�E�E
				�܂�Ə��� a[3][2] �� b[0][0]�����Ă͂߂�悤�ɂ���
				�������邱�Ƃ����݂�B
				*/
	
	//���Ȃ� ���܂�������return 0
	//�u���b�N���A�V���
	//�Ԃ����������炵���Ȃ� return ��O;
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

// b[][]�� 0�N���A����
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

// e[][]�� 0�N���A����
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
//c[][]�� b[][]�� �_�������ɓ\��t���悤�Ǝ��݂�
/*���ӁI�I b[][]�͈̔͂𒴂���ʒu��
�����������������Ƃ��Ă͂����܂���B
VisualStudio�ł͂���ł� �Ȃ�Ƃ����삳���悤�Ƃ��邯�ǁA
��΂� �_�����ʂ�ɂ� �����Ȃ��B
*/
int AttemptMergeCwithB(void){
	auto int cx=0,cy=0;
	auto int bx=0,by=0;
	auto int YoutryedToPutOnOverThere=0;
	refleshB();
	for(cy=0,by=block_core_y-2;cy<5;cy++,by++){
		for(cx=0,bx=block_core_x-2;cx<5;cx++,bx++){
			if(bx>=WIDTH || bx <= -1 || by >HEIGHT){//�v�f�ԍ����}�C�i�X1��}�C�i�X2�Ȃ�Ă��肦�Ȃ��B
				YoutryedToPutOnOverThere++;
			}else{
				b[by][bx]=c[cy][cx];
			}
		}
	}
	
	return YoutryedToPutOnOverThere;//0���A��Ȃ���΃G���[
}

// a[][] �� b[][] �𕹍�����̂����Aa[][]�̂ق��������傫���T�C�Y���B
// a[][]��(x,y)=(2,2)�� b[][]��(x,y)=(0,0)��\�荇�킹��B
/* �C���^�[�t�F�[�X
	�R���s���[�^�͐��l�Ŏw���ł��邪�A�l�͌��t�Ŏw������B
	����̓R���s���[�^�Ɛl�Ƃ̈Ⴂ�𖄂ߍ��킹�邽�߂̂��̂��B
*/
int AttemptMergeBwithA(char direction)
{
	//char direction ��  L R D U��3��ނ�����B
	//D���������������ׂ�B����A�����Ȃ����𒲂ׂ��Ⴂ�Ȃ�B
	//���\���G�ɂȂ��Ă��܂����˂��B�̂̎�����������r���ŋx���v�����Ă���Ƃ��낾��B
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
		//Turn ������ ���݂Ă��߂������Ƃ��Ă��Q�[���I�[�o�[�ɂ��Ȃ��B
		correspondence=AttemptMergeBwithADo(anotherX,anotherY,direction);
	}else if(direction == 'U'){//�Q�[���I�[�o�[�H
		correspondence=AttemptMergeBwithADo(anotherX,anotherY-1,direction);
	}else{
		;
	}
	return correspondence;
}
int AttemptMergeBwithADo(int anotherX,int anotherY,char direction){
	auto int ax=0,ay=0,bx=0,by=0;
	auto int correspondence=0;
	
	for(ay=anotherY,by=0;by<HEIGHT;ay++,by++){ //�����ݒ��y=1�ł͂���
		for(ax=anotherX,bx=0;bx<WIDTH;ax++,bx++){//�����ݒ肪x=1�ł͂��߂����Ax<WIDTH-1���~
			if(a[ay][ax]==1 && b[by][bx]==1 && correspondence==0 /* && (direction != 'U')*/){
				correspondence=1;//���Ɋm��ς݃u���b�N�Ɍ��˂��鋰�ꂠ��B
				break;
			}
			
			if(a[ay][ax]==-1 && b[by][bx]==1  && correspondence==0 ){
				correspondence=-1;//�ǂɌ��˂��鋰�ꂠ��B
				// -1����ꂽ�����̂܂܃��[�v�𑱂��Ă��Ă�
				// correspondence++ ����
				// ��ŁA����correspondence��0�ɂȂ���
				// �߂�l��0��+3�ɂȂ肩�˂Ȃ��B
				break;
			}
			//printf("ax=%d,ay=%d,bx=%d,by=%d\n",ax,ay,bx,by);
		}
		if(correspondence !=0 ){
			break;
		}
	}
	
	return correspondence;
	//����ȂƂ���0���߂����
	//�ُ킪������΁A��O�y�� �[���z �� �߂����B
}
//���g�p�ꎞ���~
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
//�����傫�߂̕��ׂ�������B
int wait(void){
	auto unsigned int pressure=1000;//1000
	double aaa=0.1;
	for(pressure=1000000;pressure > 0;pressure--){
		aaa+=aaa/10;
		rand();// ���� ���ׂ������邾���̂��߂̂���
	}
	/*���̂�肩�����ƃn�[�h�E�F�A�ɂ����
	�҂����Ԃ��΂�΂�ɂȂ�B
	clock()�֐����g���ׂ����B*/
	return 0;
}
/*
�ĉj�Ђ̐ԐF��C���ꎫ�T
176�y�[�W���Q�Ƃ��ĂˁB
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
//�Q�[����ʂ���邽�߂̏���
//
int mergeAandBtoE(void){
	/*�����܂莆�̏�� �����ꖇ�����܂莆�ŕ����Ƃ����z�����Ă݂Ăق����B
	1�Ae[][]�����t���b�V������B
	2�Aa[][]��e[][]�̏�ɏd�ˍ��킹��B
	3�A���̂��ƁA������ c[][]��b[][]�Ɍ������A
	4�Ab[][]��e[][]�Ɍ�������
	*/
	auto int ay=0,ax=0;
	auto int bx=0,by=0;
	auto int ex=0,ey=0;


	//�P�A
	refleshE();
	/*
		����֐���return 0;�̗��p��
		if(!refleshE()){
			printf("E���t���b�V������!\n");
		}
		���̂悤�ɁA�o�O���ŗ��p�ł��������B
	*/

	//2�A
	for(ay=0,ey=0;ay<HEIGHT+4;ay++,ey++){
		for(ax=0,ex=0;ax<WIDTH+4;ax++,ex++){
			e[ey][ex]=a[ay][ax];
		}
	}


	//3�A
	refleshB();
	AttemptMergeCwithB();//C��B�Ɍ���

	//4�A
	for(ey=2,by=0;ey<HEIGHT+4;ey++,by++){
		for(ex=2,bx=0;ex<WIDTH+4;ex++,bx++){
		/*����if���ɂ���
			2�Ԃ̓�d���[�v�ł��Ae[][]��a[][]���������̂�
			������S���������Ă��Ab[][]���㏑�������Ⴄ�Ƃ��A
			�u���b�N�����A�������̂��̂������A�ʂ����Ȃ����Ⴄ�`��
			*/
			if(e[ey][ex]==0){
				e[ey][ex]=b[by][bx];
			}
		}
	}
	/*
		������ NEXT�u���b�N�̊G�� e[][]�ɓ\��t����\�[�X�R�[�h�������񂾁B
	*/
	//printf("---===NEXT�u���b�N�̊G===- \n");
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
				//�g��2�d�����\������Ƃ��͑����܂܂��̂܂�
				//�\������K�v�͂���܂��񂩂�B
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
	printf("��:���q    �f��:��\n");
	printf(" ��:�c��    �s��:Turn   Z��:�I��\n");
	printf("����LINE��:%d\n",point);
	for(y=0;y<HEIGHT+4;y++){
		for(x=0;x<WIDTH+20;x++){
			if(e[y][x]==-1){
				printf("�w");
			}
			if(e[y][x]==1){
				printf("��");
			}
			if(e[y][x]==0){
				if( y==2 && x<WIDTH+LEFTFRAME  ){
					printf("��");
				}else if(y==3 && x==19){
					printf("�m");
				}else if(y==3 && x==20){
					printf("�d");
				}else if(y==3 && x==21){
					printf("�w");
				}else if(y==3 && x==22){
					printf("�s");
				}else if(y==11 && x==21){
					printf("��");
				}else{
					printf("  ");
				}
			}
		}
		printf("\n");
	}
	

}
/*����commitA������
�����ЂƂ��Ɉړ��ł��Ȃ��ꍇ�̂�
�Ăяo�����ׂ����B
�u���b�N�� fall down �����Ƃ������łȂ�
������� �u���b�N�̌��Ԃ�
�������̃u���b�N���������܂ꂽ�Ƃ���
�Ăяo����Ȃ���΂Ȃ�Ȃ��B
*/
/*commitA�͒��������̈ꕔ��*/
int commitA(void){
	auto int ax=2,ay=2,bx=0,by=0;
	auto int result=0;
	
	for(ay=2,by=0;by<HEIGHT;ay++,by++){
		for(ax=2,bx=0;bx<WIDTH;ax++,bx++){
			if(a[ay][ax]==0 && b[by][bx]==1){
				a[ay][ax]=1;//a�Ƀu���b�N��`��
			}else{
				result++;
			}
		}
	}
	
	return result;
	//����ȂƂ���0���߂����
	//�ُ킪������΁A��O�y�� �[���z �� �߂����B
}
//�g�͏����Ă͂����Ȃ��B
int erase(void){
	eraseALine();// 2012�N5��10�� eraseALine�͂��܂������Ă���
	refleshE();
	mergeAandBtoE();
	return 0;
}

//a[][]�ɂ����āA�Ƃ����s�� �u���b�N�Ŗ��܂��Ă��邩�ǂ����B
//y�ɂ� �ǂ̍s�𒲂ׂ�̂����w�肷��B��̓I�ɉ��s�ڂ��H
int checkALine(int y){
	auto int x=0,total=0;
	
	for(x=0;x<WIDTH+LEFTFRAME;x++){
		if(a[y][x]==1){
			total++;
		}
	}
	return total;
}
//�Ȃ񂾂��񂾂ł�������B
int eraseALine(void){
	auto int x=0,y=0,dx=0,dy=0,result=0,count=0;
	for(y=HEIGHT+CEILING;y>CEILING;y--){
		result=checkALine(y);
		//printf("result=%d\n",result);
		if(result == WIDTH){
//result��WIDTH�Ɠ����Ȃ��1�s���u���b�N�Ŗ��܂��Ă���Ƃ�����B
			for(dy=y;dy>CEILING;dy--){
				for(dx=0;dx<WIDTH+4;dx++){
					a[dy][dx]=a[dy-1][dx];
				}
			}
			count++;
			point++;
			
		}
	}


	
	return count;//��������
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
	//printf("���̓~�X�ł��B\n");
	//printf("���̓f�[�^�F");
	return mozi;
}
/*
�ҏW��L
���Ăɂ����˂�ŁA
�ǂ����̓V�˂��A
Javascript���g����
������7�s�� �e�g���X���R�[�f�B���O����
�\�[�X�t�@�C�������J�����B
���̃\�[�X�t�@�C����tab�ŊԊu�󂯂Ȃ����A�����������Ȃ��ŁA
7����s���������̂��̂������B
����ɔ�ׂ��炱�̃\�[�X�t�@�C���͐����ƒ����B
�����������疳�ʂȋL�q�������܂�ł���̂�������Ȃ��B
�����ǁAGUI���ꂩ����̂ɉ��p�ł���悤�ȋZ��
�܂܂�Ă���̂������ł���܂��傤�B
�ł������񎟌��̃r�b�g�z��ɁA
�������񎟌��̃r�b�g�z����A
OR���Z���邩�̂悤�� ��@�� �d�ˍ��킹�āA
��̉摜����邱�Ƃ��ł��邱�Ƃ��ؖ������B
*/