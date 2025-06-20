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


/*
    남은 숙제
    2. 건물 짓기 (중요도 높음)
    4. 무인도 (중요도 높음) 완료
    5. 무인도 탈출 사용 (중요도 높음) 완료
    8. Landing_price += building / hotel / house price (2 선행 되어야 함)
    1. 파산시 집 팔기 (중요도 중간 2 선행 되어야 함)
    3. 황금 열쇠 (중요도 중간 땜빵 가능)
    6. 입력 버퍼 초기화 (중요도 낮음)
    7. 게임 설명 (중요도 낮음, 재량임)
*/

/// COLORS DEFINING
#define RESET   "\033[0m"
#define BLACK   "\033[0m\033[30m"      /* Black */
#define RED     "\033[0m\033[31m"      /* Red */
#define GREEN   "\033[0m\033[32m"      /* Green */
#define YELLOW  "\033[0m\033[33m"      /* Yellow */
#define BLUE    "\033[0m\033[34m"      /* Blue */
#define MAGENTA "\033[0m\033[35m"      /* Magenta */
#define CYAN    "\033[0m\033[36m"      /* Cyan */
#define WHITE   "\033[0m\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */
#define REDB    "\033[0m\033[41m"
#define BLUEB    "\033[0m\033[44m"


unsigned long dw;
int comment_start = 57;
int board[15][15];
const int BOARD_SIZE = 40;
int golden_key_number[105];
int* screen;
int next;
int sahwei = 0;

typedef BOOL (WINAPI* pSCFEX)(HANDLE, BOOL, PCONSOLE_FONT_INFOEX);
typedef BOOL (WINAPI* pGCFEX)(HANDLE, BOOL, PCONSOLE_FONT_INFOEX);
short cur = 0;
void board_event(int player);

struct land_info
{
    char a[105];
    char b[105];
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

struct land_info basic_land[50] =
{
    {"타이베이", "TP", 50000, 250000,150000, 50000, 2000, 250000,90000, 10000,30000},
    {"베이징","BJ", 80000, 250000,150000, 50000, 4000, 450000,180000, 20000,60000},
    {"마닐라", "MN", 80000, 250000,150000, 50000, 4000, 450000,180000, 20000,60000},
    {"제주도", "JJ",200000,-1,-1,-1,300000,-1,-1,-1,-1},
    {"싱가포르", "SP", 100000, 250000,150000, 50000, 6000, 550000,270000, 30000,90000},
    {"카이로", "CR",100000, 250000,150000, 50000, 6000, 550000,270000, 30000,90000},
    {"이스탄불", "IS", 120000, 250000,150000, 50000, 8000, 600000,300000, 40000,100000},
    {"아테네", "AT",140000, 500000,300000, 100000, 10000, 750000,450000, 50000,150000},
    {"코펜하겐", "CP", 160000,500000,300000,100000,10000,750000,450000,50000,150000},
    {"스톡홀롬", "ST", 160000, 500000,300000, 100000, 12000, 900000,500000, 60000,180000},
    {"콩코드 여객기","CO",200000,-1,-1,-1,300000,-1,-1,-1,-1},
    {"베른", "BR", 180000, 500000,300000, 100000, 14000, 950000,550000, 70000,200000},
    {"베를린", "BL",180000, 500000,300000, 100000, 14000, 950000,550000, 70000,200000},
    {"오타와", "OT",200000, 500000,300000, 100000, 16000, 1000000,600000, 80000,220000},
    {"부에노스 아이레스","BA", 220000, 750000,400000, 150000, 18000, 1050000,700000, 90000,250000},
    {"상파울루", "SA",240000, 750000,450000, 150000, 20000, 1100000,750000, 100000,300000},
    {"시드니", "SY",240000, 750000,450000, 150000, 20000, 1100000,750000, 100000,300000},
    {"부산", "BS",500000, -1, -1, -1, 600000, -1, -1, -1, -1},
    {"하와이", "HW", 260000, 750000,450000, 150000, 22000, 1150000,800000, 110000,330000},
    {"리스본","LB",260000, 750000,450000, 150000, 22000, 1150000,800000, 110000,330000},
    {"퀸 엘리자베스 호", "QE",300000,-1,-1,-1,250000,-1,-1,-1,-1},
    {"마드리드", "MD", 280000, 750000,450000, 150000, 24000, 1200000,850000, 120000,360000},
    {"도쿄", "TK", 300000, 1000000,600000, 200000, 26000, 1270000,900000, 130000,390000},
    {"컬럼비아 호", "CL", 450000,-1,-1,-1, 300000,-1,-1,-1,-1},
    {"파리", "PR", 320000, 1000000,600000, 200000, 28000, 1400000,1000000, 150000,450000},
    {"로마", "RM",320000, 1000000,600000, 200000, 28000, 1400000,1000000, 150000,450000},
    {"런던", "LD",350000, 1000000,600000, 200000, 35000, 1500000,1100000, 170000,500000},
    {"뉴욕", "NY",350000, 1000000,600000, 200000, 35000, 1500000,1100000, 170000,500000},
    {"서울", "SL",1000000,-1,-1,-1, 2000000,-1,-1,-1,-1}
};

struct box_coord
{
    int x, y;
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
    int idx, having, hotel, building, house, own;
};

struct box_coord CRD[] =
{
    {0,0},{2,1}, {7,1}, {12,1}, {17,1}, {22,1}, {27,1}, {32,1}, {37,1},
    {42,1}, {47,1}, {52,1}, {52,3}, {52,5}, {52,7}, {52,9}, {52,11},
    {52,13}, {52,15}, {52,17}, {52,19}, {52,21}, {47,21}, {42,21}, {37,21},
    {32,21}, {27,21}, {22,21}, {17,21}, {12,21}, {7,21}, {2,21}, {2,19},
    {2,17}, {2,15}, {2,13}, {2,11}, {2,9}, {2,7}, {2,5}, {2,3}
};

struct player
{
    int double_count, asset, location, muindo_exit, udae, hotel_cnt, building_cnt, house_cnt, m;
};
struct land_type state[40];
struct player p_info[5];

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

void endl()
{
    SetConsoleCursorPosition(stdHandle, (COORD)
    {
        comment_start, cur
    });
    cur++;
}

void pan()
{
    SetConsoleCursorPosition(stdHandle, (COORD)
    {
        0,0
    });

    printf(WHITE);
    printf(BOLDBLUE "+----+" WHITE "----+----+----+----+----+----+----+----+----" BOLDBLUE "+----+\n");
    printf(BOLDBLUE "|" WHITE " GO " BOLDBLUE"|" WHITE " TP | GK | BJ | MN | JJ | SP | GK | CR | IS " BOLDBLUE "|" WHITE" JA " BOLDBLUE"|\n");
    printf(BOLDBLUE "+----+" BOLDRED "----" YELLOW "+----+" BOLDRED "----+----" BOLDCYAN "+----+" BOLDRED "----" YELLOW "+----+" BOLDRED "----+----" BOLDBLUE "+----+\n");
    printf(WHITE "| SL " BOLDCYAN "|" WHITE "                                            " GREEN "|" WHITE " AT |\n");
    printf(WHITE "+----" BOLDWHITE "+" WHITE "                                            " YELLOW "+" WHITE "----+\n");
    printf(WHITE "| TX " BOLDWHITE "|" WHITE "                                            " YELLOW "|" WHITE " GK |\n");
    printf(WHITE "+----" BOLDWHITE "+" WHITE "                                            " YELLOW "+" WHITE "----+\n");
    printf(WHITE "| NY " BOLDBLACK "|" WHITE "                                            " GREEN "|" WHITE " CP |\n");
    printf(WHITE "+----" BOLDBLACK "+" WHITE "                                            " GREEN "+" WHITE "----+\n");
    printf(WHITE "| LD " BOLDBLACK "|" WHITE "                                            " GREEN "|" WHITE " ST |\n");
    printf(WHITE "+----" YELLOW "+" WHITE "                                            " BOLDCYAN "+" WHITE "----+\n");
    printf(WHITE "| GK " YELLOW "|" WHITE "                                            " BOLDCYAN "|" WHITE " CO |\n");
    printf(WHITE "+----" YELLOW "+" WHITE "                                            " BOLDCYAN "+" WHITE "----+\n");
    printf(WHITE "| RM " BOLDBLACK "|" WHITE "                                            " GREEN "|" WHITE " BR |\n");
    printf(WHITE "+----" BOLDBLACK "+" WHITE "                                            " YELLOW "+" WHITE "----+\n");
    printf(WHITE "| PR " BOLDBLACK "|" WHITE "                                            " YELLOW "|" WHITE " GK |\n");
    printf(WHITE "+----" BOLDCYAN "+" WHITE "                                            " YELLOW "+" WHITE "----+\n");
    printf(WHITE "| CL " BOLDCYAN "|" WHITE "                                            " GREEN "|" WHITE " BL |\n");
    printf(WHITE "+----" BOLDCYAN "+" WHITE "                                            " GREEN "+" WHITE "----+\n");
    printf(WHITE "| TK " BOLDBLACK "|" WHITE "                                            " GREEN "|" WHITE " OT |\n");
    printf(BOLDBLUE "+----+" BOLDYELLOW "----" BOLDCYAN "+----+" BOLDYELLOW "----+----" BOLDCYAN "+----+" BOLDYELLOW "----+----" YELLOW "+----+" BOLDYELLOW "----" BOLDBLUE "+----+\n");
    printf(BOLDBLUE "|" WHITE" SP " BOLDBLUE"|" WHITE " MD | QE | LB | HW | BS | SY | SA | GK | BA " BOLDBLUE "|" WHITE" GT " BOLDBLUE"|\n");
    printf(BOLDBLUE "+----+" WHITE "----+----+----+----+----+----+----+----+----" BOLDBLUE "+----+\n");
}

void gotoxy(SHORT x, SHORT y)
{
    COORD pos = { x, y };
    SetConsoleCursorPosition(stdHandle, pos);
}

void color(int player, int c)
{
    int loc = p_info[player].location;
    gotoxy(CRD[loc].x, CRD[loc].y);
    switch (loc)
    {
    case 1:
        if(c == 1)
        {
            if(p_info[1].location == p_info[2].location){
                if(player == 1){
                    printf(BLUE"GO", basic_land[state[loc].idx].b);
                }
                else{
                    printf(RED"GO", basic_land[state[loc].idx].b);
                }
            }
            else{
                printf(WHITE"GO", basic_land[state[loc].idx].b);
            }
        }
        else if(c == 2)
        {
            printf(RED"GO");
        }
        else
        {
            printf(BLUE"GO");
        }
        break;

    case 3:
    case 8:
    case 13:
    case 18:
    case 23:
    case 36:
        if(c == 1)
        {
            if(p_info[1].location == p_info[2].location){
                if(player == 1){
                    printf(BLUE"GK", basic_land[state[loc].idx].b);
                }
                else{
                    printf(RED"GK", basic_land[state[loc].idx].b);
                }
            }
            else{
                printf(WHITE"GK", basic_land[state[loc].idx].b);
            }
        }
        else if(c == 2)
        {
            printf(RED"GK");
        }
        else
        {
            printf(BLUE"GK");
        }
        break;

    case 11:

        if(c == 1)
        {
            if(p_info[1].location == p_info[2].location){
                if(player == 1){
                    printf(BLUE"JA", basic_land[state[loc].idx].b);
                }
                else{
                    printf(RED"JA", basic_land[state[loc].idx].b);
                }
            }
            else{
                printf(WHITE"JA", basic_land[state[loc].idx].b);
            }
        }
        else if(c == 2)
        {
            printf(RED"JA");
        }
        else
        {
            printf(BLUE"JA");
        }
        break;

    case 39:
        if(c == 1)
        {
            if(p_info[1].location == p_info[2].location){
                if(player == 1){
                    printf(BLUE"TX", basic_land[state[loc].idx].b);
                }
                else{
                    printf(RED"TX", basic_land[state[loc].idx].b);
                }
            }
            else{
                printf(WHITE"TX", basic_land[state[loc].idx].b);
            }
        }
        else if(c == 2)
        {
            printf(RED"TX");
        }
        else
        {
            printf(BLUE"TX");
        }
        break;

    case 21:
        if(c == 1)
        {
            if(p_info[1].location == p_info[2].location){
                if(player == 1){
                    printf(BLUE"GT", basic_land[state[loc].idx].b);
                }
                else{
                    printf(RED"GT", basic_land[state[loc].idx].b);
                }
            }
            else{
                printf(WHITE"GT", basic_land[state[loc].idx].b);
            }
        }
        else if(c == 2)
        {
            printf(RED"GT");
        }
        else
        {
            printf(BLUE"GT");
        }
        break;

    case 31:
        if(c == 1)
        {
            if(p_info[1].location == p_info[2].location){
                if(player == 1){
                    printf(BLUE"SP", basic_land[state[loc].idx].b);
                }
                else{
                    printf(RED"SP", basic_land[state[loc].idx].b);
                }
            }
            else{
                printf(WHITE"SP", basic_land[state[loc].idx].b);
            }
        }
        else if(c == 2)
        {
            printf(RED"SP");
        }
        else
        {
            printf(BLUE"SP");
        }
        break;

    default:
        if(c == 1)
        {
            if(p_info[1].location == p_info[2].location){
                if(player == 1){
                    printf(BLUE"%s", basic_land[state[loc].idx].b);
                }
                else{
                    printf(RED"%s", basic_land[state[loc].idx].b);
                }
            }
            else{
                printf(WHITE"%s", basic_land[state[loc].idx].b);
            }
        }
        else if(c == 2)
        {
            printf(RED"%s", basic_land[state[loc].idx].b);
        }
        else
        {
            printf(BLUE"%s", basic_land[state[loc].idx].b);
        }

        break;
    }
    printf(WHITE);
    endl();
}
void colorbg(int loc, int c)
{
    gotoxy(CRD[loc].x - 1, CRD[loc].y);
    switch (loc)
    {
    case 1:
    case 3:
    case 8:
    case 13:
    case 18:
    case 23:
    case 36:
    case 11:
    case 39:
    case 21:
    case 31:
        break;

    default:
        if(c == 1)
        {
            printf(WHITE" ");
        }
        else if(c == 2)
        {
            printf(REDB" ");
        }
        else
        {
            printf(BLUEB" ");
        }

        break;
    }

    gotoxy(CRD[loc].x + 2, CRD[loc].y);
    switch (loc)
    {
    case 1:
    case 3:
    case 8:
    case 13:
    case 18:
    case 23:
    case 36:
    case 11:
    case 39:
    case 21:
    case 31:
        break;

    default:
        if(c == 1)
        {
            printf(WHITE" ");
        }
        else if(c == 2)
        {
            printf(REDB" ");
        }
        else
        {
            printf(BLUEB" ");
        }

        break;
    }
    printf(WHITE);
    endl();
}

static int* fullscreen(int minWidth, int minHeight)
{

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

    if (!SetConsoleDisplayMode(hOut, CONSOLE_FULLSCREEN_MODE, pMode))
    {
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

    while (tries++ < maxTries)
    {
        if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
        {
            int width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
            int height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
            if (width < minWidth || height < minHeight) Sleep(50);
            else
            {
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
        if (check)
        {
            i--;
            continue;
        }
        golden_key_number[i] = random_num;
    }
}

/*
 < 황금 열쇠 >
    1   병원비지불   병원에서 건강진단을 받았습니다. 병원비 5만원을 은행에 내시오.
    2   복권당첨   축하합니다. 복권에 당첨되었습니다. 당첨금 20만원.
    3   무인도탈출
    (특수무전기)   a) 무인도에 갖혀 있을 때에 사용할 수 있음
    b) 1회 사용후 반납하며, 타인에게 팔 수 있음.
    c) 특수무전기 가격은 20만원입니다.
    4   무인도로
    가시오   폭풍을 만났습니다.
    무인도로 곧장 가시되, 출발지를 지날 때도 월급을 받지 못합니다.
    5   파티초대권   대중 앞에서 장기자랑을 하십시오
    (상대방 여러분들이 채점하신 후, 적당한 상금을 은행에서 지불해 드립니다.)
    6   관광여행
    .제주도로 가십시오
    .제주도에 통행료 지불
    .출발지를 거쳐갈 경우, 월급을 받으시오
    7   과속운전벌금   과속운전을 하였으므로, 벌금 5만원을 내시오
    8   해외유학   학교 등록금을 내시오. 등록금 10만원
    9   연금혜택   은행에서 노후 연금 5만원을 받으시오
    10   이사가시오   뒤로 세 칸 옮기시오
    11   고속도로   출발지까지 곧바로 가시오
    12   우승   자동차 경주에서 참피온이 되었습니다. 상금 10만원
    13   우대권   이 우대권을 가지고 있게 될 경우,
    상대방의 장소를 통행료 없이 머무를 수 있습니다.
    a) 1회 사용후 열쇠 함에 반납하시오
    b) 중요한 순간에만 쓰십시오.
    14   항공여행   콩코드 여객기를 타시고, 타이페이로 가시오
    a) 콩코드에 객실료를 지불하시오
    b) 출발지를 거쳐가실 경우에는 월급을 타시오
    15   건물수리비
    지불   정기적으로 건물을 수리하여야 합니다.
    (각 건물별로 다음과 같이 은행에서 지불하시오)
    호텔(HOTEL) .......... 10만원
    빌딩(BUILDING) ....... 6만원
    별장(VILLA) ......... 3만원
    16   방범비   방범비를 각 건물별로 다음과 같이 은행에 지불하시오
    호텔(HOTEL) .......... 5만원
    빌딩(BUILDING) ....... 3만원
    별장(VILLA) ......... 1만원
    17   우주여행
    초청장   우주항공국에서 우주여행 초청장이 왔습니다.
    a) 무료이므로 콜럼비아호에게 탑승료를 지불하지 않도록 되며,
        우주정류장으로 가십시요.
    b) 출발지를 거치실 경우에는 월급을 받으시오
    18   관광여행   부산으로 가십시오
    . 부산에 통행료 지불
    . 출발지를 거쳐서 갈 경우, 월급을 받으시오
    19   생일을
    축하합니다.   "HAPPY BIRTHDAY TO YOU"
    (해피 버스 데이 투 유!)
    모두에게 생일 축하를 받으시오(축하금 천원)
    20   장학금 혜택   은행에서 장학금 10만원을 받으시오
    21   정기종합
    소득세   종합소득세를 각 건물별로 아래와 같이 지불하시오
    a)호텔(HOTEL) .......... 15만원
    b)빌딩(BUILDING) ....... 10만원
    c)별장(VILLA) ......... 3만원
    22   노벨평화상
    수상   당신은 세계 평화를 위하여 공헌하였으므로,
    은행으로부터 상금 30만원을 배당 받습니다.
    23   관광여행   88년도 올림픽개최지인 서울로 가십시오
    서울올림픽에 통행료 지불(200만원)
    24   반액대매출   당신의 재산 중에서 제일 비싼 곳을 반액으로 은행에 파십시오
    (건물이 지어진 경우에는 함께 처분해야 합니다)
    25   우주여행
    초청장   우주항공국에서 우주여행초청장이 왔습니다.
    A) 무료이므로 콜럼비아호에게 탑승료를 지불하지 않도록 되며,
        우주정류장으로 가십시오
    B) 출발지를 거치실 경우에는 월급을 받으시오
    26   우대권   이 우대권을 가지고 있게 될 경우,
    상대방의 장소를 통행료 없이 머무를 수 있습니다.
    a) 1회 사용후 열쇠 함에 반납하시오
    b) 중요한 순간에만 쓰십시오.
    27   세계일주
    초대권   축하합니다. 현재위치에서부터 한바퀴 돌아오십시오.
    (다른 곳으로는 갈 수 없으며, 현위치로 돌아올 수 있음)
    A) 출발지를 거쳐가면서 월급을 받으시오
    B) 복지기금을 거쳐가면서 모아놓은 기금을 받으시오.
    28   이사가시오   뒤로 두 칸 옮기시오
    29   사회복지기금
    배당   사회복지기금 접수처로 가시오
    출발지를 거치실 경우에는 월급을 받으시오
    30   반액대매출   당신의 재산 중에서 제일 비싼 곳을 반액으로 은행에 파십시오
    (건물이 지어진 경우에는 함께 처분해야 합니다)

*/

int non_default_land[] = {1, 3, 8, 11, 13, 18, 21, 23, 31, 36, 39};

void print_land_idx()
{
    int cnt = 0;
    printf(GREEN);
    for(int i = 1; i <= BOARD_SIZE; i++)
    {
        if(i % 5 == 0 && i % 2 == 0){
            printf(GREEN);
        }
        else if(i % 5 == 0){
            printf(BOLDBLUE);
        }
        for(int j = 0; j <= 10; j++)
        {
            gotoxy(150, i);
            if(i == non_default_land[j])
            {
                switch (i)
                {
                case 1:
                    printf(" 1. GO : 시작지");
                    break;
                case 3:
                case 8:
                case 13:
                case 18:
                case 23:
                case 36:
                    printf("%2d. GK : 황금열쇠", i);
                    break;

                case 11:
                    printf("%2d. JA : 무인도", i);
                    break;

                case 39:
                    printf("%2d. TX : 사회복지기금 기부처", i);
                    break;
                case 21:
                    printf("%2d. GT : 사회복지기금 접수처", i);

                    break;
                case 31:
                    printf("%2d. SP : 우주여행", i);
                }
                i++;
                j = 0;
                continue;
            }
        }
        printf("%2d. %s : %s", i, basic_land[state[i].idx].b, basic_land[state[i].idx].a);
    }

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

        p_info[player].muindo_exit = 1;
        break;

    case 4:
        printf("무인도로 가시오");
        endl();
        printf("폭풍을 만났습니다.");
        endl();
        printf("무인도로 곧장 가시되, 출발지를 지날 때도 월급을 받지 못합니다.");
        endl();
        color(player, 1);
        p_info[player].location = 11;
        color(player, player + 1);
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

        gotoxy(0, 24);
        printf("p1 잔액 : %7d \t p2 잔액 : %7d", p_info[1].asset, p_info[2].asset);
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

        color(player, 1);
        p_info[player].location = 6;
        color(player, player + 1);
        board_event(player);
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
        color(player,1);
        p_info[player].location -= 3;
        color(player,player+1);
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
        color(player, 1);
        p_info[player].location = 1;
        color(player, player + 1);
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
        if(state[16].own != player)
        {
            printf("콩코드 여객기에 객실료 지불!");
            endl();
            p_info[player].asset -= basic_land[16].landing_price;
        }
        color(player, 1);
        p_info[player].location = 2;
        color(player, player + 1);
        board_event(player);
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
    Sleep(3000);
}

void board_init()
{
    int cnt = 0;
    for(int i = 1; i <= BOARD_SIZE; i++)
    {
        for(int j = 0; j <= 10; j++)
        {
            if(i == non_default_land[j])
            {
                i++, j = 0;
                continue;
            }
        }
        state[i].idx = cnt;
        cnt++;
    }
}

int flag = 0;

void board_event(int player)
{
    int loc = p_info[player].location;
    switch (loc)
    {
    case 1:
        printf("시작지에 도착했습니다!");
        Sleep(2000);
        break;
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
        p_info[player].m = 1;
        Sleep(2000);
        endl();
        break;

    case 39:
        printf("사회복지기금 기부처입니다. 10만원 기부!");
        Sleep(2000);
        endl();
        p_info[player].asset -= 100000;
        sahwei += 100000;
        break;
    case 21:
        printf("사회복지기금 접수처입니다. 현재까지 모인 기금을 받아갑니다.");
        endl();
        Sleep(2000);
        p_info[player].asset += sahwei;

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
        color(player, 1);
        p_info[player].location = target;
        color(player, player + 1);
        board_event(player);
        break;

    default:
        endl();
        printf("도시에 도착했습니다. 소유 상태 확인 중...");


        if (state[loc].own == 0)
        {
            endl();
            printf("구매 가능한 땅입니다.");
            endl();
            printf("======도시 정보======");
            endl();
            printf(" 도시 이름 : %s", basic_land[state[loc].idx].a);
            endl();
            printf(" 도시 가격 : %d", basic_land[state[loc].idx].buy_price);
            endl();
            printf("무엇을 하시겠습니까?");
            endl();
            printf("1. 구매");
            endl();
            printf("2. 구매포기");
            endl();
            printf("3. 기권");
            endl();
            printf(" > ");
            int command;
            scanf("%d", &command);
            getchar();
            endl();
            if(command == 1)
            {
                printf("정말 구매하시겠습니까? (y/n)");
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
                        Sleep(1000);
                        endl();
                    }
                    else
                    {
                        endl();
                        printf("구입 완료!");
                        Sleep(1000);
                        endl();

                        p_info[player].asset -= basic_land[state[loc].idx].buy_price;
                        state[loc].own = player;
                    }
                }
            }
            if(command == 3)
            {
                flag = player;
                return;
            }
        }
        else if (state[loc].own != player)
        {
            endl();
            if(!p_info[player].udae){
                printf("상대방이 소유한 땅입니다. 통행료를 지불합니다.");
                int toll = basic_land[state[loc].idx].landing_price; // 통행료
                p_info[player].asset -=  toll;
                p_info[state[loc].own].asset += toll;
            }
            else{
                printf("우대권이 있습니다. 사용하시겠습니까? (y / n)");
                endl();
                printf(" > ");
                char udae_use;
                scanf("%c", &udae_use);
                getchar();
                endl();
                if(udae_use == 'y'){
                    p_info[player].udae = 0;
                    return;
                }
                printf("상대방이 소유한 땅입니다. 통행료를 지불합니다.");
                endl();
                int toll = basic_land[state[loc].idx].landing_price; // landing_price = landing_price + hotel / house / building price
                p_info[player].asset -=  toll;
                p_info[state[loc].own].asset += toll;

            }
            Sleep(1000);
            while(p_info[player].asset < 0){
                gotoxy(0, 24);
                printf("p1 잔액 : %7d \t p2 잔액 : %7d", p_info[1].asset, p_info[2].asset);
                printf("당신에게 상환 받을 돈이 부족합니다.");
                endl();
                printf("무엇을 하시겠습니까?");
                endl();
                printf("1. 땅 팔기");
                endl();
                printf("2. 건물 팔기");
                endl();
                printf("3. 기권 하기");
                endl();
                printf(" > ");
                int minus_com;
                scanf("%d", &minus_com);
                getchar();
                endl();
                switch(minus_com){
                case 1:
                    for(int i = 1; i <= BOARD_SIZE; i++){
                        if(state[i].own == player){
                            printf("%d번, %d원 : %s", i, basic_land[state[i].idx].buy_price, basic_land[state[i].idx].a);
                            endl();
                        }
                    }
                    printf("판매하실 번호를 선택하시오.");
                    endl();
                    int sell_num;
                    printf(" > ");
                    scanf("%d", &sell_num);
                    getchar();
                    endl();
                    if(state[sell_num].own != player){
                        printf("당신의 상품이 아닙니다.");
                        endl();
                        break;
                    }
                    else{
                        state[sell_num].own = 0;
                        p_info[player].asset += basic_land[state[sell_num].idx].buy_price;
                        break;
                    }
                case 2:
                    /* 숙제 */

                case 3:
                    flag = player;
                    return;

                }
            }
        }
        else
        {
            goto CMD;
CMD:
            endl();
            printf("당신이 소유한 땅입니다.");
            endl();
            printf("무엇을 하시겠습니까?");
            endl();
            printf("1. 건물 구매");
            endl();
            printf("2. 아무것도 하지 않음");
            endl();
            printf("3. 기권");
            endl();
            printf(" > ");
            int command;
            scanf("%d", &command);
            getchar();
            endl();
            if(command == 1)
            {
                printf("지으실 건물을 선택하시오.");
                endl();
                printf("1. 별장 1채");
                endl();
                printf("2. 별장 2채");
                endl();
                printf("3. 별장 3채");
                endl();
                printf("4. 별장 4채");
                endl();
                printf("5. 아무것도 하지 않음");
                endl();
                printf("6. 되돌아가기");
                endl();
                int com;
                printf(" > ");
                scanf("%d", &com);
                getchar();
                if(com == 1)
                {

                }
                else if(com == 2)
                {

                }
                else if(com == 3)
                {

                }
                else if(com == 4)
                {

                }
                else if(com == 6)
                {
                    goto CMD;
                }
            }

            Sleep(1000);
        }
        break;
    }
    ClearConsoleRect(stdHandle, comment_start, 0, comment_start + 70, 100);
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
    color(player_info, player_info + 1);
    board_event(player_info);
}

void move_printing(int meter, int player_info)
{
    printf("%d칸 이동",meter);
    color(player_info, 1);
    Sleep(1000);
    endl();

    moveto(meter, player_info);
    gotoxy(0, 24);
    printf("p1 잔액 : %7d \t p2 잔액 : %7d", p_info[1].asset, p_info[2].asset);
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

    if(p_info[player].m){
        Sleep(1000);
        p_info[player].m++;
        if(d.dice1 == d.dice2){
            printf("탈출!");
            endl();
            p_info[player].m = 0;
            player_turn(player);
        }
        else if(p_info[player].muindo_exit){
            printf("무인도 탈출권이 있습니다. 사용하시겠습니까? (y / n)");
            char m_yn;
            endl();
            scanf("%c", &m_yn);
            if(m_yn == 'y'){
                printf("무인도 탈출 사용! 주사위를 한 번 더 굴립니다.");
                endl();
                p_info[player].muindo_exit -=1;
                Sleep(1000);
                player_turn(player);
            }
            else{
                return;
            }
        }
        else{
            return;
        }
    }
    if(d.dice1 == d.dice2)
    {
        p_info[player].double_count++;
        if(p_info[player].double_count == 3)
        {
            printf("더블 3번! 무인도 이동");
            endl();
            color(player, 1);
            p_info[player].location = 11;
            color(player, player+1);
        }
        else
        {
            printf("더블! ");
            endl();
            move_printing(d.dice1 + d.dice2, player);
            if(flag)
        {
            gotoxy(0, 25);
            printf("%d님 패배!", flag);
            exit(0);
        }
            player_turn(player);
        }
    }
    else
    {
        p_info[player].double_count = 0;
        move_printing(d.dice1 + d.dice2, player);
    }

    if(flag)
    {
        gotoxy(0, 25);
        printf("%d님 패배!", flag);
        exit(0);
    }

    for(int i = 1; i <= BOARD_SIZE; i++){
        if(state[i].own == 1){
            colorbg(i, 2);
        }
        else if (state[i].own == 2){
            colorbg(i, 3);
        }
    }

    gotoxy(0, comment_start);
    return;
}


void onep()
{
    pan();
    print_land_idx();
    golden_key_init();
    p_info[1].location = 1;
    p_info[2].location = 1;
    while(1)
    {
        printf(WHITE);
        gotoxy(0, 24);
        printf("p1 잔액 : %7d \t p2 잔액 : %7d", p_info[1].asset, p_info[2].asset);
        player_turn(1);
        if(flag)break;
        player_turn(2);
        if(flag)break;
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
    getchar();
    if(playstyle == 1)
    {
        board_init();
        gotoxy(0, 0);
        FillConsoleOutputCharacter(stdHandle, ' ', 50 * 50, (COORD){ 0, 0}, &dw);
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
    printf(BOLDCYAN);
    printf("****************************************************************\n");
    printf(" _____ __    _____ _____    _____ _____ _____ _____ __    _____ \n");
    printf("| __  |  |  |  |  |   __|  |     |  _  | __  | __  |  |  |   __|\n");
    printf("| __ -|  |__|  |  |   __|  | | | |     |    -| __ -|  |__|   __|\n");
    printf("|_____|_____|_____|_____|  |_|_|_|__|__|__|__|_____|_____|_____|\n\n");
    printf("****************************************************************\n");
    printf(WHITE);
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

    SetConsoleOutputCP(CP_UTF8);

    CONSOLE_CURSOR_INFO cursorInfo = { 0, };

    cursorInfo.dwSize = 1;
    cursorInfo.bVisible = 0;
    SetConsoleCursorInfo(stdHandle, &cursorInfo);
    HWND hwnd = GetConsoleWindow();
    Sleep(10);
    HWND owner = GetWindow(hwnd, GW_OWNER);

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
    {
        int width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        int height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
        screen = fullscreen(width + 1, height + 1);
    }
    else screen = fullscreen(0, 0);

    start_page();
    return 0;
}
