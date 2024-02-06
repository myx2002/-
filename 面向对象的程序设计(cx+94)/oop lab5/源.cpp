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
    //���ڼ�¼��ӳλ��Ԫ���Ƿ��ѱ���ʼ��
    //����ʵ��[]�ĸ�ֵ
    int* used;
};

//����ʵ��
//Ĭ�Ϲ��캯��
template<class T>
Vector<T>::Vector() 
{
    //������ʱʹ��
    cout << "����Ĭ�Ϲ��캯��" << endl;
}
//���캯��
template<class T>
Vector<T>::Vector(int size) :m_nCapacity(size),m_nSize(0)
{
    m_pElements = new T[size];
    used = new int[size];
    //��ʼ��used��Ԫ�ؾ�δ����ʼ��
    for (int i = 0;i < size;i++)
        used[i] = 0;
    //������ʱʹ��
    cout << "���ǳ�ʼ������" << endl;
}
//�������캯��
template<class T>
Vector<T>::Vector(const Vector& r) :m_nCapacity(r.m_nCapacity), m_nSize(r.m_nSize)
{
    //����ָ���ر���
    m_pElements = new T[r.m_nCapacity];
    for (int i = 0;i < r.m_nCapacity;i++)
        *(m_pElements + i) = *(r.m_pElements + i);
    //�����Ƿ��ʼ��״̬��¼����
    used = new int[r.m_nCapacity];
    for (int i = 0;i < r.m_nCapacity;i++)
        used[i] = r.used[i];
    //������ʱʹ��
    cout << "���ǿ������캯��" << endl;
}
//��������
template<class T>
Vector<T>::~Vector()
{
    //delete[] m_pElements;
    //delete[] used;
    //������ʱʹ��
    cout <<"����sizeΪ"<< size() <<"����������" << endl;
}

//���������
template<class T>
T& Vector<T>::operator[](int index)
{
    //����Ԫ���Ƿ��ѳ�ʼ���Դ�[]����
    if (used[index] == 1);
    //��Ԫ��δ��ʼ��������Ӧ����m_nSize�Ĵ�С
    else if (used[index] == 0)
    {
        used[index] = 1;
        m_nSize++;
    }
    return *(m_pElements + index);
}
//at����
template<class T>
T& Vector<T>::at(int index)
{
    //����Խ��index���
    if (index < 0 || index >= m_nSize)
    {
        cout << "std::out_of_range!" << endl;
        exit(404);
    }
    return *(m_pElements + index);
}

//���ຯ��
//���ؽṹ�洢����
template<class T>
int Vector<T>::size()const
{
    return m_nCapacity;
}
//���Ӻ���
template<class T>
void Vector<T>::push_back(const T& x)
{
    //�����ܴ�С��������Χ�ڣ�ֱ�ӽ�������
    if (m_nSize < m_nCapacity)
    {
        m_pElements[m_nSize] = x;
        used[m_nSize] = 1;
        m_nSize++;
    }
    //�����ܺ��С������������ʹ�������������������
    else
    {
        inflate();
        m_pElements[m_nSize] = x;
        used[m_nSize] = 1;
        m_nSize++;
    }
}
//�������
template<class T>
void Vector<T>::clear()
{
    //��ǰ����ֱ������
    m_nSize = 0;
    for (int i = 0;i < m_nCapacity;i++)
        used[i] = 0;
    //����û��һ���ı�Ҫ�������
    delete[] m_pElements;
    m_pElements = new T[m_nCapacity];
}
//�ж��Ƿ�Ϊ�պ���
template<class T>
bool Vector<T>::empty() const
{
    //���ݵ�ǰ�����ж��Ƿ�Ϊ��
    if (m_nSize == 0)
        return true;
    else
        return false;
}

//���ݺ���
template<class T>
void Vector<T>::inflate()
{
    //������ָ���¼����
    T* temp = new T[m_nCapacity];
    int* intemp = new int[m_nCapacity];
    //��������
    m_nCapacity = m_nCapacity * 2;

    //������ֵ
    memcpy(temp, m_pElements, m_nSize);
    //����������С������
    delete[] m_pElements;
    m_pElements = new T[m_nCapacity];
    //����������
    memcpy(m_pElements, temp, m_nSize);
    delete[] temp;

    //����Ԫ���Ƿ��ʼ�����
    memcpy(intemp, used, m_nSize);
    delete[] used;
    used = new int[m_nCapacity];
    memcpy(used, intemp, m_nSize);
    //�����ݵ�Ԫ��״̬����
    for (int i = m_nSize;i < m_nCapacity;i++)
        used[i] = 0;
    delete[] intemp;
}

int main()
{
    //����intΪ�����Ժ���
    cout << "����int����Ϊ��" << endl;
    //����/��ʼ����������
    Vector<int> first;
    cout << "first" << endl;
    Vector<int> second(3);
    cout << "second(3)" << endl;
    cout << endl;
    
    //����[]��������ͨ��push_back����
    second.push_back(1);
    second[1] = 2;
    //second.push_back(2);
    second.push_back(3);
    //�����ʲ��������쳣
    cout << "second[2] = " << second[2] <<", (Խ��)second[3] = " << second[3] << endl;
    cout << endl;
    //���Կ������캯����
    Vector<int> third(second);
    cout << "third(second)" << endl;
    cout <<"third.at(0) = "<<third.at(0) << ", third.at(1) = " << third.at(1) << ", third.at(2) = " << third.at(2) << endl;
    //Խ��check��Ҫ�ֶ�����
    //cout <<"(Խ��)third.at(3) = "<< third.at(3) << endl;
    cout << endl;

    //����puch_back��Խ�硢inflate����������size����
    cout <<"second.size() = " << second.size() << endl;
    cout << "second.push_back(4)������(��������������)��" << endl;
    second.push_back(4);
    cout <<"second[3] = "<< second[3] <<",  new second.size() = "<< second.size() << endl;
    cout << endl;

    //����empty��clear����(true/false ��1/0���ǣ�
    cout << "�������true/false��1/0����" << endl;
    cout <<"third.empty() = "<< third.empty() << endl;
    cout <<"first.empty() = "<< first.empty() << endl;
    cout << "ִ��third.clear()��:" << endl;
    third.clear();
    cout <<"third.size() = "<< third.size() << "(��������), third.empty() = " << third.empty() << endl;
    cout << endl;

    //����double����Ϊ��
    cout << "����double����Ϊ��" << endl;
    Vector<double> one;
    cout << "one" << endl;
    Vector<double> two(3);
    cout << "two(3)" << endl;
    cout << endl;

    //����[]��������ͨ��push_back����
    two.push_back(1.1);
    two[1] = 2.2;
    //second.push_back(2);
    two.push_back(3.3);
    //�����ʲ��������쳣
    cout << "two[2] = " << two[2] <<", (Խ��)two[3] = " << two[3] << endl;
    cout << endl;
    //���Կ������캯����
    Vector<double> three(two);
    cout << "three(two)" << endl;
    cout << "three.at(0) = " << three.at(0) << ", three.at(1) = " << three.at(1) <<", three.at(2) = " << three.at(2) << endl;
    //Խ��check��Ҫ�ֶ�����
    //cout << "(Խ��)three.at(3) = " << three.at(3) << endl;
    cout << endl;

    //����puch_back��Խ�硢inflate����������size����
    cout << "two.size() = " << two.size() << endl;
    cout << "two.push_back(4.4)������(��������������)��" << endl;
    two.push_back(4.4);
    cout << "two[3] = " << two[3] << ",  new two.size() = " << two.size() << endl;
    cout << endl;

    //����empty��clear����(true/false ��1/0���ǣ�
    cout << "�������true/false��1/0����" << endl;
    cout << "three.empty() = " << three.empty() << endl;
    cout << "one.empty() = " << one.empty() << endl;
    cout << "ִ��three.clear()��:" << endl;
    three.clear();
    cout << "three.size() = " << three.size() << "(��������), three.empty() = " << three.empty() << endl;
    cout << endl;

    //���������Բ���
    cout << "������������" << endl;
    return 0;
}