#include "management.h"

// fmt must be string format("")
// Usage : DEBUG("message : %d", 123);
#define DEBUG(fmt, ...) fprintf(stderr, "[%d:%s] \n" fmt, __LINE__, __FUNCTION__, ##__VA_ARGS__)


int main(){
    int exit = 0, i = 0;
    int login_flag = 0, year_flag = 0, num_flag = 0;
    int temppw_flag = 0, login_menu_exit = 0;
    char year[5], num[5];

    YEAR *head_YEAR = NULL, *cur_YEAR = NULL, *prev_year = NULL;
    STUDENT *head_STUDENT = NULL, *cur_STUDENT = NULL, *prev_student = NULL; 
    
    Create_Struct();
   
    initscr();

    while(!exit){ 
        login_menu_exit = 0;
        clear();
        
        switch(Account_Manage()){
            clear();
            year_flag = 0; num_flag = 0;
            case MENU_LOGIN :
                clear();
                printw("Student Number : ");
                scanw("%s", Curr_Num);
                for(i = 0; i < 4; i++)
                    year[i] = Curr_Num[i];
                for(i = 4; i < 8; i++)
                    num[i - 4] = Curr_Num[i];//copying st number


                cur_YEAR = head_YEAR = TOP->ST_YEAR;
                prev_year = cur_YEAR;

                for(i = 0; i < TOP->Year_Size; i++){
                    if(!strncmp(cur_YEAR->year, year, 4)){
                        year_flag = 1;
                        break;
                    }
                    prev_year = cur_YEAR;
                    cur_YEAR = cur_YEAR->link;
                }


                if(year_flag == 1){
                    head_STUDENT = cur_YEAR->ST_NUM;
                    cur_STUDENT = head_STUDENT;
                    prev_student = cur_STUDENT;

                    for(i = 0; i < cur_YEAR->Num_Size; i++){
                        if(!strncmp(cur_STUDENT->number, num, 4)){
                            num_flag = 1;
                            break;
                        }
                        prev_student = cur_STUDENT;
                        cur_STUDENT = cur_STUDENT->link;
                    }
                }

                if(num_flag == 1) {
                    printw("Password : ");
                    noecho();
                    scanw("%s", Curr_Pass);
                    echo();

                    login_flag = login();
                    if(login_flag == 1){
                        while(!login_menu_exit){
                            clear();
                            switch(menu()){
                                case MENU_ASSIGN :
                                    Search_Assign();
                                    break;

                                case MENU_CGPA :
                                    Search_CGPA();
                                    break;                            

                                case MENU_CHANGE :
                                    Change_Password();
                                    break;

                                case MENU_LOGOUT :
                                    login_menu_exit = 1;
                                    break;

                                default :
                                    clear();
                                    printw("Wrong Key\n");
                                    printw("Press any key to return\n");
                                    getch();
                                    break;
                            }
                        }
                    }
                }
                if(login_flag == 0 && num_flag == 1){
                    printw("\nThe password of account <%s> is wrong.\n", Curr_Num);
                    printw("Press any key to return\n");
                    getch();
                }
                else if(login_flag == 0 && num_flag == 0){
                    printw("\nThe account <%s> does not exist.\n", Curr_Num);
                    printw("Press any key to return\n");
                    getch();
                }
                else if(login_flag == 0 && year_flag == 0){
                    printw("\nThe account <%s> does not exist.\n", Curr_Num);
                    printw("Press any key to return\n");
                    getch();
                }
                break;
            
            case MENU_NEW : 
                New_Account();
                break;
            
            case MENU_DEL :
                Delete_Account();
                break;
            
            case MENU_TEMP :
                Temp_Password();
                getch();
                break;
            
            case MENU_QUIT :
                exit = 1;
                break;
            
            default :
                clear();
                printw("Wrong Key\n");
                printw("Press any key to return\n");
                getch();
                break;
        }
    }
    endwin();
    Save_Data();
    system("clear");
    return 0;
}
void Save_Data(){
    int i,j,k;
    int current_year;
    int current_num;
    int current_assign;
    int current_cgpa;
    FILE* fpoint;

    YEAR *head_YEAR = NULL, *cur_YEAR = NULL, *prev_year = NULL;
    STUDENT *head_STUDENT = NULL, *cur_STUDENT = NULL, *prev_student = NULL;
    CGPA *head_CGPA = NULL, *cur_CGPA = NULL, *prev_cgpa = NULL;
    ASSIGN *head_ASSIGN = NULL, *cur_ASSIGN = NULL, *prev_assign = NULL;

    fpoint=fopen("check_data.txt","w");

    fprintf(fpoint, "%d\n", TOP->Year_Size);

    cur_YEAR = head_YEAR = TOP->ST_YEAR;


    for(i = 0; i < TOP->Year_Size; i++){
        fprintf(fpoint, "%s %d\n", cur_YEAR->year, cur_YEAR->Num_Size);

        head_STUDENT = cur_YEAR->ST_NUM;
        cur_STUDENT = head_STUDENT;

        for(j = 0; j < cur_YEAR->Num_Size; j++){
            fprintf(fpoint, "%4s%4s\n", cur_YEAR->year, cur_STUDENT->number);
            fprintf(fpoint, "%s\n", cur_STUDENT->password);
            fprintf(fpoint, "%d %d\n", cur_STUDENT->Assign_Size, cur_STUDENT->CGPA_Size);

            head_ASSIGN = cur_STUDENT->Child_A;
            cur_ASSIGN = head_ASSIGN;
            head_CGPA = cur_STUDENT->Child_C;
            cur_CGPA = head_CGPA;

            for(k = 0; k < cur_STUDENT->Assign_Size; k++){
                fprintf(fpoint, "%s/%s/%s/%d %d\n", cur_ASSIGN->name, cur_ASSIGN->describe, cur_ASSIGN->professor, cur_ASSIGN->date[0], cur_ASSIGN->date[1]);
                prev_assign = cur_ASSIGN;
                free(prev_assign); DEBUG("");
                cur_ASSIGN = cur_ASSIGN->link;
            }

            for(k = 0; k < cur_STUDENT->CGPA_Size; k++){
                fprintf(fpoint, "%d %.2f\n", cur_CGPA->semester, cur_CGPA->score);
                prev_cgpa = cur_CGPA;
                free(prev_cgpa); DEBUG("");
                cur_CGPA = cur_CGPA->link;    
            }
            prev_student = cur_STUDENT;
            free(prev_student); DEBUG("");
            cur_STUDENT = cur_STUDENT->link;
        }
        prev_year = cur_YEAR;
        free(prev_year); DEBUG("");
        cur_YEAR = cur_YEAR->link;
        fprintf(fpoint, "\n");
    }

    free(TOP); DEBUG("");

    fclose(fpoint);

}
void Create_Struct(){
    int i,j,k;
    int exit = 0, flag_exit;
    int current_year;
    int current_num;
    int current_assign;
    int current_cgpa;
    char* token;//didn't use tokenizing
    char temp[350];
    char temp_year[5], temp_num[5], temp_snum[9];
    FILE* fpoint = fopen("data.txt", "r");

    YEAR *head_YEAR = NULL, *new_YEAR = NULL, *tail_YEAR = NULL;
    STUDENT *head_STUDENT = NULL, *new_STUDENT = NULL, *tail_STUDENT = NULL;
    CGPA *head_CGPA = NULL, *new_CGPA = NULL, *tail_CGPA = NULL;
    ASSIGN *head_ASSIGN = NULL, *new_ASSIGN = NULL, *tail_ASSIGN = NULL;

    TOP = malloc(sizeof(TREE_HEAD));
    fscanf(fpoint, "%d", &TOP->Year_Size);


    for(i = 0; i < TOP->Year_Size; i++){
        new_YEAR = malloc(sizeof(YEAR));
        fscanf(fpoint, "%s %d", new_YEAR->year, &new_YEAR->Num_Size);

        if(head_YEAR == NULL){
            head_YEAR = new_YEAR;
            TOP->ST_YEAR = head_YEAR; 
            tail_YEAR = new_YEAR;
        }
        else{
            tail_YEAR->link = new_YEAR;
            tail_YEAR = new_YEAR;
        }

        head_STUDENT = tail_YEAR->ST_NUM;
        tail_STUDENT = tail_YEAR->ST_NUM;

        for(j = 0; j < new_YEAR->Num_Size; j++){
            new_STUDENT = malloc(sizeof(STUDENT));
            fscanf(fpoint, "%*4s%4s\n", new_STUDENT->number);
            fscanf(fpoint, "%s\n", new_STUDENT->password);
            fscanf(fpoint, "%d %d\n", &new_STUDENT->Assign_Size, &new_STUDENT->CGPA_Size);

            if(head_STUDENT == NULL){
                head_STUDENT = new_STUDENT;    
                tail_YEAR->ST_NUM = head_STUDENT;
                tail_STUDENT = new_STUDENT;    
            }
            else{
                tail_STUDENT->link = new_STUDENT;
                tail_STUDENT = new_STUDENT;
            }

            head_ASSIGN = new_STUDENT->Child_A;
            tail_ASSIGN = new_STUDENT->Child_A;
            head_CGPA = new_STUDENT->Child_C;
            tail_CGPA = new_STUDENT->Child_C;

            for(k = 0; k < new_STUDENT->Assign_Size; k++){
                new_ASSIGN = malloc(sizeof(ASSIGN));
                fscanf(fpoint, "%100[^/] %*c %100[^/] %*c %100[^/] %*c %d %d\n", new_ASSIGN->name, new_ASSIGN->describe, new_ASSIGN->professor, &new_ASSIGN->date[0], &new_ASSIGN->date[1]);

                if(head_ASSIGN == NULL){
                    head_ASSIGN = new_ASSIGN;
                    new_STUDENT->Child_A = head_ASSIGN;
                    tail_ASSIGN = new_ASSIGN;
                }
                else{
                    tail_ASSIGN->link = new_ASSIGN;
                    tail_ASSIGN = new_ASSIGN;
                }
            }

            for(k = 0; k < new_STUDENT->CGPA_Size; k++){
                new_CGPA = malloc(sizeof(CGPA));
                fscanf(fpoint, "%d %f\n", &new_CGPA->semester, &new_CGPA->score);

                if(head_CGPA == NULL){
                    head_CGPA = new_CGPA;
                    new_STUDENT->Child_C = head_CGPA;
                    tail_CGPA = new_CGPA;
                }
                else{
                    tail_CGPA->link = new_CGPA;
                    tail_CGPA = new_CGPA;
                }
            }
        }
        flag_exit = fscanf(fpoint, "\n");
        if(flag_exit == EOF) break;
    }

    fclose(fpoint);
}
char Account_Manage()
{
    printw("1. Login\n");
    printw("2. Create new Account\n");
    printw("3. Delete Account\n");
    printw("4. Get Temporary Password\n");
    printw("5. Exit\n");
    return wgetch(stdscr);
}
char menu()
{
    printw("1. Assignment Management\n");
    printw("2. CGPA Management\n");
    printw("3. Change Password\n");
    printw("4. Logout\n");
    return wgetch(stdscr);
}
void Search_Assign()
{
    int Asize;
    char input;

    clear();
    printw("<Assignment Management for %s>\n",Curr_Num);

    Asize = node_login_Num->Assign_Size;

    if(Asize != 0){
        Sort_Assign();
        Print_Assign(Asize);
    }
    else{
        printw("Congratulations!! There is no assignment left!\n\n");
    }

    printw("1. New Assignment\n2. Delete Assignment\n3. Return to main menu\n");
    noecho();
    input=wgetch(stdscr);
    echo();
    switch(input)
    {
        case '1': Add_Assign(); break;
        case '2': Delete_Assign(); break;
        case '3': return; break;
        default: 
            clear();
            printw("Wrong Key\n");
            printw("Press any key to return\n");
            getch();
            break;
    }
    clear();
}

void Print_Assign(int Asize) {
    int k,D_day = 0,thistime,thattime;
    int tday_mon, tday_date, D_mon;
    time_t t;
    struct tm *today;

    int i = 0;

    STUDENT *cur_STUDENT = NULL;
    ASSIGN *head_ASSIGN = NULL, *cur_ASSIGN = NULL;


    cur_STUDENT = node_login_Num;
    cur_ASSIGN = head_ASSIGN = cur_STUDENT->Child_A;

    t = time(NULL);
    today = localtime(&t);
    tday_mon = today->tm_mon + 1;
    tday_date = today->tm_mday;
    D_mon = cur_ASSIGN->date[0];


    for(i = 1; i <= cur_STUDENT->Assign_Size; i++){
        printw("<%d> Name\t: %s\n", i, cur_ASSIGN->name);
        printw("    Describe\t: %s\n", cur_ASSIGN->describe);
        printw("    Professor\t: %s\n", cur_ASSIGN->professor);
        printw("    Due\t: %d/%2d\n", cur_ASSIGN->date[0], cur_ASSIGN->date[1]);

        D_day = 0;
        if(cur_ASSIGN->date[0] == tday_mon && cur_ASSIGN->date[1] == tday_date){
            printw("    D-day\t: TODAY!\n");
        } 
        else if(cur_ASSIGN->date[0] < tday_mon){
            printw("    D-day over!!\n");
        } 
        else if(cur_ASSIGN->date[0] == tday_mon && cur_ASSIGN->date[1] < tday_date){
            printw("    D-day over!!\n");
        } 
        else{
            if(cur_ASSIGN->date[0] > tday_mon){
                while(1){
                    if(D_mon == 1||D_mon == 3||D_mon == 5||D_mon == 7||D_mon == 8||D_mon == 10 ||D_mon == 12)
                        D_day += 31;
                    else if(D_mon == 4||D_mon == 6||D_mon == 9||D_mon == 11)
                        D_day += 30;
                    else if(D_mon == 2)        
                        D_day += 28;
                    tday_mon++;
                    if(cur_ASSIGN->date[0] == tday_mon) break;
                }
            }
            D_day -= tday_date;
            D_day += cur_ASSIGN->date[1];
            D_day *= -1;
            printw("    D-day\t: %d\n", D_day);
        }

        printw("\n");
        cur_ASSIGN = cur_ASSIGN->link;
    }
}

char cgpa_menu(){
    printw("1. Add new GPA or Change existing GPA\n");
    printw("2. View all\n");
    printw("3. Exit\n");
    return wgetch(stdscr);
}
void Search_CGPA() {
    int exit;

    while(!exit) {
        clear();

        switch(cgpa_menu()){
            case CGPA_ADD: Add_GPA(); break;
            case CGPA_VIEW: Print_CGPA(); break;
            case CGPA_QUIT: exit = 1; break;
            default: 
                            clear();
                            printw("Wrong Key\n");
                            printw("Press any key to return\n");
                            getch();
                            break;
        }
    }
    exit = 0;
}
void Add_GPA(){
    int semester;//changed to semster
    float gpa;
    int gpa_size;
    int i = 0;

    STUDENT *cur_STUDENT = NULL;
    CGPA *head_CGPA = NULL, *new_CGPA = NULL, *cur_CGPA = NULL;

    clear();
    echo();
    printw("Input the semester of GPA : ");
    scanw("%d", &semester);

    cur_STUDENT = node_login_Num;
    gpa_size = cur_STUDENT->CGPA_Size;  

    if(semester > (gpa_size + 1) || semester < 1) {
        printw("You entered wrong number!\n");
        getch();
        return ;
    } //if semester is smaller than 1 or larger than next semster, print error
    printw("Input the GPA : ");
    scanw("%f", &gpa);

    if(gpa < 0 || gpa > 4.31) {
        printw("You entered wrong number!\n");
        getch();
        return ;
    }

    if(semester <= gpa_size) {
        Cor_GPA(semester, gpa);
        return ;
    }//edit the semster which are already saved

    cur_STUDENT->CGPA_Size++;
    gpa_size++;

    head_CGPA = cur_STUDENT->Child_C;
    cur_CGPA = head_CGPA;

    while(cur_CGPA->link != NULL){
        cur_CGPA = cur_CGPA->link;
    }

    new_CGPA = malloc(sizeof(CGPA));
    new_CGPA->semester = semester;
    new_CGPA->score = gpa;
    cur_CGPA->link = new_CGPA;

    return ;
}
void Cor_GPA(int semester, float gpa) {
    int i;

    STUDENT *cur_STUDENT = NULL;
    CGPA *cur_CGPA = NULL;

    cur_STUDENT = node_login_Num;
    cur_CGPA = cur_STUDENT->Child_C;

    for(i = 0;  i < cur_STUDENT->CGPA_Size; i++){
        if(cur_CGPA->semester == semester){
            cur_CGPA->score = gpa;
            break;
        }
        else cur_CGPA = cur_CGPA->link;
    }
    printw("GPA is changed successfully!\n");
    getch();
    return ;

}
void Print_CGPA() //Additional
{
    int i;
    int tmpSemester;
    float tmpGPA;
    float sum = 0;

    STUDENT *cur_STUDENT = NULL;
    CGPA *cur_CGPA = NULL;

    cur_STUDENT = node_login_Num;
    cur_CGPA = cur_STUDENT->Child_C;

    clear();
    printw("<CGPA Management for %s>\n", Curr_Num);
    printw("------------------------------\n");
    printw("  Semester  |  GPA  |  CGPA  |  \n");
    for(i = 0 ; i < cur_STUDENT->CGPA_Size ; i++) {
        tmpSemester = cur_CGPA->semester;
        tmpGPA = cur_CGPA->score;
        sum += tmpGPA;

        printw("------------------------------\n");
        printw("     %d      |  %.2f |  %.2f  |\n", tmpSemester, tmpGPA, sum/(i + 1));

        cur_CGPA = cur_CGPA->link;
    }
    printw("------------------------------\n\n");
    printw("<CGPA Management by Graph for %s>\n", Curr_Num);
    printw("(x-axis : semester,  y-axis : score)\n");
    Print_CGPA_Graph();
    getch();
}
void Print_CGPA_Graph() {
    int i, ver = 0, hor = 8, num = 0; 
    int tmpSemester;
    float tmpGPA, gpa = 4;
    float sum = 0;

    STUDENT *cur_STUDENT = NULL;
    CGPA *cur_CGPA = NULL;

    cur_STUDENT = node_login_Num;
    cur_CGPA = cur_STUDENT->Child_C;

    for(i = 0; i < 23; i++){
        if((i - 2)%5 == 0){
            printw(" %.1f |\n", gpa);
            gpa--;
        } 
        else
            printw("     |\n");
    }
    printw("------------------------------------------------------\n");
    for(i = 0; i <= 8; i++){
        if(i == 0)
            printw("     |");
        else
            printw("  %d  |", i);
    }

    num = 2 * cur_STUDENT->CGPA_Size + 7;
    for(i = 0; i < cur_STUDENT->CGPA_Size; i++){
        ver = (cur_CGPA->score)/0.2 + 0.5; 
        if(0 <= cur_CGPA->score && cur_CGPA->score < 0.2){
            mvprintw(22 + num - ver, cur_CGPA->semester + hor, "*", cur_CGPA->score);
            mvprintw(22 + num - ver - 1, cur_CGPA->semester + hor - 1, "%.2f", cur_CGPA->score);
            hor += 6;
        }
        else{
            mvprintw(22 + num - ver, cur_CGPA->semester + hor, "*", cur_CGPA->score);
            mvprintw(22 + num - ver + 1, cur_CGPA->semester + hor - 1, "%.2f", cur_CGPA->score);
            hor += 6;
        }
        cur_CGPA = cur_CGPA->link;
    }
}
void Sort_Assign(){
    typedef struct {
        int date[2];
        ASSIGN *current;
    } date_address;

    date_address *info;
    date_address temp;
    int i = 0, j = 0, k = 0, tp, Asize = 0;

    STUDENT *head_STUDENT = NULL, *cur_STUDENT = NULL;
    ASSIGN *cur_ASSIGN = NULL;

    cur_STUDENT = head_STUDENT = node_login_Num;
    cur_ASSIGN = cur_STUDENT->Child_A;

    Asize = cur_STUDENT->Assign_Size;

    info = malloc(sizeof(date_address) * cur_STUDENT->Assign_Size);
    for(i = 0; i < cur_STUDENT->Assign_Size; i++){
        if(i == 0){
            info[i].current = cur_ASSIGN;
            info[i].date[0] = cur_ASSIGN->date[0];
            info[i].date[1] = cur_ASSIGN->date[1];
        }
        else {
            info[i].date[0] = cur_ASSIGN->date[0];
            info[i].date[1] = cur_ASSIGN->date[1];
            info[i].current = cur_ASSIGN;
        }

        cur_ASSIGN = cur_ASSIGN->link;
    }

    for(i = 1; i < cur_STUDENT->Assign_Size; i++){
        temp = info[i];
        k = i;
        for(j = i - 1; j >= 0; j--){
            if(temp.date[0] < info[j].date[0]){
                info[j + 1].date[0] = info[j].date[0];
                info[j + 1].date[1] = info[j].date[1];
                info[j + 1].current = info[j].current;
                k = j;
            }
            else if(temp.date[0] == info[j].date[0]){
                if(temp.date[1] < info[j].date[1]){
                    info[j + 1].date[0] = info[j].date[0];
                    info[j + 1].date[1] = info[j].date[1];
                    info[j + 1].current = info[j].current;
                    k = j;
                }
            }
        }
        info[k] = temp;
    }

    for(i = 0; i < cur_STUDENT->Assign_Size; i++){
        if(i == 0){
            cur_ASSIGN = cur_STUDENT->Child_A = info[i].current;
        }
        else{
            cur_ASSIGN->link = info[i].current;
            cur_ASSIGN = cur_ASSIGN->link;
            if(i == Asize - 1)
                cur_ASSIGN = NULL;
        }
    }

    free(info);

}
void Add_Assign(){
    int gpa_size;
    int i = 0, name_flag = 0;

    STUDENT *cur_STUDENT = NULL;
    ASSIGN *head_ASSIGN = NULL, *cur_ASSIGN = NULL, *new_ASSIGN = NULL;

    echo();

    cur_STUDENT = node_login_Num;

    new_ASSIGN = malloc(sizeof(ASSIGN));
    while(!name_flag){
        clear();
        name_flag = 0;
        cur_ASSIGN = node_login_Num->Child_A;
        printw("Enter the name of new assignment : \n");
        scanw("%s", new_ASSIGN->name);

        for(i = 0; i < cur_STUDENT->Assign_Size; i++){
            if(strcmp(new_ASSIGN->name, cur_ASSIGN->name) == 0){
                printw("There exists the same name of assignment.\nChoose different name.\n\n");
                printw("Press any key to continue\n");
                getch();
                name_flag = 0;
                break;
            }
            else name_flag = 1;
            cur_ASSIGN = cur_ASSIGN->link;
        }
        if(cur_ASSIGN == NULL) name_flag = 1;
    }

    printw("Enter rhe description of new assignment : \n");
    scanw("%s", new_ASSIGN->describe);

    printw("Enter the professor of new assignment : \n");
    scanw("%s", new_ASSIGN->professor);

    printw("Enter the due month of new assignment : ");
    scanw("%d", &new_ASSIGN->date[0]);
    printw("Enter the due date of new assignment : ");
    scanw("%d", &new_ASSIGN->date[1]);


    cur_STUDENT = node_login_Num;
    head_ASSIGN = cur_STUDENT->Child_A;

    cur_STUDENT->Assign_Size++;
    new_ASSIGN->link = head_ASSIGN;
    cur_STUDENT->Child_A = new_ASSIGN;
}
void Delete_Assign() {
    ASSIGN *prev_assign = NULL, *head_ASSIGN = NULL, *cur_ASSIGN = NULL;
    STUDENT *cur_STUDENT = NULL;

    int i = 0, assign_flag = 0; 
    char target_assign[100];

    clear();
    echo();

    cur_STUDENT = node_login_Num; 
    cur_ASSIGN = head_ASSIGN = cur_STUDENT->Child_A;
    prev_assign = cur_ASSIGN;

    if(cur_STUDENT->Assign_Size == 0){
        printw("No assign to delete.\n");
        printw("\nPress Any key to continue...\n");
        getch();
        return;
    }

    printw("Enter the assignment to delete : ");
    scanw("%100[^\n]", target_assign);
    
    
    for(i = 0; i < cur_STUDENT->Assign_Size; i++){
        if(!strcmp(cur_ASSIGN->name, target_assign)){
            assign_flag = 1;
            break;
        }
        prev_assign = cur_ASSIGN;
        cur_ASSIGN = cur_ASSIGN->link;
    }

    if(assign_flag == 1) {
        cur_STUDENT->Assign_Size--;
        if(i == 0){
            cur_STUDENT->Child_A = cur_ASSIGN->link;
            free(cur_ASSIGN);
            cur_ASSIGN = head_ASSIGN;
        }
        else{
            prev_assign->link = cur_ASSIGN->link;
            free(cur_ASSIGN);
            cur_ASSIGN = prev_assign;
        }

        printw("Assignment <%s> Successfully Deleted\n", target_assign);
        getch();
    }
    else{
        printw("Assignment <%s> does not exist\n");
        getch();
        return;
    }
}
void New_Account() {
    int i, year_flag = 0, num_flag = 0;
    char pw[17], confirm_pw[17];
    char year[5], num[5];

    YEAR *head_YEAR = NULL, *new_YEAR = NULL, *tail_YEAR = NULL, *cur_YEAR = NULL;
    STUDENT *head_STUDENT = NULL, *new_STUDENT = NULL, *cur_STUDENT = NULL;

    clear();
    printw("New Account Number : ");
    getstr(New_Num);

    if(strlen(New_Num) != 8){
        printw("The account number must be length of 8\n");
        getch();
        return;
    }

    cur_YEAR = head_YEAR = TOP->ST_YEAR;

    for(i = 0; i < 4; i++)
        year[i] = New_Num[i];
    for(i = 4; i < 8; i++)
        num[i - 4] = New_Num[i];//copying st number
    year[4] = 0;
    num[4] = 0;

    for(i = 0; i < TOP->Year_Size; i++){
        if(strcmp(cur_YEAR->year, year) == 0){
            year_flag = 0;
            break;
        }
        if(i == TOP->Year_Size - 1) {
            year_flag = 1;
            num_flag = 1;
        }
        cur_YEAR = cur_YEAR->link;
    }


    if(year_flag == 0){
        cur_STUDENT = head_STUDENT = cur_YEAR->ST_NUM;

        for(i = 0; i < cur_YEAR->Num_Size; i++){
            if(strcmp(cur_STUDENT->number, num) == 0){
                num_flag = 0;
                break;
            }
            cur_STUDENT = cur_STUDENT->link;
        }
        if(i == cur_YEAR->Num_Size) num_flag = 1;
    }

    if(num_flag == 1){   
        noecho();
        printw("The password must be less than 16 letters\n");
        printw("Password : ");
        getstr(pw);

        if(strlen(pw) <= 5){
            printw("Password is too short.\n");
            getch();
            echo();
            return;
        }
        printw("Confirm new password : ");
        getstr(confirm_pw);
        echo();

        if(!strcmp(pw, confirm_pw)){
            head_YEAR = TOP->ST_YEAR;
            cur_YEAR = head_YEAR;
            tail_YEAR = cur_YEAR;

            new_STUDENT = malloc(sizeof(STUDENT));
            strncpy(new_STUDENT->number, num, 5);
            strncpy(new_STUDENT->password, confirm_pw, 16);
            new_STUDENT->Assign_Size = 0;
            new_STUDENT->CGPA_Size = 1;
            new_STUDENT->Child_C = malloc(sizeof(CGPA));
            new_STUDENT->Child_C->semester = 1;
            new_STUDENT->Child_C->score = 0;


            for(i = 0; i < TOP->Year_Size; i++){
                if(!strcmp(cur_YEAR->year, year)) break;
                cur_YEAR = cur_YEAR->link;
            }


            if(cur_YEAR == NULL){
                cur_YEAR = head_YEAR;
                while(cur_YEAR->link != NULL){
                    cur_YEAR = cur_YEAR->link;
                }
                tail_YEAR = cur_YEAR;

                TOP->Year_Size++;
                new_YEAR = malloc(sizeof(YEAR));
                strncpy(new_YEAR->year, year, 5);
                new_YEAR->Num_Size = 1;
                new_YEAR->ST_NUM = new_STUDENT;
                tail_YEAR->link = new_YEAR;
                tail_YEAR = new_YEAR;
            }//case of new year added


            else{
                head_STUDENT = cur_YEAR->ST_NUM;
                cur_STUDENT = head_STUDENT;
                cur_YEAR->Num_Size++;

                while(cur_STUDENT->link != NULL){
                    cur_STUDENT = cur_STUDENT->link;
                }
                cur_STUDENT->link = new_STUDENT;
            }
            printw("Account <%s> is successfully created.\n", New_Num);
            getch();
        } 
        else{
            clear();
            printw("The confirm password is DIFFERENT\n");
            printw("Press any key to return\n");
            getch();
        }
    }

    else{
        printw("The account <%s> already exists.\n", New_Num);
        getch();
    }
}
void Delete_Account(){
    int i = 0, year_flag = 0, num_flag = 0, pw_flag = 0, cgpa_flag = 0;
    float temp;
    char pw[17];
    char year[5], num[5];

    YEAR *head_YEAR = NULL, *cur_YEAR = NULL, *prev_year = NULL;
    STUDENT *head_STUDENT = NULL, *cur_STUDENT = NULL, *prev_student = NULL; 
    CGPA *head_CGPA = NULL, *cur_CGPA = NULL, *prev_cgpa = NULL;
    ASSIGN *head_ASSIGN = NULL, *cur_ASSIGN = NULL, *prev_assign = NULL;    

    clear();
    echo();
    printw("Student Number : ");
    scanw("%s",Curr_Num);

    for(i = 0; i < 4; i++)
        year[i] = Curr_Num[i];
    for(i = 4; i < 8; i++)
        num[i - 4] = Curr_Num[i];//copying st number


    cur_YEAR = head_YEAR = TOP->ST_YEAR;
    prev_year = cur_YEAR;

    for(i = 0; i < TOP->Year_Size; i++){
        if(!strncmp(cur_YEAR->year, year, 4)){
            year_flag = 1;
            break;
        }
        prev_year = cur_YEAR;
        cur_YEAR = cur_YEAR->link;
    }


    if(year_flag == 1){
        head_STUDENT = cur_YEAR->ST_NUM;
        cur_STUDENT = head_STUDENT;
        prev_student = cur_STUDENT;

        for(i = 0; i < cur_YEAR->Num_Size; i++){
            if(!strcmp(cur_STUDENT->number, num)){
                num_flag = 1;
                break;
            }
            prev_student = cur_STUDENT;
            cur_STUDENT = cur_STUDENT->link;
        }
    }


    if(num_flag == 1){
        noecho();
        printw("Password : ");
        scanw("%s", pw);
        echo();

        if(!strcmp(pw, cur_STUDENT->password)){
            pw_flag = 1;
        }
    }
    if(pw_flag == 1){
        if(cur_STUDENT -> CGPA_Size != 0){
            printw("Input latest semester's GPA : ");
            scanw("%f", &temp);

            head_CGPA = cur_STUDENT->Child_C;
            cur_CGPA = head_CGPA;

            for(i = 0; i < cur_STUDENT->CGPA_Size - 1; i++)
                cur_CGPA = cur_CGPA->link;

            if(cur_CGPA->score == temp) cgpa_flag = 1;
        }
        else if(cur_STUDENT->CGPA_Size == 0){
            cgpa_flag = 1;
        }
    }

    if(cgpa_flag == 1){
        cur_YEAR->Num_Size--;

        cur_ASSIGN = cur_STUDENT->Child_A;
        cur_CGPA = cur_STUDENT->Child_C;
        for(i = 0; i < cur_STUDENT->Assign_Size; i++){
            prev_assign = cur_ASSIGN->link;
            free(cur_ASSIGN);
            cur_ASSIGN = prev_assign;
        }
        for(i = 0; i < cur_STUDENT->CGPA_Size; i++){
            prev_cgpa = cur_CGPA->link;
            free(cur_CGPA);
            cur_CGPA = prev_cgpa;
        }

        if(prev_student == cur_STUDENT){
            cur_YEAR->ST_NUM = cur_STUDENT->link;
            free(cur_STUDENT);
            cur_STUDENT = cur_YEAR->ST_NUM;
        }
        else{
            prev_student->link = cur_STUDENT->link;
            free(cur_STUDENT);
            cur_STUDENT = prev_student;
        }

        printw("Account <%s> Successfully Deleted\n", Curr_Num);
        getch();
    }
    else{
        printw("Wrong Information\n");
        getch();
        return;
    }
}
void Change_Password(){
    char new_pw[17], confirm_pw[17];
    int len_flag = 0;

    YEAR *cur_YEAR = NULL;
    STUDENT *cur_STUDENT = NULL;

    cur_STUDENT = node_login_Num;
    cur_YEAR = node_login_Year;

    clear();
    while(!len_flag){
        printw("The new password must be less than 16 letters\n");
        printw("Enter new password : ");
        noecho();
        scanw("%s", new_pw);

        if(strlen(new_pw) <= 5) {
            printw("The password is too short.\n");
            getch();
            clear();
        }
        else len_flag = 1;
    }
    printw("Confirm new password : ");
    scanw("%s", confirm_pw);
    echo();

    if(!strcmp(new_pw, confirm_pw)){
        strcpy(cur_STUDENT->password, confirm_pw);
        printw("Password of Account <%s> is successfully changed.\n", Curr_Num);
        getch();
    } 
    else{
        clear();
        printw("The confirm password is DIFFERENT\n");
        printw("Press any key to return\n");
        getch();
    }
}

int login(){
    int i,j;
    int year_flag=0;
    int num_flag=0;
    int pass_flag=0;
    char year[5]={};
    char num[5]={};

    YEAR *head_YEAR = NULL, *cur_YEAR = NULL;
    STUDENT *head_STUDENT = NULL, *cur_STUDENT = NULL;

    for(i = 0; i < 4; i++)
        year[i] = Curr_Num[i];
    for(i = 4; i < 8; i++)
        num[i - 4] = Curr_Num[i];//copying st number

    cur_YEAR = head_YEAR = TOP->ST_YEAR;

    for(i = 0; i < TOP->Year_Size; i++){
        if(!strcmp(cur_YEAR->year, year)){
            year_flag = 1;
            break;
        }
        cur_YEAR = cur_YEAR->link;
    }
    if(year_flag == 1){
        head_STUDENT = cur_YEAR->ST_NUM;
        cur_STUDENT = head_STUDENT;

        for(j = 0; j < cur_YEAR->Num_Size;j++){
            if(!strcmp(cur_STUDENT->number, num)){
                num_flag = 1;
                break;
            }
            cur_STUDENT = cur_STUDENT->link;
        }
    }
    if(num_flag == 1){
        if(!strcmp(cur_STUDENT->password, Curr_Pass)){
            pass_flag = 1;
            node_login_Year = cur_YEAR;
            node_login_Num = cur_STUDENT;
        }
    }
    if(pass_flag==1)
        return 1;
    return 0;
}

void Temp_Password(){
    int flag1=0;
    int year_flag = 0, num_flag = 0;//added->to check info
    int i; //j, k; used linked list, no need to use repeats
    char num[5]={};
    char year[5]={};
    int temp_num;//to make random Password
    char temppassword[8];
    float temp;

    YEAR *head_YEAR = NULL, *cur_YEAR = NULL;
    STUDENT *head_STUDENT = NULL, *cur_STUDENT = NULL;
    CGPA *head_CGPA = NULL, *cur_CGPA = NULL;

    clear();
    echo();
    srand(time(NULL));
    printw("Student Number:");
    scanw("%s",Curr_Num);

    for(i = 0; i < 4; i++)
        year[i] = Curr_Num[i];
    for(i = 4; i < 8; i++)
        num[i - 4] = Curr_Num[i];//copying st number

    cur_YEAR = head_YEAR = TOP->ST_YEAR;

    for(i = 0; i < TOP->Year_Size; i++){
        if(!strcmp(cur_YEAR->year, year)){
            year_flag = 1;
            break;
        }
        cur_YEAR = cur_YEAR->link;
    }

    if(year_flag == 1){
        cur_STUDENT = head_STUDENT = cur_YEAR->ST_NUM;

        for(i = 0; i < cur_YEAR->Num_Size; i++){
            if(!strcmp(cur_STUDENT->number, num)){
                num_flag = 1;
                break;
            }
            cur_STUDENT = cur_STUDENT->link;
        }
    }

    if(num_flag == 1){
        printw("Input latest semester's GPA : ");
        scanw("%f", &temp);

        head_CGPA = cur_STUDENT->Child_C;
        cur_CGPA = head_CGPA;

        for(i = 0; i < cur_STUDENT->CGPA_Size - 1; i++)
            cur_CGPA = cur_CGPA->link;

        if(cur_CGPA->score == temp) flag1 = 1;
    }

    if(num_flag == 0 || year_flag == 0){
        printw("\nAccount <%s> does not exist.\n", Curr_Num);
        printw("Press any key to continue...\n");
    }

    if(flag1 == 1){
        for(i = 0; i < 8; i++){
            temp_num = rand() % 93 + 21;
            temppassword[i] = (char)temp_num;
        }
        printw("Temporary Password is <%s>\n", temppassword);
        strcpy(cur_STUDENT->password, temppassword);
    }
}
