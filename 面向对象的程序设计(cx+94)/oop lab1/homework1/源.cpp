#include<iostream>
#include<string>
#include<iomanip>
#include<algorithm>
//宏定义输入个数
#define n 10
using namespace std;

int main()
{
	//定义存贮数组
	string name[n];
	//整型数组第11项用于存贮前10项之和
	int score1[11] = { 0 };
	int score2[11] = { 0 };
	int score3[11] = { 0 };
	//max，min数组用于记录每一成绩最大最小值
	int minnum[3] = { 6,6,6 };
	int maxnum[3] = { 0 };
	int maxlength = 0;
	int length,result;

	//输入要求提示语
	cout << "please input 10 records of student with score1,score2 and score3 :" << endl;
	//给予输入样例
	cout << "example : meiyanxiao 5 2 1 " << endl;
	cout << "please enter your record" << endl;
	//读入十项数据
	for ( int i = 0;i < n;i++)
	{
		cin >> name[i];
		cin >> score1[i] >> score2[i] >> score3[i];
		//累计计算各项成绩总和
		score1[10] += score1[i];
		score2[10] += score2[i];
		score3[10] += score3[i];
		//记录name属性最长取值，防止输出格式溢出
		length = name[i].length();
		if (length > maxlength)
			maxlength = length;
		//更新每一成绩max与min数值
		if (score1[i] > maxnum[0])
			maxnum[0] = score1[i];
		if (score1[i] < minnum[0])
			minnum[0] = score1[i];
		if (score2[i] > maxnum[1])
			maxnum[1] = score2[i];
		if (score2[i] < minnum[1])
			minnum[1] = score2[i];
		if (score3[i] > maxnum[2])
			maxnum[2] = score3[i];
		if (score3[i] < minnum[2])
			minnum[2] = score3[i];
	}

	//输出结果提示
	cout << endl << "The result is output there." << endl;
	//转换输出格式为左对齐，fixed；并控制相应格式输出宽度，且用空格填补对齐空位
	cout << setiosflags(ios_base::left) << setiosflags(ios_base::fixed);
	cout.fill();
	length = max(maxlength, 4) + 1;
	//输出属性标题行
	cout << setw(8) << "no" << setw(length) << "name" << setw(8) << "score1"
		<< setw(8) << "score2" << setw(8) << "score3" << setw(7) << "average" << endl;
	//单独成行输出数据
	for (int i = 0;i < n;i++)
	{
		cout << setw(8) << i + 1 << setw(length) << name[i] << setw(8) << score1[i]
			<< setw(8) << score2[i] << setw(8) << score3[i];
		//按照每一行平均值结果决定输出格式
		result = score1[i] + score2[i] + score3[i];
		//若为整数，则正常输出
		if (result % 3 == 0)
			cout << setw(7) << result / 3 << endl;
		//若为小数，则输出指定精度
		else
		{
			cout.precision(5);
			cout << setw(7) << result * 1.0 / 3 << endl;
		}
	}
	cout.precision(1);
	//按指定格式输出各项成绩平均值及最大最小值
	cout << setw(8) << " " << setw(length) << "average" << setw(8) << score1[10] * 1.0 / n
		<< setw(8) << score2[10] * 1.0 / n << setw(8) << score3[10] * 1.0 / n << endl;
	cout<< setw(8) << " " << setw(length) << "min" << setw(8) << minnum[0]
		<< setw(8) << minnum[1] << setw(8) << minnum[2] << endl;
	cout << setw(8) << " " << setw(length) << "max" << setw(8) << maxnum[0]
		<< setw(8) << maxnum[1] << setw(8) << maxnum[2] << endl;

	return 0;
}