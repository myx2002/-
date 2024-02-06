#include<iostream>
using namespace std;

template <class T>
class Vector {
public:
    Vector();                      // creates an empty vector
    Vector(int size);              // creates a vector for holding 'size' elements
    Vector(const Vector& r);       // the copy ctor
    ~Vector();                     // destructs the vector 
    T& operator[](int index);      // accesses the specified element without bounds checking
    T& at(int index);              // accesses the specified element, throws an exception of
                                   // type 'std::out_of_range' when index <0 or >=m_nSize
    int size() const;              // return the size of the container
    void push_back(const T& x);    // adds an element to the end 
    void clear();                  // clears the contents
    bool empty() const;            // checks whether the container is empty 
private:
    void inflate();                // expand the storage of the container to a new capacity,
                                   // e.g. 2*m_nCapacity
    T* m_pElements;                // pointer to the dynamically allocated storage
    int m_nSize;                   // the number of elements in the container
    int m_nCapacity;               // the total number of elements that can be held in the
                                   // allocated storage
    //用于记录对映位置元素是否已被初始化
    //便于实现[]的赋值
    int* used;
};

//函数实现
//默认构造函数
template<class T>
Vector<T>::Vector() 
{
    //仅测试时使用
    cout << "这是默认构造函数" << endl;
}
//构造函数
template<class T>
Vector<T>::Vector(int size) :m_nCapacity(size),m_nSize(0)
{
    m_pElements = new T[size];
    used = new int[size];
    //初始化used，元素均未被初始化
    for (int i = 0;i < size;i++)
        used[i] = 0;
    //仅测试时使用
    cout << "这是初始化函数" << endl;
}
//拷贝构造函数
template<class T>
Vector<T>::Vector(const Vector& r) :m_nCapacity(r.m_nCapacity), m_nSize(r.m_nSize)
{
    //对于指针特别处理
    m_pElements = new T[r.m_nCapacity];
    for (int i = 0;i < r.m_nCapacity;i++)
        *(m_pElements + i) = *(r.m_pElements + i);
    //拷贝是否初始化状态记录数组
    used = new int[r.m_nCapacity];
    for (int i = 0;i < r.m_nCapacity;i++)
        used[i] = r.used[i];
    //仅测试时使用
    cout << "这是拷贝构造函数" << endl;
}
//析构函数
template<class T>
Vector<T>::~Vector()
{
    //delete[] m_pElements;
    //delete[] used;
    //经测试时使用
    cout <<"这是size为"<< size() <<"的析构函数" << endl;
}

//运算符重载
template<class T>
T& Vector<T>::operator[](int index)
{
    //根据元素是否已初始化对待[]重载
    if (used[index] == 1);
    //若元素未初始化，则相应增加m_nSize的大小
    else if (used[index] == 0)
    {
        used[index] = 1;
        m_nSize++;
    }
    return *(m_pElements + index);
}
//at函数
template<class T>
T& Vector<T>::at(int index)
{
    //处理越界index情况
    if (index < 0 || index >= m_nSize)
    {
        cout << "std::out_of_range!" << endl;
        exit(404);
    }
    return *(m_pElements + index);
}

//其余函数
//返回结构存储能力
template<class T>
int Vector<T>::size()const
{
    return m_nCapacity;
}
//增加函数
template<class T>
void Vector<T>::push_back(const T& x)
{
    //若接受大小在能力范围内，直接接受数据
    if (m_nSize < m_nCapacity)
    {
        m_pElements[m_nSize] = x;
        used[m_nSize] = 1;
        m_nSize++;
    }
    //若接受后大小超过容量，先使容量扩大两倍后再填充
    else
    {
        inflate();
        m_pElements[m_nSize] = x;
        used[m_nSize] = 1;
        m_nSize++;
    }
}
//清除函数
template<class T>
void Vector<T>::clear()
{
    //当前容量直接置零
    m_nSize = 0;
    for (int i = 0;i < m_nCapacity;i++)
        used[i] = 0;
    //好像没有一定的必要清除数据
    delete[] m_pElements;
    m_pElements = new T[m_nCapacity];
}
//判断是否为空函数
template<class T>
bool Vector<T>::empty() const
{
    //根据当前容量判断是否为空
    if (m_nSize == 0)
        return true;
    else
        return false;
}

//扩容函数
template<class T>
void Vector<T>::inflate()
{
    //定义新指针记录数据
    T* temp = new T[m_nCapacity];
    int* intemp = new int[m_nCapacity];
    //扩大容量
    m_nCapacity = m_nCapacity * 2;

    //保存数值
    memcpy(temp, m_pElements, m_nSize);
    //更新容器大小至两倍
    delete[] m_pElements;
    m_pElements = new T[m_nCapacity];
    //拷贝回数据
    memcpy(m_pElements, temp, m_nSize);
    delete[] temp;

    //扩容元素是否初始化情况
    memcpy(intemp, used, m_nSize);
    delete[] used;
    used = new int[m_nCapacity];
    memcpy(used, intemp, m_nSize);
    //对扩容的元素状态归零
    for (int i = m_nSize;i < m_nCapacity;i++)
        used[i] = 0;
    delete[] intemp;
}

int main()
{
    //先以int为例测试函数
    cout << "先以int类型为例" << endl;
    //构造/初始化函数检验
    Vector<int> first;
    cout << "first" << endl;
    Vector<int> second(3);
    cout << "second(3)" << endl;
    cout << endl;
    
    //测试[]重载与普通的push_back函数
    second.push_back(1);
    second[1] = 2;
    //second.push_back(2);
    second.push_back(3);
    //跨界访问不会引发异常
    cout << "second[2] = " << second[2] <<", (越界)second[3] = " << second[3] << endl;
    cout << endl;
    //测试拷贝构造函数与
    Vector<int> third(second);
    cout << "third(second)" << endl;
    cout <<"third.at(0) = "<<third.at(0) << ", third.at(1) = " << third.at(1) << ", third.at(2) = " << third.at(2) << endl;
    //越界check需要手动启动
    //cout <<"(越界)third.at(3) = "<< third.at(3) << endl;
    cout << endl;

    //测试puch_back的越界、inflate函数与容量size函数
    cout <<"second.size() = " << second.size() << endl;
    cout << "second.push_back(4)函数后(扩大容量至两倍)：" << endl;
    second.push_back(4);
    cout <<"second[3] = "<< second[3] <<",  new second.size() = "<< second.size() << endl;
    cout << endl;

    //测试empty与clear函数(true/false 被1/0覆盖）
    cout << "以下输出true/false被1/0覆盖" << endl;
    cout <<"third.empty() = "<< third.empty() << endl;
    cout <<"first.empty() = "<< first.empty() << endl;
    cout << "执行third.clear()后:" << endl;
    third.clear();
    cout <<"third.size() = "<< third.size() << "(容量不变), third.empty() = " << third.empty() << endl;
    cout << endl;

    //再以double类型为例
    cout << "再以double类型为例" << endl;
    Vector<double> one;
    cout << "one" << endl;
    Vector<double> two(3);
    cout << "two(3)" << endl;
    cout << endl;

    //测试[]重载与普通的push_back函数
    two.push_back(1.1);
    two[1] = 2.2;
    //second.push_back(2);
    two.push_back(3.3);
    //跨界访问不会引发异常
    cout << "two[2] = " << two[2] <<", (越界)two[3] = " << two[3] << endl;
    cout << endl;
    //测试拷贝构造函数与
    Vector<double> three(two);
    cout << "three(two)" << endl;
    cout << "three.at(0) = " << three.at(0) << ", three.at(1) = " << three.at(1) <<", three.at(2) = " << three.at(2) << endl;
    //越界check需要手动启动
    //cout << "(越界)three.at(3) = " << three.at(3) << endl;
    cout << endl;

    //测试puch_back的越界、inflate函数与容量size函数
    cout << "two.size() = " << two.size() << endl;
    cout << "two.push_back(4.4)函数后(扩大容量至两倍)：" << endl;
    two.push_back(4.4);
    cout << "two[3] = " << two[3] << ",  new two.size() = " << two.size() << endl;
    cout << endl;

    //测试empty与clear函数(true/false 被1/0覆盖）
    cout << "以下输出true/false被1/0覆盖" << endl;
    cout << "three.empty() = " << three.empty() << endl;
    cout << "one.empty() = " << one.empty() << endl;
    cout << "执行three.clear()后:" << endl;
    three.clear();
    cout << "three.size() = " << three.size() << "(容量不变), three.empty() = " << three.empty() << endl;
    cout << endl;

    //析构函数自测试
    cout << "析构函数测试" << endl;
    return 0;
}