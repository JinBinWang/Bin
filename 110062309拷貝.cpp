#include <iostream>
#include <queue>
#include <algorithm>
#include <climits>
#include <cstdlib>
#include <fstream>
#include <set>
#include <ctime>
#include <array>
using namespace std;
int alpha=10000000000000000;
int beta=-10000000000000000;
int minimax(int row, int col, int alpha, int beta, bool maximizingPlayer, int depth);
int evaluate(int row, int col, int SPOT_STATE);
enum SPOT_STATE {
    EMPTY = 0,
    BLACK = 1,
    WHITE = 2
};

int player;
bool current_attack_state = false;
const int SIZE = 15;
array<array<int, SIZE>, SIZE> board;
array<array<int, SIZE>, SIZE > check = { 0 };

class point  //二維棋盤(x,y)座標系統
{
public:
    int x;
    int y;
    point(int row = 0, int column = 0){
        x=row;
        y=column;
    }
    bool operator < (const point& rhs) const;
};
bool point::operator <(const point &rhs)const{
    if (x != rhs.x) return x < rhs.x;
    if (y != rhs.y) return y < rhs.y;
    return 0;
}
set <point> rec1;//記錄哪些點出現過(set)
queue<point> rec2;//記錄哪些點出現過(point)

void read_board(ifstream& fin)
{
    fin >> player;
    for (int row = 0; row < SIZE; row++) {
        for (int column = 0; column < SIZE; column++) {
            fin >> board[row][column];
            if (board[row][column] ==WHITE||board[row][column]==BLACK)
            {
                rec2.push(point(row, column));
                rec1.insert(point(row, column));
            }

        }
    }
    if (rec2.empty())
        current_attack_state = true;
}
void write_valid_spot(ofstream& fout) {
    int heuristic = INT_MIN;
    
    
        if (current_attack_state)
    {
        board[7][7] = player; //先手天元
        fout << 7 << " " << 7 << endl;
    }
    int tmp;
    while (!rec2.empty())
    {
        int a;
        cout<<"in write_valid_spot"<<rec2.front().x<<' '<<rec2.front().y<<endl;
        int currentrow = rec2.front().x;
        int currentcolumn = rec2.front().y;
        
        for (int i = -2; i <= 2; i++)
        {
            for (int j = -2; j <= 2; j++)
            {
                if (i == 0 && j == 0) continue;//自己不能算
                if (rec1.find(point(currentrow + i, currentcolumn + j)) != rec1.end()) continue;//扣除棋盤上已出現的點
                if (currentrow + i >= 0  && currentcolumn + j >= 0 && currentcolumn + j < SIZE&& currentrow + i < SIZE)
                {
                    
                    tmp = minimax(currentrow + i, currentcolumn + j, INT_MIN, INT_MAX,true, 0);
                    a = board[currentrow][currentcolumn];

                    if (tmp > heuristic)
                    {
                        heuristic = tmp>heuristic?tmp:heuristic;
                        fout << currentrow + i << " " << currentcolumn + j << endl;
                        cout << heuristic << '\n';
                    }
                }

            }
        }

        rec2.pop();

    }

}

int main(int, char** argv) {
    std::ifstream fin(argv[1]);
    std::ofstream fout(argv[2]);
    read_board(fin);
    write_valid_spot(fout);
    fin.close();
    fout.close();
    return 0;
}
int evaluate(int row, int col, int SPOT_STATE)
{
    //攻擊分數
    //左上
    int upleft = 0;
    for (int i = 1, j = 1; row - i >= 0 && col - j >= 0; i++, j++)
    {
        if (board[row - i][col - j] == EMPTY||board[row-i][col-j]==BLACK||board[row-i][col-j]==WHITE)
            upleft++;
        else
            break;
    }
    //右下
    int downright = 0;
    for (int i = 1, j = 1; row + i <= SIZE && col + j <= SIZE; i++, j++)
    {
        if (board[row + i][col + j] == EMPTY||board[row+i][col+j]==BLACK||board[row+i][col+j]==WHITE)
            downright++;
        else
            break;
    }
    
    
    //右上
    int upright = 0;
    for (int i = 1, j = 1; row - i >= 0 && col + j < SIZE; i++, j++)
    {
        if (board[row - i][col + j] == EMPTY||board[row-i][col+j]==BLACK||board[row-i][col+j]==WHITE)
            upright++;
        
        else
            break;
    }
    //左下
    int downleft = 0;
    for (int i = 1, j = 1; row + i < SIZE && col - j >= 0; i++, j++)
    {
        if (board[row + i][col - j] == EMPTY||board[row+i][col-j]==BLACK||board[row+i][col-j]==WHITE)
            downleft++;
        
        else
            break;
    }
    
    //上
    int up = 0;
    for (int i = 1; row - i >= 0; i++)
    {
        if (board[row - i][col] == 0||board[row-i][col]==1||board[row-i][col]==2)
            up++;
        else
            break;
    }
    //下
    int down = 0;
    for (int i = 1; row + i < SIZE; i++)
    {
        if (board[row + i][col] == 0||board[row+i][col]==1||board[row+i][col]==2)
            down++;
        
        else
            break;
    }
    //左
    int left = 0;
    for (int i = 1; col - i >= 0; i++)
    {
        if (board[row ][col - i] == 0||board[row][col-i]==1||board[row][col-i]==2)
            left++;
        else
            break;
    }
    //右
    int right = 0;
    for (int i = 1; col + i < SIZE; i++)
    {
        if (board[row][col +i] == 0||board[row][col+i]==1||board[row][col+i]==2)
            right++;
        
        else
            break;
    }
    
    
    
    int attackvalue = max(max(up+down+1, left+right+1), max(upleft + downright + 1, upright + downleft + 1));

    if(attackvalue==5){
        attackvalue=10e7;
    }
    else if(attackvalue==4){
        attackvalue=10e5;
    }
    else if(attackvalue==3){
        attackvalue=1e3;
    }
    else if(attackvalue==2){
        attackvalue=100;
    }
    else if(attackvalue==1){
        attackvalue=10;
    }
    else{
        attackvalue=0;
    }
    
    //Defense
    
    //左上
     int enemyupleft = 0;
     for (int i = 1, j = 1; row - i >= 0 && col - j >= 0; i++, j++)
     {
         if (board[row - i][col - j] == (3 - SPOT_STATE))
             enemyupleft++;
         else
             break;
     }
     //右下
     int enemydownright = 0;
     for (int i = 1, j = 1; row + i <= SIZE && col + j <= SIZE; i++, j++)
     {
         if (board[row + i][col + j] == (3 - SPOT_STATE))
             enemydownright++;
         else
             break;
     }

    //右上
    int enemyupright = 0;
    for (int i = 1, j = 1; row - i >= 0 && col + j < SIZE; i++, j++)
    {
        if (board[row - i][col + j] == (3 - SPOT_STATE))
            enemyupright++;
        else
            break;
    }
    //左下
    int enemydownleft = 0;
    for (int i = 1, j = 1; row + i < SIZE && col - j >= 0; i++, j++)
    {
        if (board[row + i][col - j] == (3 - SPOT_STATE))
            enemydownleft++;
        else
            break;
    }

    

        //上
    int enemyup = 0;
    for (int i = 1; row - i >= 0; i++)
    {
        if (board[row - i][col] == (3 - SPOT_STATE))//白2黑1
            enemyup++;
        else
            break;
    }
    //下
    int enemydown = 0;
    for (int i = 1; row + i < SIZE; i++)
    {
        if (board[row + i][col] == (3 - SPOT_STATE))
            enemydown++;
        else
            break;
    }


    //左
    int enemyleft= 0;
    for (int i = 1; col - i >= 0; i++)
    {
        if (board[row][col - i] == (3 - SPOT_STATE))
            enemyleft++;
        else
            break;
    }
    //右
    int enemyright = 0;
    for (int i = 1; col + i < SIZE; i++)
    {
        if (board[row][col + i] == (3 - SPOT_STATE))
            enemyright++;
        else
            break;
    }


   
    int defensevalue;
    defensevalue = max(max(enemydown + enemyup, enemyright + enemyleft), max(enemyupleft+enemydownright, enemyupright + enemydownleft));


    if(defensevalue==5){
        defensevalue=-99900;
    }
    else if(defensevalue==4){
        defensevalue=-10000;
    }
    else if(defensevalue==3){
        defensevalue=-100;
    }
    else if(defensevalue==2){
        defensevalue=-10;
    }
    else if(defensevalue==1){
        defensevalue=-1;
    }
    else{
        defensevalue=0;
    }

    int value = attackvalue + defensevalue;
    cout<<"row"<<row<<' '<<"col"<<col<<endl;
    cout<<value<<endl;
    return value;
}
int minimax(int row, int col, int alpha, int beta, bool MAXmin,int depth)
{
    cout<<"in minimax"<<row<<' '<<col<<endl;
    
    if (depth == 0)
    {
        int ans;
        int meorenemy;
        if (MAXmin == true)
        {
            meorenemy = player;
            ans = evaluate(row, col, meorenemy);
        }
        else
        {
            meorenemy = 3 - player;
            ans =  evaluate(row, col, meorenemy);
        }
        return ans;
    }

    if (MAXmin)
    {
        int maxevaluate = INT_MIN;
        if (board[row][col] == EMPTY)
        {
            board[row][col] = player;
        }
        for (int i = -3; i <= 3; i++)
        {
            for (int j = -3; j <= 3; j++)
            {
                
                if (row + i >= SIZE || row + i < 0 || col + j >= SIZE || col + j < 0) continue;
                if(i==0&&j==0)continue;
                if (board[row + i][col + j] == EMPTY)
                {
                    int eval1;
                    int eval2;
                    eval1 = minimax(row + i, col + j, alpha, beta,false, depth-1);
                    eval2 = minimax(7,7,alpha,beta,false,depth-1);//add
                    maxevaluate = eval1>maxevaluate?eval1:maxevaluate;
                    maxevaluate = maxevaluate>eval2?maxevaluate:eval2;
                    alpha =alpha>eval1?alpha:eval1;
                    alpha=alpha>eval2?alpha:eval2;
                    board[row][col] = 0;
                    if (alpha >= beta)
                        break;
                    
                }

            }
            
        }
        return maxevaluate;
    }
    else
    {
        int minevaluate = INT_MAX;
        if (board[row][col] == EMPTY)
        {
            board[row][col] = 3 - player;
        }
        for (int i = -3; i <= 3; i++)
        {
            for (int j = -3; j <= 3; j++)
            {
                
                if (row + i >= SIZE || row + i < 0 || col + j >= SIZE || col + j < 0) continue;
                if(i==0&&j==0) continue;
                if (board[row + i][col + j] == EMPTY)
                {
                    int eval;
                    eval = minimax(row + i, col + j, alpha, beta, true,depth-1);
                    
                    minevaluate = eval>minevaluate?minevaluate:eval;
                    beta = beta>eval?eval:beta;
                    board[row][col] = 0;
                    
                }
            }
        }
            return minevaluate;
    }
    return 0;
}
