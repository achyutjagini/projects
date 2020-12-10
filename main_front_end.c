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

_Bool valid_name(char name[])
{
	for (int i = 0; name[i] != '\0'; ++i)
		if (!((name[i] == ' ') || (name[i] >= 'A' && name[i] <= 'Z') || (name[i] >= 'a' && name[i] <= 'z')))
			return 0;
	return 1;
}

_Bool valid_date(char date[])
{
	if (strlen(date) != 10 || date[2] != '/' || date[5] != '/')
		return 0;
	if (!(isdigit(date[0]) && isdigit(date[1]) && isdigit(date[3]) && isdigit(date[4]) && isdigit(date[6]) && isdigit(date[7]) && isdigit(date[8]) && isdigit(date[9])))
		return 0;

	int dd = (date[0] - '0') * 10 + date[1] - '0';
	int mm = (date[3] - '0') * 10 + date[4] - '0';
	int yy = (((date[6] - '0') * 10 + date[7] - '0') * 10 + date[8] - '0') * 10 + date[9] - '0';

	if (yy >= 2020)
	{
		if (mm >= 1 && mm <= 12)
		{
			if ((dd >= 1 && dd <= 31) && (mm == 1 || mm == 3 || mm == 5 || mm == 7 || mm == 8 || mm == 10 || mm == 12))
				return 1;
			else if ((dd >= 1 && dd <= 30) && (mm == 4 || mm == 6 || mm == 9 || mm == 11))
				return 1;
			else if ((dd >= 1 && dd <= 28) && (mm == 2))
				return 1;
			else if (dd == 29 && mm == 2 && (yy % 400 == 0 || (yy % 4 == 0 && yy % 100 != 0)))
				return 1;
			else
				return 0;
		}
		else
			return 0;
	}
	else
		return 0;
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

_Bool valid_seat(char seat_number[])
{
	if (strlen(seat_number) == 2)
		if (seat_number[0] >= 'A' && seat_number[0] <= 'E')
			if (seat_number[1] >= '1' && seat_number[1] <= '8')
				return 1;
	return 0;
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

_Bool valid_int(char integer[])
{
	for (int i = 0; integer[i] != '\0'; ++i)
	{
		if ((integer[i] >= '0' && integer[i] <= '9'))
			continue;
		else
			return 0;
	}
	return 1;
}

_Bool valid_char_yn(char input[])
{
	if (strlen(input) == 1 && (input[0] == 'Y' || input[0] == 'N'))
		return 1;
	else
		return 0;
}

_Bool valid_seatCodes(int movie_number, char date[], char time[], char input[], int no_of_tickets)
{
	_Bool check_seatFree(int, char *, char *, char *);

	if (strlen(input) != ((3 * no_of_tickets) - 1))
		return 0;

	char seat_code[3];
	seat_code[2] = '\0';

	for (int i = 0; i < 3 * no_of_tickets; i += 3)
	{
		seat_code[0] = input[i];
		seat_code[1] = input[i + 1];
		if (!valid_seat(seat_code))
			return 0;
		else if (!check_seatFree(movie_number, date, time, seat_code))
			return 0;
	}
	return 1;
}

_Bool valid_ticketCode(char input[])
{
	if (strlen(input) != 16)
		return 0;

	for (int i = 0; i < 14; ++i)
		if (!isdigit(input[i]))
			return 0;
	if (!isdigit(input[15]))
		return 0;

	if (!(input[14] >= 'A' && input[14] <= 'E'))
		return 0;

	return 1;
}

// --------------------------- FUNCTION DEFINITIONS --------------------------- //

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

int seat_index(char seat_code[3])
{
	return ((seat_code[0] - 'A') * 8 + seat_code[1] - '0') - 1;
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

const char *InputDate()
{
	_Bool correct = 1;
	char temp_input[50];
	do
	{
		printf("Enter Date: ");
		scanf(" %[^\n]", temp_input);

		if (valid_date(temp_input))
		{
			if (CompareDate(temp_input, NextDate(Date(), 0)) == 0 || CompareDate(temp_input, NextDate(Date(), 1)) == 0 || CompareDate(temp_input, NextDate(Date(), 2)) == 0)
				correct = 0;
			else
				printf("~Invalid Input~\n");
		}
		else
			printf("~Invalid Input~\n");
	} while (correct);

	char *date = malloc(11);
	strcpy(date, temp_input);
	return date;
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

	return;
}

void print_allMovie_withTimes(char date[])
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
	_Bool NotFirst = 0;

	while (!feof(fptr))
	{
		fread(&T, sizeof(Ticket), 1, fptr);
		if (feof(fptr))
			break;
		if (strcmp(movie_name, T.M.name) && !strcmp(date, T.date))
		{
			strcpy(movie_name, T.M.name);
			if (NotFirst)
				printf("\n\n");
			else
				NotFirst = 1;
			printf("%d. %s\nTimes: %s\t", T.M.movie_number, movie_name, T.time);
		}
		else if (!strcmp(date, T.date))
		{
			printf("%s\t", T.time);
		}
	}
	printf("\n");

	fclose(fptr);
	return;
}

void all_records(char name[])
{
	update_file();

	FILE *fptr = fopen(name, "rb");
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
		printf("%d. %s        %s        %s\n", T.M.movie_number, T.M.name, T.date, T.time);
		for (int i = 0; i < 40; ++i)
			printf(T.seats[i] ? "1" : "0");
		printf("\n");
	}
	printf("\n");

	fclose(fptr);
}

const char *input_time_forMovie(int movie_number, char date[])
{
	_Bool correct = 1, correct2;
	Ticket T;
	char temp_input[50];
	do
	{
		printf("Enter Time: ");
		scanf(" %[^\n]", temp_input);

		if (valid_time(temp_input))
		{
			FILE *fptr = fopen("ticket.bin", "rb");
			if (fptr == NULL)
			{
				printf("~Error Opening File~");
				exit(1);
			}
			correct2 = 1;
			while (!feof(fptr))
			{
				fread(&T, sizeof(T), 1, fptr);
				if (feof(fptr))
					break;
				if (T.M.movie_number == movie_number && !strcmp(date, T.date) && !strcmp(T.time, temp_input))
				{
					correct = 0;
					correct2 = 0;
				}
			}
			fclose(fptr);
			if (correct2)
				printf("~Invalid Input~\n");
		}
		else
			printf("~Invalid Input~\n");
	} while (correct);

	char *time = malloc(6);
	strcpy(time, temp_input);
	return time;
}

_Bool check_seatAvailability(int movie_number, char date[], char time[], int no_of_tickets)
{
	FILE *fptr = fopen("ticket.bin", "rb");
	if (fptr == NULL)
	{
		printf("~Error Opening Files!~\n");
		exit(1);
	}

	Ticket T;
	int count = 0;

	while (!feof(fptr))
	{
		fread(&T, sizeof(Ticket), 1, fptr);
		if (feof(fptr))
			break;
		if (movie_number == T.M.movie_number && (strcmp(date, T.date) == 0) && (strcmp(time, T.time) == 0))
		{
			for (int i = 0; i < 40; ++i)
				if (!T.seats[i])
					count += 1;
			break;
		}
	}
	fclose(fptr);

	if (count >= no_of_tickets)
		return 1;
	else
		return 0;
}

_Bool InArray(int num, int arr[], int n)
{
	for (int i = 0; i < n; ++i)
		if (arr[i] == num)
			return 1;
	return 0;
}

Ticket *search_movieTicket(int movie_number, char date[], char time[])
{
	FILE *p = fopen("ticket.bin", "rb");
	Ticket *T = malloc(sizeof(Ticket));

	while (!feof(p))
	{
		fread(T, sizeof(Ticket), 1, p);
		if (feof(p))
			break;
		if ((T->M.movie_number == movie_number) && !strcmp(date, T->date) && !strcmp(time, T->time))
			break;
	}

	fclose(p);
	return T;
}

_Bool movies_available(char date[])
{
	update_file();
	_Bool Yes = 0;

	FILE *p = fopen("ticket.bin", "rb");
	if (p == NULL)
	{
		printf("~Error Opening File~");
		exit(1);
	}

	Ticket T;

	while (!feof(p))
	{
		fread(&T, sizeof(Ticket), 1, p);
		if (feof(p))
			break;
		if (!strcmp(T.date, date))
		{
			Yes = 1;
			break;
		}
	}

	fclose(p);
	if (Yes)
		return 1;
	else
		return 0;
}

_Bool check_seatFree(int movie_number, char date[], char time[], char seat_code[3])
{
	FILE *fptr = fopen("ticket.bin", "rb");
	if (fptr == NULL)
	{
		printf("~Error Opening Files!~\n");
		exit(1);
	}

	Ticket T;
	_Bool free = 1;

	while (!feof(fptr))
	{
		fread(&T, sizeof(Ticket), 1, fptr);
		if (feof(fptr))
			break;
		if ((T.M.movie_number == movie_number) && (strcmp(date, T.date) == 0) && (strcmp(time, T.time) == 0))
		{
			if (T.seats[seat_index(seat_code)])
				free = 0;
			break;
		}
	}

	fclose(fptr);
	if (free)
		return 1;
	else
		return 0;
}

int print_mainScreen(int movie_number, char date[], char time[], char status[], int selected[], int n, _Bool first, _Bool just_view)
{
	clear_screen();
	char temp_input[100];

	printf("\n\t\t======================================\n\t\t\t^^ SCREEN THIS SIDE ^^\n\t\t======================================\n\n\n");
	printf("\t  ");
	printf("      1   2   3   4   5   6         7   8\n");
	printf("\t  ");
	printf("    -----------------------------------------\n");

	Ticket *T = search_movieTicket(movie_number, date, time);

	char code = 'A';
	int i = 0;

	for (int row = 0; row < 11; ++row)
	{
		printf("\t  ");
		if (row % 2 == 0 && row != 4)
		{
			printf("'%c' | ", code);
			++code;

			while (i < 40)
			{
				if (InArray(i, selected, n))
					printf("S   ");
				else if (T->seats[i])
					printf("B   ");
				else
					printf("_   ");

				if ((i + 1) % 8 == 0)
				{
					++i;
					printf("|\n");
					break;
				}
				if ((i + 1) % 8 == 6)
					printf("      ");
				++i;
			}
		}
		else
			printf("    |                                       |\n");
	}

	printf("\t      -----------------------------------------\n\n");
	printf("------------------------------------------------------------------------------------\n");
	printf("%s", status);
	printf("------------------------------------------------------------------------------------\n\n");
	if (!just_view)
	{
		if (first)
		{
			printf("Enter Seat Codes: ");
			scanf(" %[^\n]", temp_input);

			if (valid_seatCodes(movie_number, date, time, temp_input, n))
			{
				char seat_code[3];
				seat_code[2] = '\0';
				for (int i = 0; i < 3 * n; i += 3)
				{
					seat_code[0] = temp_input[i];
					seat_code[1] = temp_input[i + 1];
					selected[i / 3] = seat_index(seat_code);
				}
				return 2;
			}
			else
				return 1;
		}
		else
		{
			char confirm;
			do
			{
				printf("Confirm Seats (Y/N): ");
				scanf(" %[^\n]", temp_input);

				if (!valid_char_yn(temp_input))
				{
					printf("~Invalid Input~\n");
				}
				else
				{
					confirm = temp_input[0];
				}
			} while (!valid_char_yn(temp_input));

			if (confirm == 'Y')
				return 0;
			else
			{
				for (int i = 0; i < n; ++i)
					selected[i] = -1;
				return -1;
			}
		}
	}
	else
		return 0;
}

const char *num_to_ticketCode(int num)
{
	char *code = malloc(3);
	code[2] = '\0';
	code[0] = 'A' + (num / 8);
	code[1] = '1' + (num % 8);
	return code;
}

void generate_ticket(char name[], Ticket T, int arr[], int n)
{
	char file_name[100];
	sprintf(file_name, "%s.txt", name);

	FILE *ptr = fopen(file_name, "w");
	if (ptr == NULL)
	{
		printf("~Error Opening File~");
		exit(1);
	}

	fprintf(ptr, "\n\t======================================================\n\t\t\tWELCOME TO PZR CINEMAS\n\t======================================================\n\n");
	fprintf(ptr, "Movie Name: %s\nDate: %s\nTime: %s\nTicket Codes (Seat Numbers):\n", T.M.name, T.date, T.time);

	char ticket_code[17];
	for (int i = 0; i < n; ++i)
	{
		sprintf(ticket_code, "%02d%c%c%c%c%c%c%c%c%c%c%c%c%s", T.M.movie_number, T.date[0], T.date[1], T.date[3], T.date[4], T.date[6], T.date[7], T.date[8], T.date[9], T.time[0], T.time[1], T.time[3], T.time[4], num_to_ticketCode(arr[i]));
		fprintf(ptr, "  %s (%s)\n", ticket_code, num_to_ticketCode(arr[i]));
	}
	fprintf(ptr, "\n~Enjoy Your Movie!~");
	fclose(ptr);
	getch();
	return;
}

Ticket *decode_ticketCode(char ticket_code[])
{
	Ticket *T = malloc(sizeof(Ticket));

	T->M.movie_number = (ticket_code[0] - '0') * 10 + ticket_code[1] - '0';

	T->date[0] = ticket_code[2];
	T->date[1] = ticket_code[3];
	T->date[2] = '/';
	T->date[3] = ticket_code[4];
	T->date[4] = ticket_code[5];
	T->date[5] = '/';
	T->date[6] = ticket_code[6];
	T->date[7] = ticket_code[7];
	T->date[8] = ticket_code[8];
	T->date[9] = ticket_code[9];
	T->date[10] = '\0';

	T->time[0] = ticket_code[10];
	T->time[1] = ticket_code[11];
	T->time[2] = ':';
	T->time[3] = ticket_code[12];
	T->time[4] = ticket_code[13];
	T->time[5] = '\0';

	return T;
}

int cancel_ticket(Ticket *T, char seat_code[])
{
	FILE *fptr = fopen("ticket.bin", "rb");
	FILE *fptr2 = fopen("temp.bin", "wb");

	if (fptr == NULL || fptr2 == NULL)
	{
		printf("~Error Opening Files~");
		exit(1);
	}

	Ticket T1;
	_Bool Exists = 0;

	while (!feof(fptr))
	{
		fread(&T1, sizeof(Ticket), 1, fptr);
		if (feof(fptr))
			break;
		if (T1.M.movie_number == T->M.movie_number && !strcmp(T1.date, T->date) && !strcmp(T1.time, T->time))
		{
			if (!check_seatFree(T1.M.movie_number, T1.date, T1.time, seat_code))
			{
				T1.seats[seat_index(seat_code)] = 0;
				Exists = 1;
			}
			else
				printf("~Seat Is Not Booked!~\n");
		}
		fwrite(&T1, sizeof(Ticket), 1, fptr2);
	}

	fclose(fptr);
	fclose(fptr2);
	remove("ticket.bin");
	rename("temp.bin", "ticket.bin");

	if (!Exists)
	{
		printf("Error! Invalid Ticket Code!\n");
		return -1;
	}

	return 0;
}

_Bool check_movie_number(int movie_number)
{
	_Bool available = 0;
	FILE *fptr = fopen("ticket.bin", "rb");
	Ticket T;

	while (!feof(fptr))
	{
		fread(&T, sizeof(Ticket), 1, fptr);
		if (feof(fptr))
			break;
		if (T.M.movie_number == movie_number)
		{
			available = 1;
			break;
		}
	}

	fclose(fptr);

	if (available)
		return 1;
	else
		return 0;
}

// --------------------------- MAIN FUNCTION --------------------------- //

int main(void)
{
	char temp_input[50], input_date[11], input_time[6];
	int ch, input_movie_number, n, n_cancel;
	_Bool quit = 1;
	int output;
	_Bool finish = 1;
	_Bool first = 1;
	Ticket T, T_input;
	float total_price;
	Ticket *T1;
	_Bool correct = 1;
	char seat_code[3];
	int arr[10];

	while (quit)
	{
		clear_screen();
		printf("Enter:\n");
		printf("1. Book Tickets\n2. Cancel Tickets\n3. View All Available Movies\n4. Exit\n\n");
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
			printf("\t     --------------------------------------------\n\t\t\t     BOOK TICKETS\n\t     --------------------------------------------\n\n");
			printf("  CHOOSE DATE\n---------------\n%s\t%s\t%s\n\n", NextDate(Date(), 0), NextDate(Date(), 1), NextDate(Date(), 2));

			strcpy(input_date, InputDate());
			if (!movies_available(input_date))
			{
				printf("\n~Movies Unavailable At This Date!~\n\n");
				getch();
				break;
			}
			printf("\n\n  AVAILABLE MOVIES\n--------------------\n");
			print_allMovie_withTimes(input_date);

			printf("\n\n  PICK YOUR MOVIE!\n--------------------\n");

			correct = 1;
			do
			{
				printf("Enter Movie Number: ");
				scanf(" %[^\n]", temp_input);

				if (!valid_int(temp_input))
					printf("~Invalid Input~\n");
				else
				{
					input_movie_number = atoi(temp_input);
					if (check_movie_number(input_movie_number))
						correct = 0;
					else
						printf("~Invalid Input!~\n");
				}
			} while (correct);

			printf("\n");

			strcpy(input_time, input_time_forMovie(input_movie_number, input_date));
			printf("\n");

			do
			{
				printf("Enter No. of Tickets: ");
				scanf(" %[^\n]", temp_input);

				if (!valid_int(temp_input))
					printf("~Invalid Input~\n");
				else
					n = atoi(temp_input);
			} while (!valid_int(temp_input));

			if (check_seatAvailability(input_movie_number, input_date, input_time, n))
			{
				char status[500];
				sprintf(status, "'_' - Free Seats\n'B' - Booked Seats\n'S' - Selected Seats\nEnter %d Seat Numbers Separated By Spaces: (Ex) A1 A2 A5..\n", n);

				for (int i = 0; i < n; ++i)
					arr[i] = -1;

				finish = 1;
				first = 1;

				while (finish)
				{
					output = print_mainScreen(input_movie_number, input_date, input_time, status, arr, n, first, 0);
					switch (output)
					{
					case 1:
						sprintf(status, "'_' - Free Seats\n'B' - Booked Seats\n'S' - Selected Seats\nEnter %d Seat Codes Separated By Spaces: (Ex) A1 A2 A5..\nError: Invalid Input!\n", n);
						first = 1;
						break;

					case 2:
						sprintf(status, "'_' - Free Seats\n'B' - Booked Seats\n'S' - Selected Seats\n");
						first = 0;
						break;

					case 0:
						finish = 0;
						break;

					case -1:
						sprintf(status, "'_' - Free Seats\n'B' - Booked Seats\n'S' - Selected Seats\nEnter %d Seat Numbers Separated By Spaces: (Ex) A1 A2 A5..\n", n);
						first = 1;
						break;
					}
				}

				FILE *fptr = fopen("ticket.bin", "rb");
				FILE *fptr2 = fopen("temp.bin", "wb");
				if (fptr == NULL || fptr2 == NULL)
				{
					printf("~Error Opening Files!~");
					exit(1);
				}

				while (!feof(fptr))
				{
					fread(&T, sizeof(Ticket), 1, fptr);
					if (feof(fptr))
						break;
					if (T.M.movie_number == input_movie_number && !strcmp(T.date, input_date) && !strcmp(T.time, input_time))
					{
						for (int i = 0; i < n; ++i)
							T.seats[arr[i]] = 1;
						total_price = T.M.price * n;
						T_input = T;
					}
					fwrite(&T, sizeof(Ticket), 1, fptr2);
				}

				fclose(fptr);
				fclose(fptr2);

				remove("ticket.bin");
				rename("temp.bin", "ticket.bin");

				clear_screen();

				do
				{
					printf("Enter Your Name: ");
					scanf(" %[^\n]", temp_input);

					if (!valid_name(temp_input))
					{
						printf("~Invalid Input~\n");
					}
				} while (!valid_name(temp_input));

				printf("~Ticket Successfull Booked!~\n\nTotal Price To Be Paid: %.2f\n\n", total_price);
				generate_ticket(temp_input, T_input, arr, n);
			}
			else
				printf("\n~Sorry! Seats Unavailable!~\n");

			getch();
			break;

		case 2:
			clear_screen();
			printf("\t    ----------------------------------------------\n\t\t\t    CANCEL TICKETS\n\t    ----------------------------------------------\n\n");

			do
			{
				printf("Enter Number of Tickets To Cancel: ");
				scanf(" %[^\n]", temp_input);

				if (!valid_int(temp_input))
				{
					printf("~Invalid Input~\n");
				}
				else
					n_cancel = atoi(temp_input);
			} while (!valid_int(temp_input));
			printf("\n");

			correct = 1;
			seat_code[2] = '\0';

			for (int i = 0; i < n_cancel; ++i)
			{
				do
				{
					printf("Enter Ticket Code: ");
					scanf(" %[^\n]", temp_input);

					if (!valid_ticketCode(temp_input))
					{
						printf("~Invalid Input~\n");
					}
					else
					{
						seat_code[0] = temp_input[14];
						seat_code[1] = temp_input[15];
						if (valid_seat(seat_code))
							correct = 0;
						else
							printf("~Invalid Input!~\n");
					}

				} while (correct);

				T1 = decode_ticketCode(temp_input);
				i += cancel_ticket(T1, seat_code);
			}

			clear_screen();
			printf("~Tickets Successfully Cancelled!~");
			getch();
			break;

		case 3:
			clear_screen();
			printf("\t     --------------------------------------------\n\t\t\t     CHECK STATUS\n\t     --------------------------------------------\n\n");

			printf("  CHOOSE DATE\n---------------\n%s\t%s\t%s\n\n", NextDate(Date(), 0), NextDate(Date(), 1), NextDate(Date(), 2));

			strcpy(input_date, InputDate());
			if (!movies_available(input_date))
			{
				printf("\n~Movies Unavailable At This Date!~\n\n");
				getch();
				break;
			}
			printf("\n  AVAILABLE MOVIES\n--------------------\n");
			print_allMovie_withTimes(input_date);
			printf("\n");

			printf("  PICK YOUR MOVIE!\n--------------------\n");

			do
			{
				printf("Enter Movie Number: ");
				scanf(" %[^\n]", temp_input);

				if (!valid_int(temp_input))
					printf("~Invalid Input~\n");
				else
					input_movie_number = atoi(temp_input);
			} while (!valid_int(temp_input));
			printf("\n");

			strcpy(input_time, input_time_forMovie(input_movie_number, input_date));

			clear_screen();
			print_mainScreen(input_movie_number, input_date, input_time, "~Press Any Key To Continue~\n", arr, 0, 1, 1);
			getch();
			break;

		case 4:
			quit = 0;
			break;

			case 5:
			clear_screen();
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