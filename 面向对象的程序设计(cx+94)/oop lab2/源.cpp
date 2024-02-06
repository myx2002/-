#include<iostream>
#include<string>
#include<iomanip>
#include<algorithm>
#include<vector>
#include<map>

using namespace std;
//建立每个同学相应的记录结构
class information {
	public:
		//记录所有的课程
		vector<string> cnameid;
		//记录课程对映的成绩
		vector<double> cgrade;
		//计算平均成绩函数
		double average();
};

//个人平均成绩计算函数
double information::average()
{
	double total = 0;
	for (int i = 0;i < cnameid.size();i++)
		total += cgrade[i];
	return total / cnameid.size();
}

int main()
{
	//学生序列及姓名数组
	vector<int> id;
	vector<string> name;
	//课程名及最高最低均值数组
	vector<string> course;
	vector<int> number;
	vector<double>avegra;
	vector<double>max;
	vector<double>min;
	//学生序列结构对映图
	map<int, information> mp;

	int rec = 0, i, z;
	int lenname = 0;
	int lencourse = 0;
	double rec1;
	string ch, ch1;

	//输入要求提示语
	cout << "Please input the records of students with different courses and grades:" << endl;
	//给予输入样例
	cout << "Example : 1 meiyanxiao chinese 100 math 84 -1. " << endl << endl;
	//说明输入格式限制
	cout << "We specify the use of - 1 to indicate that a student's grades in each subject have been fully entered, "
		<< "or that all students' grades in each subject have been fully entered." << endl;
	//等待输入
	cout <<endl<< "Please enter your record(only take one decimal place for grades input):" << endl;

	while (1)
	{
		cin >> rec;
		if (rec == -1)
			break;
		//增加输入学生id及姓名
		id.push_back(rec);
		cin >> ch;
		name.push_back(ch);
		cin >> ch1;
		//记录最长字节数，用以控制输出格式
		if (ch.length()+1 > lenname)
			lenname = ch.length()+1;

		while (ch1 != "-1")
		{
			//在图对映结构中记录学生各科目名称及成绩
			mp[rec].cnameid.push_back(ch1);
			cin >> rec1;
			mp[rec].cgrade.push_back(rec1);
			//若科目已存在与科目大集合中
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
			//更新科目统计集合
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
	//计算各科目平均值
	for (i=0;i < course.size();i++)
		avegra[i] = avegra[i] / number[i];

	//输出结果提示
	cout << endl << "The result is output there." << endl;
	//转换输出格式为左对齐，fixed；并控制相应格式输出宽度，且用空格填补对齐空位
	cout << setiosflags(ios_base::left) << setiosflags(ios_base::fixed);
	cout.fill();
	//输出属性标题行
	cout << setw(8) << "no" << setw(lenname) << "name";
	for (i = 0;i < course.size();i++)
		cout << setw(lencourse) << course[i];
	cout << setw(7) << "average" << endl;
	
	//输出各学生统计数据
	for (i = 0;i < id.size();i++)
	{
		cout << setw(8) << id[i] << setw(lenname) << name[i];
		cout.precision(1);
		//按科目总表顺序输出学生信息
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
		//调节学生雇个人平均值输出精度
		cout.precision(5);
		//控制整数结果与浮点数结果的输出格式
		double result = mp[id[i]].average();
		if (result == int(result))
			cout << setw(7) << int(result) << endl;
		else
			cout << setw(7) << result << endl;
	}

	//输出各科目的统计信息
	cout.precision(1);
	//输出平均值
	cout << setw(8)<<' '<< setw(lenname) << "average";
	for (double e : avegra)
		cout <<setw(lencourse)<< e;
	cout << endl;

	//输出最小值
	cout <<setw(8)<<' '<<setw(lenname)<< "min";
	for (double e : min)
		cout <<setw(lencourse)<< e ;
	cout << endl;

	//输出最大值
	cout << setw(8) << ' ' << setw(lenname) << "max";
	for (double e : max)
		cout <<setw(lencourse)<< e ;

	return 0;
}
