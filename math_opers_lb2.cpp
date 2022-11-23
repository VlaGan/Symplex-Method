#include <iostream>
#include <string>
#include <vector>
#include <conio.h>
#include <stdlib.h>

// Для дослідження ЗЛП на чутливість
//#define PLUS_ONE_RES //- розв'язок + розв'язок і порівняння початкової таблиці з доданою 1 до ресурсів
//#define PLUS_ONE_PROD //- звичайний розв'язок і додавання нового продукту
//#define CHANGE_RESOURCES //- заміна ресурсів на інші

// Тестування
//#define DEBUG //- введення даних вручну
//#define SHOW_ALL_INF //- виведення додаткових даних при розв'язку (Aij/bi , i && j вибраного елемента)
//#define PROD_DEBUG //- введення нового продукту вручну

constexpr auto bval = 100000;

void print_array(float** mass, const int h, const int w, std::vector<std::string> xi) {
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++)
			std::cout << mass[i][j] << "\t";
		std::cout << std::endl;
	}
	std::cout << std::endl;
	for (int i = 0; i < xi.size(); i++) {
		if (i < xi.size() - 1)
			std::cout << "X" << xi[i] << " = " << mass[i][w - 1] << ", ";
		else
			std::cout << "X" << xi[i] << " = " << mass[i][w - 1] << std::endl;
	}
}

void get_i_j(float** mass, const int h, const int w, int& pi, int& pj) {
	int p1 = 0;
	int p2 = 0;

	float minc = mass[h - 1][0];
	for (int i = 0; i < w; i++) {
		if (minc > mass[h - 1][i]) {
			minc = mass[h - 1][i];
			p2 = i;
		}
	}

	float* q = new float[h - 1];
	for (int i = 0; i < h - 1;i++) {
		if (mass[i][p2] > 0)
			q[i] = mass[i][w - 1] / mass[i][p2];
		else
			q[i] = -100;
	}

	float min = bval;
	for (int i = 0; i < h - 1;i++) {
		if (q[i] >= 0 && min > q[i]) {
			min = q[i];
			p1 = i;
		}
	}

#ifdef SHOW_ALL_INF
	std::cout << "i = " << p1 << " j = " << p2 << std::endl;
	std::cout << "\nAij/bi!!!\n";
	for (int i = 0; i < h - 1;i++)
		std::cout << q[i] << "\t";
	std::cout << "\n\n";
#endif

	delete[] q;
	q = nullptr;
	pi = p1;
	pj = p2;
}

void symplex_method(float** mas, const int hight, const int width, std::vector<std::string> &xi) {
	int p1 = 0;
	int p2 = 0;
	get_i_j(mas, hight, width, p1, p2);

	if (mas[p1][p2] != 0) {


		if (mas[p1][p2] != 1 && mas[p1][p2] != 0) {
			float det = mas[p1][p2];
			for (int j = 0; j < width; j++) {
				if (p2 != j)
					mas[p1][j] /= det;
			}
			mas[p1][p2] /= det;
		}
			
		for (int i = 0; i < hight; i++) {
			if (i != p1) {
				for (int j = 0; j < width; j++) {
					if (j != p2) {
						mas[i][j] = mas[i][j] + (mas[p1][j] * (-mas[i][p2]));
					}
				}
				mas[i][p2] = 0;
			}
		}

		xi.at(p1) = std::to_string(p2+1);

	}
	print_array(mas, hight, width, xi);
}

void get_cond(float** mas, const int hight, const int width, std::vector<std::string> xi) {
	bool not_minus_c = true;
	for (int k = 0; k < width - 1;k++) {
		if (mas[hight - 1][k] < 0) {
			not_minus_c = false;
		}
	}

	if (!not_minus_c) {
		symplex_method(mas, hight, width, xi);
		return get_cond(mas, hight, width, xi);
	}else
		std::cout << std::endl << "F(x) = " << mas[hight - 1][width - 1] << std::endl;
}

int CheckDefines() {
#if (defined(PLUS_ONE_RES) && (defined(PLUS_ONE_PROD) || defined(CHANGE_RESOURCES))) || (defined(PLUS_ONE_PROD) && (defined(PLUS_ONE_RES) || defined(CHANGE_RESOURCES))) ||  (defined(CHANGE_RESOURCES) && (defined(PLUS_ONE_RES) || defined(PLUS_ONE_PROD)))
	return 1;
#endif
	return 0;
}

int main() {

	if (CheckDefines()) {
		std::cout << "MUST BE DEFINED only PLUS_ONE_RES or PLUS_ONE_PROD or CHANGE_RESOURCES!";
		return -1;
	}

#ifndef DEBUG
	int h, w;
	std::cout << "Enter the num of rows: ";
	std::cin >> h;
	std::cout << "Enter the num of colls: ";
	std::cin >> w;
#else
	// input your data
	int h = 3, w = 5;
	float ms[3][5] = {
	{1,2,1,0,70},
	{3,1,0,1,25},
	{-5,-3,0,0,0}
	};
#endif


#ifdef PROD_DEBUG
	float prod_ms[3][6] = {
		{1,2,4,1,0,70},
		{3,1,2,0,1,25},
		{-5,-3,-3,0,0,0}
	};
#endif


#if defined(PLUS_ONE_RES) || defined (PLUS_ONE_PROD) || defined(CHANGE_RESOURCES)
	float** arr = new float* [h];
	float** arr_opt = new float* [h];
	for (int i = 0; i < h; i++) {
		arr[i] = new float[w];
		arr_opt[i] = new float[w];
	}
#else
	float** arr = new float* [h];
	for (int i = 0; i < h; i++)
		arr[i] = new float[w];
#endif

	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
#ifndef DEBUG
			std::cout << "Enter a[" << i + 1 << "][" << j + 1 << "]: ";
			std::cin >> arr[i][j];
#else
			arr[i][j] = ms[i][j];
#endif
#if defined(PLUS_ONE_RES) || defined(PLUS_ONE_PROD) || defined(CHANGE_RESOURCES)
			arr_opt[i][j] = arr[i][j];
#endif
		}
	}
	std::cout << std::endl;


	std::vector<std::string> xi(h - 1);
	for (int i = 0; i < h - 1; i++)
		xi[i] = std::to_string(w - h + i + 1);

	print_array(arr, h, w, xi);
	get_cond(arr, h, w, xi);

#ifdef PLUS_ONE_PROD
	const int prod_w = w + 1;
	float** arr_prod = new float* [h];
	for (int i = 0; i < h; i++)
		arr_prod[i] = new float[prod_w];

#ifndef PROD_DEBUG
	std::cout << "\nEnter the table again with adding of new product: \n";

	const int prod_pos = prod_w - h-1;
	for (int i = 0; i < h; i++)
		for (int j = 0; j < prod_w; j++)
			if (j == prod_pos) {
				std::cout << "Enter new prod a[" << i + 1 << "][" << j + 1 << "]: ";
				std::cin >> arr_prod[i][j];
			}
			else if (j < prod_pos)
				arr_prod[i][j] = arr_opt[i][j];
			else
				arr_prod[i][j] = arr_opt[i][j - 1];
#else
	for (int i = 0; i < h; i++)
		for (int j = 0; j < prod_w; j++)
			arr_prod[i][j] = prod_ms[i][j];
#endif
#endif

#if defined(PLUS_ONE_RES) || defined(PLUS_ONE_PROD) || defined(CHANGE_RESOURCES)
	const float result_one = arr[h-1][w-1];
	float deltaf{};

#ifdef PLUS_ONE_RES
	for (int i = 0; i < h - 1; i++)
		arr_opt[i][w - 1]++;

	for (int i = 0; i < h - 1; i++)
		xi[i] = std::to_string(w - h + i + 1);

	std::cout << std::endl << "After plus 1 unit to resources: " << std::endl;
	print_array(arr_opt, h, w, xi);
	get_cond(arr_opt, h, w, xi);

	deltaf = arr_opt[h - 1][w - 1] - result_one;
	std::cout << std::endl;
	if (deltaf)
		std::cout << "Increasing resources is cost effective = " << deltaf << " units.";
	else
		std::cout << "Increasing resources is NOT cost effective = " << deltaf << " units.";

#endif

#ifdef PLUS_ONE_PROD

	std::vector<std::string> xi_prod(h - 1);
	for (int i = 0; i < h - 1; i++)
		xi_prod[i] = std::to_string(prod_w - h + i + 1);

	std::cout << std::endl << "After adding 1 product: " << std::endl;
	print_array(arr_prod, h, prod_w, xi_prod);
	get_cond(arr_prod, h, prod_w, xi_prod);

	deltaf = arr_prod[h - 1][prod_w - 1] - result_one;
	std::cout << std::endl;
	if (deltaf)
		std::cout << "Increasing adding new product is cost effective = " << deltaf << " units.";
	else
		std::cout << "Increasing adding new product is NOT cost effective = " << deltaf << " units.";
#endif


#ifdef CHANGE_RESOURCES
	for (int i = 0; i < h - 1; i++)
		arr_opt[i][w - 1]++;

	for (int i = 0; i < h - 1; i++)
		xi[i] = std::to_string(w - h + i + 1);

	for (int i = 0; i < h - 1; i++) {
		std::cout << "Enter new num of resources number" << i+1<<": ";
		std::cin >> arr_opt[i][w - 1];
	}
	std::cout << std::endl;
	print_array(arr_opt, h, w, xi);
	get_cond(arr_opt, h, w, xi);

	deltaf = arr_opt[h - 1][w - 1] - result_one;
	std::cout << std::endl;
	if (deltaf)
		std::cout << "Change num of resources is cost effective = " << deltaf << " units.";
	else
		std::cout << "Change num of resources NOT cost effective = " << deltaf << " units.";

#endif

#endif

#if defined(PLUS_ONE_RES) || defined(PLUS_ONE_PROD) || defined(CHANGE_RESOURCES)
	for (int i = 0; i < h; i++) {
		delete[] arr[i];
		delete[] arr_opt[i];
	}
	delete[] arr;
	delete[] arr_opt;І
#else
	for (int i = 0; i < h; i++)
		delete[] arr[i];
	delete[] arr;
#endif

#ifdef PLUS_ONE_PROD
	for (int i = 0; i < h; i++)
		delete[] arr_prod[i];
	delete[] arr_prod;

#endif

	return _getch();
}