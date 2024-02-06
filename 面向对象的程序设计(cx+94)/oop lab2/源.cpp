#include<iostream>
#include<string>
#include<iomanip>
#include<algorithm>
#include<vector>
#include<map>

using namespace std;
//����ÿ��ͬѧ��Ӧ�ļ�¼�ṹ
class information {
	public:
		//��¼���еĿγ�
		vector<string> cnameid;
		//��¼�γ̶�ӳ�ĳɼ�
		vector<double> cgrade;
		//����ƽ���ɼ�����
		double average();
};

//����ƽ���ɼ����㺯��
double information::average()
{
	double total = 0;
	for (int i = 0;i < cnameid.size();i++)
		total += cgrade[i];
	return total / cnameid.size();
}

int main()
{
	//ѧ�����м���������
	vector<int> id;
	vector<string> name;
	//�γ����������;�ֵ����
	vector<string> course;
	vector<int> number;
	vector<double>avegra;
	vector<double>max;
	vector<double>min;
	//ѧ�����нṹ��ӳͼ
	map<int, information> mp;

	int rec = 0, i, z;
	int lenname = 0;
	int lencourse = 0;
	double rec1;
	string ch, ch1;

	//����Ҫ����ʾ��
	cout << "Please input the records of students with different courses and grades:" << endl;
	//������������
	cout << "Example : 1 meiyanxiao chinese 100 math 84 -1. " << endl << endl;
	//˵�������ʽ����
	cout << "We specify the use of - 1 to indicate that a student's grades in each subject have been fully entered, "
		<< "or that all students' grades in each subject have been fully entered." << endl;
	//�ȴ�����
	cout <<endl<< "Please enter your record(only take one decimal place for grades input):" << endl;

	while (1)
	{
		cin >> rec;
		if (rec == -1)
			break;
		//��������ѧ��id������
		id.push_back(rec);
		cin >> ch;
		name.push_back(ch);
		cin >> ch1;
		//��¼��ֽ��������Կ��������ʽ
		if (ch.length()+1 > lenname)
			lenname = ch.length()+1;

		while (ch1 != "-1")
		{
			//��ͼ��ӳ�ṹ�м�¼ѧ������Ŀ���Ƽ��ɼ�
			mp[rec].cnameid.push_back(ch1);
			cin >> rec1;
			mp[rec].cgrade.push_back(rec1);
			//����Ŀ�Ѵ������Ŀ�󼯺���
			for ( i = 0;i < course.size();i++)
				if (course[i] == ch1)
				{
					avegra[i] += rec1;
					number[i] += 1;
					if (rec1 > max[i])
						max[i] = rec1;
					else if (rec1 < min[i])
						min[i] = rec1;
					break;
				}
			//���¿�Ŀͳ�Ƽ���
			if (i >= course.size())
			{
				course.push_back(ch1);
				avegra.push_back(rec1);
				max.push_back(rec1);
				min.push_back(rec1);
				number.push_back(1);
			}
			if (ch1.length() > lencourse)
				lencourse = ch.length();
			cin >> ch1;
		}
	}
	//�������Ŀƽ��ֵ
	for (i=0;i < course.size();i++)
		avegra[i] = avegra[i] / number[i];

	//��������ʾ
	cout << endl << "The result is output there." << endl;
	//ת�������ʽΪ����룬fixed����������Ӧ��ʽ�����ȣ����ÿո�������λ
	cout << setiosflags(ios_base::left) << setiosflags(ios_base::fixed);
	cout.fill();
	//������Ա�����
	cout << setw(8) << "no" << setw(lenname) << "name";
	for (i = 0;i < course.size();i++)
		cout << setw(lencourse) << course[i];
	cout << setw(7) << "average" << endl;
	
	//�����ѧ��ͳ������
	for (i = 0;i < id.size();i++)
	{
		cout << setw(8) << id[i] << setw(lenname) << name[i];
		cout.precision(1);
		//����Ŀ�ܱ�˳�����ѧ����Ϣ
		for (int j = 0;j < course.size();j++)
		{
			for (z = 0;z < mp[id[i]].cnameid.size();z++)
				if (mp[id[i]].cnameid[z] == course[j])
				{
					cout << setw(lencourse) << mp[id[i]].cgrade[z];
					break;
				}
			if (z >= mp[id[i]].cnameid.size())
				cout << setw(lencourse) << "none";
		}
		//����ѧ���͸���ƽ��ֵ�������
		cout.precision(5);
		//������������븡��������������ʽ
		double result = mp[id[i]].average();
		if (result == int(result))
			cout << setw(7) << int(result) << endl;
		else
			cout << setw(7) << result << endl;
	}

	//�������Ŀ��ͳ����Ϣ
	cout.precision(1);
	//���ƽ��ֵ
	cout << setw(8)<<' '<< setw(lenname) << "average";
	for (double e : avegra)
		cout <<setw(lencourse)<< e;
	cout << endl;

	//�����Сֵ
	cout <<setw(8)<<' '<<setw(lenname)<< "min";
	for (double e : min)
		cout <<setw(lencourse)<< e ;
	cout << endl;

	//������ֵ
	cout << setw(8) << ' ' << setw(lenname) << "max";
	for (double e : max)
		cout <<setw(lencourse)<< e ;

	return 0;
}
