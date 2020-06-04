#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>

#define LONGLINE "-------------------------------------------------------------------------------------\n"

struct Vertex {
	std::map<char, int> next;   //Контейнер переходов по символу char в вершину с номером int
	std::map<char, int> jump;     //массив переходов (запоминаем переходы в ленивой рекурсии), используемый для вычисления суффиксных ссылок
	bool is_terminal = false;    //Является ли терминальной (конечной) вершиной (на которой заканчивается паттерн)
	int prev;                   //Номер предыдущей вершины (родителя)
	char prev_char;              //Символ, по которому пришли в вершину
	int suffix;                 //Суффиксная ссылка
	std::vector<int> number;    //Какой по счёту считанный паттерн заканчивается на этой вершине (только для вывода)
	int deep;                   //глубина в боре, равная длине строки-паттерна, заканчивающегося в этой терминальной вершине
};

int jump(int index, char symb, std::vector<Vertex>& arr_vertex, std::ostream& out);

void addString(const std::string& str, std::vector<Vertex>& arr_vertex, int& count, std::ostream& out) {  //Функция добавления строки-паттерна в бор
	std::cout << "Adding string \"" << str << "\" in the bohr" << std::endl;
	if (str.empty())
		return;

	int current = 0;
	for (int i = 0; i < str.size(); i++) 
	{
		std::cout << "\tCurrent symbol: \'" << str[i] << "\'\n";
		std::cout << "\tCurrent vertex: " << current << std::endl;

		if (arr_vertex[current].next.find(str[i]) == arr_vertex[current].next.end())     //Если переход по символу не обнаружен и
		{
			std::cout << "\tWay through \'" << str[i] << "\' wasn't found. Adding new vertex with number " << arr_vertex.size() << std::endl;
			Vertex ver;                                                                 //итератор указывает на конец мэпа, то
			ver.suffix = -1;                                                            //создаём новую вершину
			ver.prev = current;
			std::cout << "\t*previous vertex is " << current << ", the symbol of incoming path \'" << str[i] << "\'\n";
			ver.prev_char = str[i];
			arr_vertex.push_back(ver);
			arr_vertex[current].next[str[i]] = arr_vertex.size() - 1;                     //У предыдущей вершины переход в эту
		}                                                                               //по текущему символу
		else 
			std::cout << "The way through the symbol exist" << std::endl;

		std::cout << std::endl;
		current = arr_vertex[current].next[str[i]];                                      //Переход к следующей вершине
	}
	std::cout << "The number of this pattern is " << count + 1 << std::endl;
	std::cout << "Vertex " << current << " is terminal, deep of the vertex is " << str.size() << "\n\n";

	arr_vertex[current].number.push_back(++count);       //Устанавливаем номер считанного паттерна,
	arr_vertex[current].is_terminal = true;               //Терминальную вершину
	arr_vertex[current].deep = str.size();               //Глубину
}


int getSuffix(int index, std::vector<Vertex>& arr_vertex, std::ostream& out) {   //Функция поиска суффиксной ссылки для вершины index
	std::cout << "\t\t\tGetting suffix-link from vertex " << index << std::endl;
	if (arr_vertex[index].suffix == -1)                     //Если суффиксная ссылка ещё не была найдена
	{
		if (index == 0 || arr_vertex[index].prev == 0)     //Если вершина - корень или сын корня
		{
			arr_vertex[index].suffix = 0;
			(index == 0) ? std::cout << "\t\t\tThis is root, suffix-link vertex = 0" << std::endl : std::cout << "\t\t\tThis is a vertex with deep = 1, suffix-link = 0" << std::endl;
		}
		else                                               //Рекурсивный поиск суфф. ссылки. Получаем ссылку родителя и выполняем
		{
			std::cout << "\t\t\tFinding suffix-link from suffix of parent-vertex (" << arr_vertex[index].prev << ") through " << arr_vertex[index].prev_char << std::endl;
			arr_vertex[index].suffix = jump(getSuffix(arr_vertex[index].prev, arr_vertex, out), arr_vertex[index].prev_char, arr_vertex, out);
		}                                                   //из неё переход по символу, по которому попали в вершину, для
	}                                                       //которой и ищется суфф. ссылка
	std::cout << "\t\t\tSuffix-link from vertex " << index << " is " << arr_vertex[index].suffix << "\n\n";
	return arr_vertex[index].suffix;
}


int jump(int index, char symb, std::vector<Vertex>& arr_vertex, std::ostream& out){     //Функция перехода из вершины index по символу symb. Если прямой переход
	std::cout << "\t\t\t*Finding the way from " << index << " through \'" << symb << "\'\n"; //невозможен, перейдёт по ссылке
	if (arr_vertex[index].jump.find(symb) == arr_vertex[index].jump.end())             //Если путь в массиве переходов ещё не был найден
	{
		if (arr_vertex[index].next.find(symb) != arr_vertex[index].next.end())     //Если найден прямой переход по символу в боре
			arr_vertex[index].jump[symb] = arr_vertex[index].next[symb];              //Добавляем в контейнер возможных переходов
		else                                                                //Если прямого перехода нет, получаем суфф. ссылку
		{																	//и ищем переход из суффиксной ссылки по заданному символу
			if (index == 0)                                                   
				std::cout << "\t\t\t*This is root" << std::endl;
			else
				std::cout << "\t\t\t*No straight path. Finding the way from suffix-link of this vertex through \'" << symb << "\'\n";

			arr_vertex[index].jump[symb] = (index == 0 ? 0 : jump(getSuffix(index, arr_vertex, out), symb, arr_vertex, out));
		}
	}
	std::cout << "\t\t\t*Found way from " << index << " through \'" << symb << "\' is " << arr_vertex[index].jump[symb] << std::endl;
	return arr_vertex[index].jump[symb];
}


void search(const std::string& text, std::vector<Vertex>& arr_vertex, std::vector<int>& res, const std::vector<int>& pattern_offset_arr, int pattern_len, const std::vector<std::string>& arr_pattern, std::ostream& out){
	std::cout << "Searching begin" << std::endl;
	int curr = 0;

	for (int i = 0; i < text.size(); i++)                                  //Перебираем все символы текста
	{
		std::cout << "\tCurrent symbol is \'" << text[i] << "\' from text..." << std::endl;
		std::cout << "\tCurrent vertex is " << curr << std::endl;
		curr = jump(curr, text[i], arr_vertex, out);                               //Осуществляем переход в автомате по считанному символу
		std::cout << "\tAchieved vertex " << curr << std::endl;
		std::cout << "\tFinding possible entrance with end suffix-links:\n";

		for (int tmp = curr; tmp != 0; tmp = getSuffix(tmp, arr_vertex, out))   //Сам множественный поиск через суфф. ссылки
		{
			std::cout << "\t\tCurrent suffix-link vertex: " << tmp << std::endl;
			if (arr_vertex[tmp].is_terminal)                                //Если какая-то из них конечная,
			{																//увеличиваем под символом текста число вхождений паттернов
				for (int j = 0; j < arr_vertex[tmp].number.size(); j++) 
				{      
					if (i + 1 - pattern_offset_arr[arr_vertex[tmp].number[j] - 1] - arr_vertex[tmp].deep >= 0 &&
						i + 1 - pattern_offset_arr[arr_vertex[tmp].number[j] - 1] - arr_vertex[tmp].deep <= text.size() - pattern_len) 
					{
						res[i + 1 - pattern_offset_arr[arr_vertex[tmp].number[j] - 1] - arr_vertex[tmp].deep]++;
						std::cout << "\t\tThe vertex is terminal (end suffix-link). The entrance found, index = " <<
							i + 1 - pattern_offset_arr[arr_vertex[tmp].number[j] - 1] - arr_vertex[tmp].deep << " (pattern = \"" << arr_pattern[arr_vertex[tmp].number[j] - 1] <<
							"\"). Count of entrance is " << res[i + 1 - pattern_offset_arr[arr_vertex[tmp].number[j] - 1] - arr_vertex[tmp].deep] <<
							" from " << pattern_offset_arr.size() << " possible\n\n";
					}
				}
			}
			else
				std::cout << "\t\tIt's not terminal vertex, getting suffix-link from this vertex\n\n";
		}
		std::cout << "\t\tRoot is arrived, reading new symbol from the text" << std::endl;
		std::cout << "\t" << LONGLINE;
		std::cout << "\t" << LONGLINE;
	}
	std::cout << LONGLINE;
	std::cout << LONGLINE;
}


void printRes(const std::vector<int>& res, int pattern_cnt, std::string& cutted_text, int pattern_len, const std::string& text, std::ostream& out)
{
	std::cout << "Total indexes of entrance (beginning from 1):" << std::endl;
	std::vector<bool> cut_str(text.size());          //Индексы символов в строке, которые будут вырезаны

	for (int i = 0; i < res.size(); i++) 
		if (res[i] == pattern_cnt)                  //Если под текущим символом текста совпали все паттерны,
		{											 //то вхождение найдено
			std::cout << i + 1 << std::endl;            
			for (int j = 0; j < pattern_len; j++)    //Перебираем все символы строки, образующие паттерн
				cut_str[i + j] = true;               //Помечаем индексы символов в строке, подлежащие удалению
		}

	for (int i = 0; i < cut_str.size(); i++) 
		if (!cut_str[i])
			cutted_text.push_back(text[i]);            //Сохраняем только неудалённые символы
}

//Функция разбивает строку-паттерн с джокерами на массив строк-паттернов без них и запоминает их индексы в первоначальной строке
void split(std::string str, char joker, std::vector<std::string>& arr_pattern, std::vector<int>& pattern_offset_arr, std::ostream& out) {
	std::cout << "Begin splitting" << std::endl;
	std::string buf = "";

	for (int i = 0; i < str.size(); i++) 
	{
		if (str[i] == joker) 
		{
			if (buf.size() > 0)                   //Пропускаем пустые строки (если джокеры идут подряд)
			{
				arr_pattern.push_back(buf);            //Сохраняем паттерн
				std::cout << "\tWas found new pattern: " << buf << std::endl;
				pattern_offset_arr.push_back(i - buf.size());   //и его индекс вхождения в строку с джокерами
				std::cout << "\tIndex of entrance in total pattern: " << i - buf.size() << std::endl;
				buf = "";
			}
		}
		else 
		{
			buf.push_back(str[i]);                  //Формируем строку-паттерна без джокеров
			if (i == str.size() - 1)               //Если достигнут конец паттерна
			{
				arr_pattern.push_back(buf);            //Сохраняем последний полученный паттерн без джокера
				std::cout << "\tWas found new pattern: " << buf << std::endl;
				pattern_offset_arr.push_back(i - buf.size() + 1);
				std::cout << "\tIndex of entrance in total pattern: " << i - buf.size() + 1 << std::endl;
			}
		}
	}
}

void readPattern(std::vector<Vertex>& arr_vertex, char& joker, std::vector<int>& pattern_offset_arr, int& pattern_len, std::vector<std::string>& arr_pattern, std::istream& in, std::ostream& out) {
	Vertex root;                            //Инициализация корня
	root.prev = -1;
	root.suffix = -1;
	arr_vertex.push_back(root);
	int count = 0;

	std::cout << "Enter pattern:" << std::endl;
	std::string pattern_str;                                    //Строка-паттерн
	std::cin >> pattern_str;
	std::cout << "Enter joker:" << std::endl;
	std::cin >> joker;
	pattern_len = pattern_str.size();                            //Длина паттерна
	std::cout << LONGLINE;

	split(pattern_str, joker, arr_pattern, pattern_offset_arr, out);
	std::cout << LONGLINE;
	std::cout << LONGLINE;
	std::cout << "Begin bohr building" << std::endl;

	for (auto pattern : arr_pattern)
		addString(pattern, arr_vertex, count, out);    //Формируем бор

	std::cout << LONGLINE;
	std::cout << LONGLINE;
}

//Функция поиска максимального числа исходящих дуг из одной вершины бора
int findMaxSons(std::vector<Vertex> arr_vertex) {
	int max = arr_vertex[0].next.size();

	for (int i = 1; i < arr_vertex.size(); i++) 
	{
		if (arr_vertex[i].next.size() > max)
			max = arr_vertex[i].next.size();
	}
	return max;
}

void printAutomate(std::vector <Vertex> arr_vertex, std::ostream& out) {
	std::cout << LONGLINE;
	std::cout << "Total automate:" << std::endl;

	for (int i = 0; i < arr_vertex.size(); i++)
	{
		std::cout << "Connections from vertex " << i << ":\n";
		auto iter = arr_vertex[i].jump.begin();

		for (int j = 0; j < arr_vertex[i].jump.size(); j++)
		{
			std::cout << "\t" << i << "  --" << iter->first << "->  " << iter->second << std::endl;
			iter++;
		}
	}
}


void dialog(std::istream& in, std::ostream& out) {
	std::cout << LONGLINE;
	std::cout << "Enter text:" << std::endl;
	std::string text, cutted_text;
	std::cin >> text;

	std::vector<Vertex> arr_vertex;      //Массив вершин
	std::vector<std::string> arr_pattern;
	std::vector<int> res(110000);    //Массив числа совпадений паттернов под каждым символом строки
	std::vector<int> pattern_offset_arr;
	int pattern_len;                     //Длина паттерна

	for (int i = 0; i < 110000; i++)
		res[i] = 0;

	char joker;

	readPattern(arr_vertex, joker, pattern_offset_arr, pattern_len, arr_pattern, in, out);
	search(text, arr_vertex, res, pattern_offset_arr, pattern_len, arr_pattern, out);
	printRes(res, arr_pattern.size(), cutted_text, pattern_len, text, out);

	std::cout << "Rest string from text after cutting patterns from it: " << cutted_text << std::endl;

	int max_cnt_sons = findMaxSons(arr_vertex);
	std::cout << "Max count of sons: " << max_cnt_sons << "\n\n";

	printAutomate(arr_vertex, out);
}

int main() {
	int choise;
	std::cout << "Read from: 1 - file, 2 - console" << std::endl;
	std::cin >> choise;

	if (choise == 1)
	{
		std::ifstream in("input.txt");
		std::ofstream out("output.txt");
		dialog(in, out);
	}
	else if (choise == 2)
		dialog(std::cin, std::cout);
	else
		std::cout << "durak" << std::endl;

	return 0;
}
