#include<iostream>
#include<string>
#include<iomanip>
#include<algorithm>
//�궨���������
#define n 10
using namespace std;

int main()
{
	//�����������
	string name[n];
	//���������11�����ڴ���ǰ10��֮��
	int score1[11] = { 0 };
	int score2[11] = { 0 };
	int score3[11] = { 0 };
	//max��min�������ڼ�¼ÿһ�ɼ������Сֵ
	int minnum[3] = { 6,6,6 };
	int maxnum[3] = { 0 };
	int maxlength = 0;
	int length,result;

	//����Ҫ����ʾ��
	cout << "please input 10 records of student with score1,score2 and score3 :" << endl;
	//������������
	cout << "example : meiyanxiao 5 2 1 " << endl;
	cout << "please enter your record" << endl;
	//����ʮ������
	for ( int i = 0;i < n;i++)
	{
		cin >> name[i];
		cin >> score1[i] >> score2[i] >> score3[i];
		//�ۼƼ������ɼ��ܺ�
		score1[10] += score1[i];
		score2[10] += score2[i];
		score3[10] += score3[i];
		//��¼name�����ȡֵ����ֹ�����ʽ���
		length = name[i].length();
		if (length > maxlength)
			maxlength = length;
		//����ÿһ�ɼ�max��min��ֵ
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

	//��������ʾ
	cout << endl << "The result is output there." << endl;
	//ת�������ʽΪ����룬fixed����������Ӧ��ʽ�����ȣ����ÿո�������λ
	cout << setiosflags(ios_base::left) << setiosflags(ios_base::fixed);
	cout.fill();
	length = max(maxlength, 4) + 1;
	//������Ա�����
	cout << setw(8) << "no" << setw(length) << "name" << setw(8) << "score1"
		<< setw(8) << "score2" << setw(8) << "score3" << setw(7) << "average" << endl;
	//���������������
	for (int i = 0;i < n;i++)
	{
		cout << setw(8) << i + 1 << setw(length) << name[i] << setw(8) << score1[i]
			<< setw(8) << score2[i] << setw(8) << score3[i];
		//����ÿһ��ƽ��ֵ������������ʽ
		result = score1[i] + score2[i] + score3[i];
		//��Ϊ���������������
		if (result % 3 == 0)
			cout << setw(7) << result / 3 << endl;
		//��ΪС���������ָ������
		else
		{
			cout.precision(5);
			cout << setw(7) << result * 1.0 / 3 << endl;
		}
	}
	cout.precision(1);
	//��ָ����ʽ�������ɼ�ƽ��ֵ�������Сֵ
	cout << setw(8) << " " << setw(length) << "average" << setw(8) << score1[10] * 1.0 / n
		<< setw(8) << score2[10] * 1.0 / n << setw(8) << score3[10] * 1.0 / n << endl;
	cout<< setw(8) << " " << setw(length) << "min" << setw(8) << minnum[0]
		<< setw(8) << minnum[1] << setw(8) << minnum[2] << endl;
	cout << setw(8) << " " << setw(length) << "max" << setw(8) << maxnum[0]
		<< setw(8) << maxnum[1] << setw(8) << maxnum[2] << endl;

	return 0;
}