#include <stdio.h>
#include <conio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// --------------------------- STRUCTURE DEFINITIONS --------------------------- //

typedef struct movie
{
	int movie_number;
	char name[20];
	float price;
} Movie;

typedef struct ticket
{
	Movie M;
	char time[6];
	char date[11];
	_Bool seats[40];
} Ticket;

// --------------------------- VALIDATIONS --------------------------- //

_Bool valid_int(char integer[])
{
	if (isalpha(integer[0]))
		return 0;
	else
		return 1;
}

_Bool valid_price(char price[])
{
	for (int i = 0; price[i] != '\0'; ++i)
	{
		if ((price[i] >= '0' && price[i] <= '9'))
			continue;
		else if (price[i] == '.' && price[i + 1] >= '0' && price[i + 1] <= '9')
			continue;
		else
			return 0;
	}
	return 1;
}

_Bool valid_time(char time[])
{
	if (strlen(time) != 5 || time[2] != ':')
		return 0;
	if (!(isdigit(time[0]) && isdigit(time[1]) && isdigit(time[3]) && isdigit(time[4])))
		return 0;

	int h = (time[0] - '0') * 10 + time[1] - '0';
	int m = (time[3] - '0') * 10 + time[4] - '0';

	if (!(h >= 0 && h <= 23))
		return 0;
	if (!(m >= 0 && h <= 59))
		return 0;

	return 1;
}

// --------------------------- FUNCTION DEFINITIONS --------------------------- //

/*
Date():
Input: -NIL- 
Returns: Current Date String of Format: "08/03/2020"
*/

const char *Date(void)
{
	time_t mytime = time(NULL);
	char *timeInfo = ctime(&mytime);
	char *date = malloc(11);

	date[0] = timeInfo[8];
	date[1] = timeInfo[9];
	date[2] = '/';
	if (timeInfo[4] == 'J')
	{
		if (timeInfo[5] == 'a')
		{
			date[3] = '0';
			date[4] = '1';
		}
		else if (timeInfo[6] == 'n')
		{
			date[3] = '0';
			date[4] = '6';
		}
		else
		{
			date[3] = '0';
			date[4] = '7';
		}
	}
	else if (timeInfo[4] == 'F')
	{
		date[3] = '0';
		date[4] = '2';
	}
	else if (timeInfo[4] == 'M')
	{
		if (timeInfo[6] == 'r')
		{
			date[3] = '0';
			date[4] = '3';
		}
		else
		{
			date[3] = '0';
			date[4] = '5';
		}
	}
	else if (timeInfo[4] == 'A')
	{
		if (timeInfo[5] == 'p')
		{
			date[3] = '0';
			date[4] = '4';
		}
		else
		{
			date[3] = '0';
			date[4] = '8';
		}
	}
	else if (timeInfo[4] == 'S')
	{
		date[3] = '0';
		date[4] = '9';
	}
	else if (timeInfo[4] == 'O')
	{
		date[3] = '1';
		date[4] = '0';
	}
	else if (timeInfo[4] == 'N')
	{
		date[3] = '1';
		date[4] = '1';
	}
	else if (timeInfo[4] == 'D')
	{
		date[3] = '1';
		date[4] = '2';
	}
	date[5] = '/';
	date[6] = timeInfo[20];
	date[7] = timeInfo[21];
	date[8] = timeInfo[22];
	date[9] = timeInfo[23];
	date[10] = '\0';

	return date;
}

/*
Time():
Input: -NIL-
Returns: Current Time String of Format: "14:55"
*/

const char *Time(void)
{
	time_t mytime = time(NULL);
	char *timeInfo = ctime(&mytime);

	char *time = malloc(6);
	for (int i = 0; i < 5; ++i)
		time[i] = timeInfo[11 + i];
	time[5] = '\0';
	return time;
}

/*
CompareTime():
Input: Time1 and Time2 as strings
Output: Returns 1, 0 or -1 if Time1 is >, = or < Time2
*/

int CompareTime(const char *time1, const char *time2)
{
	int h1 = (time1[0] - '0') * 10 + time1[1] - '0';
	int h2 = (time2[0] - '0') * 10 + time2[1] - '0';
	int m1 = (time1[3] - '0') * 10 + time1[4] - '0';
	int m2 = (time2[3] - '0') * 10 + time2[4] - '0';
	if (h1 > h2)
		return 1;
	else if (h1 == h2)
	{
		if (m1 > m2)
			return 1;
		else if (m1 == m2)
			return 0;
		else
			return -1;
	}
	else
		return -1;
}

/*
CompareDate():
Input: Date1 and Date2 as strings
Output: Returns 1, 0 or -1 if Date1 is >, = or < Date2
*/

int CompareDate(const char *date1, const char *date2)
{
	int d1 = (date1[0] - '0') * 10 + date1[1] - '0';
	int d2 = (date2[0] - '0') * 10 + date2[1] - '0';
	int m1 = (date1[3] - '0') * 10 + date1[4] - '0';
	int m2 = (date2[3] - '0') * 10 + date2[4] - '0';
	int y1 = (((date1[6] - '0') * 10 + date1[7] - '0') * 10 + date1[8] - '0') * 10 + date1[9] - '0';
	int y2 = (((date2[6] - '0') * 10 + date2[7] - '0') * 10 + date2[8] - '0') * 10 + date2[9] - '0';
	if (y1 > y2)
		return 1;
	else if (y1 == y2)
	{
		if (m1 > m2)
			return 1;
		else if (m1 == m2)
		{
			if (d1 > d2)
				return 1;
			else if (d1 == d2)
				return 0;
			else
				return -1;
		}
		else
			return -1;
	}
	else
		return -1;
}

const char *NextDate(const char *date, int times)
{
	int days_total;

	int dd = (date[0] - '0') * 10 + date[1] - '0';
	int mm = (date[3] - '0') * 10 + date[4] - '0';
	int yyyy = (((date[6] - '0') * 10 + date[7] - '0') * 10 + date[8] - '0') * 10 + date[9] - '0';

	switch (mm)
	{
	case 1:
	case 3:
	case 5:
	case 7:
	case 8:
	case 10:
	case 12:
		days_total = 31;
		break;

	case 4:
	case 6:
	case 9:
	case 11:
		days_total = 30;
		break;

	case 2:
		if (yyyy % 400 == 0 || (yyyy % 4 == 0 && yyyy % 100 != 0))
			days_total = 29;
		else
			days_total = 28;
		break;
	}

	while (times--)
	{
		if (++dd > days_total)
		{
			dd = 1;
			++mm;
		}

		if (mm > 12)
		{
			mm = 1;
			++yyyy;
		}
	}

	char *new_date = malloc(11);
	sprintf(new_date, "%02d/%02d/%d", dd, mm, yyyy);
	return new_date;
}

void input_movieDetails()
{
	Ticket temp;
	char temp_input[50];

	printf("Enter Movie Details:\n\n");

	printf("Movie Name: ");
	scanf(" %[^\n]", temp.M.name);
	// No Validations For Movie Name

	do
	{
		printf("Price per Ticket: ");
		scanf(" %[^\n]", temp_input);

		if (!valid_price(temp_input))
			printf("~Invalid Input~\n");
		else
			temp.M.price = atof(temp_input);
	} while (!valid_price(temp_input));

	int no_shows, no_days;

	do
	{
		printf("Number of Shows per Day: ");
		scanf(" %[^\n]", temp_input);

		if (!valid_int(temp_input))
			printf("~Invalid Input~\n");
		else
			no_shows = atoi(temp_input);
	} while (!valid_int(temp_input));

	char ar_time[no_shows][6];

	for (int i = 0; i < no_shows; ++i)
	{
		do
		{
			printf("Enter Time %d: ", i + 1);
			scanf(" %[^\n]", temp_input);

			if (!valid_time(temp_input))
				printf("~Invalid Input~\n");
			else
				strcpy(ar_time[i], temp_input);
		} while (!valid_time(temp_input));
	}

	do
	{
		printf("How Many Days Should It Run? ");
		scanf(" %[^\n]", temp_input);

		if (!valid_int(temp_input))
			printf("~Invalid Input~\n");
		else
			no_days = atoi(temp_input);
	} while (!valid_int(temp_input));

	for (int i = 0; i < 40; ++i)
		temp.seats[i] = 0;
	temp.M.movie_number = 0; // Changed By Another Function

	FILE *file = fopen("ticket.bin", "ab");

	for (int i = 0; i < no_days; ++i)
	{
		for (int j = 0; j < no_shows; ++j)
		{
			strcpy(temp.date, NextDate(Date(), i));
			strcpy(temp.time, ar_time[j]);
			fwrite(&temp, sizeof(temp), 1, file);
		}
	}

	fclose(file);
	printf("\n~Warning: Only Movie Name and Price Can Be Modified!~\n");
	return;
}

void update_movie_numbers()
{
	FILE *fptr = fopen("ticket.bin", "rb");
	FILE *fptr2 = fopen("temp.bin", "wb");
	if (fptr == NULL || fptr2 == NULL)
	{
		printf("~Error Opening Files~\n");
		exit(0);
	}

	Ticket T;
	int movie_number = 0;
	char temp_name[20] = "";

	while (!feof(fptr))
	{
		fread(&T, sizeof(T), 1, fptr);
		if (feof(fptr))
			break;
		if (strcmp(temp_name, T.M.name))
		{
			movie_number += 1;
			strcpy(temp_name, T.M.name);
		}
		T.M.movie_number = movie_number;
		fwrite(&T, sizeof(Ticket), 1, fptr2);
	}

	fclose(fptr);
	fclose(fptr2);
	remove("ticket.bin");
	rename("temp.bin", "ticket.bin");
	return;
}

void update_file(void)
{
	FILE *fptr = fopen("ticket.bin", "rb");
	FILE *fptr2 = fopen("temp.bin", "wb");
	if (fptr == NULL || fptr2 == NULL)
	{
		printf("~Error Opening Files~");
		exit(1);
	}

	Ticket T;

	while (!feof(fptr))
	{
		fread(&T, sizeof(Ticket), 1, fptr);
		if (feof(fptr))
			break;
		if ((CompareDate(T.date, Date()) == 1) || ((CompareDate(T.date, Date()) == 0 && CompareTime(T.time, Time()) >= 0)))
			fwrite(&T, sizeof(Ticket), 1, fptr2);
	}

	fclose(fptr);
	fclose(fptr2);
	remove("ticket.bin");
	rename("temp.bin", "ticket.bin");

	update_movie_numbers();
	return;
}

/*
print_allMovieNames()
Input: -NIL-
Prints all available movie names along with their movie numbers.
*/

void print_allMovieNames(void)
{
	update_file();
	char movie_name[20] = "";
	FILE *fptr = fopen("ticket.bin", "rb");
	if (fptr == NULL)
	{
		printf("~Error Opening File~");
		exit(1);
	}

	Ticket T;

	while (!feof(fptr))
	{
		fread(&T, sizeof(Ticket), 1, fptr);
		if (feof(fptr))
			break;
		if (strcmp(movie_name, T.M.name))
		{
			strcpy(movie_name, T.M.name);
			printf("%d. %s\n", T.M.movie_number, movie_name);
		}
	}
	printf("\n");

	fclose(fptr);
	return;
}

void edit_movieDetails(int movie_number)
{
	Ticket T;
	_Bool temp = 0;
	int ch;
	char temp_input[50];
	char temp_name[20];
	strcpy(temp_name, "");
	strcpy(temp_input, "");

	FILE *file = fopen("ticket.bin", "rb");
	FILE *file2 = fopen("temp.bin", "wb");
	if (file == NULL || file2 == NULL)
	{
		printf("~Error Opening Files!~");
		exit(0);
	}

	while (!feof(file))
	{
		fread(&T, sizeof(Ticket), 1, file);
		if (feof(file))
			break;

		if ((T.M.movie_number == movie_number) && !temp)
		{
			printf("\nEnter:\n1. Edit Movie Name\n2. Edit Price\n\n");
			do
			{
				printf("Enter Choice: ");
				scanf(" %[^\n]", temp_input);

				if (!valid_int(temp_input))
					printf("~Invalid Input~\n");
				else
					ch = atoi(temp_input);
			} while (!valid_int(temp_input));

			strcpy(temp_name, T.M.name);

			switch (ch)
			{
			case 1:
				printf("\nEnter New Name: ");
				scanf(" %[^\n]", temp_input);
				strcpy(T.M.name, temp_input);
				break;

			case 2:
				do
				{
					printf("\nEnter New Price: ");
					scanf(" %[^\n]", temp_input);

					if (!valid_price(temp_input))
						printf("~Invalid Input~");
					else
						T.M.price = atof(temp_input);
				} while (!valid_price(temp_input));
				break;

			default:
				printf("~Error! Invalid Choice!~\n");
			}
			printf("~Successfully Edited!~\n");
			temp = 1;
		}
		if (!strcmp(T.M.name, temp_name))
		{
			switch (ch)
			{
			case 1:
				strcpy(T.M.name, temp_input);
				break;

			case 2:
				T.M.price = atof(temp_input);
				break;
			}
		}
		fwrite(&T, sizeof(T), 1, file2);
	}

	if (!temp)
		printf("\n Record Not Found ");

	fclose(file);
	fclose(file2);

	remove("ticket.bin");
	rename("temp.bin", "ticket.bin");
	return;
}

void remove_movie(int movie_number)
{
	FILE *fptr = fopen("ticket.bin", "rb");
	FILE *ftemp = fopen("temp.bin", "wb");

	if (fptr == NULL || ftemp == NULL)
	{
		printf("~Error In Opening Files!~\n");
		exit(1);
	}

	Ticket T;
	_Bool found = 0;

	while (!feof(fptr))
	{
		fread(&T, sizeof(Ticket), 1, fptr);
		if (feof(fptr))
			break;
		if (T.M.movie_number != movie_number)
			fwrite(&T, sizeof(Ticket), 1, ftemp);
		else
			found = 1;
	}

	if (!found)
		printf("~No Records Found!~\n");
	else
		printf("~Movie Successfully Removed!~\n");

	fclose(fptr);
	fclose(ftemp);

	remove("ticket.bin");
	rename("temp.bin", "ticket.bin");
	return;
}

void header(void)
{
	printf("\n\t======================================================\n\t\t\tWELCOME TO PZR CINEMAS\n\t======================================================\n\n");
}

void clear_screen(void)
{
	system("cls");
	// Manually Changable Command (Depending on the OS)
	header();
}


void all_records(char name[]) {
    FILE * fptr = fopen(name, "rb");
    if(fptr == NULL) {
        printf("~Error Opening File~");
        exit(1);
    }
    
    Ticket T;

    while(!feof(fptr)) {
        fread(&T, sizeof(Ticket), 1, fptr);
	if(feof(fptr))
		break;
        printf("%d. %s        %s        %s\n", T.M.movie_number, T.M.name, T.date, T.time);
		for(int i = 0; i < 40; ++i)
			printf(T.seats[i] ? "1" : "0");
	printf("\n");
    }
    printf("\n");
    
    fclose(fptr);
}


_Bool any_movies()
{
	FILE *fptr = fopen("ticket.bin", "rb");
	if (fptr == NULL)
	{
		printf("~Error Opening Files!~\n");
		exit(1);
	}

	_Bool available = 1;
	Ticket T;

	fread(&T, sizeof(Ticket), 1, fptr);
	if (feof(fptr))
		available = 0;

	fclose(fptr);
	return available;
}

// --------------------------- MAIN FUNCTION --------------------------- //

int main(void)
{
	char temp_input[50];
	int ch, movie_number_input;
	_Bool quit = 1;

	while (quit)
	{
		clear_screen();
		printf("Enter:\n");
		printf("1. Add Movie\n2. Edit Movie\n3. Remove Movie\n4. Exit\n\n");
		do
		{
			printf("Enter Choice: ");
			scanf(" %[^\n]", temp_input);

			if (!valid_int(temp_input))
				printf("~Invalid Input~\n");
			else
				ch = atoi(temp_input);
		} while (!valid_int(temp_input));

		switch (ch)
		{

		case 1:
			clear_screen();
			printf("\t\t  ----------------------------------\n\t\t            ADDING A MOVIE\n\t\t  ----------------------------------\n\n");
			input_movieDetails();
			printf("~Movie Sucessfully Added~");
			update_file();
			getch();
			break;

		case 2:
			clear_screen();
			printf("\t    ----------------------------------------------\n\t              EDITING DETAILS OF A MOVIE\n\t    ----------------------------------------------\n\n");
			printf("Choose A Movie To Edit:\n");
			print_allMovieNames();
			do
			{
				printf("Enter Choice: ");
				scanf(" %[^\n]", temp_input);

				if (!valid_int(temp_input))
					printf("~Invalid Input~\n");
				else
					movie_number_input = atoi(temp_input);
			} while (!valid_int(temp_input));

			edit_movieDetails(movie_number_input);
			update_file();
			getch();
			break;

		case 3:
			clear_screen();
			printf("\t\t ------------------------------------\n\t\t           REMOVING A MOVIE\n\t\t ------------------------------------\n\n");
			if (!any_movies())
			{
				printf("~No Movies Available!~");
				getch();
				break;
			}
			printf("Choose Movie To Remove:\n");
			print_allMovieNames();

			do
			{
				printf("Enter Choice: ");
				scanf(" %[^\n]", temp_input);

				if (!valid_int(temp_input))
					printf("~Invalid Input~\n");
				else
					movie_number_input = atoi(temp_input);
			} while (!valid_int(temp_input));

			remove_movie(movie_number_input);
			update_file();
			getch();
			break;

		case 4:
			quit = 0;
			break;

			case 5:
			all_records("ticket.bin");
			getch();
			break;

		default:
			printf("~Invalid Choice!~");
			getch();
			break;
		}
	}
	return 0;
}

// --------------------------- END OF PROGRAM --------------------------- //