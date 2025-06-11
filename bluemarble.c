#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#define stdHandle GetStdHandle(STD_OUTPUT_HANDLE)

unsigned long dw;

int random_num[35];
int board[15][15];
const int BOARD_SIZE = 40;
int golden_key_number[105];
int next;
///⬛ = p1, ⚫ = p2;

typedef BOOL (WINAPI* pSCFEX)(HANDLE, BOOL, PCONSOLE_FONT_INFOEX);
typedef BOOL (WINAPI* pGCFEX)(HANDLE, BOOL, PCONSOLE_FONT_INFOEX);


void ClearConsoleRect(HANDLE hConsole, SHORT left, SHORT top, SHORT right, SHORT bottom)
{
    DWORD dwWritten;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);

    SHORT width = right - left + 1;

    for (SHORT y = top; y <= bottom; ++y)
    {
        COORD coord = { left, y };
        FillConsoleOutputCharacter(hConsole, ' ', width, coord, &dwWritten);
        FillConsoleOutputAttribute(hConsole, csbi.wAttributes, width, coord, &dwWritten);
    }
}

short cur = 0;

void endl()
{
    SetConsoleCursorPosition(stdHandle, (COORD){ 145, cur });
    cur++;
}

void pan()
{
    SetConsoleCursorPosition(stdHandle, (COORD){0,0});
    printf("+------------+------------+------------+------------+------------+------------+------------+------------+------------+------------+------------+\n");
    printf("|            |            |            |            |            |            |            |            |            |            |            |\n");
    printf("|            |            |            |            |            |            |            |            |            |            |            |\n");
    printf("|   출발지   |  타이페이  |  황금열쇠  |   베이징   |   마닐라   |    제주    |  싱가포르  |  황금열쇠  |   카이로   |  이스탄불  |   무인도   |\n");
    printf("|   ----->   |            |            |            |            |            |            |            |            |            |            |\n");
    printf("|            |            |            |            |            |            |            |            |            |            |            |\n");
    printf("+------------+------------+------------+------------+------------+------------+------------+------------+------------+------------+------------+\n");
    printf("|            |                    ＊                                                                                              |            |\n");
    printf("|            |                  ＊     ＊                                                                                         |            |\n");
    printf("|    서울    |                ＊            ＊                                                                                    |   아테네   |\n");
    printf("|            |              ＊            ＊                                                                                      |            |\n");
    printf("|            |            ＊            ＊                                                                                        |            |\n");
    printf("+------------+          ＊            ＊                                                                                          +------------+\n");
    printf("|    사회    |        ＊            ＊                                                                                            |            |\n");
    printf("|            |      ＊            ＊                                                                                              |            |\n");
    printf("|    복지    |    ＊            ＊                                                                                                |  황금열쇠  |\n");
    printf("|            |  ＊            ＊                                                                                                  |            |\n");
    printf("|    기금    |＊            ＊                                                                                                    |            |\n");
    printf("+------------+     ＊     ＊                                                                                                      +------------+\n");
    printf("|            |          ＊                                                                                                        |            |\n");
    printf("|            |                                                                                                                    |            |\n");
    printf("|    뉴욕    |                                                                                                                    |  코펜하겐  |\n");
    printf("|            |                                                                                                                    |            |\n");
    printf("|            |                                                                                                                    |            |\n");
    printf("+------------+                                                                                                                    +------------+\n");
    printf("|            |                                                                                                                    |            |\n");
    printf("|            |                                                                                                                    |            |\n");
    printf("|    런던    |                                                                                                                    |  스톡홀롬  |\n");
    printf("|            |                                                                                                                    |            |\n");
    printf("|            |                                                                                                                    |            |\n");
    printf("+------------+                                                                                                                    +------------+\n");
    printf("|            |                                                                                                                    |            |\n");
    printf("|            |                                                                                                                    |   콩코드   |\n");
    printf("|  황금열쇠  |                                                                                                                    |   여객기   |\n");
    printf("|            |                                                                                                                    |            |\n");
    printf("|            |                                                                                                                    |            |\n");
    printf("+------------+                                                                                                                    +------------+\n");
    printf("|            |                                                                                                                    |            |\n");
    printf("|            |                                                                                                                    |            |\n");
    printf("|    로마    |                                                                                                                    |    베른    |\n");
    printf("|            |                                                                                                                    |            |\n");
    printf("|            |                                                                                                                    |            |\n");
    printf("+------------+                                                                                                                    +------------+\n");
    printf("|            |                                                                                                                    |            |\n");
    printf("|            |                                                                                                                    |            |\n");
    printf("|    파리    |                                                                                                                    |  황금열쇠  |\n");
    printf("|            |                                                                                                                    |            |\n");
    printf("|            |                                                                                                                    |            |\n");
    printf("+------------+                                                                                                                    +------------+\n");
    printf("|            |                                                                                                                    |            |\n");
    printf("|            |                                                                                                                    |            |\n");
    printf("| 컬럼비아호 |                                                                                                                    |   베를린   |\n");
    printf("|            |                                                                                                                    |            |\n");
    printf("|            |                                                                                                                    |            |\n");
    printf("+------------+                                                                                                                    +------------+\n");
    printf("|            |                                                                                                                    |            |\n");
    printf("|            |                                                                                                                    |            |\n");
    printf("|    도쿄    |                                                                                                                    |   오타와   |\n");
    printf("|            |                                                                                                                    |            |\n");
    printf("|            |                                                                                                                    |            |\n");
    printf("+------------+------------+------------+------------+------------+------------+------------+------------+------------+------------+------------+\n");
    printf("|            |            |            |            |            |            |            |            |            |            |            |\n");
    printf("|            |            |     퀸     |            |            |            |            |            |            |  부에노스  |    사회    |\n");
    printf("|  우주여행  |  마드리드  | 엘리자베스 |   리스본   |   하와이   |    부산    |   시드니   |  상파울루  |  황금열쇠  |  아이레스  |    복지    |\n");
    printf("|            |            |     호     |            |            |            |            |            |            |            |    기금    |\n");
    printf("|            |            |            |            |            |            |            |            |            |            |   접수처   |\n");
    printf("+------------+------------+------------+------------+------------+------------+------------+------------+------------+------------+------------+\n");
}



/*
 < 황금 열쇠 >
    1	병원비지불	병원에서 건강진단을 받았습니다. 병원비 5만원을 은행에 내시오.
    2	복권당첨	축하합니다. 복권에 당첨되었습니다. 당첨금 20만원.
    3	무인도탈출
    (특수무전기)	a) 무인도에 갖혀 있을 때에 사용할 수 있음
    b) 1회 사용후 반납하며, 타인에게 팔 수 있음.
    c) 특수무전기 가격은 20만원입니다.
    4	무인도로
    가시오	폭풍을 만났습니다.
    무인도로 곧장 가시되, 출발지를 지날 때도 월급을 받지 못합니다.
    5	파티초대권	대중 앞에서 장기자랑을 하십시오
    (상대방 여러분들이 채점하신 후, 적당한 상금을 은행에서 지불해 드립니다.)
    6	관광여행
    .제주도로 가십시오
    .제주도에 통행료 지불
    .출발지를 거쳐갈 경우, 월급을 받으시오
    7	과속운전벌금	과속운전을 하였으므로, 벌금 5만원을 내시오
    8	해외유학	학교 등록금을 내시오. 등록금 10만원
    9	연금혜택	은행에서 노후 연금 5만원을 받으시오
    10	이사가시오	뒤로 세 칸 옮기시오
    11	고속도로	출발지까지 곧바로 가시오
    12	우승	자동차 경주에서 참피온이 되었습니다. 상금 10만원
    13	우대권	이 우대권을 가지고 있게 될 경우,
    상대방의 장소를 통행료 없이 머무를 수 있습니다.
    a) 1회 사용후 열쇠 함에 반납하시오
    b) 중요한 순간에만 쓰십시오.
    14	항공여행	콩코드 여객기를 타시고, 타이페이로 가시오
    a) 콩코드에 객실료를 지불하시오
    b) 출발지를 거쳐가실 경우에는 월급을 타시오
    15	건물수리비
    지불	정기적으로 건물을 수리하여야 합니다.
    (각 건물별로 다음과 같이 은행에서 지불하시오)
    호텔(HOTEL) .......... 10만원
    빌딩(BUILDING) ....... 6만원
    별장(VILLA) ......... 3만원
    16	방범비	방범비를 각 건물별로 다음과 같이 은행에 지불하시오
    호텔(HOTEL) .......... 5만원
    빌딩(BUILDING) ....... 3만원
    별장(VILLA) ......... 1만원
    17	우주여행
    초청장	우주항공국에서 우주여행 초청장이 왔습니다.
    a) 무료이므로 콜럼비아호에게 탑승료를 지불하지 않도록 되며,
        우주정류장으로 가십시요.
    b) 출발지를 거치실 경우에는 월급을 받으시오
    18	관광여행	부산으로 가십시오
    . 부산에 통행료 지불
    . 출발지를 거쳐서 갈 경우, 월급을 받으시오
    19	생일을
    축하합니다.	"HAPPY BIRTHDAY TO YOU"
    (해피 버스 데이 투 유!)
    모두에게 생일 축하를 받으시오(축하금 천원)
    20	장학금 혜택	은행에서 장학금 10만원을 받으시오
    21	정기종합
    소득세	종합소득세를 각 건물별로 아래와 같이 지불하시오
    a)호텔(HOTEL) .......... 15만원
    b)빌딩(BUILDING) ....... 10만원
    c)별장(VILLA) ......... 3만원
    22	노벨평화상
    수상	당신은 세계 평화를 위하여 공헌하였으므로,
    은행으로부터 상금 30만원을 배당 받습니다.
    23	관광여행	88년도 올림픽개최지인 서울로 가십시오
    서울올림픽에 통행료 지불(200만원)
    24	반액대매출	당신의 재산 중에서 제일 비싼 곳을 반액으로 은행에 파십시오
    (건물이 지어진 경우에는 함께 처분해야 합니다)
    25	우주여행
    초청장	우주항공국에서 우주여행초청장이 왔습니다.
    A) 무료이므로 콜럼비아호에게 탑승료를 지불하지 않도록 되며,
        우주정류장으로 가십시오
    B) 출발지를 거치실 경우에는 월급을 받으시오
    26	우대권	이 우대권을 가지고 있게 될 경우,
    상대방의 장소를 통행료 없이 머무를 수 있습니다.
    a) 1회 사용후 열쇠 함에 반납하시오
    b) 중요한 순간에만 쓰십시오.
    27	세계일주
    초대권	축하합니다. 현재위치에서부터 한바퀴 돌아오십시오.
    (다른 곳으로는 갈 수 없으며, 현위치로 돌아올 수 있음)
    A) 출발지를 거쳐가면서 월급을 받으시오
    B) 복지기금을 거쳐가면서 모아놓은 기금을 받으시오.
    28	이사가시오	뒤로 두 칸 옮기시오
    29	사회복지기금
    배당	사회복지기금 접수처로 가시오
    출발지를 거치실 경우에는 월급을 받으시오
    30	반액대매출	당신의 재산 중에서 제일 비싼 곳을 반액으로 은행에 파십시오
    (건물이 지어진 경우에는 함께 처분해야 합니다)

*/

const int Black = 0;
const int Blue = 1;
const int Green = 2;
const int BlueGreen = 3;
const int Red = 4;
const int Purple = 5;
const int Yellow = 6;
const int White = 7;
const int Gray = 8;
const int LightBlue = 9;
const int LightGreen = 10;
const int LightBlueGreen = 11;
const int LightRed = 12;
const int LightPurple = 13;
const int LightYellow = 14;
const int LightWhite = 15;

void setColor(int col)
{
    SetConsoleTextAttribute(stdHandle, col);
}

struct land_info
{
    char a[105];
    int buy_price;
    int hotel_price;
    int building_price;
    int house_price;
    int landing_price;
    int hotel_landing_price;
    int building_landing_price;
    int one_house_landing_price;
    int two_house_landing_price;
};

struct player
{
    int double_count, asset, location, muindo, udae, hotel_cnt, building_cnt, house_cnt;
};

struct dicetype
{
    int dice1, dice2;
};

struct dicetype dice()
{
    struct dicetype result;
    result.dice1 = rand() % 6 + 1;
    result.dice2 = rand() % 6 + 1;
    return result;
}


struct land_type
{
    int idx, having, hotel, building, house;
};

struct land_type state[40];
struct player p_info[5];


void golden_key_init()
{
    for (int i = 0; i < 30; i++)
    {
        random_num[i] = (rand() % 30) + 1;
        golden_key_number[i] = random_num[i];
        for (int j = 0; j < i; j++)
        {
            if (random_num[i] == random_num[j]) i--;
        }
    }
}

void gotoxy(SHORT x, SHORT y)
{
    COORD pos = { x, y };
    SetConsoleCursorPosition(stdHandle, pos);
}

void golden_key(int player)
{
    int top = golden_key_number[next];
    next++;
    next %= 30;
    switch(top)
    {
    case 1:
        printf("병원비지불");
        endl();
        printf("병원에서 건강진단을 받았습니다. 병원비 5만원을 은행에 내시오.");
        endl();
        printf("p%d의 잔액이 50000원 감소하였습니다!", player);
        endl();
        p_info[player].asset -= 50000;
        break;

    case 2:
        printf("복권당첨");
        endl();
        printf("축하합니다.복권에 당첨되었습니다. 당첨금 20만원.");
        endl();
        printf("p%d의 잔액이 200000원 증가합니다!", player);
        endl();
        p_info[player].asset += 200000;
        break;

    case 3:
        printf("무인도탈출");
        endl();
        printf("(특수무전기)");
        endl();
        printf("a) 무인도에 갖혀 있을 때에 사용할 수 있음");
        endl();
        printf("b) 1회 사용후 반납하며, 타인에게 팔 수 있음.");
        endl();
        printf("c) 특수무전기 가격은 20만원입니다.");
        endl();

        p_info[player].muindo = 1;
        break;

    case 4:
        printf("무인도로 가시오");
        endl();
        printf("폭풍을 만났습니다.");
        endl();
        printf("무인도로 곧장 가시되, 출발지를 지날 때도 월급을 받지 못합니다.");
        endl();
        p_info[player].location = 11;
        break;

    case 5:
        printf("파티초대권");
        endl();
        printf("대중 앞에서 장기자랑을 하십시오");
        endl();
        printf("(상대방 여러분들이 채점하신 후, 적당한 상금을 은행에서 지불해 드립니다.)");
        endl();
        printf("추가하실 상금을 입력하십시오.");
        endl();
        int money_adding;
        scanf("%d", &money_adding);
        p_info[player].asset += money_adding;
        break;

    case 6:
        printf("관광여행");
        endl();
        printf("제주도로 가십시오");
        endl();
        printf("    .제주도에 통행료 지불");
        endl();
        printf("    .출발지를 거쳐갈 경우, 월급을 받으시오");
        endl();
        if(p_info[player].location > 6)
        {
            printf("출발지 경유 월급 20만원");
            endl();
            p_info[player].asset += 200000;
        }
        p_info[player].location = 6;
        break;

    case 7:
        printf("과속운전벌금");
        endl();
        printf("과속운전을 하였으므로, 벌금 5만원을 내시오");
        endl();
        printf("p%d의 잔액이 50000원 감소하였습니다!", player);
        endl();
        p_info[player].asset -= 50000;
        break;

    case 8:
        printf("해외유학");
        endl();
        printf("학교 등록금을 내시오. 등록금 10만원");
        endl();
        printf("p%d의 잔액이 100000원 감소하였습니다!", player);
        endl();
        p_info[player].asset -= 100000;
        break;

    case 9:
        printf("연금혜택");
        endl();
        printf("은행에서 노후 연금 5만원을 받으시오");
        endl();
        printf("p%d의 잔액이 50000원 증가합니다!", player);
        endl();
        p_info[player].asset += 50000;
        break;

    case 10:
        printf("이사가시오");
        endl();
        printf("뒤로 세 칸 옮기시오");
        endl();
        p_info[player].location -= 3;
        if(p_info[player].location <= 0) p_info[player].location += 40;
        if(p_info[player].location == 1)
        {
            printf("출발지 경유 월급 20만원");
            endl();
            p_info[player].asset += 200000;
        }
        break;

    case 11:
        printf("고속도로");
        endl();
        printf("출발지까지 곧바로 가시오");
        endl();
        p_info[player].location = 1;
        printf("출발지 경유 월급 20만원");
        endl();
        p_info[player].asset += 200000;
        break;

    case 12:
        printf("우승");
        endl();
        printf("자동차 경주에서 참피온이 되었습니다. 상금 10만원");
        endl();
        printf("p%d의 잔액이 100000원 증가합니다!", player);
        endl();
        p_info[player].asset += 100000;
        break;

    case 13:
        printf("우대권 이 우대권을 가지고 있게 될 경우,");
        endl();
        printf("상대방의 장소를 통행료 없이 머무를 수 있습니다.");
        endl();
        printf("a) 1회 사용후 열쇠 함에 반납하시오");
        endl();
        printf("b) 중요한 순간에만 쓰십시오.");
        endl();
        p_info[player].udae++;
        break;

    case 14:
        printf("항공여행");
        endl();
        printf("콩코드 여객기를 타시고, 타이페이로 가시오");
        endl();
        printf("a) 콩코드에 객실료를 지불하시오");
        endl();
        printf("b) 출발지를 거쳐가실 경우에는 월급을 타시오");
        endl();
        if(p_info[player].location > 16)
        {
            printf("출발지 경유 월급 20만원");
            endl();
            p_info[player].asset += 200000;
        }
        p_info[player].location = 16;
        break;

    case 15:
        printf("건물수리비");
        endl();
        printf("지불\t정기적으로 건물을 수리하여야 합니다.");
        endl();
        printf("(각 건물별로 다음과 같이 은행에서 지불하시오)");
        endl();
        printf("호텔(HOTEL) .......... 10만원");
        endl();
        printf("빌딩(BUILDING) ....... 6만원");
        endl();
        printf("별장(VILLA) ......... 3만원");
        endl();
        break;


    case 16:

        break;

    case 17:

        break;

    case 18:

        break;

    case 19:

        break;

    case 20:

        break;

    case 21:

        break;

    case 22:

        break;

    case 23:

        break;

    case 24:

        break;

    case 25:

        break;

    case 26:

        break;

    case 27:

        break;

    case 28:

        break;

    case 29:

        break;

    case 30:

        break;
    }
}


struct land_info basic_land[50] =
{
    {"타이베이", 50000, 250000,150000, 50000, 2000, 250000,90000, 10000,30000},
    {"베이징", 80000, 250000,150000, 50000, 4000, 450000,180000, 20000,60000},
    {"마닐라", 80000, 250000,150000, 50000, 4000, 450000,180000, 20000,60000},
    {"제주도",200000,-1,-1,-1,300000,-1,-1,-1,-1},
    {"싱가포르", 100000, 250000,150000, 50000, 6000, 550000,270000, 30000,90000},
    {"카이로", 100000, 250000,150000, 50000, 6000, 550000,270000, 30000,90000},
    {"이스탄불", 120000, 250000,150000, 50000, 8000, 600000,300000, 40000,100000},
    {"아테네", 140000, 500000,300000, 100000, 10000, 750000,450000, 50000,150000},
    {"코펜하겐", 160000,500000,300000,100000,10000,750000,450000,50000,150000},
    {"콩코드 여객기",200000,-1,-1,-1,300000,-1,-1,-1,-1},
    {"스톡홀롬", 160000, 500000,300000, 100000, 12000, 900000,500000, 60000,180000},
    {"베른", 180000, 500000,300000, 100000, 14000, 950000,550000, 70000,200000},
    {"베를린", 180000, 500000,300000, 100000, 14000, 950000,550000, 70000,200000},
    {"오타와", 200000, 500000,300000, 100000, 16000, 1000000,600000, 80000,220000},
    {"부에노스 아이레스", 220000, 750000,400000, 150000, 18000, 1050000,700000, 90000,250000},
    {"상파울루", 240000, 750000,450000, 150000, 20000, 1100000,750000, 100000,300000},
    {"시드니", 240000, 750000,450000, 150000, 20000, 1100000,750000, 100000,300000},
    {"부산", 500000, -1, -1, -1, 600000, -1, -1, -1, -1},
    {"하와이", 260000, 750000,450000, 150000, 22000, 1150000,800000, 110000,330000},
    {"리스본", 260000, 750000,450000, 150000, 22000, 1150000,800000, 110000,330000},
    {"퀸 엘리자베스 호",300000,-1,-1,-1,250000,-1,-1,-1,-1},
    {"마드리드", 280000, 750000,450000, 150000, 24000, 1200000,850000, 120000,360000},
    {"도쿄", 300000, 1000000,600000, 200000, 26000, 1270000,900000, 130000,390000},
    {"컬럼비아 호", 450000,-1,-1,-1, 300000,-1,-1,-1,-1},
    {"파리", 320000, 1000000,600000, 200000, 28000, 1400000,1000000, 150000,450000},
    {"로마", 320000, 1000000,600000, 200000, 28000, 1400000,1000000, 150000,450000},
    {"런던", 350000, 1000000,600000, 200000, 35000, 1500000,1100000, 170000,500000},
    {"뉴욕", 350000, 1000000,600000, 200000, 35000, 1500000,1100000, 170000,500000},
    {"서울", 1000000,-1,-1,-1, 2000000,-1,-1,-1,-1}
};


void board_event(int player)
{
    int loc = p_info[player].location;
    switch (loc)
    {
    case 3:
    case 8:
    case 13:
    case 18:
    case 23:
    case 36:
        printf("황금열쇠 칸입니다!");
        endl();
        golden_key(player);
        break;

    case 11:
        printf("무인도입니다! 3턴 동안 움직일 수 없습니다.");
        endl();
        break;

    case 39:
        printf("사회복지기금 접수처입니다. 10만원 기부!");
        endl();
        p_info[player].asset -= 100000;
        break;

    case 31:
        printf("우주여행 칸입니다! 아무 타일로 이동할 수 있습니다.");
        endl();
        int target;
        printf("이동할 칸 번호를 입력하세요 (1~40): ");
        endl();
        scanf("%d", &target);
        endl();
        p_info[player].location = target;
        board_event(player);
        break;

    default:
        endl();
        printf("도시에 도착했습니다. 소유 상태 확인 중...");


        if (state[loc].idx == 0)
        {
            endl();
            printf("구매 가능한 땅입니다. 구입하시겠습니까? (y/n)");
            endl();
            printf(" < ");

            char buy;
            scanf("%c", &buy);
            getchar();
            if (buy == 'y')
            {
                if(p_info[player].asset < basic_land[state[loc].idx].buy_price)
                {
                    endl();
                    printf("돈이 부족합니다.");
                }
                else
                {
                    endl();
                    printf("구입 완료!");
                    endl();

                    state[loc].idx = player;
                }
            }
        }
        else if (state[loc].idx != player)
        {
            endl();
            printf("상대방이 소유한 땅입니다. 통행료를 지불합니다.");
            int toll = 50000; // 통행료
            p_info[player].asset -= toll;
            p_info[state[loc].idx].asset += toll;
        }
        else
        {
            endl();
            printf("당신이 소유한 땅입니다.");
            endl();
            printf("무엇을 하시겠습니까?");
            endl();
        }
        break;
    }
    Sleep(1500);
}

void moveto(int meter, int player_info)
{
    p_info[player_info].location += meter;
    if(p_info[player_info].location > BOARD_SIZE)
    {
        printf("출발지 경유 월급 20만원");
        endl();
        p_info[player_info].asset += 200000;
        p_info[player_info].location -= 40;
    }
    board_event(player_info);
}

void move_printing(int meter, int player_info)
{
    printf("%d칸 이동",meter);
    endl();

    endl();
    Sleep(1000);
    moveto(meter, player_info);
}


void p1_turn()
{
    cur = 0;
    struct dicetype d = dice();
    gotoxy(145, 0);
    ClearConsoleRect(stdHandle, 145, 0, 245, 100);
    endl();

    printf("p1 당신의 차례입니다!");
    endl();
    printf("주사위 굴리는 중...");
    endl();
    Sleep(1500);

    printf("첫 번째 주사위: %d, 두 번째 주사위: %d",d.dice1, d.dice2);
    endl();

    if(d.dice1 == d.dice2)
    {
        if(p_info[1].double_count == 3)
        {
            printf("더블 3번! 무인도 이동");
            endl();
            p_info[1].location = 11;
        }
        else
        {
            printf("더블! ");
            endl();
            move_printing(d.dice1 + d.dice2, 1);
            p_info[1].double_count++;
            p1_turn();
        }
    }
    else
    {

        p_info[1].double_count = 0;
        move_printing(d.dice1 + d.dice2, 1);
    }
    endl();

    return;
}

void p2_turn()
{
    cur = 0;
    ClearConsoleRect(stdHandle, 145, 0, 245, 100);
    gotoxy(145, 0);
    struct dicetype d = dice();
    endl();
    printf("p2 당신의 차례입니다!");
    endl();
    printf("주사위 굴리는 중...");
    endl();

    Sleep(1500);

    printf("첫 번째 주사위 %d, 두 번째 주사위 %d",d.dice1, d.dice2);
    endl();
    if(d.dice1 == d.dice2)
    {
        if(p_info[2].double_count == 3)
        {
            printf("더블 3번! 무인도 이동");
            endl();
            FillConsoleOutputCharacter(stdHandle, ' ', 100 * 100, (COORD){ 0, 145 }, &dw);
            SetConsoleCursorPosition(stdHandle, (COORD){ 0, 145 });
            p_info[2].location = 11;
        }
        else
        {
            printf("더블! ");
            endl();
            move_printing(d.dice1 + d.dice2, 2);
            p_info[2].double_count++;
            FillConsoleOutputCharacter(stdHandle, ' ', 100 * 100, (COORD){ 0, 145 }, &dw);
            SetConsoleCursorPosition(stdHandle, (COORD){ 0, 145 });
            p2_turn();
        }
    }
    else
    {
        p_info[2].double_count = 0;
        move_printing(d.dice1 + d.dice2, 2);
    }
    SetConsoleCursorPosition(stdHandle, (COORD){ 0, 145 });
    FillConsoleOutputCharacter(stdHandle, ' ', 100 * 100, (COORD){ 0, 145 }, &dw);
    SetConsoleCursorPosition(stdHandle, (COORD){ 0, 145 });
}

void onep()
{
    pan();
    golden_key_init();
    while(1)
    {
        p1_turn();
        p2_turn();
    }
}

void twop()
{

}

void game()
{
    printf("\nTo play 1p press 1\nTo play 2p press 2\nTo exit press 3\n> ");
    int playstyle;
    scanf("%d", &playstyle);
    if(playstyle == 1)
    {
        SetConsoleCursorPosition(stdHandle, (COORD){ 0, 0 });
        FillConsoleOutputCharacter(stdHandle, ' ', 50 * 50, (COORD){ 0, 0 }, &dw);
        getchar();
        onep();
    }
    else if(playstyle == 2)
    {
        twop();
    }
    else
    {
        return;
    }
}

void start_page()
{
    p_info[1].asset = 3430000;
    p_info[2].asset = 3430000;
    setColor(LightBlueGreen);
    printf("****************************************************************\n");
    printf(" _____ __    _____ _____    _____ _____ _____ _____ __    _____ \n");
    printf("| __  |  |  |  |  |   __|  |     |  _  | __  | __  |  |  |   __|\n");
    printf("| __ -|  |__|  |  |   __|  | | | |     |    -| __ -|  |__|   __|\n");
    printf("|_____|_____|_____|_____|  |_|_|_|__|__|__|__|_____|_____|_____|\n\n");
    printf("****************************************************************\n");
    setColor(White);
    printf("\n");
    printf("Welcome to BLUEMARBLE!\nBLUEMARBLE is a simple online game based on monopoly, you can play a game with one computer or you can play it with two computer.\n\nTo start a game press 1. To exit a game press 2\n");
    printf("start the game : 1\nend the game : 2\n> ");

    int st_ex;
    scanf("%d", &st_ex);
    if(st_ex == 1)
    {
        game();
    }
    else
    {
        printf("\nThank You for Playing the Game!\nexiting program");
        return;
    }
}



int main()
{
    srand(time(NULL));

    system("chcp 65001 > nul");
    system("mode con: cols=500 lines=500");

    CONSOLE_CURSOR_INFO cursorInfo = { 0, };

    cursorInfo.dwSize = 1;
    cursorInfo.bVisible = 0;
    SetConsoleCursorInfo(stdHandle, &cursorInfo);
    HWND hwnd = GetConsoleWindow();
    Sleep(10);
    HWND owner = GetWindow(hwnd, GW_OWNER);


    if (owner == NULL)
    {
        // Windows 10
        SetWindowPos(hwnd, NULL, 0, 0, 1920, 1080, SWP_NOZORDER|SWP_NOMOVE);
    }
    else
    {
        // Windows 11
        SetWindowPos(owner, NULL, 0, 0, 1920, 1080, SWP_NOZORDER|SWP_NOMOVE);
    }
    start_page();
    return 0;
}
