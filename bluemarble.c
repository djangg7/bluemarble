#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <stdbool.h>

#define stdHandle GetStdHandle(STD_OUTPUT_HANDLE)
#ifndef WINVER
#define WINVER 0x0501
#endif
#ifndef _WIN32_WINNT
#define _WIN32_WINNT  0x0501
#endif

#ifndef CONSOLE_FULLSCREEN_MODE
#define CONSOLE_FULLSCREEN_MODE 1
#endif

#ifdef _MSC_VER
int ide = 1;
#elif __MINGW32__ || __MINGW64__ || __GNUC__
int ide = 2;
#else
int ide = 0;
#endif

enum {
    Black = 0,
    Blue, Green, BlueGreen, Red, Purple, Yellow, White, Gray,
    LightBlue, LightGreen, LightBlueGreen, LightRed, LightPurple, LightYellow, LightWhite
};

unsigned long dw;
int comment_start = 57;
int board[15][15];
const int BOARD_SIZE = 40;
int golden_key_number[105];
int* screen;
int next;

/// ⬛ = p1, ⚫ = p2;

typedef BOOL (WINAPI* pSCFEX)(HANDLE, BOOL, PCONSOLE_FONT_INFOEX);
typedef BOOL (WINAPI* pGCFEX)(HANDLE, BOOL, PCONSOLE_FONT_INFOEX);
short cur = 0;

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

void setColor(int col)
{
    SetConsoleTextAttribute(stdHandle, col);
}

void endl()
{
    SetConsoleCursorPosition(stdHandle, (COORD){ comment_start, cur });
    cur++;
}

void pan()
{
    SetConsoleCursorPosition(stdHandle, (COORD){0,0});
    setColor(Green);

    printf("+----+----+----+----+----+----+----+----+----+----+----+\n");
    printf("| GO | TP | GK | BJ | MN | JJ | SP | GK | CR | IS | JA |\n");
    printf("+----+----+----+----+----+----+----+----+----+----+----+\n");
    printf("| SL |                                            | AT |\n");
    printf("+----+                                            +----+\n");
    printf("| TX |                                            | GK |\n");
    printf("+----+                                            +----+\n");
    printf("| NY |                                            | CP |\n");
    printf("+----+                                            +----+\n");
    printf("| LD |                                            | ST |\n");
    printf("+----+                                            +----+\n");
    printf("| GK |                                            | CO |\n");
    printf("+----+                                            +----+\n");
    printf("| RM |                                            | BR |\n");
    printf("+----+                                            +----+\n");
    printf("| PR |                                            | GK |\n");
    printf("+----+                                            +----+\n");
    printf("| COL|                                            | BL |\n");
    printf("+----+                                            +----+\n");
    printf("| TK |                                            | OT |\n");
    printf("+----+----+----+----+----+----+----+----+----+----+----+\n");
    printf("| SP | MD | QE | LB | HW | BS | SY | SA | GK | BA | GT |\n");
    printf("+----+----+----+----+----+----+----+----+----+----+----+\n");
}

static int* fullscreen(int minWidth, int minHeight) {

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    HWND Hwnd = GetForegroundWindow();
    int x = GetSystemMetrics(SM_CXSCREEN);
    int y = GetSystemMetrics(SM_CYSCREEN);
    LONG winstyle = GetWindowLong(Hwnd, GWL_STYLE);
    DWORD mode = 0;
    PCOORD pMode = 0;
    int* scale = (int*)malloc(sizeof(int) * 2);
    const int maxTries = 40;
    int tries = 0;

    COORD newSize = { (SHORT)minWidth, (SHORT)minHeight };

    if (ide == 1) goto VisualStudio;
    else if (ide == 2) goto CodeBlocks;
    else goto Others;

VisualStudio:

    SetWindowLong(Hwnd, GWL_STYLE, (winstyle | WS_POPUP | WS_MAXIMIZE) & ~WS_CAPTION & ~WS_THICKFRAME & ~WS_BORDER);

    SetWindowPos(
        Hwnd,
        HWND_TOP,
        0, 0,
        x, y,
        0
    );

    goto Exit;

CodeBlocks:

    SetConsoleScreenBufferSize(hOut, newSize);

    winstyle &= ~(WS_BORDER | WS_DLGFRAME | WS_CAPTION | WS_THICKFRAME);
    winstyle |= WS_POPUP | WS_VISIBLE;
    SetWindowLong(Hwnd, GWL_STYLE, winstyle);

    SetWindowPos(
        Hwnd,
        HWND_TOPMOST,
        0, 0,
        x, y,
        SWP_FRAMECHANGED | SWP_SHOWWINDOW
    );

    goto Exit;

Others:

    if (!SetConsoleDisplayMode(hOut, CONSOLE_FULLSCREEN_MODE, pMode)) {
        winstyle &= ~(WS_BORDER | WS_DLGFRAME | WS_CAPTION | WS_THICKFRAME);
        winstyle |= WS_POPUP | WS_VISIBLE;
        SetWindowLong(Hwnd, GWL_STYLE, winstyle);
        SetWindowPos(
            Hwnd,
            HWND_TOPMOST,
            0, 0,
            x, y,
            SWP_FRAMECHANGED | SWP_SHOWWINDOW
        );
    }

    goto Exit;

Exit:

    while (tries++ < maxTries) {
        if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
            int width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
            int height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
            if (width < minWidth || height < minHeight) Sleep(50);
            else {
                scale[0] = width;
                scale[1] = height;
                return scale;
            }
        }
    }

    int* screen = (int*)malloc(sizeof(int) * 2);
    screen[0] = -1;
    screen[1] = -1;
    return screen;
}

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

void gotoxy(SHORT x, SHORT y)
{
    COORD pos = { x, y };
    SetConsoleCursorPosition(stdHandle, pos);
}

void golden_key_init()
{
    for (int i = 0; i < 30; i++)
    {
        int random_num = (rand() % 30) + 1;
        bool check = false;
        for (int j = 0; j < i; j++)
        {
            if (random_num == golden_key_number[j]) check = true;
        }
        if (check) {
            i--;
            continue;
        }
        golden_key_number[i] = random_num;
    }
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
        printf(" > ");
        scanf("%d", &money_adding);
        p_info[player].asset += money_adding;
        break;

    case 6:
        printf("관광여행");
        endl();
        printf("제주도로 가십시오");
        endl();
        printf("제주도에 통행료 지불");
        endl();
        printf("출발지를 거쳐갈 경우, 월급을 받으시오");
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
        Sleep(2000);
        break;

    case 11:
        printf("무인도입니다! 3턴 동안 움직일 수 없습니다.");
        Sleep(2000);
        endl();
        break;

    case 39:
        printf("사회복지기금 접수처입니다. 10만원 기부!");
        Sleep(2000);
        endl();
        p_info[player].asset -= 100000;
        break;

    case 31:
        printf("우주여행 칸입니다! 아무 타일로 이동할 수 있습니다.");
        endl();
        int target;
        printf("이동할 칸 번호를 입력하세요 (1~40): ");
        Sleep(2000);
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
            printf(" > ");

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
                    Sleep(1000);

                    state[loc].idx = player;
                    p_info[player].asset -= basic_land[state[loc].idx].buy_price;

                    gotoxy(0, 24);
                    printf("p1 잔액 : %d \t p2 잔액 : %d", p_info[1].asset, p_info[2].asset);
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
    ClearConsoleRect(stdHandle, comment_start, 0, screen[0] - 1, 100);
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


void player_turn(int player)
{
    cur = 0;
    struct dicetype d = dice();
    gotoxy(comment_start, 0);
    endl();

    printf("p%d 당신의 차례입니다!", player);
    Sleep(1500);
    endl();
    printf("주사위 굴리는 중...");
    endl();
    Sleep(1500);

    printf("첫 번째 주사위: %d, 두 번째 주사위: %d",d.dice1, d.dice2);
    endl();

    if(d.dice1 == d.dice2)
    {
        p_info[player].double_count++;
        if(p_info[player].double_count == 3)
        {
            printf("더블 3번! 무인도 이동");
            endl();
            p_info[player].location = 11;
        }
        else
        {
            printf("더블! ");
            endl();
            move_printing(d.dice1 + d.dice2, player);
            player_turn(player);
        }
    }
    else
    {

        p_info[player].double_count = 0;
        move_printing(d.dice1 + d.dice2, player);
    }

    SetConsoleCursorPosition(stdHandle, (COORD){ 0, comment_start });
    FillConsoleOutputCharacter(stdHandle, ' ', 100 * 100, (COORD){ 0, comment_start }, &dw);
    SetConsoleCursorPosition(stdHandle, (COORD){ 0, comment_start });
    return;
}


void onep()
{
    pan();
    golden_key_init();
    while(1)
    {
        gotoxy(0, 24);
        printf("p1 잔액 : %d \t p2 잔액 : %d", p_info[1].asset, p_info[2].asset);
        player_turn(1);
        player_turn(2);
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

    CONSOLE_CURSOR_INFO cursorInfo = { 0, };

    cursorInfo.dwSize = 1;
    cursorInfo.bVisible = 0;
    SetConsoleCursorInfo(stdHandle, &cursorInfo);
    HWND hwnd = GetConsoleWindow();
    Sleep(10);
    HWND owner = GetWindow(hwnd, GW_OWNER);

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        int width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        int height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
        screen = fullscreen(width + 1, height + 1);
    }
    else screen = fullscreen(0, 0);

    start_page();
    return 0;
}
