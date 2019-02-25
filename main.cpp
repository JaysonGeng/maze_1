#include<iostream>

using namespace std;


//���������Թ��е�ǰλ�õĽṹ����
class Point {
public:

    Point(int x, int y);

    Point();

    //x����ǰλ�õ�������
    int x;
    //y����ǰλ�õ�������
    int y;
    //0:��Ч,1:��,2:��,3:��,4:��
    int dir;
};


//������
class LinkNode {
    friend class Stack;

public:
    Point data;
    LinkNode *next{};
};

class MyStack {
private:
    //ָ���һ������ջ��ָ��
    LinkNode *top;
public:
    //���캯�����ÿ�ջ
    MyStack();

    //��������
    ~MyStack();

    //��Ԫ��dataѹ��ջ��
    void Push(Point e);

    //ʹջ��Ԫ�س�ջ
    Point Pop();

    //ȡ��ջ��Ԫ��
    Point getPop();

    //��ջ���
    void Clear();

    //�ж�ջ�Ƿ�Ϊ�գ����Ϊ���򷵻�1�����򷵻�0
    bool empty();
};


//�Թ���
class Maze {

private:
    //�Թ�
    int **maze{};

    //�Թ���С
    int m = 0, n = 0;

    //���/�յ�
    Point start, final;

public:
    //���嵱ǰλ���ƶ���4������
    int action[4][2] = {{0,  1},
                        {1,  0},
                        {0,  -1},
                        {-1, 0}};

    Point getSize();

    //��ȡ�Թ�
    //���ش�ȡ�Թ��Ķ�άָ��
    void initMaze();

//Ѱ���Թ�maze�дӣ�0��0������m,n����·��
//���򷵻�true,���򷵻�false
    bool MazePath();

//����Թ���·��
    void PrintPath(MyStack path_stack);

//�ָ��Թ�
    void Restore(int m, int n);

    const Point &getStart() const;

    void setStart(const Point &start);

    const Point &getFinal() const;

    void setFinal(const Point &final);

};


int main() {

    Maze maze;
    //����initMaze()�������õ��Թ�
    maze.initMaze();
    //����Mazepath()������ȡ·��
    if (maze.MazePath())
        cout << "�Թ�·��̽���ɹ�!\n";
    else cout << "·��������!\n";
    return 0;
}


Point::Point(int x, int y) {
    this->x = x;
    this->y = y;
}

Point::Point() {

}

//���캯�����ÿ�ջ
MyStack::MyStack() {
    top = nullptr;
}

//��������
MyStack::~MyStack() {
}

//��Ԫ��xѹ��ջ��
void MyStack::Push(Point e) {
    LinkNode *P;
    P = new LinkNode;
    P->data = e;
    P->next = top;
    top = P;
}

//ʹջ��Ԫ�س�ջ
Point MyStack::Pop() {
    Point temp;
    LinkNode *P;
    P = top;
    top = top->next;
    temp = P->data;
    delete P;
    return temp;
}

//ȡ��ջ��Ԫ��
Point MyStack::getPop() {
    return top->data;
}

//��ջ���
void MyStack::Clear() {
    top = nullptr;
}

//�ж�ջ�Ƿ�Ϊ�գ����Ϊ���򷵻�1�����򷵻�0
bool MyStack::empty() {
    return top == nullptr;
}


Point Maze::getSize() {
    return {this->m, this->n};
}

//���ش�ȡ�Թ��Ķ�άָ��
void Maze::initMaze() {
    //�����άָ���ȡ�Թ�
    int **maze;
    int i = 0, j = 0;
    cout << "�������Թ��ĳ��Ϳ�:";
    //�����Թ��ĳ��Ϳ�
    //m,n�ֱ�����Թ�������������
    cin >> m >> n;


    int start_x = 0, start_y = 0, final_x = 0, final_y = 0;
    cout << "���������x��y �յ�x��y:";
    //�������x��y �յ�x��y
    cin >> start_x >> start_y >> final_x >> final_y;
    setStart(Point(start_x, start_y));
    setFinal(Point(final_x, final_y));


    cout << "�������Թ�����:\n";
    //���볤�ȵ���������2�Ķ���ָ��
    maze = new int *[m + 2];
    //����ÿ����άָ��Ŀռ�
    for (i = 0; i < m + 2; i++) {
        maze[i] = new int[n + 2];
    }
    //�����Թ������ݣ�0�����ͨ��1����ͨ
    for (i = 1; i <= m; i++)
        for (j = 1; j <= n; j++)
            cin >> maze[i][j];
    for (i = 0; i < m + 2; i++)
        maze[i][0] = maze[i][n + 1] = 1;
    for (i = 0; i < n + 2; i++)
        maze[0][i] = maze[m + 1][i] = 1;
    this->maze = maze;
};

//Ѱ���Թ�maze�дӣ�0��0������m,n����·��
bool Maze::MazePath()
//���򷵻�true,���򷵻�false
{
    //����ջp��qury_stack,�ֱ��̽���Թ��Ĺ��̺ʹ洢·��
    MyStack qury_stack, path_stack;
    Point Temp1, Temp2;
    int x, y, i;
    Temp1.x = getStart().x;
    Temp1.y = getStart().y;
    //�����λ����ջ
    qury_stack.Push(Temp1);
    path_stack.Push(Temp1);
    //��־���λ���ѵ����
    maze[getStart().x][getStart().y] = -1;
    //ջq�ǿգ��򷴸�̽��
    while (!qury_stack.empty()) {
        //��ȡջ��Ԫ��
        Temp2 = qury_stack.getPop();
        if (!(path_stack.getPop().x == qury_stack.getPop().x && path_stack.getPop().y == qury_stack.getPop().y))
            //�������λ����ջ�������һ��̽����λ�ô���ջp
            path_stack.Push(Temp2);
        //̽����ǰλ�õ�4������λ��
        for (i = 0; i < 4; i++) {
            //�������λ��xλ��ֵ
            x = Temp2.x + action[i][0];
            //�������λ��yλ��ֵ
            y = Temp2.y + action[i][1];
            //�ж���λ���Ƿ�ɴ�
            if (maze[x][y] == 0) {
                Temp1.x = x;
                Temp1.y = y;
                //��־��λ���ѵ����
                maze[x][y] = -1;
                //��λ����ջ
                qury_stack.Push(Temp1);
            }
            //�ɹ��������
            if ((x == (getFinal().x)) && (y == getFinal().y)) {
                Temp1.x = m;
                Temp1.y = n;
                Temp1.dir = 0;
                //�����һ��λ����ջ
                path_stack.Push(Temp1);
                //���·��
                PrintPath(path_stack);
                //�ָ�·��
                Restore(m, n);
                //��ʾ�ɹ��ҵ�·��
                return true;
            }
        }
        //���û����λ����ջ���򷵻ص���һ��λ��
        if (path_stack.getPop().x == qury_stack.getPop().x && path_stack.getPop().y == qury_stack.getPop().y) {
            path_stack.Pop();
            qury_stack.Pop();
        }
    }
    //��ʾ����ʧ�ܣ����Թ���·��
    return false;
}

//���·��
void Maze::PrintPath(MyStack path_stack) {
    //����һ��ջ��������ڵ����ڴ�ȡ·��
    cout << "�Թ���·��Ϊ(������,������,���ֻ�����,����)\n";
    MyStack t;
    int a, b;
    Point data;
    LinkNode *temp;
    //����ռ�
    temp = new LinkNode;
    //ȡջp�Ķ���Ԫ�أ�����һ��λ��
    temp->data = path_stack.Pop();
    //��һ��λ����ջt
    t.Push(temp->data);
    //�ͷſռ�
    delete temp;
    //ջp�ǿգ��򷴸�ת��
    while (!path_stack.empty()) {
        temp = new LinkNode;
        //��ȡ��һ��λ��
        temp->data = path_stack.Pop();
        //�õ����߷���
        //�����귽��
        a = t.getPop().x - temp->data.x;
        //�����귽��
        b = t.getPop().y - temp->data.y;
        //�������£���1��ʾ
        if (a == 1)
            temp->data.dir = 1;
            //�������ң���2��ʾ
        else if (b == 1)
            temp->data.dir = 2;
            //�������ϣ���3��ʾ
        else if (a == -1)
            temp->data.dir = 3;
            //����������4��ʾ
        else if (b == -1)
            temp->data.dir = 4;
        //����λ����ջ
        t.Push(temp->data);
        delete temp;
    }
    //���·�������������꣬�����꣬��һ��λ�÷���
    //ջ�ǿգ��������
    while (!t.empty()) {
        data = t.Pop();
        //��������꣬������
        cout << '(' << data.x << ',' << data.y << ',' << data.dir;
        //�����Ӧ�ķ���
        switch (data.dir) {
            case 1:
                cout << ",��)\n";
                break;
            case 2:
                cout << ",��)\n";
                break;
            case 3:
                cout << ",��)\n";
                break;
            case 4:
                cout << ",��)\n";
                break;
            case 0:
                cout << ")\n";
                break;
            default:
                break;
        }
    }
}

//�ָ��Թ�
void Maze::Restore(int m, int n) {
    int i, j;
    //����ָ��
    for (i = 0; i < m + 2; i++)
        for (j = 0; j < n + 2; j++) {
            //�ָ�̽����λ�ã�����-1�ָ�Ϊ0
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


