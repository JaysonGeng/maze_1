#include<iostream>

using namespace std;


//定义描述迷宫中当前位置的结构类型
class Point {
public:

    Point(int x, int y);

    Point();

    //x代表当前位置的行坐标
    int x;
    //y代表当前位置的列坐标
    int y;
    //0:无效,1:东,2:南,3:西,4:北
    int dir;
};


//链表结点
class LinkNode {
    friend class Stack;

public:
    Point data;
    LinkNode *next{};
};

class MyStack {
private:
    //指向第一个结点的栈顶指针
    LinkNode *top;
public:
    //构造函数，置空栈
    MyStack();

    //析构函数
    ~MyStack();

    //把元素data压入栈中
    void Push(Point e);

    //使栈顶元素出栈
    Point Pop();

    //取出栈顶元素
    Point getPop();

    //把栈清空
    void Clear();

    //判断栈是否为空，如果为空则返回1，否则返回0
    bool empty();
};


//迷宫类
class Maze {

private:
    //迷宫
    int **maze{};

    //迷宫大小
    int m = 0, n = 0;

    //起点/终点
    Point start, final;

public:
    //定义当前位置移动的4个方向
    int action[4][2] = {{0,  1},
                        {1,  0},
                        {0,  -1},
                        {-1, 0}};

    Point getSize();

    //获取迷宫
    //返回存取迷宫的二维指针
    void initMaze();

//寻找迷宫maze中从（0，0）到（m,n）的路径
//到则返回true,否则返回false
    bool MazePath();

//输出迷宫的路径
    void PrintPath(MyStack path_stack);

//恢复迷宫
    void Restore(int m, int n);

    const Point &getStart() const;

    void setStart(const Point &start);

    const Point &getFinal() const;

    void setFinal(const Point &final);

};


int main() {

    Maze maze;
    //调用initMaze()函数，得到迷宫
    maze.initMaze();
    //调用Mazepath()函数获取路径
    if (maze.MazePath())
        cout << "迷宫路径探索成功!\n";
    else cout << "路径不存在!\n";
    return 0;
}


Point::Point(int x, int y) {
    this->x = x;
    this->y = y;
}

Point::Point() {

}

//构造函数，置空栈
MyStack::MyStack() {
    top = nullptr;
}

//析构函数
MyStack::~MyStack() {
}

//把元素x压入栈中
void MyStack::Push(Point e) {
    LinkNode *P;
    P = new LinkNode;
    P->data = e;
    P->next = top;
    top = P;
}

//使栈顶元素出栈
Point MyStack::Pop() {
    Point temp;
    LinkNode *P;
    P = top;
    top = top->next;
    temp = P->data;
    delete P;
    return temp;
}

//取出栈顶元素
Point MyStack::getPop() {
    return top->data;
}

//把栈清空
void MyStack::Clear() {
    top = nullptr;
}

//判断栈是否为空，如果为空则返回1，否则返回0
bool MyStack::empty() {
    return top == nullptr;
}


Point Maze::getSize() {
    return {this->m, this->n};
}

//返回存取迷宫的二维指针
void Maze::initMaze() {
    //定义二维指针存取迷宫
    int **maze;
    int i = 0, j = 0;
    cout << "请输入迷宫的长和宽:";
    //输入迷宫的长和宽
    //m,n分别代表迷宫的行数和列数
    cin >> m >> n;


    int start_x = 0, start_y = 0, final_x = 0, final_y = 0;
    cout << "请输入起点x，y 终点x，y:";
    //输入起点x，y 终点x，y
    cin >> start_x >> start_y >> final_x >> final_y;
    setStart(Point(start_x, start_y));
    setFinal(Point(final_x, final_y));


    cout << "请输入迷宫内容:\n";
    //申请长度等于行数加2的二级指针
    maze = new int *[m + 2];
    //申请每个二维指针的空间
    for (i = 0; i < m + 2; i++) {
        maze[i] = new int[n + 2];
    }
    //输入迷宫的内容，0代表可通，1代表不通
    for (i = 1; i <= m; i++)
        for (j = 1; j <= n; j++)
            cin >> maze[i][j];
    for (i = 0; i < m + 2; i++)
        maze[i][0] = maze[i][n + 1] = 1;
    for (i = 0; i < n + 2; i++)
        maze[0][i] = maze[m + 1][i] = 1;
    this->maze = maze;
};

//寻找迷宫maze中从（0，0）到（m,n）的路径
bool Maze::MazePath()
//到则返回true,否则返回false
{
    //定义栈p、qury_stack,分别存探索迷宫的过程和存储路径
    MyStack qury_stack, path_stack;
    Point Temp1, Temp2;
    int x, y, i;
    Temp1.x = getStart().x;
    Temp1.y = getStart().y;
    //将入口位置入栈
    qury_stack.Push(Temp1);
    path_stack.Push(Temp1);
    //标志入口位置已到达过
    maze[getStart().x][getStart().y] = -1;
    //栈q非空，则反复探索
    while (!qury_stack.empty()) {
        //获取栈顶元素
        Temp2 = qury_stack.getPop();
        if (!(path_stack.getPop().x == qury_stack.getPop().x && path_stack.getPop().y == qury_stack.getPop().y))
            //如果有新位置入栈，则把上一个探索的位置存入栈p
            path_stack.Push(Temp2);
        //探索当前位置的4个相邻位置
        for (i = 0; i < 4; i++) {
            //计算出新位置x位置值
            x = Temp2.x + action[i][0];
            //计算出新位置y位置值
            y = Temp2.y + action[i][1];
            //判断新位置是否可达
            if (maze[x][y] == 0) {
                Temp1.x = x;
                Temp1.y = y;
                //标志新位置已到达过
                maze[x][y] = -1;
                //新位置入栈
                qury_stack.Push(Temp1);
            }
            //成功到达出口
            if ((x == (getFinal().x)) && (y == getFinal().y)) {
                Temp1.x = m;
                Temp1.y = n;
                Temp1.dir = 0;
                //把最后一个位置入栈
                path_stack.Push(Temp1);
                //输出路径
                PrintPath(path_stack);
                //恢复路径
                Restore(m, n);
                //表示成功找到路径
                return true;
            }
        }
        //如果没有新位置入栈，则返回到上一个位置
        if (path_stack.getPop().x == qury_stack.getPop().x && path_stack.getPop().y == qury_stack.getPop().y) {
            path_stack.Pop();
            qury_stack.Pop();
        }
    }
    //表示查找失败，即迷宫无路经
    return false;
}

//输出路径
void Maze::PrintPath(MyStack path_stack) {
    //定义一个栈，按从入口到出口存取路径
    cout << "迷宫的路径为(行坐标,列坐标,数字化方向,方向)\n";
    MyStack t;
    int a, b;
    Point data;
    LinkNode *temp;
    //申请空间
    temp = new LinkNode;
    //取栈p的顶点元素，即第一个位置
    temp->data = path_stack.Pop();
    //第一个位置入栈t
    t.Push(temp->data);
    //释放空间
    delete temp;
    //栈p非空，则反复转移
    while (!path_stack.empty()) {
        temp = new LinkNode;
        //获取下一个位置
        temp->data = path_stack.Pop();
        //得到行走方向
        //行坐标方向
        a = t.getPop().x - temp->data.x;
        //列坐标方向
        b = t.getPop().y - temp->data.y;
        //方向向下，用1表示
        if (a == 1)
            temp->data.dir = 1;
            //方向向右，用2表示
        else if (b == 1)
            temp->data.dir = 2;
            //方向向上，用3表示
        else if (a == -1)
            temp->data.dir = 3;
            //方向向左，用4表示
        else if (b == -1)
            temp->data.dir = 4;
        //把新位置入栈
        t.Push(temp->data);
        delete temp;
    }
    //输出路径，包括行坐标，列坐标，下一个位置方向
    //栈非空，继续输出
    while (!t.empty()) {
        data = t.Pop();
        //输出行坐标，列坐标
        cout << '(' << data.x << ',' << data.y << ',' << data.dir;
        //输出相应的方向
        switch (data.dir) {
            case 1:
                cout << ",↓)\n";
                break;
            case 2:
                cout << ",→)\n";
                break;
            case 3:
                cout << ",↑)\n";
                break;
            case 4:
                cout << ",←)\n";
                break;
            case 0:
                cout << ")\n";
                break;
            default:
                break;
        }
    }
}

//恢复迷宫
void Maze::Restore(int m, int n) {
    int i, j;
    //遍历指针
    for (i = 0; i < m + 2; i++)
        for (j = 0; j < n + 2; j++) {
            //恢复探索过位置，即把-1恢复为0
            if (maze[i][j] == -1)
                maze[i][j] = 0;
        }
}

const Point &Maze::getStart() const {
    return start;
}

void Maze::setStart(const Point &start) {
    Maze::start = start;
}

const Point &Maze::getFinal() const {
    return final;
}

void Maze::setFinal(const Point &final) {
    Maze::final = final;
}


