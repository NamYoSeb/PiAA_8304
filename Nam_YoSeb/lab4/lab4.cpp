#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <algorithm>
#include <set>


std::vector<int> prefix_function(std::string s) {
	int n = (int)s.length();
	std::vector<int> pi(n, 0);
	int i = 1, j = 0;

	std::cout << "-----------------------------------" << std::endl;
	std::cout << s << std::endl;

	while (i < n) 
	{
		for (int h = 0; h < n; h++)
			std::cout << pi[h] << ' ';
		std::cout << std::endl;

		if (s[i] == s[j]) 
		{
			std::cout << s[i] << " == " << s[j] << " index: " << i << ' ' << j << "; pi[" << i << "] => " << j + 1 << std::endl;
			pi[i] = j + 1;
			i++;
			j++;
		}
		else 
		{
			if (j == 0) 
			{
				std::cout << s[i] << " != " << s[j] << " index: " << i << ' ' << j << "; pi[" << i << "] => " << 0 << std::endl;
				pi[i] = 0;
				i++;
			}
			else 
			{
				std::cout << s[i] << " != " << s[j] << " index: " << i << ' ' << j << "; j => " << pi[j - 1] << std::endl;
				j = pi[j - 1];
			}
		}
	}
	for (int h = 0; h < n; h++)
		std::cout << pi[h] << ' ';
	std::cout << std::endl;
	std::cout << "--------------------------------------------" << std::endl;
	return pi;
}

std::vector<int> KMP(std::string t, std::string p, std::vector<int>& pi) {
	std::vector<int> ans;

	std::cout << "�������-������� ��� �������  " << p << std::endl;
	for (int i = 0; i < pi.size(); i++)
		std::cout << pi[i] << ' ';
	std::cout << std::endl;

	int n = t.length();
	int m = p.length();
	int k = 0, l = 0;

	while (k < n)
	{
		if (t[k] == p[l]) 
		{
			std::cout << "����������:   " << t[k] << "==" << p[l] << " index: " << k << " " << l << std::endl;
			k++; 
			l++;
			if (l == m) { ans.push_back(k - l); std::cout << "������� ���������\n-----------------------" << std::endl; }
		}
		else 
		{
			if (l == 0)
			{
				std::cout << "������������: " << t[k] << "!=" << p[l] << " index: " << k << " " << l << std::endl;
				k++;
			}
			else 
			{
				std::cout << "������������: " << t[k] << "!=" << p[l] << " index: " << k << " " << l << std::endl;
				l = pi[l - 1];
			}
		}
	}

	return ans;
}

void split(std::string t, std::string p, int k, std::vector<std::string>& str, std::vector<int>& ans_current, std::set<int>& ans_all, std::vector<int>& pi) {
	int len_parts, flag = 0;
	int k1;
	//---------------------------------------------------
	//���������� ������ ������ �����
	if (t.length() % k) 
	{
		len_parts = int(t.length() / k) + 1; //������ ����� ������
		flag = 1;
		k1 = k - 1;
	}
	else 
	{
		k1 = k;
		len_parts = t.length() / k;
	}
	//---------------------------------------------------
	int begin = 0;
	std::string part = "";
	//���� ��� ��������� ������� �������� �� ������
	while (k1 > 0) 
	{
		part = "";
		part.append(t, begin, len_parts);
		str.push_back(part);
		begin += len_parts;
		k1--;
	}
	if (flag) 
	{
		part = "";
		part.append(t, begin, (t.length() - (len_parts * (k - 1))));
		str.push_back(part);
	}

	//���� ��� ��������� � �������� �������� �� ������ �� ������ ����� ����������� 2 �����
	k1 = 1;
	while (k1 < k) 
	{
		part = "";
		part.append(t, (len_parts * k1) - p.length() + 1, 2 * p.length() - 2);
		int top = (len_parts * k1) - p.length() + 1;

		std::cout << "---------------------------------------" << std::endl;
		std::cout << "��������� � ������� �� ����� ������� - " << part << std::endl;
		std::cout << "������� � �������� ������: ";
		for (int i = 0; i < part.size(); i++) 
			std::cout << i + top << ' ';
		std::cout << std::endl;

		std::cout << "�������:                   ";
		for (int i = 0; i < part.size(); i++) 
		{
			if (i + top > 9)
				std::cout << i << "  ";
			else 
				std::cout << i << " ";
		}
		std::cout << std::endl;

		std::cout << "������� ���������:         ";
		for (int i = 0; i < part.size(); i++)
		{
			if (i + top > 9)
				std::cout << part[i] << "  ";
			else 
				std::cout << part[i] << " ";
		}
		std::cout << std::endl;

		ans_current = KMP(part, p, pi);
		if (ans_current.size() > 0) 
		{
			for (int i = 0; i < ans_current.size(); i++)
			{
				ans_current[i] += top; //���������� ����� ������� ������ ��������� � �������� ������
				ans_all.insert(ans_current[i]);
			}
		}

		k1++;
	}
}


int main(){
	setlocale(LC_ALL, "Russian");

	std::cout << "\t�������\n����� ��������� ��������� ������� ����� ������ ��� �� ��������.\n"
		"\t������� ��� ��������� ������� � ������:\n����� ������ - 1\n�������� - KMP ��� kmp\n"
		"\t����������, �������� �� ����� 1 ����������� ������� ������ 2:\n����� ������ - 2\n�������� - Rotation ��� rotation" << std::endl;
	std::cout << std::endl;

	std::string task;
	std::cout << "������� ����� ������ ��� �������� ���������" << std::endl;
	std::getline(std::cin, task);

	if (task == "KMP" or task == "kmp" or task == "1") 
	{
		std::string p, t;
		std::cout << "������� �����" << std::endl;
		std::getline(std::cin, t);
		std::cout << "������� ������� (������� ���������)" << std::endl;
		std::getline(std::cin, p);

		if (t.length() < p.length()) 
		{
			std::cout << "������� �� ����� ���� ������ ������!" << std::endl;
			std::cout << -1 << std::endl;
			return 0;
		}

		int max_threads = sizeof(std::thread); // ���������� ����������� ��������� ����� �������
		//---------------------------------------------------
		// ���������� �� ������� ������ ����� �������� ������
		double alpha = (double)t.length() / (double)p.length();
		max_threads = std::min(max_threads, int(alpha) - 1);

		if (max_threads <= 0)
			max_threads = 1;
		int k;//= max_threads;

		if (max_threads == 1) {
			std::cout << "����� ��������� ������ ������������ ��� ������� ������" << std::endl;
			k = 1;
		}
		else
		{
			std::cout << "������� ����� �� 1 �� " << max_threads << std::endl;
			std::cin >> k;

			while (k < 1 or k > max_threads) 
			{
				std::cout << "������� ����� �� 1 �� " << max_threads << std::endl;
				std::cin >> k;

			}
		}
		//---------------------------------------------------
		std::vector<int> pi = prefix_function(p);
		std::vector<int> ans, ans_current;
		std::vector<std::string> str;
		std::set<int> ans_all;

		if (k > 1) 
		{
			std::cout << "--------------------------------------------" << std::endl;
			std::cout << "������ ����� ��������� �� " << k << " ������" << std::endl;
		}

		if (k == 1) 
		{
			ans = KMP(t, p, pi);
			for (int j = 0; j < ans.size(); j++)
				ans_all.insert(ans[j]);
		}
		else
		{
			//---------------------------------------------------
			// ���������� ������ ������ �����
			int len_parts;
			if (t.length() % k) 
				len_parts = int(t.length() / k) + 1; //������ ����� ������
			else 
				len_parts = t.length() / k;

			std::cout << "������������ ������ ����� ��������� ������ - " << len_parts << std::endl;
			std::cout << "--------------------------------------------" << std::endl;
			std::cout << std::endl;

			split(t, p, k, str, ans_current, ans_all, pi);
			//---------------------------------------------------
			//��������� �������� ������ �������
			for (int i = 0; i < str.size(); i++) 
			{
				std::cout << "------------------------\n����� ��������� ������     " << str[i] << std::endl;
				std::cout << "������� � �������� ������: ";
				for (int j = 0; j < str[i].size(); j++) 
					std::cout << j + len_parts * i << ' ';
				std::cout << std::endl;

				std::cout << "�������:                   ";
				for (int j = 0; j < str[i].size(); j++) {
					if (j + len_parts * i > 9)
						std::cout << j << "  ";
					else 
						std::cout << j << " ";
				}
				std::cout << std::endl;

				std::cout << "������� ���������:         ";
				for (int j = 0; j < str[i].size(); j++) {
					if (j + len_parts * i > 9)
						std::cout << str[i][j] << "  ";
					else 
						std::cout << str[i][j] << " ";
				}
				std::cout << std::endl;

				ans_current = KMP(str[i], p, pi);
				if (ans_current.size() > 0) {
					for (int j = 0; j < ans_current.size(); j++)
						ans_current[j] += (len_parts * i); // ���������� ����� ������� ������ ������� � �������� ������
					for (int j = 0; j < ans_current.size(); j++)
						ans_all.insert(ans_current[j]);
				}
			}
		}
		// ������ ������
		if (!ans_all.empty()) 
		{
			int end = *ans_all.rbegin();
			ans_all.erase(end);
			std::cout << end << std::endl;
		}
		else 
			std::cout << -1 << std::endl;
	}
	else 
	{
		if (task == "Rotation" or task == "rotation" or task == "2") 
		{
			std::string a, b;
			std::cout << "������� ������ 1 � 2" << std::endl;
			std::cin >> a >> b;
			std::vector<int> pi = prefix_function(b);
			std::cout << "�������-������� ��� ������ 2" << std::endl;

			for (int i = 0; i < pi.size(); i++)
				std::cout << pi[i] << ' ';
			std::cout << std::endl;

			if (b.length() != a.length())
			{
				std::cout << "������ ������ �����!" << std::endl;
				std::cout << "-1" << std::endl;
				return 0;
			}
			if (a == b) {
				std::cout << "������ ���������" << std::endl;
				std::cout << 0 << std::endl;
				return 0;
			}

			std::cout << "it_a - ��������� �� ������� ������ � ������ 1" << std::endl;
			std::cout << "it_b - ��������� �� ������� ������ � ������ 2" << std::endl;

			int it_a = 0, it_b = 0;
			int cikle = 0;
			int al = a.length();

			while (true) 
			{
				if (a[it_a] == b[it_b]) 
				{
					std::cout << "����������:   " << a[it_a] << "==" << b[it_b] << " index: " << it_a << " " << it_b << std::endl;
					it_a++;
					it_b++;
				}
				if (it_a == al) 
				{
					it_a = 0;
					cikle++;
				}
				if (it_b == al)
				{
					std::cout << "����: ";
					std::cout << it_a << std::endl;
					std::cout << "������� �������� ������� it_a + 1, �.�. �� ������ ��� ������ � it_a ��������� �� �� �����" << std::endl;
					return 0;
				}
				else
				{
					if (a[it_a] != b[it_b]) 
					{
						std::cout << "������������: " << a[it_a] << "!=" << b[it_b] << " index: " << it_a << " " << it_b << std::endl;
						if (it_b == 0)
						{
							it_a++;
							std::cout << "����������� it_a" << std::endl;
						}
						else
						{
							it_b = pi[it_b - 1];
							std::cout << "��������� it_b" << std::endl;
						}
					}
				}
				if (cikle > 1) 
				{
					std::cout << -1 << std::endl;
					return 0;
				}

			}
		}
	}
	return 0;
}