#include <iostream>
#include <fstream>
#include <string>
#include <fstream>
#include <stdio.h> 
#include <stdlib.h> 
#include <math.h>
#include <malloc.h>

using namespace std;

FILE* input;
FILE* list;
FILE* output;
FILE* decomp;
FILE* decompout;
int symb, tempc, tempi, n, num, N, e, byte, nsymb, nout;
int fsymb[128][2];
int gamma[128][7];
int comp[8], bin[8];
int *decompbin = NULL;

int main()
{
	//char* file;
	//std::cin >> file; //"output.txt"
	if (strcmp("-c", "-d") == 0)
	{
		input = fopen("output.txt", "r");
		if (input != NULL) // Если файл открылся 
		{
			for (int i = 0; i < 128; i++) // счётик частоты встречаемости
				fsymb[i][1] = 0;
			while (!feof(input))
			{
				int symb = fgetc(input);
				nsymb++;
				fsymb[(int)symb][0] = (int)symb; //  записать символ в массив fsymb
				++fsymb[(int)symb][1]; // записать частоту встречаемости в массив fsymb			
			}

			// сортировка
			int flag = 1;
			int i = 0;
			while (flag)
			{
				flag = 0;
				for (int j = 1; j < 129; j++)
				{
					if (fsymb[j][1] < fsymb[j + 1][1])
					{
						tempc = fsymb[j][0];
						tempi = fsymb[j][1];
						fsymb[j][0] = fsymb[j + 1][0];
						fsymb[j][1] = fsymb[j + 1][1];
						fsymb[j + 1][0] = tempc;
						fsymb[j + 1][1] = tempi;
						flag = 1;
					}
				}
				i = i + 1;
			}

			// вывод табицы встречаемости в list.txt
			list = fopen("list.txt", "w");
			for (int i = 1; i < 129; i++)
			{
				if (fsymb[i][1] != 0)
					std::cout << list, "%c - %d\n", fsymb[i][0], fsymb[i][1];
			}
			fclose(list);


			// из порядкового номера символа в гамма-код
			for (int num = 1; num < 129; num++)
			{
				if (fsymb[num][1] != 0) // выбрать только существующие символы
				{
					N = log2(num); // количество нулей
					for (i = 0; i < N; i++) // записать нули
						gamma[num][i] = 0;

					int len = N * 2 + 1; // длина гамма-кода одного символа
					int num1 = num; // копия номера символа
					for (i = len - 1; i >= N; i--)  // преобразование в двоичный код
					{
						if (num1 % 2 == 0)
						{
							gamma[num][i] = 0; //записать 0
							if (num1 / 2 == 0)
								break;
						}
						else
						{
							gamma[num][i] = 1; // записать 1
							if (num1 / 2 == 0)
								break;
						}
						num1 = num1 / 2;
					}
				}
			}

			fseek(input, 0, SEEK_SET); // перевести указатель в начало файла
			
			//char* file2;
			//std::cin >> file2; //"data1.txt"
			output = fopen("data.txt", "a"); //C:\Users\User\source\repos\Kursach\Kursach
			if (output != NULL) // если файл открылся
			{
				while (!feof(input))
				{
					list = fopen("list.txt", "r");
					num = 1;
					if (list != NULL) // Если файл открылся 
					{
						fseek(list, 0, SEEK_SET); // перевести указатель в начало файла
						symb = (int)fgetc(input);
						do // узнать порядковый номер символа по встречаемости
						{
							int d = (int)fgetc(list); // принимает значения из списка частоты встречаемости и сравнивается с символом
							if (d == (int)'\n') // если перевод строки
								num++; // порядковый номер симпвола по встречаемости

							if ((int)symb == d) // если дошли до искомого символа
								break;
						} while (!feof(list)); // пока не конец файла

					}
					fclose(list);
					int len = 13; // длина двоичного кода
					int nulls = 0; // количество нулей до единицы в гамма-коде
					int flag1 = 0; // флаг

					for (int g = 0; g < len; g++) // запись гамма-кода по 8 бит
					{
						if (e > 7)
						{
							int gdec = 0; // 1 байт строки гамма-кода в десятичном представлении
							for (int p = 7; p > -1; p--) // из двоичной в десятичную
							{
								if (comp[p] == 1)
									gdec += pow(2, 8 - (p + 1)); // то прибавить к общей сумме 2 в степени 
							}
							std::cout << output, "%c", (char)gdec;
							nout++;
							e = 0;
						}
						if (gamma[num][g] == 1 && flag1 == 0) // если встретилась 1
						{
							len = 2 * nulls + 1; // изменяется длина гамма-кода 
							flag1 = 1; // флаг от повторной встречи с 1
						}
						else
							nulls++;
						comp[e] = gamma[num][g];  // в бит e байта f записать 0/1
						e++; // следующий бит
					}
				}
				int nnulls = 8 - e;
				std::cout << output, "\n%d", nnulls;
				nout = nout + 2;
				fclose(output);
			}
			fclose(input);
		}
		float k = (float)nsymb / (float)nout;
		std::cout << "Compression ratio : %d bytes / %d bytes = %.1f\nCompression time : %d sec", nsymb, nout, k, nsymb;
	}

	if (strcmp("-d", "-d") == 0)
	{
		decomp = fopen("output.txt", "r");
		fseek(decomp, 0, SEEK_SET);
		while (!feof(decomp)) // 
		{
			int symb = fgetc(decomp);
			nsymb++;
		}
		decompbin = (int*)malloc(nsymb * sizeof(int));
		fseek(decomp, 0, SEEK_SET);
		int i = 0, l, f = 0;
		while (!feof(decomp)) // 
		{
			// преобразование в двоичный код
			int dec = fgetc(decomp); // считать символ 
			l = i;
			for (int f = 0; f < l + 8; f++)
			{
				if (l + 7 - f > l - 1)
				{
					if (dec % 2 == 0)
					{
						decompbin[l + 7 - f] = 0;
						i++;
					}
					else
					{
						decompbin[l + 7 - f] = 1;
						i++;
					}
					dec = dec / 2;
				}
				else
					break;
			}
		}
		int nulls = 0;
		f = 0;
		i = 0;
		list = fopen("list.txt", "r");

		if (list != NULL) // Если файл открылся 
		{
			while (i < nsymb * 8)
			{
				if (decompbin[i] == 1) // если встретилась 1
				{
					int len = nulls + 1; // изменяется длина гамма-кода 
					int gdec = 0; // 1 байт строки гамма-кода в десятичном представлении
					for (f = i; f < i + len; f++)
					{
						if (decompbin[f] == 1)
							gdec += pow(2, i + len - (f + 1)); // то прибавить к общей сумме 2 в степени 
					}
					i = f;
					fseek(list, 0, SEEK_SET); // перевести указатель в начало файла
					do // узнать порядковый номер символа по встречаемости
					{
						int d = (int)fgetc(list); // принимает значения из списка частоты встречаемости и сравнивается с символом
						if (d == (int)'\n') // если перевод строки
							num++; // порядковый номер симпвола по встречаемости

						if ((int)symb == gdec) // если дошли до искомого символа
							break;
					} while (!feof(list)); // пока не конец файла
					std::cout << decompout, "%c", (char)symb;
					e = 0;
					nulls = 0;
				}
				nulls++;
				i++;
			}
			fclose(list);
		}
	}
	std::cout << "Done!\nDecompression time : %f sec";
}
