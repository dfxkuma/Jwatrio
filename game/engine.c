#include<stdio.h>
#include<windows.h>
#include<conio.h>
#include<time.h>
#include<stdlib.h>

#include "event.h"
#include "engine.h"
#include "draw.h"
#include "../utils/color.h"
#include "../utils/keyboard.h"
#include "../render/text.h"

#define false 0
#define true 1

#define ACTIVE_BLOCK -2 // 게임판배열에 저장될 블록의 상태들
#define CEILLING -1     // 블록이 이동할 수 있는 공간은 0 또는 음의 정수료 표현
#define EMPTY 0         // 블록이 이동할 수 없는 공간은 양수로 표현
#define WALL 1
#define BOTTOM_WALL 3
#define INACTIVE_BLOCK 2 // 이동이 완료된 블록값

#define MAIN_X 11 //게임판 가로크기
#define MAIN_Y 24 //게임판 세로크기
#define MAIN_X_ADJ 3 //게임판 위치조정
#define MAIN_Y_ADJ 1 //게임판 위치조정

#define BUFFER_SIZE 2048 // 서버 버퍼

#define STATUS_X_ADJ (MAIN_X_ADJ+MAIN_X+1) //게임정보표시 위치조정

int STATUS_Y_GOAL; //GOAL 정보표시위치Y 좌표 저장
int STATUS_Y_LEVEL; //LEVEL 정보표시위치Y 좌표 저장
int STATUS_Y_SCORE; //SCORE 정보표시위치Y 좌표 저장

int b_type; //블록 종류를 저장
int b_rotation; //블록 회전값 저장
int b_type_next; //다음 블록값 저장

int main_org[MAIN_Y][MAIN_X]; //게임판의 정보를 저장하는 배열 모니터에 표시후에 main_cpy로 복사됨
int main_cpy[MAIN_Y][MAIN_X]; //즉 maincpy는 게임판이 모니터에 표시되기 전의 정보를 가지고 있음
//main의 전체를 계속 모니터에 표시하지 않고(이렇게 하면 모니터가 깜빡거림)
//main_cpy와 배열을 비교해서 값이 달라진 곳만 모니터에 고침
int bx,by; //이동중인 블록의 게임판상의 x,y좌표를 저장

int key; //키보드로 입력받은 키값을 저장

int speed; //게임진행속도
int level; //현재 level
int level_goal; //다음레벨로 넘어가기 위한 목표점수
int cnt; //현재 레벨에서 제거한 줄 수를 저장
int score; //현재 점수
int last_score=0; //마지막게임점수
int best_score=0; //최고게임점수

// 점수 관리
int crushed_lines = 0; // 지운 줄 수
int used_blocks = 0;

int new_block_on=0; //새로운 블럭이 필요함을 알리는 flag
int crush_on=0; //현재 이동중인 블록이 충돌상태인지 알려주는 flag
int level_up_on=0; //다음레벨로 진행(현재 레벨목표가 완료되었음을) 알리는 flag
int space_key_on=0; //hard drop상태임을 알려주는 flag

int startNetworkGameEngine(int screenX, int screenY, char* username, char* ip) {
    onGameEngineStart(username, ip);
    system("cls");

    int i;
    srand((unsigned)time(NULL)); //난수표생성
    setCursorType(NOCURSOR); //커서 없앰
    reset(); //게임판 리셋
    drawNextBlockUI(screenX, screenY);
    drawInfoTextUI(40);

    onGameReady();

    while(1) {
        for(i=0;i<5;i++){ //블록이 한칸떨어지는동안 5번 키입력받을 수 있음
            check_key(screenX, screenY); //키입력확인
            draw_main(); //화면을 그림
            Sleep(speed); //게임속도조절
            if(crush_on&&check_crush(bx,by+1, b_rotation)==false) Sleep(100);
            //블록이 충돌중인경우 추가로 이동및 회전할 시간을 갖음
            if(space_key_on==1) { //스페이스바를 누른경우(hard drop) 추가로 이동및 회전할수 없음 break;
                space_key_on=0;
                break;
            }
        }
        drop_block(); // 블록을 한칸 내림
        check_level_up(); // 레벨업을 체크
        check_game_over(); //게임오버를 체크
        if(new_block_on==1) new_block(); // 뉴 블럭 flag가 있는 경우 새로운 블럭 생성
    }
    // 위에 솔로게임 UI 만들기
}

int startSoloGameEngine(int screenX, int screenY) {
    system("cls");

    int i;
    srand((unsigned)time(NULL)); //난수표생성
    setCursorType(NOCURSOR); //커서 없앰
    reset(); //게임판 리셋
    drawNextBlockUI(screenX, screenY);
    drawInfoTextUI(40);

    while(1) {
        for(i=0;i<5;i++){ //블록이 한칸떨어지는동안 5번 키입력받을 수 있음
            check_key(screenX, screenY); //키입력확인
            draw_main(); //화면을 그림
            Sleep(speed); //게임속도조절
            if(crush_on&&check_crush(bx,by+1, b_rotation)==false) Sleep(100);
            //블록이 충돌중인경우 추가로 이동및 회전할 시간을 갖음
            if(space_key_on==1) { //스페이스바를 누른경우(hard drop) 추가로 이동및 회전할수 없음 break;
                space_key_on=0;
                break;
            }
        }
        drop_block(); // 블록을 한칸 내림
        check_level_up(); // 레벨업을 체크
        check_game_over(); //게임오버를 체크
        if(new_block_on==1) new_block(); // 뉴 블럭 flag가 있는 경우 새로운 블럭 생성
    }
}


void reset(void){
    best_score=0;

    level=1; //각종변수 초기화
    score=0;
    level_goal=1000;
    key=0;
    crush_on=0;
    cnt=0;
    speed=100;

    system("cls"); //화면지움
    reset_main(); // main_org를 초기화
    draw_main(); // 게임판을 그림


    b_type_next=rand()%7; //다음번에 나올 블록 종류를 랜덤하게 생성
    new_block(); //새로운 블록을 하나 만듦

}

void reset_main(void){ //게임판을 초기화
    int i,j;

    for(i=0;i<MAIN_Y;i++){ // 게임판을 0으로 초기화
        for(j=0;j<MAIN_X;j++){
            main_org[i][j]=0;
            main_cpy[i][j]=100;
        }
    }
    for(j=1;j<MAIN_X;j++){ //y값이 3인 위치에 천장을 만듦
        main_org[3][j]=CEILLING;
    }
    for(i=1;i<MAIN_Y;i++){ //좌우 벽을 만듦
        main_org[i][0]=WALL;
        main_org[i][MAIN_X-1]=WALL;
    }
    for(j=0;j<MAIN_X;j++){ //바닥벽을 만듦
        main_org[MAIN_Y-1][j]=BOTTOM_WALL;
    }
}

void reset_main_cpy(void){ //main_cpy를 초기화
    int i, j;

    for(i=0;i<MAIN_Y;i++){         //게임판에 게임에 사용되지 않는 숫자를 넣음
        for(j=0;j<MAIN_X;j++){  //이는 main_org와 같은 숫자가 없게 하기 위함
            main_cpy[i][j]=100;
        }
    }
}


void draw_main(void){ //게임판 그리는 함수
    int i, j;

    for(j=1;j<MAIN_X-1;j++){ //천장은 계속 새로운블럭이 지나가서 지워지면 새로 그려줌
        if(main_org[3][j]==EMPTY) main_org[3][j]=CEILLING;
    }
    for(i=0;i<MAIN_Y+1;i++){
        for(j=0;j<MAIN_X;j++){
            if(main_cpy[i][j]!=main_org[i][j]){ //cpy랑 비교해서 값이 달라진 부분만 새로 그려줌.
                //이게 없으면 게임판전체를 계속 그려서 느려지고 반짝거림
                switch(main_org[i][j]){
                    case BOTTOM_WALL: //바닥벽모양
                        printf("▔");
                        drawText(MAIN_X_ADJ+5+j,MAIN_Y_ADJ+2+i,"▔", COLOR_DEFAULT);
                        break;
                    case EMPTY: //빈칸모양
                        drawText(MAIN_X_ADJ+5+j,MAIN_Y_ADJ+2+i,"  ", COLOR_DEFAULT);
                        break;
                    case CEILLING: //천장모양
                        drawText(MAIN_X_ADJ+5+j,MAIN_Y_ADJ+2+i,". ", COLOR_DEFAULT);
                        break;
                    case WALL: //벽모양
                        drawText(MAIN_X_ADJ+5+j,MAIN_Y_ADJ+2+i,"║", COLOR_DEFAULT);
                        break;
                    case INACTIVE_BLOCK: //굳은 블럭 모양
                        drawText(MAIN_X_ADJ+5+j,MAIN_Y_ADJ+2+i,"□", COLOR_DEFAULT);
                        break;
                    case ACTIVE_BLOCK: //움직이고있는 블럭 모양
                        drawText(MAIN_X_ADJ+5+j,MAIN_Y_ADJ+2+i,"■", COLOR_DEFAULT);
                        break;
                }
            }
        }
    }
    for(i=0;i<MAIN_Y;i++){ //게임판을 그린 후 main_cpy에 복사
        for(j=0;j<MAIN_X;j++){
            main_cpy[i][j]=main_org[i][j];
        }
    }
}

void new_block(void){ //새로운 블록 생성
    int i, j;

    bx=(MAIN_X/2)-1; //블록 생성 위치x좌표(게임판의 가운데)
    by=0;  //블록 생성위치 y좌표(제일 위)
    b_type=b_type_next; //다음블럭값을 가져옴
    b_type_next=rand()%7; //다음 블럭을 만듦
    b_rotation=0;  //회전은 0번으로 가져옴

    new_block_on=0; //new_block flag를 끔

    for(i=0;i<4;i++){ //게임판 bx, by위치에 블럭생성
        for(j=0;j<4;j++){
            if(TETRIS_BLOCK[b_type][b_rotation][i][j]==1) main_org[by+i][bx+j]=ACTIVE_BLOCK;
        }
    }
    for(i=1;i<3;i++){ //게임상태표시에 다음에 나올블럭을 그림
        for(j=0;j<4;j++){
            if(TETRIS_BLOCK[b_type_next][0][i][j]==1) {
                drawText(STATUS_X_ADJ+7+j,i+5,"■", COLOR_DEFAULT);
            }
            else{
                drawText(STATUS_X_ADJ+7+j,i+5, "  ", COLOR_DEFAULT);
            }
        }
    }
}

void check_key(int screenX, int screenY){
    key=0; //키값 초기화

    if(kbhit()){ //키입력이 있는 경우
        key=getch(); //키값을 받음
        if(key==224){ //방향키인경우
            do{key=getch();} while(key==224);//방향키지시값을 버림
            switch(key){
                case KEY_LEFT: //왼쪽키 눌렀을때
                    if(check_crush(bx-1,by,b_rotation)==true) move_block(KEY_LEFT);
                    break;                            //왼쪽으로 갈 수 있는지 체크 후 가능하면 이동
                case KEY_RIGHT: //오른쪽 방향키 눌렀을때- 위와 동일하게 처리됨
                    if(check_crush(bx+1,by,b_rotation)==true) move_block(KEY_RIGHT);
                    break;
                case KEY_DOWN: //아래쪽 방향키 눌렀을때-위와 동일하게 처리됨
                    if(check_crush(bx,by+1,b_rotation)==true) move_block(KEY_DOWN);
                    break;
                case KEY_UP: //위쪽 방향키 눌렀을때
                    if(check_crush(bx,by,(b_rotation+1)%4)==true) move_block(KEY_UP);
                        //회전할 수 있는지 체크 후 가능하면 회전
                    else if(crush_on==1&&check_crush(bx,by-1,(b_rotation+1)%4)==true) move_block(100);
            }                    //바닥에 닿은 경우 위쪽으로 한칸띄워서 회전이 가능하면 그렇게 함(특수동작)
        }
        else{ //방향키가 아닌경우
            switch(key){
                case KEY_SPACE: //스페이스키 눌렀을때
                    space_key_on=1; //스페이스키 flag를 띄움
                    while(crush_on==0){ //바닥에 닿을때까지 이동시킴
                        drop_block();
                        score+=level; // hard drop 보너스
                        drawText(STATUS_X_ADJ, STATUS_Y_SCORE, "        ", COLOR_DEFAULT); //점수표시를 지움
//                        gotoxy(STATUS_X_ADJ, STATUS_Y_SCORE); printf("        %6d", score); //점수 표시
                    }
                    break;
                case KEY_P: //P(대문자) 눌렀을때
                case KEY_p: //p(소문자) 눌렀을때
                    pause(screenX, screenY); //일시정지
                    break;
                case KEY_ESC: //ESC눌렀을때
                    system("cls"); //화면을 지우고
                    exit(0); //게임종료
            }
        }
    }
    while (kbhit()) getch(); //키버퍼를 비움
}

void drop_block(void){
    int i,j;

    if(crush_on&&check_crush(bx,by+1, b_rotation)==true) crush_on=0; //밑이 비어있으면 crush flag 끔
    if(crush_on&&check_crush(bx,by+1, b_rotation)==false){ //밑이 비어있지않고 crush flag가 켜저있으면
        for(i=0;i<MAIN_Y;i++){ //현재 조작중인 블럭을 굳힘
            for(j=0;j<MAIN_X;j++){
                if(main_org[i][j]==ACTIVE_BLOCK) main_org[i][j]=INACTIVE_BLOCK;
            }
        }
        crush_on=0; //flag를 끔
        check_line(); //라인체크를 함
        new_block_on=1; //새로운 블럭생성 flag를 켬
        used_blocks++;
        drawInfoPIECES(used_blocks);
        return; //함수 종료
    }
    if(check_crush(bx,by+1, b_rotation)==true) move_block(KEY_DOWN); //밑이 비어있으면 밑으로 한칸 이동
    if(check_crush(bx,by+1, b_rotation)==false) crush_on++; //밑으로 이동이 안되면  crush flag를 켬
}


int check_crush(int bx, int by, int b_rotation){ //지정된 좌표와 회전값으로 충돌이 있는지 검사
    int i,j;

    for(i=0;i<4;i++){
        for(j=0;j<4;j++){ //지정된 위치의 게임판과 블럭모양을 비교해서 겹치면 false를 리턴
            if(TETRIS_BLOCK[b_type][b_rotation][i][j]==1&&main_org[by+i][bx+j]>0) return false;
        }
    }
    return true; //하나도 안겹치면 true리턴
};

void move_block(int dir){ //블록을 이동시킴
    int i,j;

    switch(dir){
        case KEY_LEFT: //왼쪽방향
            for(i=0;i<4;i++){ //현재좌표의 블럭을 지움
                for(j=0;j<4;j++){
                    if(TETRIS_BLOCK[b_type][b_rotation][i][j]==1) main_org[by+i][bx+j]=EMPTY;
                }
            }
            for(i=0;i<4;i++){ //왼쪽으로 한칸가서 active block을 찍음
                for(j=0;j<4;j++){
                    if(TETRIS_BLOCK[b_type][b_rotation][i][j]==1) main_org[by+i][bx+j-1]=ACTIVE_BLOCK;
                }
            }
            bx--; //좌표값 이동
            break;

        case KEY_RIGHT:    //오른쪽 방향. 왼쪽방향이랑 같은 원리로 동작
            for(i=0;i<4;i++){
                for(j=0;j<4;j++){
                    if(TETRIS_BLOCK[b_type][b_rotation][i][j]==1) main_org[by+i][bx+j]=EMPTY;
                }
            }
            for(i=0;i<4;i++){
                for(j=0;j<4;j++){
                    if(TETRIS_BLOCK[b_type][b_rotation][i][j]==1) main_org[by+i][bx+j+1]=ACTIVE_BLOCK;
                }
            }
            bx++;
            break;

        case KEY_DOWN:    //아래쪽 방향. 왼쪽방향이랑 같은 원리로 동작
            for(i=0;i<4;i++){
                for(j=0;j<4;j++){
                    if(TETRIS_BLOCK[b_type][b_rotation][i][j]==1) main_org[by+i][bx+j]=EMPTY;
                }
            }
            for(i=0;i<4;i++){
                for(j=0;j<4;j++){
                    if(TETRIS_BLOCK[b_type][b_rotation][i][j]==1) main_org[by+i+1][bx+j]=ACTIVE_BLOCK;
                }
            }
            by++;
            break;

        case KEY_UP: //키보드 위쪽 눌렀을때 회전시킴.
            for(i=0;i<4;i++){ //현재좌표의 블럭을 지움
                for(j=0;j<4;j++){
                    if(TETRIS_BLOCK[b_type][b_rotation][i][j]==1) main_org[by+i][bx+j]=EMPTY;
                }
            }
            b_rotation=(b_rotation+1)%4; //회전값을 1증가시킴(3에서 4가 되는 경우는 0으로 되돌림)
            for(i=0;i<4;i++){ //회전된 블록을 찍음
                for(j=0;j<4;j++){
                    if(TETRIS_BLOCK[b_type][b_rotation][i][j]==1) main_org[by+i][bx+j]=ACTIVE_BLOCK;
                }
            }
            break;

        case 100: //블록이 바닥, 혹은 다른 블록과 닿은 상태에서 한칸위로 올려 회전이 가능한 경우
            //이를 동작시키는 특수동작
            for(i=0;i<4;i++){
                for(j=0;j<4;j++){
                    if(TETRIS_BLOCK[b_type][b_rotation][i][j]==1) main_org[by+i][bx+j]=EMPTY;
                }
            }
            b_rotation=(b_rotation+1)%4;
            for(i=0;i<4;i++){
                for(j=0;j<4;j++){
                    if(TETRIS_BLOCK[b_type][b_rotation][i][j]==1) main_org[by+i-1][bx+j]=ACTIVE_BLOCK;
                }
            }
            by--;
            break;
    }
}

void check_line(void){
    int i, j, k, l;

    int    block_amount; //한줄의 블록갯수를 저장하는 변수
    int combo=0; //콤보갯수 저장하는 변수 지정및 초기화

    for(i=MAIN_Y-2;i>3;){ //i=MAIN_Y-2 : 밑쪽벽의 윗칸부터,  i>3 : 천장(3)아래까지 검사
        block_amount=0; //블록갯수 저장 변수 초기화
        for(j=1;j<MAIN_X-1;j++){ //벽과 벽사이의 블록갯루를 셈
            if(main_org[i][j]>0) block_amount++;
        }
        if(block_amount==MAIN_X-2){ //블록이 가득 찬 경우
            crushed_lines++; //지운 줄 수 증가
            drawInfoLINES(crushed_lines,40);
            if(level_up_on==0){ //레벨업상태가 아닌 경우에(레벨업이 되면 자동 줄삭제가 있음)
                score+=100*level; //점수추가
                cnt++; //지운 줄 갯수 카운트 증가
                combo++; //콤보수 증가
            }
            for(k=i;k>1;k--){ //윗줄을 한칸씩 모두 내림(윗줄이 천장이 아닌 경우에만)
                for(l=1;l<MAIN_X-1;l++){
                    if(main_org[k-1][l]!=CEILLING) main_org[k][l]=main_org[k-1][l];
                    if(main_org[k-1][l]==CEILLING) main_org[k][l]=EMPTY;
//윗줄이 천장인 경우에는 천장을 한칸 내리면 안되니까 빈칸을 넣음
                }
            }
        }
        else i--;
    }
    if(combo){ //줄 삭제가 있는 경우 점수와 레벨 목표를 새로 표시함
        if(combo>1){ //2콤보이상인 경우 경우 보너스및 메세지를 게임판에 띄웠다가 지움
//            gotoxy(MAIN_X_ADJ+(MAIN_X/2)-1,MAIN_Y_ADJ+by-2);printf("%d COMBO!",combo);
            drawText(MAIN_X_ADJ+(MAIN_X/2)-1,MAIN_Y_ADJ+by-2,"COMBO!", COLOR_DEFAULT);
            Sleep(500);
            score+=(combo*level*100);
            reset_main_cpy(); //텍스트를 지우기 위해 main_cpy을 초기화.
//(main_cpy와 main_org가 전부 다르므로 다음번 draw()호출시 게임판 전체를 새로 그리게 됨)
        }
//        gotoxy(STATUS_X_ADJ, STATUS_Y_GOAL); printf(" GOAL  : %5d", (cnt<=10)?10-cnt:0);
//        gotoxy(STATUS_X_ADJ, STATUS_Y_SCORE); printf("        %6d", score);
    }
}

void check_level_up(void){
    int i, j;

    if(cnt>=10){ //레벨별로 10줄씩 없애야함. 10줄이상 없앤 경우
        draw_main();
        level_up_on=1; //레벨업 flag를 띄움
        level+=1; //레벨을 1 올림
        cnt=0; //지운 줄수 초기화

        for(i=0;i<4;i++){
//            gotoxy(MAIN_X_ADJ+(MAIN_X/2)-3,MAIN_Y_ADJ+4);
//            printf("             ");
//            gotoxy(MAIN_X_ADJ+(MAIN_X/2)-2,MAIN_Y_ADJ+6);
//            printf("             ");
//            Sleep(200);
//
//            gotoxy(MAIN_X_ADJ+(MAIN_X/2)-3,MAIN_Y_ADJ+4);
//            printf("☆LEVEL UP!☆");
//            gotoxy(MAIN_X_ADJ+(MAIN_X/2)-2,MAIN_Y_ADJ+6);
//            printf("☆SPEED UP!☆");
//            Sleep(200);
        }
        reset_main_cpy(); //텍스트를 지우기 위해 main_cpy을 초기화.
//(main_cpy와 main_org가 전부 다르므로 다음번 draw()호출시 게임판 전체를 새로 그리게 됨)

        for(i=MAIN_Y-2;i>MAIN_Y-2-(level-1);i--){ //레벨업보상으로 각 레벨-1의 수만큼 아랫쪽 줄을 지워줌
            for(j=1;j<MAIN_X-1;j++){
                main_org[i][j]=INACTIVE_BLOCK; // 줄을 블록으로 모두 채우고
//                gotoxy(MAIN_X_ADJ+j,MAIN_Y_ADJ+i); // 별을 찍어줌.. 이뻐보이게
                printf("★");
                Sleep(20);
            }
        }
        Sleep(100); //별찍은거 보여주기 위해 delay
        check_line(); //블록으로 모두 채운것 지우기
//.check_line()함수 내부에서 level up flag가 켜져있는 경우 점수는 없음.
        switch(level){ //레벨별로 속도를 조절해줌.
            case 2:
                speed=50;
                break;
            case 3:
                speed=25;
                break;
            case 4:
                speed=10;
                break;
            case 5:
                speed=5;
                break;
            case 6:
                speed=4;
                break;
            case 7:
                speed=3;
                break;
            case 8:
                speed=2;
                break;
            case 9:
                speed=1;
                break;
            case 10:
                speed=0;
                break;
        }
        level_up_on=0; //레벨업 flag꺼줌

//        gotoxy(STATUS_X_ADJ, STATUS_Y_LEVEL); printf(" LEVEL : %5d", level); //레벨표시
//        gotoxy(STATUS_X_ADJ, STATUS_Y_GOAL); printf(" GOAL  : %5d", 10-cnt); // 레벨목표 표시

    }
}

void check_game_over(void){
    int i;

    int x=5;
    int y=5;

    for(i=1;i<MAIN_X-2;i++){
        if(main_org[3][i]>0){ //천장(위에서 세번째 줄)에 inactive가 생성되면 게임 오버
//            gotoxy(x,y+0); printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤"); //게임오버 메세지
//            gotoxy(x,y+1); printf("▤                              ▤");
//            gotoxy(x,y+2); printf("▤  +-----------------------+   ▤");
//            gotoxy(x,y+3); printf("▤  |  G A M E  O V E R..   |   ▤");
//            gotoxy(x,y+4); printf("▤  +-----------------------+   ▤");
//            gotoxy(x,y+5); printf("▤   YOUR SCORE: %6d         ▤",score);
//            gotoxy(x,y+6); printf("▤                              ▤");
//            gotoxy(x,y+7); printf("▤  Press any key to restart..  ▤");
//            gotoxy(x,y+8); printf("▤                              ▤");
//            gotoxy(x,y+9); printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤");
            last_score=score; //게임점수를 옮김

            if(score>best_score){ //최고기록 갱신시
                FILE* file=fopen("score.dat", "wt"); //score.dat에 점수 저장

//                gotoxy(x,y+6); printf("▤  ★★★ BEST SCORE! ★★★   ▤  ");

                if(file==0){ //파일 에러메세지
//                    gotoxy(0,0);
                    printf("FILE ERROR: SYSTEM CANNOT WRITE BEST SCORE ON \"SCORE.DAT\"");
                }
                else{
                    fprintf(file,"%d", score);
                    fclose(file);
                }
            }
            Sleep(1000);
            while (kbhit()) getch();
            key=getch();
            reset();
        }
    }
}

void pause(int screenX, int screenY){ //게임 일시정지 함수
    int i,j;

    int x=5;
    int y=5;

    for(i=1;i<MAIN_X-2;i++){ //게임 일시정지 메세지
        drawText(10, 17, "    PAUSE    ", 240);
    }
    getch(); //키입력시까지 대기

    system("cls"); //화면 지우고 새로 그림
    reset_main_cpy();
    drawNextBlockUI(screenX, screenY);
    drawInfoTextUI(40);
    draw_main();

    for(i=1;i<3;i++){ // 다음블록 그림
        for(j=0;j<4;j++){
            if(TETRIS_BLOCK[b_type_next][0][i][j]==1) {
                drawText(MAIN_X+MAIN_X_ADJ+3+j,i+6,"■", COLOR_DEFAULT);
            }
            else{
                drawText(MAIN_X+MAIN_X_ADJ+3+j,i+6,"  ", COLOR_DEFAULT);
            }
        }
    }
} //끝!

